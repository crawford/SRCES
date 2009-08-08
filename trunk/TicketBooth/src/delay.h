#define WAIT_5US(x) \
	delay = x; \
	_asm CALL _DELAY_5US _endasm;

extern void DELAY_5US();
extern char delay;

