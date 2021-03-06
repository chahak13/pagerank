#!/usr/bin/env bash

for thread in 1 2 4 8 16 32 56; do
    echo "Running $thread threads"
    OMP_NUM_THREADS=$thread ./pagerank -a 0.8 -d " " -f ../web-Google.txt > outputs/google_$thread.out
done
