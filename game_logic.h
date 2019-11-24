#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include <stdbool.h>

typedef struct coordinate_t coordinate_t;
struct coordinate_t{
	int x,y;
};

void setVelocity(char direction);
void moveSnakeHead();
void moveSnakeBody(bool grow);
bool collisionDetected();

coordinate_t generateApplePosition();
bool checkTailForApple();
void putApple();
bool appleEaten();

void initGameLogic();
// void gameOver();

#endif