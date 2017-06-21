#!/bin/bash

if [[ $# != 1 ]]
then
  echo "Usage:"
  echo "$0 student_login"
  exit
fi

login=$1
workdir=../../../../studenci/${login}/zadanie1
result=0
test_count=4

echo === Checking ${login} ===

if [[ -e ${workdir} ]]
then
  cp test_main.c ${workdir}
  pushd ${workdir} >/dev/null
  gcc -Wall -O2 test_main.c stack_show.c -o test_main
  if [[ $? == 0 ]]
  then
    for test in `seq 1 ${test_count}`
    do
      echo Test ${test}
      timeout 10s ./test_main ${test} test_out_correct_${test} >test_out_program_${test}
      if [[ $? == 0 ]]
      then
        diff -iwB test_out_correct_${test} test_out_program_${test}
        if [[ $? == 0 ]]
        then
          echo Test ${test} pass
          (( result++ ))
        else
          echo Test ${test} output error
        fi
      else
        echo Test ${test} program fail
      fi
    done
  fi
  rm -f test*
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
