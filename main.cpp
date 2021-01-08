#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "processes.h"
#include "functions.h"
#include <allegro5/allegro.h>

void* fun(void* varg) {
    printf("Test watek\n");
    return NULL;
}


int main(int argc, char **argv)
{
    std::cout << "Liczba rdzeni procesora: " << sysconf(_SC_NPROCESSORS_ONLN) << "\n";
    Data data;
    //data = (Data*)malloc(sizeof(Data));
    
    if( !al_init() )
    {
        std::cout << "ERROR" << std::endl;
        return -1;
    }
    
    pthread_t tid[4];
    
    pthread_create(&tid[0], NULL, processA, &data);
    pthread_create(&tid[1], NULL, processB, &data);
    pthread_create(&tid[2], NULL, processC, &data);
    pthread_create(&tid[3], NULL, processD, &data);
    
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_join(tid[2], NULL);
    pthread_join(tid[3], NULL);
    
    /*for (int i = 0; i < 4; ++i) {
        pthread_join(tid[i], NULL);
    }*/
    //free(data);
    
	return 0;
}
