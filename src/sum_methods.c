#include <stdio.h>
#include <omp.h>

int main(void)
{
    const long long n = 1000000;
    const long long expected = n * (n + 1) / 2;
    long long sum_reduction = 0;
    long long sum_atomic = 0;
    long long sum_critical = 0;

    #pragma omp parallel for default(none) shared(n) \
        reduction(+:sum_reduction) schedule(static)
    for (long long i = 1; i <= n; ++i) {
        sum_reduction += i;
    }

    #pragma omp parallel for default(none) \
        shared(n, sum_atomic) schedule(static)
    for (long long i = 1; i <= n; ++i) {
        #pragma omp atomic update
        sum_atomic += i;
    }

    #pragma omp parallel for default(none) \
        shared(n, sum_critical) schedule(static)
    for (long long i = 1; i <= n; ++i) {
        #pragma omp critical(sum_guard)
        {
            sum_critical += i;
        }
    }

    printf("Expected : %lld\n", expected);
    printf("Reduction: %lld\n", sum_reduction);
    printf("Atomic   : %lld\n", sum_atomic);
    printf("Critical : %lld\n", sum_critical);
    return !(sum_reduction == expected &&
             sum_atomic == expected &&
             sum_critical == expected);
}
