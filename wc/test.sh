#!/bin/bash

OUTPUT=$(./ccwc -c test.txt | tr -s " ")
EXPECT=$(wc -c test.txt | tr -s " ")
if [ "${OUTPUT}" == "${EXPECT}" ]; then
	echo "--- Count bytes OK"
else
	echo "!!! Count bytes FAIL"
	echo "OUTPUT: ${OUTPUT} EXPECT: ${EXPECT}"
fi

OUTPUT=$(./ccwc -l test.txt | tr -s " ")
EXPECT=$(wc -l test.txt | tr -s " ")
if [ "${OUTPUT}" == "${EXPECT}" ]; then
	echo "--- Count lines OK"
else
	echo "!!! Count lines FAIL"
	echo "OUTPUT: ${OUTPUT} EXPECT: ${EXPECT}"
fi

OUTPUT=$(./ccwc -w test.txt | tr -s " ")
EXPECT=$(wc -w test.txt | tr -s " ")
if [ "${OUTPUT}" == "${EXPECT}" ]; then
	echo "--- Count words OK"
else
	echo "!!! Count words FAIL"
	echo "OUTPUT: ${OUTPUT} EXPECT: ${EXPECT}"
fi

OUTPUT=$(./ccwc test.txt | sed 's/^[ \t ]*//;s/[ \t ]*$//' | tr -s " ")
EXPECT=$(wc test.txt | sed 's/^[ \t ]*//;s/[ \t ]*$//' | tr -s " ")
if [ "${OUTPUT}" == "${EXPECT}" ]; then
	echo "--- Default option OK"
else
	echo "!!! Default option FAIL"
	echo "OUTPUT: '${OUTPUT}' EXPECT: '${EXPECT}'"
fi

OUTPUT=$(cat test.txt | ./ccwc -l | sed 's/^[ \t ]*//;s/[ \t ]*$//' | tr -s " ")
EXPECT=$(cat test.txt | wc -l | sed 's/^[ \t ]*//;s/[ \t ]*$//' | tr -s " ")
if [ "${OUTPUT}" == "${EXPECT}" ]; then
	echo "--- Read from pipe OK"
else
	echo "!!! Read from pipe FAIL"
	echo "OUTPUT: '${OUTPUT}' EXPECT: '${EXPECT}'"
fi
OUTPUT=$(cat test.txt | ./ccwc | sed 's/^[ \t ]*//;s/[ \t ]*$//' | tr -s " ")
EXPECT=$(cat test.txt | wc | sed 's/^[ \t ]*//;s/[ \t ]*$//' | tr -s " ")
if [ "${OUTPUT}" == "${EXPECT}" ]; then
	echo "--- Read from pipe OK"
else
	echo "!!! Read from pipe FAIL"
	echo "OUTPUT: '${OUTPUT}' EXPECT: '${EXPECT}'"
fi

OUTPUT=$(./ccwc < test.txt | sed 's/^[ \t ]*//;s/[ \t ]*$//' | tr -s " ")
EXPECT=$( wc < test.txt| sed 's/^[ \t ]*//;s/[ \t ]*$//' | tr -s " ")
if [ "${OUTPUT}" == "${EXPECT}" ]; then
	echo "--- Read from stdin OK"
else
	echo "!!! Read from stdin FAIL"
	echo "OUTPUT: '${OUTPUT}' EXPECT: '${EXPECT}'"
fi


