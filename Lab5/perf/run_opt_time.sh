#!/bin/bash
set -euo pipefail

# Go to project root (directory above this script)
cd "$(dirname "$0")/.."

# Compile optimized binary with -O3, etc.
gcc -std=c99 -O3 -march=native -ffast-math gpt2_optimized.c -o gpt2_opt -lm

mkdir -p performance

# CSV header
echo "run,seconds" > performance/optimized_times.csv

echo "Running optimized timing 10 times..."
for i in {1..10}; do
    # /usr/bin/time writes to stderr; capture it while discarding program output
    t=$(/usr/bin/time -f "%e" ./gpt2_opt 2>&1 >/dev/null)
    echo "$i,$t" >> performance/optimized_times.csv
    echo "run $i: $t s"
done

echo "Optimized timing complete. Results in performance/optimized_times.csv"
