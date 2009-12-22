/* This library takes care of packaging all the data for transmission. It does
 * not handle the actual transmission, just the logic of packaging. The idea is
 * so that if a different medium needs to be used, this library can still be
 * used for the hiwi protocol.
 */
#include "hiwi.h"
#include <stdlib.h>

#define HIWI_FRAME_START 0xBD
#define HIWI_FRAME_END 0xDB
#define HIWI_RESPONSE_MT 0x00
#define HIWI_COMMAND_MT 0x10
#define HIWI_QUERY_MT 0x20
#define HIWI_BROADCAST_MT 0xF0
#define HIWI_LOCKED_OPCODE 0x00
#define HIWI_PRIVACY_OPCODE 0x01
#define HIWI_1_WIRE_OPCODE 0x02
#define HIWI_FAILED_LOGINS_OPCODE 0x03
#define HIWI_INVALID_LOGIN_OPCODE 0x04

/* Frees up the hiwi_pkt_ptr structure */
void free_hiwi_pkt_ptr(hiwi_pkt_ptr pkt) {
    free(pkt);
}

hiwi_pkt_ptr init_pkt(hiwi_pkt_ptr pkt) {
    pkt->start = HIWI_FRAME_START;
    pkt->headers = 0;
    pkt->stop = HIWI_FRAME_END;

    return pkt;
}

/* This section constructs all the different query packets */
hiwi_pkt_ptr query_lock_state() {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_LOCKED_OPCODE;
    pkt->headers |= HIWI_QUERY_MT;
    pkt->size = 0;
    pkt->data = 0;

    return pkt;
}

hiwi_pkt_ptr query_priv_state() {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_PRIVACY_OPCODE;
    pkt->headers |= HIWI_QUERY_MT;
    pkt->size = 0;
    pkt->data = 0;

    return pkt;
}

hiwi_pkt_ptr query_1wire_state() {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_1_WIRE_OPCODE;
    pkt->headers |= HIWI_QUERY_MT;
    pkt->size = 0;
    pkt->data = 0;

    return pkt;
}

hiwi_pkt_ptr query_failed_logins() {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_FAILED_LOGINS_OPCODE;
    pkt->headers |= HIWI_QUERY_MT;
    pkt->size = 0;
    pkt->data = 0;

    return pkt;
}

/* This section constructs all the different command packets */
hiwi_pkt_ptr set_lock_state(unsigned char state) {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_LOCKED_OPCODE;
    pkt->headers |= HIWI_COMMAND_MT;
    pkt->size = 1;
    pkt->data = state;

    return pkt;
}

hiwi_pkt_ptr set_priv_state(unsigned char state) {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_PRIVACY_OPCODE;
    pkt->headers |= HIWI_COMMAND_MT;
    pkt->size = 1;
    pkt->data = state;

    return pkt;
}

hiwi_pkt_ptr set_1wire_state(unsigned char state) {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= HIWI_1_WIRE_OPCODE;
    pkt->headers |= HIWI_COMMAND_MT;
    pkt->size = 1;
    pkt->data = state;

    return pkt;
}

/* Generic reponse */
hiwi_pkt_ptr gen_response(unsigned char opcode, unsigned char response) {
    hiwi_pkt_ptr pkt;

    pkt = malloc(5);
    pkt = init_pkt(pkt);

    pkt->headers |= opcode;
    pkt->headers |= HIWI_RESPONSE_MT;
    pkt->size = 1;
    pkt->data = response;

    return pkt;
}

/* Packet tools */
char get_opcode(hiwi_pkt_ptr pkt) {
    char headers = pkt->headers;

    headers &= 0x01;
    return headers;
}

char get_message_type(hiwi_pkt_ptr pkt) {
    char headers = pkt->headers;

    headers >>= 4;
    return headers;
}

/* returns 1 if no data, 0 if successful
   sets data with packet data */
int get_data(hiwi_pkt_ptr pkt, int* data) {
    if (pkt->size == 0) {
        *data = 255;

        return 1;
    } else {
        /* As long as we're only sending small amounts
        of data... */
        *data = pkt->data;

        return 0;
    }
}
