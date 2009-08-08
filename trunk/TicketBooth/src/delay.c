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

