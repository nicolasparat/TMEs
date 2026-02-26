#ifndef HELLOIOCTL_H
#define HELLOIOCTL_H

struct hello_data {
    char message[32];
};

#define HELLO _IOR('N', 1, struct hello_data)

#endif