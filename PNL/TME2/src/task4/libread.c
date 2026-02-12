#define _GNU_SOURCE
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>

typedef ssize_t (*read_t)(int, void *, size_t);

ssize_t read(int fd, void *buf, size_t count)
{
    static read_t real_read = NULL;

    if (!real_read) {
        real_read = (read_t)dlsym(RTLD_NEXT, "read");
    }

    ssize_t ret = real_read(fd, buf, count);

    if (ret > 0) {
        char *c = (char *)buf;
        if (*c == 'r') {
            *c = 'i';
        }
    }

    return ret;
}

