#!/bin/bash
set -euo pipefail

# 1) Compile baseline (NO optimization flags)
gcc gpt2.c -o gpt2_baseline -lm

# 2) Warmup run (ignore output)
./gpt2_baseline > /dev/null

# 3) Run 10 times and record elapsed time
echo "run,seconds" > performance/baseline_times.csv

for i in {1..10}; do
    t=$(/usr/bin/time -f "%e" ./gpt2_baseline 2>&1 >/dev/null)
    echo "$i,$t" >> performance/baseline_times.csv
done


echo "Baseline timing done. Results in performance/baseline_times.csv"
