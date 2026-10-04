#!/bin/bash
# 5 runs per thread count, same binary, same NUM_STEPS
export OMP_DYNAMIC=FALSE
for t in 1 2 4; do
  for r in 1 2 3 4 5; do
    echo "--- threads=$t run=$r ---"
    OMP_NUM_THREADS=$t ./bin/pi_benchmark
    echo "exit=$?"
  done
done
