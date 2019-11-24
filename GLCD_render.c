#include <stdlib.h>
#include <stdio.h>
#include "GLCD.h"
#include "GLCD_render.h"

void putPixel(int x, int y, unsigned int short color) {
	GLCD_SetTextColor(color);
  // Removed Bargraph b/c it was fucking with the apple and snake positions -> we couldnt swallow the apple whole.
  // GLCD_Bargraph(x,y,PIXEL_DIM,PIXEL_DIM,1024);
	for(int i = 0; i<PIXEL_DIM; i++) {
		for(int j = 0; j<PIXEL_DIM; j++) {
			GLCD_PutPixel(x+i, y-j);
		}
	}
}

void drawBorder() {
	GLCD_SetTextColor(Blue);
	// Draw Horizontal Lines with width of 3 px
	for(int i_x = 5; i_x < 315; i_x++){
		for(int j_yleft = 5; j_yleft < 8; j_yleft++){
			GLCD_PutPixel(i_x, j_yleft);
		}
		for(int j_yright = 232; j_yright < 235; j_yright++){
			GLCD_PutPixel(i_x, j_yright);
		}
	}

	// Draw Vertical Lines with width of 3 px
	for(int i_y = 5; i_y < 235; i_y++){
		for(int j_xleft = 5; j_xleft < 8; j_xleft++){
			GLCD_PutPixel(j_xleft, i_y);
		}
		for(int j_xright = 313; j_xright < 315; j_xright++){
			GLCD_PutPixel(j_xright, i_y);
		}
	}
}

void drawSnakeBody(node_t* start) {
	node_t *current_node = start;
	while (current_node != NULL) {
		putPixel(current_node->x, current_node->y, Green);
		current_node = current_node->next;
	}
}

void drawStaticScreen() {
  GLCD_Clear(White);
	GLCD_SetTextColor(Black);
	char buffer[50];

	sprintf(buffer, "Game Over");
	GLCD_DisplayString(4, 4, 1, (unsigned char *)buffer);\

	sprintf(buffer, "Press Button");
	GLCD_DisplayString(6, 4, 1, (unsigned char *)buffer);

		sprintf(buffer, "To play again");
	GLCD_DisplayString(7, 4, 1, (unsigned char *)buffer);
}
