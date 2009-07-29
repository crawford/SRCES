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

/* Frees up the hiwi_pkt_ptr structure */
void free_hiwi_pkt_ptr(hiwi_pkt_ptr pkt) {
    if (pkt->data == 0) {
        free(pkt);
    } else {
        free(pkt->data);
        free(pkt);
    }
}

hiwi_pkt_ptr init_pkt(hiwi_pkt_ptr pkt) {
    pkt->start = HIWI_FRAME_START;
    pkt->stop = HIWI_FRAME_END;

    return pkt;
}

/* This section constructs all the different query packets */
hiwi_pkt_ptr query_locked_state() {
    hiwi_pkt_ptr pkt;

    pkt = malloc(4);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_LOCKED_OPCODE;
    pkt->headers |= HIWI_QUERY_MT;
    pkt->headers |= 0;
    pkt->data = 0;

    return pkt;
}

hiwi_pkt_ptr query_priv_state() {
    hiwi_pkt_ptr pkt;

    pkt = malloc(4);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_PRIVACY_OPCODE;
    pkt->headers |= HIWI_QUERY_MT;
    pkt->headers |= 0;
    pkt->data = 0;

    return pkt;
}

hiwi_pkt_ptr query_1wire_state() {
    hiwi_pkt_ptr pkt;

    pkt = malloc(4);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_1_WIRE_OPCODE;
    pkt->headers |= HIWI_QUERY_MT;
    pkt->headers |= 0;
    pkt->data = 0;

    return pkt;
}

hiwi_pkt_ptr query_failed_logins() {
    hiwi_pkt_ptr pkt;

    pkt = malloc(4);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_FAILED_LOGINS_OPCODE;
    pkt->headers |= HIWI_QUERY_MT;
    pkt->headers |= 0;
    pkt->data = 0;

    return pkt;
}

/* This section constructs all the different command packets */
hiwi_pkt_ptr set_lock_state(char state) {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_LOCKED_OPCODE;
    pkt->headers |= HIWI_COMMAND_MT;
    pkt->data = malloc(1);
    *pkt->data = state;
    pkt->headers |= sizeof pkt->data;

    return pkt;
}

hiwi_pkt_ptr set_priv_state(char state) {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_PRIVACY_OPCODE;
    pkt->headers |= HIWI_COMMAND_MT;
    pkt->data = malloc(1);
    *pkt->data = state;
    pkt->headers |= sizeof pkt->data;

    return pkt;
}

hiwi_pkt_ptr set_1wire_state(char state) {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_1_WIRE_OPCODE;
    pkt->headers |= HIWI_COMMAND_MT;
    pkt->data = malloc(1);
    *pkt->data = state;
    pkt->headers |= sizeof pkt->data;

    return pkt;
}
