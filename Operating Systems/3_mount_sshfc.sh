#!/usr/bin/expect -f

spawn bash
send "sshfs -p 15881 -o nonempty root@localhost:/root/ ./sshfs-path/\r"
sleep 1
expect "root@localhost's password: "
sleep 1
send "root\r"
sleep 1
interact
