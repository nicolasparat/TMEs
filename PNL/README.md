# Getting started

```
cd /tmp
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.5.7.tar.xz
tar xf linux-6.5.7.tar.xz

cd linux-6.5.7
make defconfig
make -j 64

cp /Vrac/pnl-etu/pnl-tp-2021.img /tmp
```

# Display VM output

```
dmesg
```

# Leave VM

```
CTRL + A puis c puis quit
```

# Starting a debugger

```
gdb /tmp/linux-6.5.7/vmlinux
target remote :1234
```

# Setting a breakpoint in the VM

```
echo "g" > /proc/sysrq-trigger
```

# Adding all available symbols and displaying a backtrace

```
lx_symbols
backtrace
```

# Kernel docs

```
https://elixir.bootlin.com/linux/v6.5.7/source
```