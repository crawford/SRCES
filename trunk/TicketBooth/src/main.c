#include <pic16f688.h>

// Configuration bits:
unsigned int at 0x2007 CONFIG = _INTRC_OSC_NOCLKOUT & \
								_WDT_OFF & \
								_PWRTE_OFF & \
								_MCLRE_OFF & \
								_CP_OFF & \
								_CPD_OFF & \
								_BOD_ON & \
								_IESO_ON & \
								_FCMEN_ON;

#define ONEWIRE 0
#define ONEWIRE_PULLDOWN \
	_asm bcf TRISC, ONEWIRE _endasm; \
	_asm bcf PORTC, ONEWIRE _endasm;
#define ONEWIRE_RELEASE _asm bsf TRISC, ONEWIRE _endasm;
#define ONEWIRE_INPUTMASK 1 << ONEWIRE
#define IBUTTON_FAMILYCODE 0x01
/*#define WAIT_5US(x) \
	_asm MOVLW x _endasm; \
	_asm MOVWF _delay _endasm; \
	_asm NOP _endasm; \
	_asm NOP _endasm; \
	_asm DECFSZ _delay,1 _endasm; \
	_asm GOTO $-3 _endasm;*/
#define WAIT_5US(x) \
	_asm MOVLW x _endasm; \
	_asm MOVWF _delay _endasm; \
	_asm CALL _DELAY_5US _endasm;

void initialize();
void sendString(const char*);
void sendChar(unsigned char);
char readIButtonID();

char delay;
char iobyte;
char count;
char id[8];

/*
 * Main Function
 */

char main() {
	initialize();

	while(1) {	
		//Reset iButton (wait 500us)
		ONEWIRE_PULLDOWN
		WAIT_5US(0x64)
	
		//wait for line to return to high (70us) and iButon to trigger presence pulse
		ONEWIRE_RELEASE
		WAIT_5US(0x0E)
	
		if(!(PORTC & ONEWIRE_INPUTMASK)) {
			//If the 1-wire bus is low (iButton is present)
			WAIT_5US(0x56)	//Wait 430us
	
			if(PORTC & ONEWIRE_INPUTMASK) {
				sendString("iButton Found!   ");

				readIButtonID();
				sendChar(id[0]);
				sendChar(id[1]);
				sendChar(id[2]);
				sendChar(id[3]);
				sendChar(id[4]);
				sendChar(id[5]);
				sendChar(id[6]);
				sendChar(id[7]);

				sendString("\n\r");
			}
		}

		WAIT_5US(0xFF)	//Wait 1.275ms
	}
}

/*
 * Serial Communication Functions
 */

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

/*
 * 1-Wire Functions
 */

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
			bcf 	TRISC,ONEWIRE
			bcf 	PORTC,ONEWIRE			; Pull down the bus
			NOP
			NOP
			NOP								; Wait 3us
			RRF 	_iobyte,F
			BTFSC 	STATUS,0				; Check the lsb of iobyte
			bsf 	TRISC,ONEWIRE			; Release the bus
			MOVLW 	0x0C
			MOVWF 	_delay
			CALL 	_DELAY_5US				; Wait 60us
			bsf 	TRISC,ONEWIRE			; Release the bus
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
			bcf 	TRISC,ONEWIRE
			bcf 	PORTC,ONEWIRE			; Pull down the bus
			NOP
			NOP
			NOP
			NOP
			NOP
			NOP								; Wait 6us
			bsf 	TRISC,ONEWIRE			; Release the bus
			NOP
			NOP
			NOP
			NOP								; Wait 4us
			MOVF 	PORTC,W
			ANDLW 	ONEWIRE_INPUTMASK		; Mask off the ONEWIRE_IN bit
			BCF		STATUS, 0
			ADDLW	0xFF					; C = 1 if ONEWIRE_IN = 1: C = 0 if ONEWIRE_IN = 0
			RRF		_iobyte,1				; Shift C into IOBYTE
			MOVLW 	0x0A
			MOVWF 	_delay
			CALL 	_DELAY_5US				; Wait 50us to end of time slot
			DECFSZ	_count,F				; Decrement the bit counter
			GOTO	_RXLP
	_endasm;

	return iobyte;
}

char readIButtonID() {
	char i = 8;

	//Send read-rom command
	writeByte(0x33);

	//Read the 8 byte iButton address
	do {
		i--;
		id[i] = readByte();
	} while(i);

	//Verify the family code
	if(id[0] != IBUTTON_FAMILYCODE)
		return 0;

	//Verify the checksum

	return 1;
}

/*
 * Initialization Functions
 */

void initialize() {
	// Setup Ports
	TRISA = 0x00;	//Set portA to be all ouput
	TRISC = 0x02;	//Set portC to be all ouput except pin 0
	PORTA = 0x00;
	PORTC = 0x00;
	
	// Setup Analog Inputs
	ANSEL = 0x00;	//Disable A/D convertors
	CMCON0 = 0x07;	//Disable comparators

	// Setup Pull-Up Resistors
	WPUA = 0x04;
	
	// Setup TMR0
	T0CS = 0x00;	//Bind tmr0 clock to internal oscillator
	
	// Setup Main Clock
	OSCCON = 0x60;	//Set internal oscillator to 4MHz

	// Setup Serial Communications
	SPBRG = 0x19;	//(PIC_CLK/(16 * BAUD) - 1)
	BRGH = 0x01;	//data rate for sending
	SYNC = 0x00;	//asynchronous
	SPEN = 0x01;	//enable serial port pins
	CREN = 0x01;	//enable reception
	SREN = 0x00;	//no effect
	TXIE = 0x00;	//disable tx interrupts
	RCIE = 0x00;	//disable rx interrupts
	TX9 = 0x00;		//8-bit transmission
	RX9 = 0x00;		//8-bit reception
	TXEN = 0x00;	//reset transmitter
	TXEN = 0x01;	//enable the transmitter
}

