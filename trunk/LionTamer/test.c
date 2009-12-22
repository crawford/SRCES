#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "hiwi.h"

int main() {
    int status, sock;
    struct addrinfo hints, *res, *cur;
    hiwi_pkt_ptr pkt;

    pkt = set_lock_state('g');
    //pkt = query_lock_state();

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo("127.0.0.1", "666", &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    for (cur = res; cur != NULL; cur = cur->ai_next) {
        if ((sock = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol)) == -1)
        {
            perror("socket: Could not open socket");
            continue;
        }

        if (connect(sock, cur->ai_addr, cur->ai_addrlen) == -1) {
            perror("connect: Could not connect on socket");
            continue;
        }

    }

    if(send(sock, pkt, sizeof *pkt, 0) != 0) {
        perror("send: Could not send packet");
    }

    freeaddrinfo(res);
    free(pkt);

    close(sock);

    return 0;
}
