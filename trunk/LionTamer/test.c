#include <stdio.h>
#include "hiwi.h"

int main() {

    int data;
    hiwi_pkt_ptr pkt = set_lock_state('g');

    char op = get_opcode(pkt);
    char mt = get_message_type(pkt);
    int res = get_data(pkt, &data);

    printf("Opcode is: %d\n", op);
    printf("Message type is: %d\n", mt);
    printf("Data is: %c\n", data);

    return 0;
}
