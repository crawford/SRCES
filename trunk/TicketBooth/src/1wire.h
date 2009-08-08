#define ONEWIRE 2
#define ONEWIRE_PULLDOWN \
	_asm bcf TRISA, ONEWIRE _endasm; \
	_asm bcf PORTA, ONEWIRE _endasm;
#define ONEWIRE_RELEASE _asm bsf TRISA, ONEWIRE _endasm;
#define ONEWIRE_INPUTMASK 1 << ONEWIRE
#define IBUTTON_FAMILYCODE 0x02

extern char readIButtonID();
extern char id[8];

