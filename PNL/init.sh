#!/bin/bash

# Step 1: Navigate to /tmp directory
cd /tmp

# Step 2: Download the Linux kernel tarball
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.5.7.tar.xz

# Step 3: Extract the tarball
tar xf linux-6.5.7.tar.xz

# Step 4: Change into the extracted directory
cd linux-6.5.7

# Step 5: Configure the kernel with default settings
make defconfig

# Step 6: Compile the kernel using 64 threads
make -j 64

# Step 7: Copy the specified image to /tmp
cp /Vrac/pnl-etu/pnl-tp-2021.img /tmp

# Back to assignment directory
cd ~/M1/TMEs/PNL