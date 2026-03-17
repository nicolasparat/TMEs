# KERNELDIR_LKP ?= /lib/modules/$(shell uname -r)/build
KERNELDIR_LKP ?= /tmp/linux-6.5.7

obj-m += taskmonitor.o

all:
	make -C $(KERNELDIR_LKP) M=$$PWD modules

clean:
	make -C $(KERNELDIR_LKP) M=$$PWD clean
