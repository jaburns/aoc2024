#!/usr/bin/env bash
cd "$(dirname "$0")"
set -e

BASE='
    -Wall
    -Werror
    -Wswitch-enum
    -Wno-unused-function
    -Wno-logical-op-parentheses
'

if grep -q '#define DEBUG \+1' src/main.c; then
    EXTRA='
        -g
        -O0
        -fsanitize=undefined,unsigned-integer-overflow
        -fno-omit-frame-pointer
        -Wno-unused-variable
        -Wno-unused-but-set-variable
    '
else
    EXTRA='
        -g
        -O3
    '
fi

mkdir -p bin
clang -std=c11 src/main.c -o bin/aoc2024 $BASE $EXTRA