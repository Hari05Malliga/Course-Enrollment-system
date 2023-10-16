#!/bin/bash

gcc client*.c enable_debug.c -c
gcc client*.o enable_debug.o -o client

./client

rm *.o
