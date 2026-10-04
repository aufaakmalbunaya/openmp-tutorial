/* Mini-project: parallel dense matrix multiplication C = A*B.
 * Row-major 1D storage, heap allocation, deterministic values.
 * Usage: ./matmul <n>   (n = matrix size; thread count via OMP_NUM_THREADS)
 * Reports serial vs parallel (plain + collapse(2)) timings and verification.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

static void matmul_serial(const double *a, const double *b, double *c, int n)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double local = 0.0;
            for (int k = 0; k < n; ++k) {
                local += a[i * n + k] * b[k * n + j];
            }
            c[i * n + j] = local;
        }
    }
}

static void matmul_parallel(const double *a, const double *b, double *c, int n)
{
    #pragma omp parallel for default(none) \
        shared(a, b, c, n) schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double local = 0.0;
            for (int k = 0; k < n; ++k) {
                local += a[i * n + k] * b[k * n + j];
            }
            c[i * n + j] = local;
        }
    }
}

static void matmul_collapse(const double *a, const double *b, double *c, int n)
{
    #pragma omp parallel for default(none) \
        shared(a, b, c, n) schedule(static) collapse(2)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            double local = 0.0;
            for (int k = 0; k < n; ++k) {
                local += a[i * n + k] * b[k * n + j];
            }
            c[i * n + j] = local;
        }
    }
}

/* Max absolute difference between two matrices. */
static double max_diff(const double *x, const double *y, int n)
{
    double d = 0.0;
    for (int i = 0; i < n * n; ++i) {
        double e = fabs(x[i] - y[i]);
        if (e > d) d = e;
    }
    return d;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 2;
    }
    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "n must be positive\n");
        return 2;
    }

    double *a = malloc((size_t)n * n * sizeof(*a));
    double *b = malloc((size_t)n * n * sizeof(*b));
    double *cs = malloc((size_t)n * n * sizeof(*cs));
    double *cp = malloc((size_t)n * n * sizeof(*cp));
    double *cc = malloc((size_t)n * n * sizeof(*cc));
    if (!a || !b || !cs || !cp || !cc) {
        fprintf(stderr, "Allocation failed\n");
        free(a); free(b); free(cs); free(cp); free(cc);
        return 1;
    }

    /* Phase 1: B = identity -> C must equal A. */
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            a[i * n + j] = (double)(i * n + j) / (n * n);
            b[i * n + j] = (i == j) ? 1.0 : 0.0;
        }
    matmul_serial(a, b, cs, n);
    matmul_parallel(a, b, cp, n);
    double d_ident = max_diff(cs, a, n);
    double d_ident_p = max_diff(cp, a, n);

    /* Phase 2: deterministic nontrivial values. */
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            a[i * n + j] = (double)((i * 7 + j * 13) % 97) / 97.0;
            b[i * n + j] = (double)((i * 11 - j * 5 + 200) % 89) / 89.0;
        }

    /* Warm up the thread pool outside the timers. */
    #pragma omp parallel default(none) shared(n)
    { (void)n; }

    double t0 = omp_get_wtime();
    matmul_serial(a, b, cs, n);
    double t_serial = omp_get_wtime() - t0;

    int used = 0;
    t0 = omp_get_wtime();
    #pragma omp parallel default(none) shared(used)
    {
        #pragma omp single nowait
        used = omp_get_num_threads();
    }
    matmul_parallel(a, b, cp, n);
    double t_parallel = omp_get_wtime() - t0;

    t0 = omp_get_wtime();
    matmul_collapse(a, b, cc, n);
    double t_collapse = omp_get_wtime() - t0;

    double d_par = max_diff(cs, cp, n);
    double d_col = max_diff(cs, cc, n);
    const double tol = 1e-9;
    int ok = d_ident < tol && d_ident_p < tol && d_par < tol && d_col < tol;

    printf("n=%d threads=%d\n", n, used);
    printf("identity_check: serial_vs_A=%.3e parallel_vs_A=%.3e\n",
           d_ident, d_ident_p);
    printf("verify: parallel_vs_serial=%.3e collapse_vs_serial=%.3e tol=%.0e %s\n",
           d_par, d_col, tol, ok ? "OK" : "MISMATCH");
    printf("time_serial=%.6f time_parallel=%.6f time_collapse=%.6f\n",
           t_serial, t_parallel, t_collapse);
    if (t_parallel > 0.0)
        printf("speedup_parallel=%.3f speedup_collapse=%.3f\n",
               t_serial / t_parallel, t_serial / t_collapse);

    free(a); free(b); free(cs); free(cp); free(cc);
    return ok ? 0 : 1;
}
