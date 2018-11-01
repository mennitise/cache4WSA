#!/bin/bash
printf "Compilo\n"

make

echo ""
echo "Script to evaluate times."
echo "-------------------------"
echo "Runs of the program with different files, all of them" 
echo "with the same amount of instructions but with different"
echo "Miss rates to compare times."
echo ""
echo ""

echo "Running program with a file with Miss Rate = 100%"
echo "-------------------------------------------------"
start=`date +%s`
./cache ../test_files/prueba6.mem
end=`date +%s`
runtime=$((end-start))
echo ""
echo "The execution takes " $runtime " seconds."

echo "Running program with a file with Miss Rate = 50%"
echo "-------------------------------------------------"
start=`date +%s`
./cache ../test_files/prueba7.mem
end=`date +%s`
runtime=$((end-start))
echo ""
echo "The execution takes " $runtime " seconds."
echo ""


echo "Running program with a file with Miss Rate = 10%"
echo "-------------------------------------------------"
start=`date +%s`
./cache ../test_files/prueba8.mem
end=`date +%s`
runtime=$((end-start))
echo ""
echo "The execution takes " $runtime " seconds."
echo ""