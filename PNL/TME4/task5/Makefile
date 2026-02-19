KERNELDIR_LKP ?= /tmp/linux-6.5.7

obj-m += kcpustat.o

all :
	$(info obj-m : $(obj-m))
	make -C $(KERNELDIR_LKP) M=$$PWD modules

clean:
	make -C $(KERNELDIR_LKP) M=$$PWD clean
