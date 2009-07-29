/* This library takes care of packaging all the data for transmission. It does
 * not handle the actual transmission, just the logic of packaging. The idea is
 * so that if a different medium needs to be used, this library can still be
 * used for the hiwi protocol.
 */
#include <string.h>

#define HIWI_FRAME_START 0xBD
#define HIWI_FRAME_END 0xDB
#define HIWI_RESPONSE_MT 0x000
#define HIWI_COMMAND_MT 0x100
#define HIWI_QUERY_MT 0x200
#define HIWI_BROADCAST_MT 0xF00
#define HIWI_LOCKED_OPCODE 0x000
#define HIWI_PRIVACY_OPCODE 0x010
#define HIWI_1_WIRE_OPCODE 0x020
#define HIWI_FAILED_LOGINS_OPCODE 0x030
#define HIWI_INVALID_LOGIN_OPCODE 0x040

// struct for the entire frame, data and all
struct hiwi_pkt_s {
    unsigned char start;
    unsigned int headers;
    unsigned char *data;
    unsigned char stop;
};

static int state;

char *set_state(int new_state) {
    return 0;
}

char *query_locked_state() {
    struct hiwi_pkt_s pkt;
    char *pkt_c;

    memset(&pkt, 0, sizeof pkt);

    pkt.start = HIWI_FRAME_START;
    pkt.stop = HIWI_FRAME_END;
    pkt.headers |= HIWI_LOCKED_OPCODE;
    pkt.headers |= HIWI_QUERY_MT;

    pkt_c = &pkt;
    return pkt_c;
}

int query_size() {
    //union packet_u teh_packet;
    //return sizeof pkt;
    return 0;
}
