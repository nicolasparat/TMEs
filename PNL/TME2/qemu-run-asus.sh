#! /bin/bash

# Fix the paths if necessary
HDA="-drive file=../pnl-tp-2021.img,format=raw"
HDB="-drive file=myHome.img,format=raw"
SHARED="./share"
KERNEL=../linux-6.5.7/arch/x86/boot/bzImage

if [ -z ${KDB} ]; then
    CMDLINE='root=/dev/sda1 rw console=ttyS0 kgdboc=ttyS1 nokaslr'
else
    CMDLINE='root=/dev/sda1 rw console=ttyS0 kgdboc=ttyS1 nokaslr kgdbwait'
fi

FLAGS="--enable-kvm "
VIRTFS+=" --virtfs local,path=${SHARED},mount_tag=share,security_model=passthrough,id=share "

exec qemu-system-x86_64 ${FLAGS} \
     ${HDA} ${HDB} \
     ${VIRTFS} \
     -net user -net nic \
     -serial mon:stdio -serial tcp::1234,server,nowait \
     -boot c -m 1G \
     -kernel "${KERNEL}" -nographic \
     -append "${CMDLINE}"
