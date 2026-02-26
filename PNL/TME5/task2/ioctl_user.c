#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#include "helloioctl.h"

int main() {
    struct hello_data data;

    int fd = open("/dev/hello", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    int val = ioctl(fd, HELLO, &data);
    if (val < 0) {
        perror("ioctl");
        close(fd);
        return 1;
    }

    printf("%s\n", data.message);
    close(fd);

    return 0;
}