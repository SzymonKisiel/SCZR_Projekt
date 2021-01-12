#include <unistd.h>
#include <iostream>

#include "functions.h"

#include <stdio.h>
#include <pthread.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

void draw_player(int y_pos) {
    al_draw_filled_rectangle(PLAYER_POSITION - PLAYER_SIZE, GROUND_Y - y_pos - PLAYER_SIZE, 
                             PLAYER_POSITION, GROUND_Y - y_pos, 
                             al_map_rgb(0, 255, 0)
                            );

}

void draw_obstacle(int x_pos, int obstacle_type) {
    switch(obstacle_type) {
        case 0:
            al_draw_filled_rectangle(x_pos, GROUND_Y, 
                                     x_pos + OBSTACLE_WIDTH, GROUND_Y - OBSTACLE_SMALL_HEIGHT, 
                                     al_map_rgb(255, 0, 0)
                                    );
            al_draw_filled_rectangle(x_pos, GROUND_Y - OBSTACLE_SMALL_HEIGHT - GAP, 
                                     x_pos + OBSTACLE_WIDTH, 0, 
                                     al_map_rgb(255, 0, 0)
                                    );
            break;
        case 1:
            al_draw_filled_rectangle(x_pos, GROUND_Y, 
                                     x_pos + OBSTACLE_WIDTH, GROUND_Y - OBSTACLE_BIG_HEIGHT, 
                                     al_map_rgb(255, 0, 0)
                                    );
            break;
        case 2:
            al_draw_filled_rectangle(x_pos, GROUND_Y - OBSTACLE_SMALL_HEIGHT, 
                                     x_pos + OBSTACLE_WIDTH, 0, 
                                     al_map_rgb(255, 0, 0)
                                    );
            break;
    }
}


void* processD(void* varg) {
    if (thread_to_core(3) != 0) {
        std::cout << "Nie udalo sie przypisac procesu D do rdzenia 3\n";
        return NULL;
    }
    
    SharedMemory* sm = ((SharedMemory*)varg);

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if(!queue)
    {
        printf("couldn't initialize queue\n");
        return NULL;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    if(!timer)
    {
        printf("couldn't initialize timer\n");
        return NULL;
    }
    
    ALLEGRO_DISPLAY* disp = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if(!disp)
    {
        printf("couldn't initialize display\n");
        return NULL;
    }
    
    if(!al_init_primitives_addon()) {
        printf("couldn't initialize primitives\n");
        return NULL;
    }
    
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    
    al_start_timer(timer);
    while(1)
    {
        al_wait_for_event(queue, &event);

        switch(event.type)
        {
            case ALLEGRO_EVENT_TIMER:
                // game logic goes here.
                redraw = true;
                break;
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                sm->setEnd();
                done = true;
                break;
        }

        if(sm->isEnd())
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_line(0, GROUND_Y, DISPLAY_WIDTH, GROUND_Y, al_map_rgb(255, 255, 255), 1);
            
            int playerPosY;
            int obstaclePosX[MAX_OBSTACLES];
            int obstacleType[MAX_OBSTACLES];
            sm->getGameInfo(playerPosY, obstaclePosX, obstacleType);
            
            draw_player(playerPosY);
            for (int i = 0; i < MAX_OBSTACLES; ++i) {
                draw_obstacle(obstaclePosX[i], obstacleType[i]);
            }
            
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    
    std::cout << "Proces D zakonczony\n";

    return NULL;
}