#! /bin/bash

# Fix the paths if necessary
HDA="-drive file=/tmp/pnl-tp-2021.img,format=raw"
HDB="-drive file=myHome.img,format=raw"
FLAGS=""

exec qemu-system-x86_64 ${FLAGS} \
     ${HDA} ${HDB} \
     -net user -net nic \
     -boot c -m 1G
