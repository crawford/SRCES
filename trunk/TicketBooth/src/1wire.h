#define ONEWIRE 2
#define ONEWIRE_PULLDOWN \
	_asm bcf TRISA, ONEWIRE _endasm; \
	_asm bcf PORTA, ONEWIRE _endasm;
#define ONEWIRE_RELEASE _asm bsf TRISA, ONEWIRE _endasm;
#define ONEWIRE_INPUTMASK 1 << ONEWIRE
#define IBUTTON_FAMILYCODE 0x02
/*#define WAIT_5US(x) \
	_asm MOVLW x _endasm; \
	_asm MOVWF _delay _endasm; \
	_asm NOP _endasm; \
	_asm NOP _endasm; \
	_asm DECFSZ _delay,1 _endasm; \
	_asm GOTO $-3 _endasm;*/
#define WAIT_5US(x) \
	delay = x; \
	_asm CALL _DELAY_5US _endasm;

extern char readIButtonID();
extern void DELAY_5US();

char crc;
char delay;
char iobyte;
char count;
char id[8];
