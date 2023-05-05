#!/usr/bin/bash
echo "Prog : $1"
gcc  $1.c Math_utils.c -o  output/$1 $(sdl2-config --cflags --libs) -lm -Wall -Wextra
