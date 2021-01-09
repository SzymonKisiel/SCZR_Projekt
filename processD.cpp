#include <unistd.h>
#include <iostream>

#include "functions.h"

#include <stdio.h>
#include <pthread.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

/*
#define DISPLAY_WIDTH 640
#define DISPLAY_HEIGHT 480
#define GROUND_Y DISPLAY_HEIGHT * 3 / 4
#define PLAYER_X DISPLAY_WIDTH / 4
#define PLAYER_SIZE 20

#define OBSTACLE_SMALL_HEIGHT 50
#define OBSTACLE_BIG_HEIGHT 100
#define OBSTACLE_WIDTH 20
#define OBSTACLE_SMALL_GAP 60
*/

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
    std::cout << "Proces D: " << pthread_self() << "\n";
    if (thread_to_core(3) != 0) {
        std::cout << "Nie udalo sie przypisac procesu D do rdzenia 3\n";
        return NULL;
    }
    
    Data* data = ((Data*)varg);

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

    ALLEGRO_FONT* font = al_create_builtin_font();
    if(!font)
    {
        printf("couldn't initialize font\n");
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
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue))
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
            al_draw_line(0, GROUND_Y, DISPLAY_WIDTH, GROUND_Y, al_map_rgb(255, 255, 255), 1);
            
            int playerPosX;
            int temp[MAX_OBSTACLES];
            int temp1[MAX_OBSTACLES];
            data->getGameInfo(playerPosX, temp, temp1);
            
            draw_player(playerPosX);
            for (int i = 0; i < MAX_OBSTACLES; ++i) {
                draw_obstacle(temp[i], temp1[i]);
            }
            
            al_flip_display();
            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    
    std::cout << "Proces D zakonczony\n";

    return NULL;
}