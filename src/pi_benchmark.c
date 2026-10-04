#include <stdio.h>
#include <math.h>
#include <omp.h>

#ifndef NUM_STEPS
#define NUM_STEPS 100000000LL
#endif

int main(void)
{
    const long long n = NUM_STEPS;
    if (n <= 0) {
        fprintf(stderr, "NUM_STEPS must be positive\n");
        return 1;
    }
    const double h = 1.0 / (double)n;

    /* Warm up the thread runtime outside both timed intervals. */
    int warmup_threads = 0;
    #pragma omp parallel default(none) reduction(+:warmup_threads)
    {
        warmup_threads += 1;
    }

    double serial_sum = 0.0;
    double start = omp_get_wtime();
    for (long long i = 0; i < n; ++i) {
        double x = (i + 0.5) * h;
        serial_sum += 4.0 / (1.0 + x * x);
    }
    double pi_serial = h * serial_sum;
    double serial_seconds = omp_get_wtime() - start;

    double parallel_sum = 0.0;
    int used = 0;
    start = omp_get_wtime();
    #pragma omp parallel default(none) shared(n, h, used) \
        reduction(+:parallel_sum)
    {
        #pragma omp single nowait
        used = omp_get_num_threads();

        #pragma omp for schedule(static)
        for (long long i = 0; i < n; ++i) {
            double x = (i + 0.5) * h;
            parallel_sum += 4.0 / (1.0 + x * x);
        }
    }
    double pi_parallel = h * parallel_sum;
    double parallel_seconds = omp_get_wtime() - start;

    double difference = fabs(pi_serial - pi_parallel);
    double reference_error = fabs(pi_parallel - acos(-1.0));
    printf("n=%lld warmup_threads=%d actual_threads=%d\n",
           n, warmup_threads, used);
    printf("serial_pi=%.12f parallel_pi=%.12f\n",
           pi_serial, pi_parallel);
    printf("difference=%.3e reference_error=%.3e\n",
           difference, reference_error);
    printf("serial_seconds=%.6f parallel_seconds=%.6f\n",
           serial_seconds, parallel_seconds);
    if (parallel_seconds > 0.0) {
        double speedup = serial_seconds / parallel_seconds;
        printf("speedup=%.3f efficiency=%.3f\n",
               speedup, speedup / used);
    }
    return difference > 1e-8;
}
