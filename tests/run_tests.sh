#!/bin/bash
cd ../tests

testNumbers=$(ls -alch . | grep " input\..*" -o | grep "[0-9][0-9]*" -o)

cd ../src
cmake . && make -j32

for index in $testNumbers
do
        echo -e "\e[1mRunning test $index\e[0m"
        rm /tmp/testoutput
        ./history < ../tests/input.$index > /tmp/testoutput && diff ../tests/output.$index /tmp/testoutput && echo -e "\e[32mTest $index Passed\e[0m" || echo -e "\e[31mTest $index failed\e[0m"
done


cd ../tests
