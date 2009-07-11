#include <pic16f688.h>

// Configuration bits:
typedef unsigned int word;
word at 0x2007 CONFIG = _INTRC_OSC_NOCLKOUT & _WDT_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _CPD_OFF & _BOD_ON & _IESO_ON & _FCMEN_ON;

void initialize();

int main() {
	initialize();

	while(1);
}

void initialize() {
	// Setup Ports
	TRISA = 0x00;	//Set portA to be all ouput
	TRISC = 0x01;	//Set portC to be all ouput except pin 0
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

