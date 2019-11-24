#ifndef GLCD_RENDER_H
#define GLCD_RENDER_H

#include "linked_list.h"

extern const int PIXEL_DIM;

void drawBorder();
void putPixel(int x, int y, unsigned int short color);
void drawSnakeBody(node_t* start);
void drawStaticScreen();

#endif