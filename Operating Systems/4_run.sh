#!/usr/bin/expect -f

# Script to be used every time to automatically run and login to minix

# Run minix image (copy-on-write image)
spawn bash
send "qemu-system-x86_64 -curses -drive file=/home/marcin/Documents/SO/Minix/minix.img -localtime -net user,hostfwd=tcp::15881-:22 -net nic,model=virtio \r"

sleep 3
send " \r \r"
expect "login: "
sleep 1
send "root\r"
sleep 1
expect "Password:"
sleep 1
send "root\r"

expect "login: "
send "root\r"
expect "Password:"
send "root\r"
send "clear\r"

interact

sleep 1