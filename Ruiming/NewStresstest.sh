#!/bin/bash
# stress.sh — compares sol.cpp against brute.cpp on many random inputs.
# Usage: bash stress.sh            (put your solution in sol.cpp)

set -e   # stop if compilation fails

g++ -O2 -o sol   sol.cpp
g++ -O2 -o brute brute.cpp

for i in $(seq 1 10000); do
    python3 gen.py $i > in.txt     # 1. make a random test (seed = i)
    ./sol   < in.txt > out1.txt    # 2. run your solution
    ./brute < in.txt > out2.txt    # 3. run the brute force on the SAME input

    # 4. compare; cmp -s is silent and returns non-zero if files differ
    if ! cmp -s out1.txt out2.txt; then
        echo "Mismatch on test $i"
        echo "--- input ---";      cat in.txt
        echo "--- sol says ---";   cat out1.txt
        echo "--- brute says ---"; cat out2.txt
        exit 1
    fi
done
echo "All 10000 tests passed"
