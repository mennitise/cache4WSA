#!/bin/bash
printf "Compilo\n"

make

for filename in ../test_files/*.mem
do
	echo "Running file " $filename
	echo ""
	./cache echo "$filename"
	echo ""
done