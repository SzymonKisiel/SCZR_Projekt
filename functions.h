#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "constants.h"


//debug
#include <iostream>

int thread_to_core(int core_id);

enum State
{
    noJump = 0,
    smallJump = 1,
    bigJump = 2
};

class Data {
    int x;
    bool y;
    pthread_mutex_t muteks;
    sem_t sem;
    sem_t sem2;
    
    State state;
    
    int playerPosY;
    int obstaclePosX[3];
    int obstacleType[3];
public:
    Data() {
        sem_init(&sem, 0, 1);
        sem_init(&sem2, 0, 1);
        x = 0;
        state = noJump;
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
    
    void setState(State newState) {
        sem_wait(&sem2);
        this->state = newState;
        sem_post(&sem2);
        
    }
    
    State getState() {
        State result;
        
        sem_wait(&sem2);
        result = this->state;
        sem_post(&sem2);
        
        return result;
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
