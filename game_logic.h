#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdbool.h>

typedef struct coordinate_t coordinate_t;
struct coordinate_t{
	int x,y;
};


void moveSnakeHead();
void moveSnakeBody(bool grow);
void setVelocity(char direction);

coordinate_t generateApplePosition();
bool checkTailForApple();
void putApple();
bool appleEaten();

bool collisionDetected();

void initGameLogic();
#endif