#include <unistd.h>
#include <iostream>
#include <allegro5/allegro.h>
#include "functions.h"


void* processB(void* varg) {
    if (thread_to_core(1) != 0) {
        std::cout << "Nie udalo sie przypisac procesu B do rdzenia 1\n";
        return NULL;
    }
    
    SharedMemory* sm = ((SharedMemory*)varg);
    
    int freq;
    State state = noJump;
    
    while (true) {
        freq = sm->getFreq(); //recieve frequency from process A
        //std::cout << freq << std::endl; 
        if (freq < 400)
            state = noJump;
        else if (freq < 800)
            state = smallJump;
        else
            state = bigJump;
        sm->setState(state); //send state to process C
    }
    
    std::cout << "Proces B zakonczony\n";
    return NULL;
}