
#include "config.h"
#include "stdio.h"

#define NPIXEL 256
#define NLINE  256

/**
 *build function
 */
unsigned char build(unsigned int x, unsigned int y, unsigned int step) {
    if (((x >> step & 0x1) && !(y >> step & 0x1)) ||
       (!(x >> step & 0x1) &&  (y >> step & 0x1))) {
        return 0xFF;
    }
    else {
        return 0;
    }
}


/**
 * main function
 */
__attribute__ ((constructor)) void main() {
    unsigned char buf[NPIXEL];
    int n = procid();
    int nprocs = NB_PROCS;

    for (int line = 0; line < NLINE; line += 1) { 
        for (int pixel = 0; pixel < NPIXEL; pixel += 1) {
            buf[pixel] = build(pixel, line, 5);
        }
        if (fb_sync_write( TO BE COMPLETED)) {
            tty_printf(" !!! wrong transfer to frame buffer for line %d\n", line);
        }
        else {
            tty_printf(" - building line %d\n", line);
        }
    }

    tty_printf("\ncycles = %d\n", proctime());
    exit(); 
}

