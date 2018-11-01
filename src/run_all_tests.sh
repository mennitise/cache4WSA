#!/bin/bash
printf "Compilo\n"

make

filename="../test_files/prueba1.mem"

response='255\s254\s248\s96\s192\s255\s254\s248\s192\s88'
echo "Running with file " $filename
echo "---------------------------------"
if ./cache echo "$filename" | grep -z $response ; then
	echo "		OK"
else
	echo "		ERROR TEST!"
fi

echo ""
echo ""

filename="../test_files/prueba2.mem"

response='0\s0\s10\s10\s20\s20\s33'
echo "Running with file " $filename
echo "---------------------------------"
if ./cache echo "$filename" | grep -z $response ; then
	echo "		OK"
else
	echo "		ERROR TEST!"
fi

echo ""
echo ""

filename="../test_files/prueba3.mem"

response='1\s2\s3\s4\s0\s0\s0\s0\s1\s2\s3\s4\s50'
echo "Running with file " $filename
echo "---------------------------------"
if ./cache echo "$filename" | grep -z $response ; then
	echo "		OK"
else
	echo "		ERROR TEST!"
fi

echo ""
echo ""

filename="../test_files/prueba4.mem"

echo "Running with file " $filename
echo "---------------------------------"
if ./cache echo "$filename" | grep -z 'ERROR: The specified value is more bigger that a byte' ; then
	echo "		OK"
else
	echo "		ERROR TEST!"
fi

echo ""
echo ""

filename="../test_files/prueba5.mem"

echo "Running with file " $filename
echo "---------------------------------"
if ./cache echo "$filename" | grep -z 'ERROR: The specified address is very large. The address limit is: 65536' ; then
	echo "		OK"
else
	echo "		ERROR TEST!"
fi

echo ""
echo ""
