#!/bin/bash
set -euo pipefail

# Go to project root
cd "$(dirname "$0")/.."

echo "Cleaning old coverage files..."
rm -f gpt2_optimized.gcno gpt2_optimized.gcda gpt2_optimized.o

echo "Compiling optimized source for GCOV (no -O3)..."
# Step 1: compile to object file, this creates gpt2_optimized.gcno
gcc --coverage -g -c gpt2_optimized.c -o gpt2_optimized.o

# Step 2: link the object file into the executable
gcc --coverage -g gpt2_optimized.o -o gpt2_opt_cov -lm

echo "Running optimized binary once..."
./gpt2_opt_cov > /dev/null

echo "Generating GCOV report..."
gcov -b gpt2_optimized.c > performance/optimized_gcov.txt

echo "Optimized GCOV profiling complete."
