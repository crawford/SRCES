#define WAIT_5US(x) \
	delay = x; \
	_asm CALL _DELAY_5US _endasm;

extern void DELAY_5US();
extern void delay_ms(unsigned char);
extern void delay_100us(unsigned char);
extern unsigned char delay;

