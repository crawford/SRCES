#define MAX_MESSAGE_IN_LENGTH 5
#define MAX_MESSAGE_OUT_LENGTH 6

#define STATE_LOCKED 1
#define STATE_UNLOCKED 2
#define STATE_PRIVATE 3
#define STATE_PUBLIC 4
#define STATE_ONEWIRE_ENABLED 5
#define STATE_ONEWIRE_DISABLED 6
#define STATE_BAD_IBUTTON 7

#define OPCODE_LOCKED 0x00
#define OPCODE_PRIVATE 0x01
#define OPCODE_ONEWIRE 0x02
#define OPCODE_LOGIN 0x0F

#define MESSTYPE_RESPONSE 0x00
#define MESSTYPE_COMMAND 0x10
#define MESSTYPE_QUERY 0x20
#define MESSTYPE_BROADCAST 0xF0

#define HIGHWIRE_START 0xBD
#define HIGHWIRE_END 0xDB


extern char checkForMessage();
extern void processMessage();
extern void broadcastState(unsigned char state);
extern void addCharToMessage(unsigned char byte);

extern unsigned char inMessageBuffer[MAX_MESSAGE_IN_LENGTH];
