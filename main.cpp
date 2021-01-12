#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "processes.h"
#include "functions.h"
#include <allegro5/allegro.h>

int main(int argc, char **argv)
{
    std::cout << "Liczba rdzeni procesora: " << sysconf(_SC_NPROCESSORS_ONLN) << "\n";
    SharedMemory sm;
    
    if( !al_init() )
    {
        std::cout << "ERROR" << std::endl;
        return -1;
    }
    
    pthread_t tid[4];
    
    pthread_create(&tid[0], NULL, processA, &sm);
    pthread_create(&tid[1], NULL, processB, &sm);
    pthread_create(&tid[2], NULL, processC, &sm);
    pthread_create(&tid[3], NULL, processD, &sm);
    
    for (int i = 0; i < 4; ++i) {
        pthread_join(tid[i], NULL);
    }
    
	return 0;
}
