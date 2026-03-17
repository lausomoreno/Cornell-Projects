#!/bin/bash
set -euo pipefail

# Go to project root (directory above this script)
cd "$(dirname "$0")/.."

# Compile baseline binary (no profiling flags)
gcc gpt2.c -o gpt2_baseline -lm

mkdir -p performance

echo "Running Cachegrind once on baseline..."
valgrind --tool=cachegrind \
         --branch-sim=yes \
         --cache-sim=yes \
         --cachegrind-out-file=performance/baseline_cachegrind.out \
         ./gpt2_baseline > /dev/null 2>&1


echo "Annotating Cachegrind output..."
cg_annotate performance/baseline_cachegrind.out \
    > performance/baseline_cachegrind.txt

echo "Baseline Cachegrind profiling complete."
