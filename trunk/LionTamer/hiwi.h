typedef struct {
    unsigned char start;
    unsigned short headers;
    //unsigned char *data;
    unsigned char stop;
} hiwi_packet_s;

hiwi_packet_s query_locked_state();
