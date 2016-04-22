#!/bin/sh

# Create static library's object file
gcc -Wall -g -c -o libsabo.o libsabo.c

# Create static library
ar rcs libsabo.a libsabo.o

# Compile demo_use program file.
gcc -Wall -g -c test.c -o test.o

# Create demo_use program; -L. causes "." to be searched during
gcc -g -o test_use test.o -L. -lsabo

# Execute the program.
./test_use
