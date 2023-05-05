#!/usr/bin/bash
basename=$(basename "$1")
basename="${basename%.*}"

echo "Compiling : $basename"
gcc  $basename.c utils/*.c -o  output/$basename $(sdl2-config --cflags --libs) -lm -Wall -Wextra