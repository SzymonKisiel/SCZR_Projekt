#include <unistd.h>
#include <iostream>

#include "functions.h"


void* processA(void* varg) {
    std::cout << "Proces A: " << pthread_self() << "\n";
    if (thread_to_core(0) != 0) {
        std::cout << "Nie udalo sie przypisac procesu A do rdzenia 0\n";
        return NULL;
    }
    
    
    
    /*Data* data = ((Data*)varg);
    std::cout << "x = " << data->getX() << '\n';
    for (int i = 0; i < 100000; ++i) {
        data->incrementX();
        std::cout << "x = " << data->getX() << '\n';
    }*/
    sleep(2);
    
    sleep(1);
    
    
    
    
    std::cout << "Proces A zakonczony\n";
    
    return NULL;
}