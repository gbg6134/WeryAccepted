#!/bin/bash
# source compile-and-run.sh
export ASAN_OPTIONS=detect_leaks=0
c() { g++ -DLOCAL -Wfatal-errors -Wall -fsanitize=address,undefined -g -O2 -o "$1" -std=gnu++23 -lrt -Wl,--whole-archive -lpthread -Wl,--no-whole-archive "$1.cpp" }
r() { systemd-run -q --scope -p MemoryMax=1024M --use "$1" }
