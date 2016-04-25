#!/bin/bash

test_file="test.c"

function usage {
  echo "./runner.sh -f <unsafe_c_file>"
  echo 'default: -f test.c'
  exit
}

while getopts ":f:o" opt; do                                                                                                                    
  case $opt in                                                                                                                                     
    f)                                                                                                                                             
      test_file=$OPTARG
      ;;
    \?)                                                                                                                                            
      usage                                                                                                                                        
      ;;                                                                                                                                           
  esac                                                                                                                                             
done

# Create output for test file
gcc -o ${test_file}.o $test_file 
chmod +x ${test_file}.o

# Create library file for dynamic linking
gcc -Wall  -shared -fPIC -o libsabo.so libsabo.c -ldl

# Run test code with libsabo library
LD_PRELOAD=$PWD/libsabo.so ./${test_file}.o
