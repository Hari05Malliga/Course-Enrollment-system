#!/bin/bash

gcc server*.c enable_debug.c -c
gcc server*.o enable_debug.o -o server

./server
