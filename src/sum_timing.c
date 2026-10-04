#include <stdio.h>
#include <omp.h>

/* Time the three accumulation methods (no printing inside timers). */
int main(void)
{
    const long long n = 10000000;
    const long long expected = n * (n + 1) / 2;
    const int reps = 5;
    double t_red = 0.0, t_atom = 0.0, t_crit = 0.0;
    long long check = 0;

    for (int r = 0; r < reps; ++r) {
        long long s = 0;
        double t0 = omp_get_wtime();
        #pragma omp parallel for default(none) shared(n) \
            reduction(+:s) schedule(static)
        for (long long i = 1; i <= n; ++i) s += i;
        t_red += omp_get_wtime() - t0;
        check += (s == expected);
    }
    for (int r = 0; r < reps; ++r) {
        long long s = 0;
        double t0 = omp_get_wtime();
        #pragma omp parallel for default(none) \
            shared(n, s) schedule(static)
        for (long long i = 1; i <= n; ++i) {
            #pragma omp atomic update
            s += i;
        }
        t_atom += omp_get_wtime() - t0;
        check += (s == expected);
    }
    for (int r = 0; r < reps; ++r) {
        long long s = 0;
        double t0 = omp_get_wtime();
        #pragma omp parallel for default(none) \
            shared(n, s) schedule(static)
        for (long long i = 1; i <= n; ++i) {
            #pragma omp critical(tguard)
            { s += i; }
        }
        t_crit += omp_get_wtime() - t0;
        check += (s == expected);
    }
    printf("n=%lld reps=%d correct=%lld/%d\n", n, reps, check, 3 * reps);
    printf("reduction: %.6f s/run\n", t_red / reps);
    printf("atomic   : %.6f s/run\n", t_atom / reps);
    printf("critical : %.6f s/run\n", t_crit / reps);
    return check != 3 * reps;
}
