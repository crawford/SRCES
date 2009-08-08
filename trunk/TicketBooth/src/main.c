#include <pic16f688.h>
#include "1wire.h"
#include "serial.h"

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

void initialize();


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
	
		if(!(PORTA & ONEWIRE_INPUTMASK)) {
			//If the 1-wire bus is low (iButton is present)
			WAIT_5US(0x56)	//Wait 430us
	
			if(PORTA & ONEWIRE_INPUTMASK) {
				sendString("iButton Found!   ");

				if(readIButtonID()) {
					sendChar(id[0]);
					sendChar(id[1]);
					sendChar(id[2]);
					sendChar(id[3]);
					sendChar(id[4]);
					sendChar(id[5]);
					sendChar(id[6]);
					sendChar(id[7]);

				}

				sendString("\n\r");
			}
		}

		WAIT_5US(0xFF)	//Wait 1.275ms
	}
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

