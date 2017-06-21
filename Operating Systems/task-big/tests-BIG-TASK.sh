#!/usr/bin/env bash


# Create large empty file (300MB)
test1 () {
    dd if=/dev/zero of=output1  bs=1000000  count=30
}

# Copy large empty file (300MB)
test2 () {
    cp output1 output1copy
}

# Remove large empty files (2 x 300MB)
test3 () {
    rm output1
    rm output1copy
}

# Create 20 small files (10MB) containing zeros.
test4 () {
    COUNTER=1
    while [ ${COUNTER} -lt 11 ]; do
        dd if=/dev/zero of=output4_${COUNTER}  bs=1000000  count=10 >/dev/null 2>&1
        let COUNTER+=1
    done
}

# Copy 20 small files (10MB) containing zeros.
test5 () {
    COUNTER=1
    while [ ${COUNTER} -lt 11 ]; do
        cp output4_${COUNTER} output4_${COUNTER}_copy
        let COUNTER+=1
    done
}

# Remove 20 small files (20MB) containing zeros.
test6 () {
    COUNTER=1
    while [ ${COUNTER} -lt 11 ]; do
        rm output4_${COUNTER}_copy
        let COUNTER+=1
    done
}

# Copy directory with small real files of various types (about 300MB)
test7 () {
    cp -r test7_small_stuff test7_small_stuff_copy
}

# Remove directory with small real files of various types (about 300MB)
test8 () {
    rm -r test7_small_stuff_copy
}

# Copy big file (300MB). (normal file, not with zeros)
test9 () {
    cp big_file big_file_copy
}

# Remove big file (300MB).
test10 () {
    rm big_file_copy
}


if [ $1 ]
then
    cd /usr/
    TEST=1
    TEST_COUNT=10
    echo Running tests...
    until [ ${TEST} -gt ${TEST_COUNT} ]; do
        echo "Test ${TEST}:" &>>/root/$1
        echo Test $TEST running...
        { time test${TEST}; } &>>/root/$1
        echo "done"
        echo &>>$1
        echo &>>$1
        let TEST+=1
    done
    echo Done.
else
    echo "USAGE: $0 logfilename (without path, saved in /root/)"
fi

exit 1