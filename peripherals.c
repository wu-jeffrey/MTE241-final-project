#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h> 
#include <lpc17xx.h>
#include <Math.h>

bool buttonPressed() {
	return !((LPC_GPIO2->FIOPIN >> 10) & 1);
}

char joystickDirection() {
	int input = LPC_GPIO1->FIOPIN;
	
	if (!((input >> 24) & 1)) {
		return 'R';
	} else if (!((input >> 26) & 1)) {
		return 'L';
	} else if (!((input >> 23) & 1)) {
		return 'U';
	} else if (!((input >> 25 )& 1)) {
		return 'D';
	} 
}

void displayScoreWithLed(int score) {

	// Initialize LEDs
	LPC_GPIO1->FIODIR |= 0xB0000000;
	LPC_GPIO2->FIODIR |= 0x0000007C;

	int _score = score % 9;
	int shift[] = {0,6,5,4,3,2,31,29,28};

	if (_score == 0) {
		LPC_GPIO2->FIOCLR = 0x7C;
		LPC_GPIO1->FIOCLR = 0xB << 28;
	} else if (_score > 0 && _score < 6) {
		LPC_GPIO2->FIOSET = 1 << shift[_score];
	} else {
		LPC_GPIO1->FIOSET = 1 << shift[_score];
	}

}