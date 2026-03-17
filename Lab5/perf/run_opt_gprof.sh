#!/bin/bash
set -euo pipefail

# Go to project root (directory above this script)
cd "$(dirname "$0")/.."

# Compile optimized binary with profiling + -O3
gcc -std=c99 -O3 -march=native -ffast-math -pg gpt2_optimized.c -o gpt2_opt_gprof -lm

mkdir -p performance/gprof_runs_opt

echo "Running optimized binary 10 times for gprof..."
for i in {1..10}; do
    echo "gprof opt run $i"
    ./gpt2_opt_gprof > /dev/null
    mv gmon.out "performance/gprof_runs_opt/gmon_opt_run_${i}.out"
done

# Use the last run's gmon.out for the main report
echo "Generating optimized gprof report..."
gprof gpt2_opt_gprof performance/gprof_runs_opt/gmon_opt_run_10.out \
    > performance/optimized_gprof.txt

echo "Optimized gprof profiling complete. See performance/optimized_gprof.txt"
