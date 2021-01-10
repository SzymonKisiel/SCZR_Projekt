#include <unistd.h>
#include <iostream>

#include <alsa/asoundlib.h>
#include <math.h>
#include <fftw3.h>

#include "functions.h"


int slice(audioInput *ai) // simple graph of spectral slice with log power scale
  {
    int N = ai->win_size;             // transform length
    int nf = ai->n_f;              // # freqs to fill in powerspec
                    // actual windowed power spectrum
      for ( int i=0; i<N; ++i )     // copy last N samples & mult by window func
	ai->bwin[i] = ai->winf[i] * ai->b[ai->mod(ai->b_ind - N + i)];

      fftwf_execute(ai->fftw_p); // do the already-planned fft

      if (nf>N/2) {fprintf(stderr,"window too short cf n_f!\n"); return -1;}
      ai->specslice[0] = ai->bwin[0]*ai->bwin[0];  // zero-freq has no imag
      for ( int i=1; i<nf; ++i )     // compute power spectrum from hc dft
	    ai->specslice[i] = ai->bwin[i]*ai->bwin[i] + ai->bwin[N-i]*ai->bwin[N-i];
    
    int max = -100;
    int freq = 0;
    for( int i=0; i<ai->n_f; i++ ) {
      if (20*log10(ai->specslice[i]) > max) {
        max = 20*log10(ai->specslice[i]);
        freq = i*ai->Hz_per_pixel;
      }
    }
    //std::cout << freq << "\t" << max << std::endl;
    return freq;
  }




void* processA(void* varg) { //-------- capture: thread runs indep --
    // still mostly Luke's code, some names changed. Aims to read 1 "period"
    // (ALSA device setting) into the current write index of our ai->b buffer.
    
    std::cout << "Proces A: " << pthread_self() << "\n";
    if (thread_to_core(0) != 0) {
        std::cout << "Nie udalo sie przypisac procesu A do rdzenia 0\n";
        return NULL;
    }
    
    Data* data = ((Data*)varg);
    
    fprintf(stderr, "audioCapture thread started...\n");
    audioInput* ai = new audioInput();//(audioInput*) a;  // shares data with main thread = cool!
    //audioInput ai;
    std::cout << "ASDJASHDJLAHSKDJHASLJDHLASHDA\n";
    
    float inv256 = 1.0 / 256.0;
    float inv256_2 = inv256*inv256;
    
    while( ! ai->quit ) {  // loops around until state of ai kills it
      int n;
      if( ! ai->pause ) {
	// keep trying to get exactly 1 "period" of raw data from sound card...
	while((n = snd_pcm_readi(ai->pcm_handle, ai->chunk, ai->frames_per_period)) < 0 ) {
	  //	  if (n == -EPIPE) fprintf(stderr, "Overrun occurred: %d\n", n); // broken pipe
	  fprintf(stderr, "Error occured while recording: %s\n", snd_strerror(n));

	  //n = snd_pcm_recover(ai->pcm_handle, n, 0); // ahb

	  //fprintf(stderr, "Error occured while recording: %s\n", snd_strerror(n));
	  snd_pcm_prepare(ai->pcm_handle);
	  //fprintf(stderr, "Dropped audio data (frames read n=%d)\n", n);
	}  // n samples were got

	unsigned char by;
	int write_ptr, read_ptr;
	for( int i = 0; i < n; i++ ) { // read chunk into our buffer ai->b ...
	  read_ptr = i * ai->bytes_per_frame;
	  write_ptr = ai->mod(ai->b_ind + i); // wraps around
	  by = ai->chunk[read_ptr];
	  // compute float in [-1/2,1/2) from 16-bit raw... (LSB unsigned char)
	  ai->b[write_ptr] = (float)ai->chunk[read_ptr+1]*inv256 + (float)by*inv256_2;
	}
	ai->b_ind = ai->mod(ai->b_ind+n);  // update index (in one go)
    
    int freq = slice(ai); // compute spectral slice of recent buffer history
    data->setFreq(freq); //send frequency to process B
      }
      else {
	usleep(10000);  // wait 0.01 sec if paused (keeps thread CPU usage low)
      }
    }
    fprintf(stderr, "audioCapture thread exiting.\n");
    
    
    std::cout << "Proces A zakonczony\n";
    return NULL;
  }                          // ----------------------- end capture thread ----