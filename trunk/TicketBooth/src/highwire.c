#include "highwire.h"
#include "serial.h"
#include "1wire.h"

unsigned char inMessageBuffer[MAX_MESSAGE_IN_LENGTH];
unsigned char inMessageBufferLength;
unsigned char outMessageBuffer[MAX_MESSAGE_OUT_LENGTH];

void processMessage() {
	
}

char checkForMessage() {
	//Check for start byte
	if(inMessageBuffer[0] != HIGHWIRE_START)
		return 0;

	//Check for end byte
	if(inMessageBuffer[inMessageBufferLength - 1] != HIGHWIRE_END)
		return 0;

	//Check payload size
	if(inMessageBuffer[2] + 4 != inMessageBufferLength)
		return 0;

	return 1;
}

void broadcastState(unsigned char state) {
	outMessageBuffer[0] = HIGHWIRE_START;
	outMessageBuffer[1] = MESSTYPE_BROADCAST;
	
	if(state != STATE_BAD_IBUTTON) {
		outMessageBuffer[2] = 1;
	
		switch(state) {
			case STATE_LOCKED:
				outMessageBuffer[1] |= OPCODE_LOCKED;
				outMessageBuffer[3] = 0x01;
			break;
			case STATE_UNLOCKED:
				outMessageBuffer[1] |= OPCODE_LOCKED;
				outMessageBuffer[3] = 0x00;
			break;
			case STATE_PRIVATE:
				outMessageBuffer[1] |= OPCODE_PRIVATE;
				outMessageBuffer[3] = 0x01;
			break;
			case STATE_PUBLIC:
				outMessageBuffer[1] |= OPCODE_PRIVATE;
				outMessageBuffer[3] = 0x00;
			break;
			case STATE_ONEWIRE_ENABLED:
				outMessageBuffer[1] |= OPCODE_ONEWIRE;
				outMessageBuffer[3] = 0x01;
			break;
			case STATE_ONEWIRE_DISABLED:
				outMessageBuffer[1] |= OPCODE_ONEWIRE;
				outMessageBuffer[3] = 0x00;
			break;
		}

		outMessageBuffer[4] = HIGHWIRE_END;
		outMessageBuffer[5] = 0;

		//Transmit the whole message
		sendString(outMessageBuffer);
	} else {
		outMessageBuffer[1] |= OPCODE_LOGIN;
		outMessageBuffer[2] = 8;
		outMessageBuffer[3] = 0;

		//Transmit the first part of the message
		sendString(outMessageBuffer);

		//Transmit the payload (iButton ID)
		sendString(id);

		//Transmit the end character
		sendChar(HIGHWIRE_END);
	}
}

void addCharToMessage(unsigned char byte) {
	//Wrap buffer if it overflows
	if(inMessageBufferLength == MAX_MESSAGE_IN_LENGTH)
		inMessageBufferLength = 0;

	//Check for start byte
	if(byte == HIGHWIRE_START) {
		inMessageBuffer[0] = HIGHWIRE_START;
		inMessageBufferLength = 1;
	} else {
		//If not, add letter to end of buffer
		inMessageBuffer[inMessageBufferLength] = byte;
		inMessageBufferLength++;
	}
}

