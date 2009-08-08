#include "serial.h"
#include <pic16f688.h>

void clear_usart_errors_inline() {
	if (OERR) {
		TXEN = 0;
		TXEN = 1;
		CREN = 0;
		CREN = 1;
	}
	if (FERR) {
		//dummy = RCREG;
		TXEN = 0;
		TXEN = 1;
	}
}

void sendChar(unsigned char c) {
	while(!TXIF) {		//set when register is empty
		clear_usart_errors_inline();
		//clrwdt();
	}
	TXREG = c;
}

void sendString(const char *str) {
	while(*str) {
		sendChar(*str);
		str++;
	}
}

