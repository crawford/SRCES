#include <pic16f688.h>
#include "1wire.h"
#include "serial.h"
#include "delay.h"
#include "servo.h"

// Pin Definitions
#define LED_INSIDE_PIN 2
#define LED_INSIDE_PORT PORTA
#define LED_OUTSIDE_PIN 3
#define LED_OUTSIDE_PORT PORTC
#define BUTTON_PIN 1

// Macro Definitions
#define SLEEP() _asm SLEEP _endasm
#define LED_INSIDE_ON() _asm bcf LED_INSIDE_PORT, LED_INSIDE_PIN _endasm
#define LED_INSIDE_OFF() _asm bsf LED_INSIDE_PORT, LED_INSIDE_PIN _endasm
#define LED_OUTSIDE_ON() _asm bcf LED_OUTSIDE_PORT, LED_OUTSIDE_PIN _endasm
#define LED_OUTSIDE_OFF() _asm bsf LED_OUTSIDE_PORT, LED_OUTSIDE_PIN _endasm
#define BUTTON_MASK (1 << BUTTON_PIN)
#define BUTTON_UP (PORTA & BUTTON_MASK)
#define BUTTON_DOWN (!BUTTON_UP)
#define IS_LOCKED(x) ((x) & TRUE)
#define IS_PRIVATE(x) ((x) & TRUE)

//Constants
#define HOLD_TIME 100
#define IBUTTON_CHECK_NUMBER 10
#define TRUE 1
#define FALSE 0

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
void handleButton();
void handleOneWire();
void toggleLock();
void togglePrivacy();

volatile unsigned char locked;
volatile unsigned char private;

unsigned char main() {
	initialize();

	locked = FALSE;
	private = FALSE;

	ONEWIRE_RELEASE();

	while(1) {
		sendString("Sleeping...\n\r");		
		SLEEP();
	}
}

void initialize() {
	// Setup Ports
	TRISA = BUTTON_MASK | ONEWIRE_IOC_MASK;// | ONEWIRE_MASK;
	TRISC = 0x20;			//Set portC to be all ouput except RC5 (Rx)
	PORTA = 0x00;
	PORTC = 0x00;
	LED_INSIDE_OFF();
	LED_OUTSIDE_OFF();

	// Setup Analog Inputs
	ANSEL = 0x00;	//Disable A/D convertors
	CMCON0 = 0x07;	//Disable comparators

	// Setup Pull-Up Resistors
	NOT_RAPU = 0;      		//Clear the PORTA Pull-up Enable bit (active low)
	WPUA = 0x00;			//Disable all pullups
	WPUA |= BUTTON_MASK;	//Enable pullup for button

	// Setup IOC
	GIE = 1;					//Enable Global Interrupt Enable bit
	PEIE = 1;					//Enable Peripheral Interrupt Enable bit
	IOCA = 0x00;				//Disable all IOCs
	IOCA |= BUTTON_MASK;		//Enable IOC for button
	IOCA |= ONEWIRE_IOC_MASK;	//Enable IOC for 1-wire
	RAIE = 1;					//PORTA Change Interrupt Enable bit

	// Setup TMR0
	/*T0CS = 0x00;	//Bind tmr0 clock to internal oscillator
	PSA = 0x00;		//Assign prescaler to TMR0
	PS2 = 0x00;		//Set prescaler to be 1:4
	PS1 = 0x00;
	PS0 = 0x01;*/

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

void portChanged() __interrupt 0 {
	if(BUTTON_DOWN) {
		handleButton();
	} else if(!IS_PRIVATE(private)) {
		handleOneWire();
	}

	_asm
		movfw  PORTA
	_endasm;

	RAIF = 0;	//Clear the PORTA IOC Flag bit
}

void handleButton() {
	unsigned char downTime = 0;
	while(BUTTON_DOWN & (downTime < HOLD_TIME)) {
		delay_ms(10);
		downTime++;
	}

	if(downTime == HOLD_TIME) {
		togglePrivacy();
	} else {
		toggleLock();
	}
}

void toggleLock() {
	//Toggle the locked state, turn off the privacy mode
	private = FALSE;
	LED_INSIDE_OFF();
	if(IS_LOCKED(locked)) {
		unlockDoor();
		locked = FALSE;
	} else {
		lockDoor();
		locked = TRUE;
	}
}

void togglePrivacy() {
	//Toggle the privacy mode
	if(IS_PRIVATE(private)) {
		private = FALSE;
		LED_INSIDE_OFF();
	} else {
		//If turning on the privacy mode, lock the door as well
		locked = TRUE;
		private = TRUE;
		LED_INSIDE_ON();
		lockDoor();
	}
}

void handleOneWire() {	
	char count = IBUTTON_CHECK_NUMBER;

	while(count) {
		//Reset iButton (wait 500us)
		ONEWIRE_PULLDOWN();
		WAIT_5US(0x64);

		//wait for line to return to high (70us) and iButon to trigger presence pulse
		ONEWIRE_RELEASE();
		WAIT_5US(0x0E);

		if(ONEWIRE_LOW) {
			//If the 1-wire bus is low (iButton is present)
			WAIT_5US(0x56);	//Wait 430us

			if(ONEWIRE_HIGH) {
				if(readIButtonID()) {
					sendString("iButton: ");
					sendChar(id[0]);
					sendChar(id[1]);
					sendChar(id[2]);
					sendChar(id[3]);
					sendChar(id[4]);
					sendChar(id[5]);
					sendChar(id[6]);
					sendChar(id[7]);
					sendString("\n\r");

					toggleLock();

					break;
				}
			} else {
				sendString("Shorted?\n\r");
			}
		}
		
		count--;
	}
}

