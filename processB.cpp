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
    /*std::cout << "x = " << data->getX() << '\n';
    for (int i = 0; i < 100000; ++i) {
        data->incrementX();
        std::cout << "x = " << data->getX() << '\n';
    }
    
    sleep(1);
    
    sleep(1);*/
    
    
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
    
    while (true) {
        
        al_wait_for_event(queue, &event);
        
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_DOWN:
                    state = smallJump;
                    break;
                case ALLEGRO_KEY_UP:
                    state = bigJump;
                    break;
                default:
                    state = noJump;
                    break;
            }
        }
        data->setState(state);
        
    }
    
    
    
    
    
    std::cout << "Proces B zakonczony\n";
    return NULL;
}