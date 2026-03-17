#!/bin/bash
set -euo pipefail

gcc --coverage gpt2.c -o gpt2_baseline_cov -lm

mkdir -p performance/gcov_runs

for i in {1..10}; do
    echo "gcov run $i"
    ./gpt2_baseline_cov > /dev/null

    # copy coverage files for this run
    cp gpt2_baseline_cov-gpt2.gcda performance/gcov_runs/gpt2_run_${i}.gcda

done

# Generate coverage report using the last run
gcov gpt2.c > performance/baseline_gcov.txt

echo "Baseline gcov profiling complete."
