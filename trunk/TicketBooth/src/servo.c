#include "servo.h"
#include "delay.h"

#define OPEN 15
#define CLOSE 8
#define MOVETIME 20
#define SERVO_PIN 0
#define SERVO_HIGH _asm bsf PORTC, SERVO_PIN _endasm;
#define SERVO_LOW _asm bcf PORTC, SERVO_PIN _endasm;

void moveMotor(char pos) {
	char time = MOVETIME;
	while(time) {
		SERVO_HIGH
		delay_100us(pos);
		SERVO_LOW
		delay_ms(20);
		time--;
	}
	
}

void lockDoor() {
	moveMotor(CLOSE);
}

void unlockDoor() {
	moveMotor(OPEN);
}

