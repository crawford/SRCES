/* This facilitates communication between ticketbooth and centerring
 * by passing and interpretting packets between them. It encapsulates
 * and de-encapsulates packets along the way, sending logical responses
 */
#include "hiwi.h"

#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <termio.h>
#include <time.h>
#include <unistd.h>

#define LT_LISTEN_PORT "6969"
#define LT_MAX_CONNECTIONS 10
#define LT_SERIAL_PORT "/dev/ttyS0"
#define LT_STATE_TIMEOUT 60

static char response_flag = 0;
int lock_state, priv_state, wire_state;
time_t lock_time, priv_time, wire_time;

// Opens the serial port, and then returns the file descriptor
int open_serial_port() {
    int fd;

    // Also, this is really easy, davids
    fd = open(LT_SERIAL_PORT, O_RDWR | O_NOCTTY);

    if (fd == -1) {
        perror("open()");
    }

    return fd;
}

void configure_port(int fd) {
    struct termios port_settings;

    // 9600-8-N-1
    cfsetispeed(&port_settings, B9600);
    cfsetospeed(&port_settings, B9600);

    port_settings.c_cflag &= ~PARENB;
    port_settings.c_cflag &= ~CSTOPB;
    port_settings.c_cflag &= ~CSIZE;
    port_settings.c_cflag |= CS8;
}

int open_network_port() {
    int status, sock;
    struct addrinfo hints, *res, *cur;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo("127.0.0.1", LT_LISTEN_PORT, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    for (cur = res; cur != NULL; cur = cur->ai_next) {
        if ((sock = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol)) == -1) {
            perror("socket()");
            continue;
        }

        if (bind(sock, cur->ai_addr, cur->ai_addrlen) == -1) {
            perror("bind()");
            continue;
        }

        if (listen(sock, LT_MAX_CONNECTIONS) == -1) {
            perror("listen()");
            continue;
        }

    }

    freeaddrinfo(res);

    return sock;
}

// Update the state variables
void update_states(const hiwi_pkt_ptr pkt) {
    char op = get_opcode(pkt);
    int res;
    size_t size = get_size(pkt);

    if (size != 0) {
        switch (op) {
            case HIWI_LOCKED_OPCODE:
                res = get_data(pkt, &lock_state);
                if (res == 0) {
                    time(&lock_time);
                }
                break;
            case HIWI_PRIVACY_OPCODE:
                res = get_data(pkt, &priv_state);
                if (res == 0) {
                    time(&priv_time);
                }
                break;
            case HIWI_1_WIRE_OPCODE:
                res = get_data(pkt, &wire_state);
                if (res == 0) {
                    time(&wire_time);
                }
                break;
        }
    }
}

void send_hiwi_pkt(const hiwi_pkt_ptr pkt, int out_interface) {
    size_t sent, size;
    do {
        size = sizeof *pkt;
        sent = send(out_interface, pkt, size, 0);
    } while (sent != size);
}

// Response handler - forward if needed
int response_handler(const hiwi_pkt_ptr pkt, int out_interface) {
    // Update states in both cases, only send if response needed
    if (response_flag) {
        send_hiwi_pkt(pkt, out_interface);
        response_flag = 0;
    }

    update_states(pkt);

    return EXIT_SUCCESS;
}

// Command handler - always forward
int command_handler(const hiwi_pkt_ptr pkt, int out_interface) {
    // We're gonna need to forward a response
    response_flag = 1;

    send_hiwi_pkt(pkt, out_interface);
    return EXIT_SUCCESS;
}

// Query handler - forward if data is stale
int query_handler(const hiwi_pkt_ptr pkt, int in_interface, int out_interface) {
    char op = get_opcode(pkt);
    int res;
    time_t cur_time;

    res = time(&cur_time);
    if (res != 0) {
        perror("query_handler()");
        return EXIT_FAILURE;
    }

    switch (op) {
        case HIWI_LOCKED_OPCODE:
            if (cur_time - lock_time > LT_STATE_TIMEOUT) {
                send_hiwi_pkt(pkt, out_interface);
                response_flag = 1;
            } else {
                hiwi_pkt_ptr new_pkt = gen_response(HIWI_LOCKED_OPCODE,
                lock_state);
                send_hiwi_pkt(new_pkt, in_interface);
            }
        case HIWI_PRIVACY_OPCODE:
            if (cur_time - priv_time > LT_STATE_TIMEOUT) {
                send_hiwi_pkt(pkt, out_interface);
                response_flag = 1;
            } else {
                hiwi_pkt_ptr new_pkt = gen_response(HIWI_PRIVACY_OPCODE,
                priv_state);
                send_hiwi_pkt(new_pkt, in_interface);
            }
        case HIWI_1_WIRE_OPCODE:
            if (cur_time - wire_time > LT_STATE_TIMEOUT) {
                send_hiwi_pkt(pkt, out_interface);
                response_flag = 1;
            } else {
                hiwi_pkt_ptr new_pkt = gen_response(HIWI_1_WIRE_OPCODE,
                wire_state);
                send_hiwi_pkt(new_pkt, in_interface);
            }
    }

    return EXIT_SUCCESS;
}

// Broadcast handler - forward all broadcast packets on the opposite iface
int broadcast_handler(const hiwi_pkt_ptr pkt, int out_interface) {
    send_hiwi_pkt(pkt, out_interface);

    update_states(pkt);

    return EXIT_SUCCESS;
}

int handle_pkts(const hiwi_pkt_ptr pkt, int in_interface, int out_interface) {
    char mt = get_message_type(pkt);

    switch (mt) {
        case HIWI_RESPONSE_MT:
            return response_handler(pkt, out_interface);
        case HIWI_COMMAND_MT:
            return command_handler(pkt, out_interface);
        case HIWI_QUERY_MT:
            return query_handler(pkt, in_interface, out_interface);
        case HIWI_BROADCAST_MT:
            return broadcast_handler(pkt, out_interface);
    }

    return EXIT_FAILURE;
}

// Let's fire off some packets to initialize my state variables
void init_states(int serial_fd) {
    int sent, size, i;
    hiwi_pkt_ptr pkts[3];

    pkts[0] = query_lock_state;
    pkts[1] = query_priv_state;
    pkts[2] = query_1wire_state;

    for (i = 0; i < 3; i++) {
        size = sizeof pkts[i];
        do {
            sent = send(serial_fd, pkts[i], size, 0);
        } while (sent != size);
    }
}

int main() {
    fd_set fds;
    int fork_err, network_fd, select_err, serial_fd;
    size_t recv_size;
    struct timeval timeout;
    void *buf;

    serial_fd = open_serial_port();
    configure_port(serial_fd);

    network_fd = open_network_port();

    if (serial_fd == -1 || network_fd == -1) {
        return EXIT_FAILURE;
    }

    init_states(serial_fd);
    while (1) {
        FD_ZERO(&fds);
        FD_SET(serial_fd, &fds);
        FD_SET(network_fd, &fds);

        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        // This watches multiple sockets for read data
        select_err = select((serial_fd > network_fd ? serial_fd + 1 : network_fd + 1),
            &fds, NULL, NULL, &timeout);

        // Let's break on error
        if (select_err == -1) {
            perror("select()");
            break;
        }

        // Check to see if the serial fd is ready to read
        if (FD_ISSET(serial_fd, &fds)) {
            fork_err = fork();
            if (fork_err == 0) {
                recv_size = read(serial_fd, buf, 5);
                if (recv_size == -1) {
                    perror("recv()");
                } else {
                    hiwi_pkt_ptr pkt = buf;
                    handle_pkts(pkt, serial_fd, network_fd);
                }

                return EXIT_SUCCESS;
            } else if (fork_err == -1) {
                perror("fork()");
            }
        }

        // Check to see if the socket is ready to read
        if (FD_ISSET(network_fd, &fds)) {
            fork_err = fork();
            if (fork_err == 0) {
                if(accept(network_fd, NULL, 0) == -1) {
                    perror("accept()");
                } else {
                    recv_size = recv(network_fd, buf, 5, 0);
                    if (recv_size == -1) {
                        perror("recv()");
                    } else {
                        hiwi_pkt_ptr pkt = buf;
                        handle_pkts(pkt, network_fd, serial_fd);
                    }
                }

                return EXIT_SUCCESS;
            } else if (fork_err == -1) {
                perror("fork()");
            }
        }
    }

    return EXIT_SUCCESS;
}
