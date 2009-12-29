#ifndef HIWI_H
#define HIWI_H

#define DR_UNLOCK 0x00
#define DR_LOCK 0x01
#define DR_PUBLIC 0x00
#define DR_PRIVATE 0x01
#define DR_1WIRE_DISABLE 0x00
#define DR_1WIRE_ENABLE 0x01
#define HIWI_LOCKED_OPCODE 0x00
#define HIWI_PRIVACY_OPCODE 0x01
#define HIWI_1_WIRE_OPCODE 0x02
#define HIWI_FAILED_LOGINS_OPCODE 0x03
#define HIWI_INVALID_LOGIN_OPCODE 0x04
#define HIWI_RESPONSE_MT 0x0
#define HIWI_COMMAND_MT 0x1
#define HIWI_QUERY_MT 0x2
#define HIWI_BROADCAST_MT 0xF

struct hiwi_pkt_s {
    unsigned char start;
    unsigned char headers;
    unsigned char size;
    unsigned char data;
    unsigned char stop;
};

typedef struct hiwi_pkt_s * hiwi_pkt_ptr;

void free_hiwi_pkt_ptr(hiwi_pkt_ptr pkt);

hiwi_pkt_ptr query_lock_state();
hiwi_pkt_ptr query_priv_state();
hiwi_pkt_ptr query_1wire_state();
hiwi_pkt_ptr query_failed_logins();
hiwi_pkt_ptr set_lock_state(unsigned char state);
hiwi_pkt_ptr set_priv_state(unsigned char state);
hiwi_pkt_ptr set_1wire_state(unsigned char state);

hiwi_pkt_ptr gen_response(unsigned char opcode, unsigned char response);

char get_opcode(hiwi_pkt_ptr pkt);
char get_message_type(hiwi_pkt_ptr pkt);
char get_size(hiwi_pkt_ptr pkt);
int get_data(hiwi_pkt_ptr pkt, int* data);

#endif
