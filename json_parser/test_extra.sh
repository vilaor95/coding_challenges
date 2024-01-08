#!/bin/bash

for file in $(ls test/pass*); do 
	./ccjp ${file} > /dev/null;
	RC=$?
	if [ ${RC} == 0 ]; then
		echo "--- ${file} parsing OK"
	else
		echo "!!! ${file} parsing FAIL"
	fi
done
for file in $(ls test/fail*); do 
	./ccjp ${file} > /dev/null;
	RC=$?
	if [ ${RC} == 1 ]; then
		echo "--- ${file} parsing OK"
	else
		echo "!!! ${file} parsing FAIL"
	fi
done


