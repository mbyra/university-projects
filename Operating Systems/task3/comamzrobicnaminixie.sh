dd bs=512 count=1 seek=2 if=/dev/c0d0 of=/dev/c0d0
dd bs=446 count=1 if=/root/bootloader of=/dev/c0d0
dd bs=512 count=1 seek=1 skip=1 if=/root/bootloader of=/dev/c0d0

echo 'NAME="$(dd count=1 skip=3 conv=silent if=/dev/c0d0 | strings -n 3)"' >> .profile
echo 'useradd -m -d /home/$NAME $NAME' >> .profile
echo 'su -K $NAME' >> .profile
