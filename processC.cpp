#include <unistd.h>

#include "functions.h"

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <allegro5/allegro.h>

enum typeOfObstacle
{
    smallDown = 0,
    bigDown = 1,
    up = 2,
};

using namespace std;

class Player
{
private:
    int positionX;
    float positionY;

    int jumpHeight;

    int speed;
    float jumpSpeed;
    float startVelocity;

    bool isInFlight;

    int score;
    int bestScore;

public :
    Player(){
        bestScore = 0;
        score = 0;

        positionX = PLAYER_POSITION;
        positionY = 0;

        speed = MINIMUM_SPEED;
        isInFlight = false;
    }

    void jump(State state){
        if (state == smallJump)
            jumpHeight = SMALL_JUMP_HEIGHT;
        else if (state == bigJump)
            jumpHeight = BIG_JUMP_HEIGHT;

        if(jumpHeight == SMALL_JUMP_HEIGHT) startVelocity = SMALL_VELOCITY;
        else if(jumpHeight == BIG_JUMP_HEIGHT) startVelocity = BIG_VELOCITY;

        jumpSpeed = startVelocity;
        isInFlight = true;
    }

    void goNext(){
        positionY += jumpSpeed;
        if(positionY < 0){
            positionY = 0;
            startVelocity = 0;
            isInFlight = false;
        }
        else if (jumpSpeed < GRAVITY && jumpSpeed > -GRAVITY) {
            jumpSpeed -= GRAVITY/FLIGHT_LENGTH;
        }
        else
            jumpSpeed -= GRAVITY;
    }

    int getPositionX(){
        return positionX;
    }
    float getPositionY(){
        return positionY;
    }

    void setScore(int score){
        this->score = score;
    }

    void setBestScore(int score){
        this->score = score;
        if( score > bestScore ) bestScore = score;
    }

    int getJumpHeight(){
        return jumpHeight;
    }

    int getSpeed(){
        return speed;
    }

    bool getIsInFlight(){
        return isInFlight;
    }

    void setSpeed(int acceleration){
        this->speed += acceleration;
    }

    int getPlayerScore(){
        return score;
    }

    int getPlayerBestScore(){
        return bestScore;
    }

};

class Obstacle
{
private:
    int positionX;
    int height;
    int width;

    typeOfObstacle type;

public:
    Obstacle(){}

    Obstacle(typeOfObstacle newType, int position){
        type = newType;
        width = OBSTACLE_WIDTH;
        positionX = position;
        if(type == smallDown) height = OBSTACLE_SMALL_HEIGHT;
        else if(type == bigDown) height = OBSTACLE_BIG_HEIGHT;
            else height = 0;
    }

    int getPositionX(){
        return positionX;
    }

    void setPositionX(int position){
        positionX = position;
    }

    int getHeight(){
        return height;
    }

    int getWidth(){
        return width;
    }

    typeOfObstacle getTypeOfObstacle(){
        return type;
    }
};

class Game
{
private:

    Player player;
    Obstacle obstacles[MAX_OBSTACLES];

    int numberOfObstacle;
	
	ALLEGRO_EVENT_QUEUE * queue = NULL;
    ALLEGRO_EVENT ev1;
	ALLEGRO_TIMER * timer_FPS = NULL;

    bool gameOverConditions(){
        for(int i=0; i<MAX_OBSTACLES; ++i){
            if(player.getPositionX() >= obstacles[i].getPositionX() && 
               player.getPositionX() - PLAYER_SIZE <= (obstacles[i].getPositionX() + obstacles[i].getWidth()) && 
               player.getPositionY() + PLAYER_SIZE > obstacles[i].getHeight() + GAP) 
                   return true; //przeszkoda u góry
            else if(player.getPositionX() >= obstacles[i].getPositionX() && 
                    player.getPositionX() - PLAYER_SIZE <= (obstacles[i].getPositionX() + obstacles[i].getWidth()) && 
                    player.getPositionY() < obstacles[i].getHeight()) 
                        return true; // przeszkoda z dołu
        }
        return false;
    }

    void generateObstacle(int position){
        typeOfObstacle newType = typeOfObstacle(rand() % MAX_OBSTACLES); //typeOfObstacle(2);//
        Obstacle newObstacle(newType, position);
        obstacles[numberOfObstacle] = newObstacle;
        numberOfObstacle = (numberOfObstacle + 1) % MAX_OBSTACLES;
    }

    void levelUp();

public:
    Game(){
        numberOfObstacle = 0;
    }

    void start(){
		queue = al_create_event_queue();
		if( queue == NULL )
		{
			cout << "QUEUE ERROR" << std::endl;
		}
		
		timer_FPS = al_create_timer( 1.0 / 60 );
		if( !timer_FPS )
		{
			cout << "TIMER ERROR" << std::endl;
		}
		
		al_register_event_source( queue, al_get_timer_event_source( timer_FPS ) );
		
		al_start_timer( timer_FPS );
		
        for(int i=1; i<=MAX_OBSTACLES; ++i){
            generateObstacle(i * DISPLAY_WIDTH / 3 + DISPLAY_WIDTH / 3);
        }
    }

    void loop(SharedMemory* sm){

        while(!gameOverConditions() && !sm->isEnd()){
            al_wait_for_event( queue, & ev1 );
            if(ev1.type == ALLEGRO_EVENT_TIMER){
                for(int i=0; i<MAX_OBSTACLES; ++i){
                    obstacles[i].setPositionX(obstacles[i].getPositionX() - player.getSpeed());
                    if(obstacles[i].getPositionX() + obstacles[i].getWidth() < 0)
                        generateObstacle(POSITION_OF_GENERATING_OBSTACLES);
                }
                int temp[MAX_OBSTACLES];
                int temp1[MAX_OBSTACLES];
                for (int i = 0; i < MAX_OBSTACLES; ++i) {
                    temp[i] = obstacles[i].getPositionX();
                    temp1[i] = obstacles[i].getTypeOfObstacle();
                }

                State state = sm->getState();

                if(!player.getIsInFlight() && state != noJump)
                    player.jump(state);

                if(player.getIsInFlight())
                    player.goNext();

                player.setScore(player.getPlayerScore()+player.getSpeed());

                sm->setGameInfo(player.getPositionY(), temp, temp1);
            }
        }



    }

    bool restart(){
        player.setBestScore(player.getPlayerScore());
        player.setScore(0);
        player.setSpeed(MINIMUM_SPEED);

        cout << "Is it all over ?" << std::endl;
        return true;
    }

    void finish(){
        player.setBestScore(player.getPlayerScore());

        std::cout << "Wynik: " << player.getPlayerBestScore() << std::endl;
        
        al_destroy_timer(timer_FPS);
        al_destroy_event_queue(queue);
    }
};

void* processC(void* varg)
{
    if (thread_to_core(2) != 0) {
        std::cout << "Nie udalo sie przypisac procesu C do rdzenia 2\n";
        return NULL;
    }
    SharedMemory* sm = ((SharedMemory*)varg);
    
    srand(time(0));

	if( !al_init() )
    {
        cout << "ERROR" << std::endl;
        return NULL;
    }

    Game game;
    game.start();
    game.loop(sm);
    game.finish();
    
    sm->setEnd();
    
    std::cout << "Proces C zakonczony\n";
    return NULL;
}
