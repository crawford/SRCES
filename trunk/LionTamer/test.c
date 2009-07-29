#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "hiwi.h"

int send_pkt(hiwi_pkt_ptr pkt, int sock) {
    if (pkt->data == 0) {
        char pkt_c[4];
        pkt_c[0] = pkt->start;
        pkt_c[1] = (pkt->headers & 0xFF00) >> 8;
        pkt_c[2] = pkt->headers & 0x00FF;
        pkt_c[3] = pkt->stop;

        send(sock, pkt_c, sizeof pkt_c, 0);

        free_hiwi_pkt_ptr(pkt);

        return 0;
    } else if (strlen(pkt->data) > 0) {
        char pkt_c[4 + strlen(pkt->data)];
        pkt_c[0] = pkt->start;
        pkt_c[1] = (pkt->headers & 0xFF00) >> 8;
        pkt_c[2] = pkt->headers & 0x00FF;
        strcpy(pkt_c + 3, pkt->data);
        pkt_c[4 + strlen(pkt->data) - 1] = pkt->stop;

        send(sock, pkt_c, sizeof pkt_c, 0);

        free_hiwi_pkt_ptr(pkt);

        return 0;
    } else {
        char pkt_c[5];
        pkt_c[0] = pkt->start;
        pkt_c[1] = (pkt->headers & 0xFF00) >> 8;
        pkt_c[2] = pkt->headers & 0x00FF;
        printf("%d\n", *(pkt->data));
        pkt_c[3] = *(pkt->data);
        pkt_c[4] = pkt->stop;
        
        send(sock, pkt_c, sizeof pkt_c, 0);

        free_hiwi_pkt_ptr(pkt);

        return 0;
    }

    return -1;
}

int main() {
    int status, sock;
    struct addrinfo hints, *res, *cur;
    hiwi_pkt_ptr pkt;

    pkt = query_locked_state();

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
            perror("socket fail");
            continue;
        }

        if (connect(sock, cur->ai_addr, cur->ai_addrlen) == -1) {
            perror("connect");
            continue;
        }

    }

    send_pkt(pkt,sock);

    freeaddrinfo(res);

    close(sock);

    return 0;
}
