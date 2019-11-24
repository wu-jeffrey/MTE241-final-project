#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 

#include "GLCD.h"
#include "GLCD_render.h"
#include "game_logic.h"
#include "linked_list.h"
#include <cmsis_os2.h>

// Declare Stuff
const int PIXEL_DIM = 10;

int tail_sz = 6;

// Declare Linked List
node_t* a;
node_t* b;
node_t* c;
node_t* d;
node_t* e;
node_t* f;
linked_list_t tail;

// Define initial apple coordinates
int apple_x = 100;
int apple_y = 100;

// Initial Position
int px = 160;
int py = 120;

// Initial Velocity
int vx = PIXEL_DIM;
int vy = 0;


void setVelocity(char direction) {	
	switch (direction) {
		case 'R':
			if (vx != -PIXEL_DIM) {
				vx = PIXEL_DIM;
				vy = 0;
			}
			break;
			
		case 'L':
			if (vx != PIXEL_DIM) {
				vx = -PIXEL_DIM;
				vy = 0;
			}
			break;
			
		case 'U':
			if (vy != PIXEL_DIM) {
				vy = -PIXEL_DIM;
				vx = 0;
			}
			break;
			
		case 'D':
			if (vy != -PIXEL_DIM) {
				vy = PIXEL_DIM;
				vx = 0;
			}
			break;
	}
}

coordinate_t generateApplePosition() {
  // Generate a random value b/w the border dimensions
  int x_coord = rand() % 300;
  int y_coord = rand() % 230;

  // Round it down to the nearest 10
	coordinate_t c = {
		.x = x_coord - x_coord % PIXEL_DIM,
		.y = y_coord - y_coord % PIXEL_DIM
	};
	
	return c;
}

bool checkTailForApple() {
	bool existsInTail = false;
  node_t* node = tail.start;
	
  while (node != NULL){
    if (apple_x == node->x && apple_y == node->y) {
      existsInTail = true;
			break;
    }
    node = node->next;
  }
	return existsInTail;
}

void putApple() {
	coordinate_t apple;
	
	// Ensure Apple does not spawn on snake's head or body/tail
	do {
		apple = generateApplePosition();	
	} while (apple.x == px && apple.y == py && checkTailForApple());

	// Set global position
	apple_x = apple.x;
	apple_y = apple.y;
	
	putPixel(apple_x, apple_y, Red);
}

/*******************************************************************************
* Gameplay Logic
*******************************************************************************/
void moveSnakeHead() {
	px+=vx; // make position and velocities globals
	py+=vy;
	putPixel(px, py, Green);
}


void moveSnakeBody(bool grow) {
	node_t* new_p = (node_t*) malloc(sizeof(node_t));
	new_p->x = px;
	new_p->y = py;
	pushFront(&tail, new_p);

	drawSnake(tail.start);
	
	if (grow) {
		tail_sz++;
	} else {
		putPixel(tail.end->x, tail.end->y, Black);
		dequeue(&tail);
	}
}

bool collisionDetected() {
	node_t* node = tail.start;
	bool collision = false;
	
	// Detect self-collision
	while (node != NULL && !collision){
		if (px == node->x && py == node->y) {
			collision = true;
		}
		node = node->next;
	}
	
	// Detect wall collision
	if (px == 320 || py == 240) {
		collision = true;
	}
	
	return collision;
}


bool appleEaten() {
	return px == apple_x && py == apple_y;
}


void initGameLogic() {
	GLCD_Init();
	GLCD_Clear(Black);
	
	// Assign nodes to initial snake tail
	a = (node_t*) malloc(sizeof(node_t));
	b = (node_t*) malloc(sizeof(node_t));
	c = (node_t*) malloc(sizeof(node_t));
	d = (node_t*) malloc(sizeof(node_t));
	e = (node_t*) malloc(sizeof(node_t));
	f = (node_t*) malloc(sizeof(node_t));

	a->x = 160;
	a->y = 120;
	a->prev = NULL;
	b->x = 150;
	b->y = 120;
	b->next = NULL;
	a->next = b;
	b->prev = a;

	tail.start = a;
	tail.end = b;

	c->x = 140;
	c->y = 120;
	d->x = 130;
	d->y = 120;
	e->x = 120;
	e->y = 120;
	f->x = 110;
	f->y = 120;

	pushEnd(&tail, c);
	pushEnd(&tail, d);
	pushEnd(&tail, e);
	pushEnd(&tail, f);
	
	// Initial Render
	putApple();
	drawBorder();
	putPixel(px, py, Green); // Draw Snakehead
	drawSnakeBody(tail.start); // Draw Snake tail/body
}