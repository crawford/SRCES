#ifndef HIWI_H
#define HIWI_H

#define DR_UNLOCK 0x00
#define DR_LOCK 0x01
#define DR_PUBLIC 0x00
#define DR_PRIVATE 0x01
#define DR_1WIRE_DISABLE 0x00
#define DR_1WIRE_ENABLE 0x01

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
hiwi_pkt_ptr set_lock_state(unsigned char state);

#endif
