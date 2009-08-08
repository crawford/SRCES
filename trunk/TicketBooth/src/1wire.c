#include "1wire.h"
#include <pic16f688.h>

char crc;
char delay;
char iobyte;
char count;
char id[8];

void DELAY_5US() {
	_asm
			NOP
			NOP
			DECFSZ _delay,1
			GOTO _DELAY_5US
	_endasm;
}

void writeByte(char c) {
	count = 8;
	iobyte = c;

	_asm
		_TXLP:
			BCF		TRISA,ONEWIRE
			BCF 	PORTA,ONEWIRE			; Pull down the bus
			NOP
			NOP
			NOP								; Wait 3us
			RRF 	_iobyte,F
			BTFSC 	STATUS,0				; Check the lsb of iobyte
			BSF 	TRISA,ONEWIRE			; Release the bus
			MOVLW 	0x0C
			MOVWF 	_delay
			CALL 	_DELAY_5US				; Wait 60us
			BSF 	TRISA,ONEWIRE			; Release the bus
			NOP
			NOP
			DECFSZ	_count,F
			GOTO 	_TXLP
	_endasm;	
}

char readByte() {
	_asm
		MOVLW	0x00
		MOVWF	_iobyte
		MOVLW	0x08
		MOVWF	_count
		_RXLP:
			BCF 	TRISA,ONEWIRE
			BCF 	PORTA,ONEWIRE			; Pull down the bus
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP								; Wait 6us
			BSF 	TRISA,ONEWIRE			; Release the bus
			NOP
			NOP
			NOP
			NOP								; Wait 4us
			MOVF 	PORTA,W
			ANDLW 	ONEWIRE_INPUTMASK		; Mask off the ONEWIRE_IN bit
			BCF		STATUS, 0
			ADDLW	0xFF					; C = 1 if ONEWIRE_IN = 1: C = 0 if ONEWIRE_IN = 0
			RRF		_iobyte,F				; Shift C into IOBYTE
			MOVLW 	0x0A
			MOVWF 	_delay
			CALL 	_DELAY_5US				; Wait 50us to end of time slot
			DECFSZ	_count,F				; Decrement the bit counter
			GOTO	_RXLP
	_endasm;

	return iobyte;
}

char readIButtonID() {
	char i;
	char temp;

	//Send read-rom command
	writeByte(0x33);

	//Read the 8 byte iButton address
	for(i = 0; i < 8; i++) {
		id[i] = readByte();
	}

	//Verify the family code
	if(id[0] != IBUTTON_FAMILYCODE)
		return 0;

	//Verify the checksum
	crc = 0;
	for(i = 0; i < 7; i++) {
		iobyte = id[i];

		for(count = 0; count < 8; count++) {
			iobyte >>= 1;
			temp = STATUS;

			_asm
				BCF	STATUS,0
				RRF _crc,F
			_endasm;
			
			if((temp ^ STATUS) & 1) {
				crc ^= 0x8C;
			}
			
		}
	}
	
	if(id[7] != crc)
		return 0;

	return 1;
}

