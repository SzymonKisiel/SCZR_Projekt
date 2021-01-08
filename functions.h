#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "constants.h"

int thread_to_core(int core_id);

class Data {
    int x;
    bool y;
    pthread_mutex_t muteks;
    sem_t sem;
    
    int playerPosY;
    int obstaclePosX[3];
    int obstacleType[3];
public:
    Data() {
        sem_init(&sem, 0, 1);
    }
    
    void incrementX() {
        //pthread_mutex_lock(&muteks); 
        sem_wait(&sem);
        ++this->x;
        //pthread_mutex_unlock(&muteks); 
        sem_post(&sem);
    }
    
    int getX() {
        int value = 0;
        //pthread_mutex_lock(&muteks);
        sem_wait(&sem);
        value = this->x;
        sem_post(&sem);
        //pthread_mutex_unlock(&muteks);
        return value;
    }
    
    void setX() {
        
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
