#include <stdio.h>
#include <omp.h>

int main(void)
{
    const long long n = 1000;
    const long long expected = n * (n + 1) * (2 * n + 1) / 6;
    long long sumsq = 0;
    #pragma omp parallel for default(none) shared(n) \
        reduction(+:sumsq) schedule(static)
    for (long long i = 1; i <= n; ++i) {
        sumsq += i * i;
    }
    printf("sum of squares 1..%lld: expected=%lld got=%lld %s\n",
           n, expected, sumsq, sumsq == expected ? "OK" : "MISMATCH");
    return sumsq != expected;
}
