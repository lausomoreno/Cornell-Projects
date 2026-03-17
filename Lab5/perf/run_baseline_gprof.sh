#!/bin/bash
set -euo pipefail

# Compile with -pg instrumentation
gcc -pg gpt2.c -o gpt2_baseline_gprof -lm

mkdir -p performance/gprof_runs

for i in {1..10}; do
    echo "gprof run $i"
    ./gpt2_baseline_gprof > /dev/null
    mv gmon.out performance/gprof_runs/gmon_$i.out
done

# Create combined report (optional)
echo "Generating combined gprof report..."
gprof gpt2_baseline_gprof performance/gprof_runs/gmon_*.out \
    > performance/baseline_gprof.txt

echo "Baseline gprof profiling complete."
