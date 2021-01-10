#include <unistd.h>
#include <iostream>
#include <allegro5/allegro.h>
#include "functions.h"


void* processB(void* varg) {
    std::cout << "Proces B: " << pthread_self() << "\n";
    if (thread_to_core(1) != 0) {
        std::cout << "Nie udalo sie przypisac procesu B do rdzenia 1\n";
        return NULL;
    }
    
    Data* data = ((Data*)varg);
    
    int freq;
    State state = noJump;
    
    while (true) {
        freq = data->getFreq(); //recieve frequency from process A
        std::cout << freq << std::endl; 
        if (freq < 400)
            state = noJump;
        else if (freq < 800)
            state = smallJump;
        else
            state = bigJump;
        data->setState(state); //send state to process C
    }
    
    /*
     // Tymczasowo input z klawiatury
     
    if (!al_install_keyboard()) {
        std::cout << "couldn't initialize keyboard\n";
        return NULL;
    }
    
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue) {
        printf("couldn't initialize queue\n");
        return NULL;
    }
    
    al_register_event_source(queue, al_get_keyboard_event_source());
    ALLEGRO_EVENT event;
    State state = noJump;
    
    bool done = false;
    while (!done) {
        
        al_wait_for_event(queue, &event);
        
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_DOWN:
                    state = smallJump;
                    break;
                case ALLEGRO_KEY_UP:
                    state = bigJump;
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    done = true;
                default:
                    state = noJump;
                    break;
            }
        }
        data->setState(state); //send state to process C
        
    }
    */
    
    std::cout << "Proces B zakonczony\n";
    return NULL;
}