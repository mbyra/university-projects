#!/bin/sh

# Skrypt uruchamiany pod MINIX-em przeprowadzający testy zadania 6

pass=0
tests=6

cd /usr/src/minix/drivers/examples
mkdir -p helloN
cd helloN
cp /root/test_helloN_default.h helloN.h
cp /root/helloN.c .
cp /root/Makefile .
make clean
make
if [ -x helloN ]
then
  make install
  cd
  service up /service/helloN -dev /dev/helloN
  make test_helloN
  if [ -x test_helloN ]
  then
    for t in `seq 1 $tests`
    do
      if [ $t -eq $tests ]
      then
        ./test_helloN 0
        cd /usr/src/minix/drivers/examples/helloN
        make clean
        make
        make install
        cd
        service update /service/helloN
      fi
      ./test_helloN $t
      if [ $? -eq 0 ]
      then
        pass=`expr $pass + 1`
      fi
    done
  fi
fi

echo -n "$pass/$tests TESTS PASS, GRADE "
if [ $pass -eq $tests ]
then
  echo 2
elif [ `expr 2 "*" $pass` -ge $tests ]
then
  echo 1
else
  echo 0
fi
