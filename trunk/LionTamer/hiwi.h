#define DR_UNLOCK 0x00
#define DR_LOCK 0x01
#define DR_PUBLIC 0x00
#define DR_PRIVATE 0x01
#define DR_1WIRE_DISABLE 0x00
#define DR_1WIRE_ENABLE 0x01

struct hiwi_pkt_s {
    unsigned char start;
    unsigned short headers;
    char *data;
    unsigned char stop;
};

typedef struct hiwi_pkt_s * hiwi_pkt_ptr;

hiwi_pkt_ptr query_locked_state();
void free_hiwi_pkt_ptr(hiwi_pkt_ptr pkt);
