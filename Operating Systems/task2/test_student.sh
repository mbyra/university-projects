#!/bin/bash

if [[ $# != 1 ]]
then
  echo "Usage:"
  echo "$0 student_login"
  exit
fi

test_table=(
  '-w 0 0                    1s'
  '-w 2147483647 0           1s'
  '-w 2147483648 0           1s'
  '-w 18000000000000000000 0 1s'
  '-w 1 1000                 3s'
  '-w 2 1000                 3s'
  '-w 7 13                   2s'
  '-w 2000000 7500          10s'
  '-s 3 1000                 3s'
  '-s 5 2222                 5s'
)

login=$1
workdir=../../../../studenci/${login}/zadanie2
result=0
test_number=0
test_count=${#test_table[@]}

echo === Checking ${login} ===

if [[ -e ${workdir} ]]
then
  cp test_main.c ${workdir}
  pushd ${workdir} >/dev/null
  nasm -f elf64 -o dijkstra_semaphore.o dijkstra_semaphore.asm
  nasm -f elf64 -o producer_consumer.o producer_consumer.asm
  gcc -Wall -O2 -lpthread -o test_main test_main.c dijkstra_semaphore.o producer_consumer.o
  if [[ $? == 0 ]]
  then
    ulimit -Sv 131072
    for test_case in "${test_table[@]}"
    do
      (( test_number++ ))
      test_params=(${test_case})
      echo Test ${test_number} \(timeout ${test_params[3]} ./test_main ${test_params[0]} ${test_params[1]} ${test_params[2]}\)
      timeout ${test_params[3]} ./test_main ${test_params[0]} ${test_params[1]} ${test_params[2]}
      if [[ $? == 0 ]]
      then
        echo Test ${test_number} pass
        (( result++ ))
      else
        echo Test ${test_number} fail
      fi
    done
    ulimit -Sv unlimited
  fi
  rm -f test* *.o
  popd >/dev/null
else
  echo Directory ${workdir} not found
fi

if (( result == test_count ))
then
  grade=2
elif (( 2 * result >= test_count ))
then
  grade=1
else
  grade=0
fi

echo Grade ${login}: ${grade}
echo
