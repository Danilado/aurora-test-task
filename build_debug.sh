#!/bin/bash

(
  cd ./src || exit 1
)

cd ./build || exit 1

gcc ../src/*.c \
-Wall -Werror -Wpedantic \
-std=c99 -c \
-Og -g3 \
-fprofile-arcs -ftest-coverage

gcc -lm -fprofile-arcs -lgcov \
*.o -o mypermcheck
