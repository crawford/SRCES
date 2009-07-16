#include "hiwi.h"
#include "serial.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <termio.h>
#include <unistd.h>

// Opens the serial port, and then returns the file descriptor
// Also hands the file descriptor to the library
// Then again, at this point I dunno if I need to
int open_port() {
    int fd;

    fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);

    if (fd == -1) {
        perror("open_port: Unable to open /dev/ttyS0");
    } else {
        printf("Port: totally open!\n");
    }

    // Hand it to the library
    if (set_file_descriptor(fd) != fd) {
        perror("open_port: Unable to pass proper file descriptor to libserial");
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
    int fd;
    static int state;
    fd = open_port();
    configure_port(fd);
    state = query_state();
    return 0;
}
