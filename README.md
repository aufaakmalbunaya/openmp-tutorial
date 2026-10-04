# Tutorial OpenMP

Hands-on OpenMP parallel programming in C on Linux — 8 exercises plus an
optional mini-project (parallel matrix multiplication), with a full lab
report in Indonesian.

All programs were compiled with GCC 13.3.0 (`-std=c11 -O2 -Wall -Wextra
-fopenmp`) and actually run on a 2-vCPU Linux VM. `evidence/` holds the raw
terminal outputs, `screenshots/` the visual evidence, and `plots/` the
benchmark charts.

## Layout

| Path | Contents |
|---|---|
| `src/` | 14 C programs: 8 tutorial exercises, requested modifications, mini-project |
| `Makefile` | builds everything into `bin/` with one `make` |
| `evidence/` | raw terminal outputs (`.txt`) and the benchmark summary (`.csv`) |
| `screenshots/` | 10 terminal screenshots documenting real runs |
| `plots/` | runtime vs thread count and speedup vs thread count (Exercise 8) |
| `latex_report/` | Indonesian lab report (`.tex` + compiled `.pdf`) |
| `tugas_openmp_Aufa_Akmal_Bunaya_overleaf.zip` | Overleaf submission bundle |

## Quick start

```bash
make
export OMP_DYNAMIC=FALSE
OMP_NUM_THREADS=4 ./bin/hello_openmp
OMP_NUM_THREADS=2 ./bin/pi_benchmark
OMP_NUM_THREADS=2 ./bin/matmul 256
```

## Key results

- **Exercise 8** (π via midpoint rule, n=10⁸, median of 5 runs): 2 threads →
  speedup 1.53 (efficiency 0.76); 4 threads → speedup 1.68 (efficiency 0.42,
  oversubscription on 2 vCPUs). Serial–parallel difference 7.4e-13 < 1e-8.
- **Mini-project** (dense 256×256 matmul): parallel result identical to serial
  (diff 0.0 < 1e-9); speedup 2.00 on 2 threads; `collapse(2)` variant included.
- **Exercise 4**: `reduction` ≈ 170× faster than `critical` for per-iteration
  accumulation (n=10⁷).
