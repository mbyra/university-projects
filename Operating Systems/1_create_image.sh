#!/bin/bash

# To be run when setting up a new Minix environment, or restoring to the original state (downloaded minix.img with no changes at all)

# Create copy-on-write image:
qemu-img create -f qcow2 -o backing_file=/media/marcin/01D1FB1E280FA6801/Minix/minix.img /home/marcin/Documents/SO/Minix/minix.img

sleep 1

./2_new_tab_run.sh

sleep 14

./5_exchange_ssh_key.sh