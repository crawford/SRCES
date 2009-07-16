static int state;

int set_file_descriptor(int fd) {
    static int filedes = -1;
    if (fd == -1) {
        return -1;
    } else {
        filedes = fd;
        return filedes;
    }
}

int set_state(int new_state) {
    // packet stuff
    return 0;
}

int query_state() {
    // packet stuff
    return 0;
}
