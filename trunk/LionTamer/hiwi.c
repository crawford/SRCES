/* This library takes care of packaging all the data for transmission. It does
 * not handle the actual transmission, just the logic of packaging. The idea is
 * so that if a different medium needs to be used, this library can still be
 * used for the hiwi protocol.
 */
#include "hiwi.h"
#include <stdlib.h>
#define HIWI_FRAME_START 0xBD
#define HIWI_FRAME_END 0xDB
#define HIWI_RESPONSE_MT 0x0000
#define HIWI_COMMAND_MT 0x1000
#define HIWI_QUERY_MT 0x2000
#define HIWI_BROADCAST_MT 0xF000
#define HIWI_LOCKED_OPCODE 0x0000
#define HIWI_PRIVACY_OPCODE 0x0100
#define HIWI_1_WIRE_OPCODE 0x0200
#define HIWI_FAILED_LOGINS_OPCODE 0x0300
#define HIWI_INVALID_LOGIN_OPCODE 0x0400

void free_hiwi_pkt_ptr(hiwi_pkt_ptr pkt) {
    if (pkt->data == 0) {
        free(pkt);
    } else {
        free(pkt->data);
        free(pkt);
    }
}

hiwi_pkt_ptr query_locked_state() {
    hiwi_pkt_ptr pkt;

    pkt = malloc(4);

    pkt->start = HIWI_FRAME_START;
    pkt->stop = HIWI_FRAME_END;
    pkt->headers |= HIWI_LOCKED_OPCODE;
    pkt->headers |= HIWI_QUERY_MT;
    pkt->headers |= 0;
    pkt->data = 0;

    return pkt;
}
