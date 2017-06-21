#!/bin/bash

# skopiowanie plików do utworzenia funkcji na serwerze pm
cp callnr.h /usr/include/minix/
cp callnr.h /usr/src/minix/include/minix/
cp proto.h /usr/src/minix/servers/pm/
cp myps.c /usr/src/minix/servers/pm
cp table.c /usr/src/minix/servers/pm
cp Makefile /usr/src/minix/servers/pm

cwd=$(pwd) # obecna sciezka

# kompilacja serwera pm
cd /usr/src/minix/servers/pm
make
make install
cd /usr/src/releasetools
make hdboot


cd $cwd/libc # powrot do poprzedniego folderu

# skopiowanie plików do utowrzenia funkcji bibliotecznej
cp myps.c /usr/src/lib/libc/misc
cp Makefile.inc /usr/src/lib/libc/misc

cp unistd.h /usr/include
cp unistd.h /usr/src/include/

# kompilacja libc
cd /usr/src/lib/libc/
make
make install


reboot