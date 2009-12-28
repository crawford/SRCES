#include <stdio.h>
#include "hiwi.h"

int main() {

    int data;
    hiwi_pkt_ptr pkt = set_lock_state(1);

    char op = get_opcode(pkt);
    char mt = get_message_type(pkt);
    char size = get_size(pkt);
    int res = get_data(pkt, &data);

    printf("Opcode is: %d\n", op);
    printf("Message type is: %d\n", mt);
    printf("Size is %d\n", size);
    printf("Data is: %d\n", data);

    return 0;
}
