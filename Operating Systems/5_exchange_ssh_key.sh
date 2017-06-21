#!/usr/bin/expect -f

# Use after first run

# send the key from host to qemu
spawn ssh-copy-id -f root@localhost -p 15881
expect "password"
send "root\r"
interact

#generate keys at quemu
spawn ssh root@localhost -p 15881
expect "# "
send "ssh-keygen -f /root/.ssh/id_rsa -t rsa -N ''\r"
expect "# "

# # install nano on the fly
# send "pkgin in nano-2.2.6nb1\r"
# expect "proceed ? \[Y/n\] "
# send "Y\r"
# expect "# "

# #and hexedit
# send "pkgin install hexedit\r"
# expect "proceed ? \[Y/n\] "
# send "Y\r"
# expect "# "

send "exit\r"
interact

#send the key from guest to host
spawn scp -P 15881 root@localhost:/root/.ssh/id_rsa.pub qemu_id_rsa.pub
interact

#add the qemu key to host authorized keys and clean up
spawn bash
send "cat qemu_id_rsa.pub >> ~/.ssh/authorized_keys\r"
interact