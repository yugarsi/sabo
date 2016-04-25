#!/bin/bash

test_file="test.c"
program_args=

function usage {
  echo "./runner.sh -f <unsafe_c_file> -a <program_arguments>"
  echo 'default: -f test.c'
  exit
}

while getopts "f:a:" opt; do
  case $opt in
    f)
      test_file=$OPTARG
      ;;
    a)
      program_args=$OPTARG
      ;;
    \?)
      usage
      ;;
  esac
done

echo "$program_args"

# Create output for test file
gcc $test_file

# Create library file for dynamic linking
gcc -Wall  -shared -fPIC -o libsabo.so libsabo.c -ldl

# Run test code with libsabo library
LD_PRELOAD=$PWD/libsabo.so ./a.out $program_args
