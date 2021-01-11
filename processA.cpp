#include <unistd.h>
#include <iostream>

#include <alsa/asoundlib.h>
#include <math.h>
#include <fftw3.h>

#include "functions.h"


int calculateFrequency(audioInput *ai)
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

    int max = -50;
    int freq = 0;
    for( int i=0; i<ai->n_f; i++ ) {
        if (20*log10(ai->specslice[i]) > max) {
            max = 20*log10(ai->specslice[i]);
            freq = i*ai->Hz_per_pixel;
        }
    }
    return freq;
}




void* processA(void* varg) { 
    if (thread_to_core(0) != 0) {
        std::cout << "Nie udalo sie przypisac procesu A do rdzenia 0\n";
        return NULL;
    }
    
    SharedMemory* sm = ((SharedMemory*)varg);
    audioInput* ai = new audioInput();
    
    float inv256 = 1.0 / 256.0;
    float inv256_2 = inv256*inv256;
    
    while(true) {
        int n;
        while((n = snd_pcm_readi(ai->pcm_handle, ai->chunk, ai->frames_per_period)) < 0 ) {
            fprintf(stderr, "Error occured while recording: %s\n", snd_strerror(n));
            snd_pcm_prepare(ai->pcm_handle);
        }
        unsigned char by;
        int write_ptr, read_ptr;
        for( int i = 0; i < n; i++ ) { 
            read_ptr = i * ai->bytes_per_frame;
            write_ptr = ai->mod(ai->b_ind + i);
            by = ai->chunk[read_ptr];
            ai->b[write_ptr] = (float)ai->chunk[read_ptr+1]*inv256 + (float)by*inv256_2;
        }
        ai->b_ind = ai->mod(ai->b_ind+n);
        
        int freq = calculateFrequency(ai);
        sm->setFreq(freq); //send frequency to process B
    }
    
    std::cout << "Proces A zakonczony\n";
    return NULL;
}