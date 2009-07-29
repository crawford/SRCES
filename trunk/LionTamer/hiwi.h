struct hiwi_pkt_s {
    unsigned char start;
    unsigned short headers;
    char *data;
    unsigned char stop;
};

typedef struct hiwi_pkt_s * hiwi_pkt_ptr;

hiwi_pkt_ptr query_locked_state();
void free_hiwi_pkt_ptr(hiwi_pkt_ptr pkt);
