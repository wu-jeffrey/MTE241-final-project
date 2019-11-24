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
int prev_score = 0;
int pollrate = 1000000;
void staticScreen() {
	while (true) {
		osMutexAcquire(mutex, osWaitForever);
		if (game_over) {
			gameOver();
			while (!buttonPressed()) {}
			game_over = false;
			score = 0;
			prev_score = 0;
			pollrate = 1000000;
			initGameLogic();
		}

		osMutexRelease(mutex);
		osThreadYield();
	}
}

void render() {
	while(1) {
		osMutexAcquire(mutex, osWaitForever);
		for(int i = 0; i < pollrate; i++); // TODO: increase delay as level increases
		
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
		if ( !(score % 9) && (prev_score != score) ) {
			pollrate /= 5;
		}
		// printf("pollrate: %d\n", pollrate);
		prev_score = score;
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
		if (buttonPressed()) {
			while(buttonPressed()) {}
			bool keepLoop = true;
			while(keepLoop) {
				if (buttonPressed()) {
					while(buttonPressed()) {}
					keepLoop = false;
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