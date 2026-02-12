#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t read(int fd, void *buf, size_t count)
{
    (void)fd;
    (void)buf;
    (void)count;

    write(1, "Ciao!\n", 6);
    return 'e';
}
