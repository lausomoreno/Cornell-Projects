#!/bin/bash
set -euo pipefail

# Go to project root (directory above this script)
cd "$(dirname "$0")/.."

# Compile optimized binary with strong compiler flags
gcc -std=c99 -O3 -march=native -ffast-math gpt2_optimized.c -o gpt2_opt -lm

mkdir -p performance

echo "Running Cachegrind once on optimized..."
valgrind --tool=cachegrind \
         --cache-sim=yes \
         --branch-sim=yes \
         --cachegrind-out-file=performance/optimized_cachegrind.out \
         ./gpt2_opt > /dev/null 2>&1

echo "Annotating Cachegrind output..."
cg_annotate --show=Ir,I1mr,D1mr,ILmr,DLmr,Dw,D1mw,DLmw performance/optimized_cachegrind.out \
    > performance/optimized_cachegrind.txt

echo "Optimized Cachegrind profiling complete."
