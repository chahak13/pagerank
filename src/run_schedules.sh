#!/usr/bin/env bash

for schedule in static dynamic guided; do
    echo "Running $schedule"
    OMP_SCHEDULE=$schedule OMP_NUM_THREADS=16 ./pagerank -a 0.8 -d " " -f ../web-Google.txt > outputs/google_$schedule.out
done
