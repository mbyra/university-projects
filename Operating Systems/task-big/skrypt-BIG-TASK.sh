#!/bin/bash

cp read.c /usr/src/minix/fs/mfs/
cd /usr/src/minix/fs/mfs/
make && make install

cd /usr/src/releasetools/
make do-hdboot

reboot