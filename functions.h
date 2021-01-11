#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include <alsa/asoundlib.h>
#include <fftw3.h>

#include "constants.h"

int thread_to_core(int core_id);

enum State
{
    noJump = 0,
    smallJump = 1,
    bigJump = 2
};

class SharedMemory {
    sem_t sem;
    sem_t sem2;
    sem_t sem3;
    
    //A i B
    int freq;
    
    //B i C
    State state;
    
    //C i D
    int playerPosY;
    int obstaclePosX[3];
    int obstacleType[3];
public:
    SharedMemory() {
        sem_init(&sem, 0, 1);
        sem_init(&sem2, 0, 1);
        sem_init(&sem3, 0, 1);
        state = noJump;
    }

    void setFreq(int freq) {
        sem_wait(&sem3);
        this->freq = freq;
        sem_post(&sem3);
    }
    
    int getFreq() {
        int result;
        
        sem_wait(&sem3);
        result = this->freq;
        sem_post(&sem3);
        
        return result;
    }
    
    void setState(State newState) {
        sem_wait(&sem2);
        this->state = newState;
        sem_post(&sem2);
        
    }
    
    State getState() {
        State result;
        
        sem_wait(&sem2);
        result = this->state;
        sem_post(&sem2);
        
        return result;
    }
    
    void setGameInfo(int playerPosY, int obstaclePosX[], int obstacleType[]) {
        sem_wait(&sem);
        this->playerPosY = playerPosY;
        for (int i = 0; i < MAX_OBSTACLES; ++i) {
            this->obstaclePosX[i] = obstaclePosX[i];
            this->obstacleType[i] = obstacleType[i];        
        }
        sem_post(&sem);
    }
    
    void getGameInfo(int & playerPosY, int obstaclePosX[], int obstacleType[])  {
        sem_wait(&sem);
        playerPosY = this->playerPosY;
        for (int i = 0; i < MAX_OBSTACLES; ++i) {
            obstaclePosX[i] = this->obstaclePosX[i];
            obstacleType[i] = this->obstacleType[i];        
        }
        sem_post(&sem);
    }
};

#define PI              3.14159265358979323846
#define TWO_WIN_SIZE    13

class audioInput {
  public:
  char *chunk;
  float *b, *specslice, *bwin, *winf, *sg;
  char *sgb;
  int b_ind, b_size, n_f, n_tw, sg_size, win_size;
  float dt, t_memory, Hz_per_pixel;

  pthread_t capture_thread;
  fftwf_plan fftw_p;

  // Audio device data (modified from ALSA tutorial)
  int bytes_per_frame, frames_per_period, nperiods, channels;
  int req_rate, rate;   /* Requested and actual sample rate */
  int dir;          /* rate == req_rate --> dir = 0 */
                    /* rate < req_rate  --> dir = -1 */
                    /* rate > req_rate  --> dir = 1 */
  snd_pcm_uframes_t period_size;     // Period size (bytes)
  snd_pcm_uframes_t req_size, size;  // requested and actual ALSA buffer size
  snd_pcm_t *pcm_handle;        /* Handle for the PCM device */ 
  snd_pcm_stream_t stream;     /* Playback stream */
  /* This structure contains information about    */
  /* the hardware and can be used to specify the  */      
  /* configuration to be used for the PCM stream. */ 
  snd_pcm_hw_params_t *hwparams;
  /* Name of the PCM device, like plughw:0,0          */
  /* The first number is the number of the soundcard, */
  /* the second number is the number of the device.   */
  char *pcm_name;

  void setupWindowFunc(float *w, int N) {
    float W;
    int i;
      // truncated Gaussian window (Gaussian tails + exp small error)
      W = N/5.0F;    // width: keep small truncation but wide to not waste FFT
      for( i=0; i<N; ++i) w[i] = exp(-(i-N/2)*(i-N/2)/(2*W*W));

  }

  audioInput() {            // nontrivial constructor
    channels = 2;       // Had to change to stereo for System76 ! (was mono)
    bytes_per_frame = 2 * channels;      // 16-bit
    req_rate = 44100;         // audio sampling rate in Hz
    frames_per_period = (int)(req_rate/60.0);   // 735 = 44100Hz/60fps assumed
    nperiods = 2;             // >=2, see ALSA manual
    t_memory = 20.0;            // memory of our circular buffer in secs

    period_size = frames_per_period * bytes_per_frame;
    chunk = new char[period_size];   // raw data buffer for PCM read: 1 period
    req_size = frames_per_period * nperiods; // ALSA device buffer size (frames)
    b_ind = 0;                // integer index where to write to in buffer    
    if( initDevice() < 0 ) // set up sound card for recording (sets rate, size)
      exit(1);
    dt = 1.0 / (float)rate;   // sampling period
    b_size = (int)(t_memory * rate);   // buffer size
    b = new float[b_size];    // our circular audio buffer
    
    win_size = 1 << 13;     // FFT size
    bwin = new float[win_size];         // windowed recent audio data
    winf = new float[win_size];     
    setupWindowFunc(winf, win_size);    // windowing function
     // set up fast in-place single-precision real-to-half-complex DFT:
    fftw_p = fftwf_plan_r2r_1d(win_size, bwin, bwin, FFTW_R2HC, FFTW_MEASURE);
    n_f = 560;  // # freqs   ...spectrogram stuff
    specslice = new float[n_f];
    n_tw = 940; // # time windows: should be multiple of 4 for glDrawPixels

    Hz_per_pixel = 1.0F / (win_size*dt);
  }
  ~audioInput() {             // destructor
    snd_pcm_close (pcm_handle);
    fftwf_destroy_plan(fftw_p);
  }
  
  int initDevice() {  // ........ set up sound card for recording ........
    // ALSA tutorial, taken from http://www.suse.de/~mana/alsa090_howto.html
    
    stream = SND_PCM_STREAM_CAPTURE;
    /* Init pcm_name. Of course, later you */
    /* will make this configurable ;-)     */
    pcm_name = strdup("plughw:0,0");
    /* Allocate the snd_pcm_hw_params_t structure on the stack. */
    snd_pcm_hw_params_alloca(&hwparams);
    /* Open PCM. The last parameter of this function is the mode. */
    /* If this is set to 0, the standard mode is used. Possible   */
    /* other values are SND_PCM_NONBLOCK and SND_PCM_ASYNC.       */ 
    /* If SND_PCM_NONBLOCK is used, read / write access to the    */
    /* PCM device will return immediately. If SND_PCM_ASYNC is    */
    /* specified, SIGIO will be emitted whenever a period has     */
    /* been completely processed by the soundcard.                */
    if (snd_pcm_open(&pcm_handle, pcm_name, stream, 0) < 0) {
      fprintf(stderr, "Error opening PCM device %s\n", pcm_name);
      return(-1);
    }
    /* Init hwparams with full configuration space */
    if (snd_pcm_hw_params_any(pcm_handle, hwparams) < 0) {
      fprintf(stderr, "Can not configure this PCM device.\n");
      return(-1);
    }
    /* Set access type. This can be either    */
    /* SND_PCM_ACCESS_RW_INTERLEAVED or       */
    /* SND_PCM_ACCESS_RW_NONINTERLEAVED.      */
    /* There are also access types for MMAPed */
    /* access, but this is beyond the scope   */
    /* of this introduction.                  */
    if (snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
      fprintf(stderr, "Error setting access.\n");
      return(-1);
    }
    
    /* Set sample format */
    if (snd_pcm_hw_params_set_format(pcm_handle, hwparams, SND_PCM_FORMAT_S16_LE) < 0) {
      fprintf(stderr, "Error setting format.\n");
      return(-1);
    }
    
    /* Set sample rate. If the requested rate is not supported */
    /* by the hardware, use nearest possible rate.         */ 
    rate = req_rate;
    if (snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, (uint*)&rate, 0) < 0) {
      fprintf(stderr, "Error setting rate.\n");
      return(-1);
    }
    if (rate != req_rate) {
      fprintf(stderr, "The rate %d Hz is not supported by your hardware.\n \
                        ==> Using %d Hz instead.\n", req_rate, rate);
    }
    
    /* Set number of channels */
    if (snd_pcm_hw_params_set_channels(pcm_handle, hwparams, channels) < 0) {
      fprintf(stderr, "Error setting channels.\n");
      return(-1);
    }
    
    /* Set number of periods. Periods used to be called fragments. */ 
    if (snd_pcm_hw_params_set_periods(pcm_handle, hwparams, nperiods, 0) < 0) {
      fprintf(stderr, "Error setting number of periods.\n");
      return(-1);
    }
    /* Set buffer size (in frames). The resulting latency is given by */
    /* latency = period_size * nperiods / (rate * bytes_per_frame)     */
    size = req_size;
    if (snd_pcm_hw_params_set_buffer_size_near(pcm_handle, hwparams, &size) < 0) {
      fprintf(stderr, "Error setting buffersize.\n");
      return(-1);
    }
    if( size != req_size ) {
      fprintf(stderr, "Buffer size %d is not supported, using %d instead.\n", (int)req_size, (int)size);
    }
    
    /* Apply HW parameter settings to PCM device and prepare device  */
    if (snd_pcm_hw_params(pcm_handle, hwparams) < 0) {
      fprintf(stderr, "Error setting HW params.\n");
      return(-1);
    }
    return 1;
  }
  
  int mod( int i ) { 
    int r = i % b_size;
    if (r<0)
      r += b_size;
    return r;
  }
};