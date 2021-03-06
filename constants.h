#include <math.h>

const int DISPLAY_WIDTH = 640;
const int DISPLAY_HEIGHT = 480;

const int GROUND_Y = DISPLAY_HEIGHT * 3 / 4;
const int PLAYER_POSITION = DISPLAY_WIDTH / 4;
const int PLAYER_SIZE = 20;

const int MAX_OBSTACLES=3;
const int OBSTACLE_SMALL_HEIGHT=50;
const int OBSTACLE_BIG_HEIGHT=100;
const int OBSTACLE_WIDTH = 10;
const int GAP = 60;

const int FLIGHT_LENGTH = 20;

const int POSITION_OF_GENERATING_OBSTACLES = DISPLAY_WIDTH;

const int MINIMUM_SPEED = 1;
const int SMALL_JUMP_HEIGHT = 70;
const int BIG_JUMP_HEIGHT = 130;
const float GRAVITY = 0.05;
const float SMALL_VELOCITY = sqrt(2*SMALL_JUMP_HEIGHT*GRAVITY);
const float BIG_VELOCITY = sqrt(2*BIG_JUMP_HEIGHT*GRAVITY);