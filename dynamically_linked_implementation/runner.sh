#!/bin/sh
gcc test.c
gcc -Wall  -shared -fPIC -o libsabo.so libsabo.c -ldl
LD_PRELOAD=$PWD/libsabo.so ./a.out 
