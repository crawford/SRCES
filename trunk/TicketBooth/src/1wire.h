#define ONEWIRE_PIN 5
#define ONEWIRE_IOC_PIN 4
#define ONEWIRE_POWER_PIN 3
#define ONEWIRE_MASK (1 << ONEWIRE_PIN)
#define ONEWIRE_IOC_MASK (1 << ONEWIRE_IOC_PIN)
#define ONEWIRE_POWER_MASK (1 << ONEWIRE_POWER_PIN)
#define ONEWIRE_HIGH (PORTA & ONEWIRE_MASK)
#define ONEWIRE_LOW (!ONEWIRE_HIGH)
#define ONEWIRE_PULLDOWN() \
	_asm bcf TRISA, ONEWIRE_PIN _endasm; \
	_asm bcf PORTA, ONEWIRE_PIN _endasm
#define ONEWIRE_RELEASE() _asm bsf TRISA, ONEWIRE_PIN _endasm
#define ONEWIRE_POWER_ON() _asm bsf PORTC, ONEWIRE_POWER_PIN _endasm
#define ONEWIRE_POWER_OFF() _asm bcf PORTC, ONEWIRE_POWER_PIN _endasm

extern unsigned char readIButtonID();
extern unsigned char id[9];

