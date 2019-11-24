#include <lpc17xx.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 
#include <Math.h>
#include "GLCD.h"
#include <cmsis_os2.h>
#include <time.h>

#include "game_logic.h"
#include "GLCD_render.h"

// Mutexes to synchronize tasks
osMutexId_t mutex;
/*******************************************************************************
* Main
*******************************************************************************/
void renderThread() {
	while(1) {
		osMutexAcquire(mutex, osWaitForever);
		for(int i = 0; i < 1000000; i++); // TODO: increase delay as level increases

		// Snake Head is one pixel
		moveSnakeHead();
		
		if (collisionDetected()) {
			printf("Game Over");
		} 
	
		if (appleEaten()) {
			putApple();
			// Snake Body/Tail is the rest of the pixels
			moveSnakeBody(true);
		} else {
			moveSnakeBody(false);
		}
		
		osMutexRelease(mutex);
		osThreadYield();
	}
}

void joystickOutputThread() {
    while(1) {
      osMutexAcquire(mutex, osWaitForever);
      int input = LPC_GPIO1->FIOPIN;

      if (!((input >> 24) & 1)) {
				setVelocity('R');
      } else if (!((input >> 26) & 1)) {
				setVelocity('L');
      } else if (!((input >> 23) & 1)) {
				setVelocity('U');
      } else if (!((input >> 25 )& 1)) {
				setVelocity('D');
      } 
      
      osMutexRelease(mutex);
      osThreadYield();
    }
}

void waitForButton() {
  while(1) {
    osMutexAcquire(mutex, osWaitForever);
    if(!((LPC_GPIO2->FIOPIN >> 10) & 1)){
      osDelay(150);
      int keepLoop = 1;
      printf("Poll works");
      while(keepLoop) {
        if(!((LPC_GPIO2->FIOPIN >> 10) & 1)){
          osDelay(150);
          keepLoop = 0;
        } 
      }
    }
    osMutexRelease(mutex);
    osThreadYield();

  }
}

int main(void) {
	printf("Initializing Kernel\n");
	
	initGameLogic();
	
	mutex = osMutexNew(NULL);
	
	osKernelInitialize();
	osThreadNew(joystickOutputThread, NULL, NULL);
	osThreadNew(renderThread, NULL, NULL);
	osThreadNew(waitForButton, NULL, NULL);

	osKernelStart();
	for( ; ; ){}

	return 0;
}