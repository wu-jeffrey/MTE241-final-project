#include <lpc17xx.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 
#include <Math.h>
#include <cmsis_os2.h>

#include "GLCD.h"
#include "peripherals.h"
#include "game_logic.h"

// Mutexes to synchronize tasks
osMutexId_t mutex;
osThreadId_t T1, T2, T3, T4, T5;
bool game_over = false;
int score = 0;

void staticScreen() {
	while (true) {
		osMutexAcquire(mutex, osWaitForever);
		if (game_over) {
			gameOver();
			while ((LPC_GPIO2->FIOPIN >> 10) & 1) {}
			game_over = false;
			score = 0;
			initGameLogic();
		}

		osMutexRelease(mutex);
		osThreadYield();
	}
}

void render() {
	while(1) {
		osMutexAcquire(mutex, osWaitForever);
		for(int i = 0; i < 1000000; i++); // TODO: increase delay as level increases
		
		// Snake Head is one pixel
		moveSnakeHead();
		if (collisionDetected()) {
			game_over = true;
		} else if (appleEaten()) {
			// Snake Body/Tail is the rest of the pixels
			moveSnakeBody(true);
			putApple();
			score++;
		} else {
			moveSnakeBody(false);
		}
		
		osMutexRelease(mutex);
		osThreadYield();
	}
}

void displayScore() {
	while(1) {
		osMutexAcquire(mutex, osWaitForever);
		displayScoreWithLed(score);
		osMutexRelease(mutex);
		osThreadYield();
	}
}

void getJoyStickInput() {
	while(1) {
		osMutexAcquire(mutex, osWaitForever);
		setVelocity(joystickDirection());			
		osMutexRelease(mutex);
		osThreadYield();
	}
}

void getButtonInput() {
  while(1) {
    osMutexAcquire(mutex, osWaitForever);
		if (!((LPC_GPIO2->FIOPIN >> 10) & 1)) {
			osDelay(150);
			int keepLoop = 1;
			while(keepLoop) {
				if (!((LPC_GPIO2->FIOPIN >> 10) & 1)) {
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
	
	
	GLCD_Init();
	initGameLogic();
	
	mutex = osMutexNew(NULL);
	
	osKernelInitialize();
	
	T1 = osThreadNew(getJoyStickInput, NULL, NULL);
	T2 = osThreadNew(render, NULL, NULL);
	T3 = osThreadNew(getButtonInput, NULL, NULL);
	T4 = osThreadNew(staticScreen, NULL, NULL);
	T5 = osThreadNew(displayScore, NULL, NULL);

	osKernelStart();
	for( ; ; ){}

	return 0;
}