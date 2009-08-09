#include <pic16f688.h>
#include "delay.h"

char delay;

void DELAY_5US() {
	_asm
			NOP
			NOP
			DECFSZ _delay,1
			GOTO _DELAY_5US
	_endasm;
}

void delay_ms(char amt) {
	while(amt) {
		/*TMR0 = 0x06;	//Set the timer to be 250 away from overflowing
		while(TMR0);*/
		WAIT_5US(0xC8)
		amt--;
	}
}

void delay_100us(char amt) {
	while(amt) {
		/*TMR0 = 0xE7;	//Set the timer to be 25 away from overflowing
		while(TMR0);*/
		WAIT_5US(0x14)
		amt--;
	}
}

