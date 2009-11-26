/* This handles sending of data over the serial port to TicketBooth. It uses
 * libhiwi to package and process all the data, and then sends it over the
 * serial port. It communicates with network.c by opening another socket and
 * receiving and sending instruction through it
 */
#include "hiwi.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termio.h>
#include <unistd.h>

#define LT_SERIAL_PORT "/dev/ttyS0"

// Opens the serial port, and then returns the file descriptor
int open_serial_port() {
    int fd;

    fd = open(LT_SERIAL_PORT, O_RDWR | O_NOCTTY);

    if (fd == -1) {
        perror("open_port: Unable to open /dev/ttyS0");
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

int main() {
    int serial_fd;
    serial_fd = open_serial_port();
    configure_port(serial_fd);

    while (1) {
        // Listen loop
    }

    return 0;
}
