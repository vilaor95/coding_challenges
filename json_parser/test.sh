#!/bin/bash

# Step 1
for file in $(ls tests/step1/valid*); do 
	./ccjp ${file} > /dev/null;
	RC=$?
	if [ ${RC} == 0 ]; then
		echo "--- ${file} parsing OK"
	else
		echo "!!! ${file} parsing FAIL"
	fi
done
for file in $(ls tests/step1/invalid*); do 
	./ccjp ${file} > /dev/null;
	RC=$?
	if [ ${RC} == 1 ]; then
		echo "--- ${file} parsing OK"
	else
		echo "!!! ${file} parsing FAIL"
	fi
done

# Step 2
for file in $(ls tests/step2/valid*); do 
	./ccjp ${file} > /dev/null;
	RC=$?
	if [ ${RC} == 0 ]; then
		echo "--- ${file} parsing OK"
	else
		echo "!!! ${file} parsing FAIL"
	fi
done
for file in $(ls tests/step2/invalid*); do 
	./ccjp ${file} > /dev/null;
	RC=$?
	if [ ${RC} == 1 ]; then
		echo "--- ${file} parsing OK"
	else echo "!!! ${file} parsing FAIL"
	fi
done

# Step 3
for file in $(ls tests/step3/valid*); do 
	./ccjp ${file} > /dev/null;
	RC=$?
	if [ ${RC} == 0 ]; then
		echo "--- ${file} parsing OK"
	else
		echo "!!! ${file} parsing FAIL"
	fi
done
for file in $(ls tests/step3/invalid*); do 
	./ccjp ${file} > /dev/null;
	RC=$?
	if [ ${RC} == 1 ]; then
		echo "--- ${file} parsing OK"
	else
		echo "!!! ${file} parsing FAIL"
	fi
done

# Step 4
for file in $(ls tests/step4/valid*); do 
	./ccjp ${file} > /dev/null;
	RC=$?
	if [ ${RC} == 0 ]; then
		echo "--- ${file} parsing OK"
	else
		echo "!!! ${file} parsing FAIL"
	fi
done
for file in $(ls tests/step4/invalid*); do 
	./ccjp ${file} > /dev/null;
	RC=$?
	if [ ${RC} == 1 ]; then
		echo "--- ${file} parsing OK"
	else
		echo "!!! ${file} parsing FAIL"
	fi
done
