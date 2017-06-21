dd bs=512 count=1 if=/dev/c0d0 of=/dev/c0d0p0s2 #kopiuje oryginalny mbr z sektora 0 do 2
dd bs=466 count=1 if=/root/bootloader of=/dev/c0d0 #na sektor 0 (i zarazem 1) kopiuje swoj 2-czesciowy kod