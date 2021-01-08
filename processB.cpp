#include <unistd.h>
#include <iostream>

#include "functions.h"

void* processB(void* varg) {
    std::cout << "Proces B: " << pthread_self() << "\n";
    if (thread_to_core(1) != 0) {
        std::cout << "Nie udalo sie przypisac procesu B do rdzenia 1\n";
        return NULL;
    }
    /*
    Data* data = ((Data*)varg);
    std::cout << "x = " << data->getX() << '\n';
    for (int i = 0; i < 100000; ++i) {
        data->incrementX();
        std::cout << "x = " << data->getX() << '\n';
    }
    
    sleep(1);
    */
    sleep(1);
    
    
    
    
    std::cout << "Proces B zakonczony\n";
    return NULL;
}