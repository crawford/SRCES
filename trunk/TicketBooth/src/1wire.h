#define ONEWIRE_PIN 2
#define ONEWIRE_PULLDOWN() \
	_asm bcf TRISA, ONEWIRE_PIN _endasm; \
	_asm bcf PORTA, ONEWIRE_PIN _endasm
#define ONEWIRE_RELEASE() _asm bsf TRISA, ONEWIRE_PIN _endasm
#define ONEWIRE_MASK 1 << ONEWIRE_PIN
#define IBUTTON_FAMILYCODE 0x02

extern unsigned char readIButtonID();
extern unsigned char id[8];

