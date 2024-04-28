#!/usr/bin/env bash

set -e

gcc -Wall -Wextra -pedantic -o game *.c -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

