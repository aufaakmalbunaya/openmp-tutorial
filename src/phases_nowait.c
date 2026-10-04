#include <stdio.h>
#include <omp.h>

int main(void)
{
    enum { N = 12 };
    double a[N];
    double mean = 0.0;

    #pragma omp parallel default(none) shared(a, mean)
    {
        #pragma omp for schedule(static) nowait
        for (int i = 0; i < N; ++i) {
            a[i] = i + 1.0;
        }
        #pragma omp barrier
        #pragma omp single
        {
            double total = 0.0;
            for (int i = 0; i < N; ++i) {
                total += a[i];
            }
            mean = total / N;
        }
        #pragma omp for schedule(static)
        for (int i = 0; i < N; ++i) {
            a[i] -= mean;
        }
    }

    double centered_sum = 0.0;
    for (int i = 0; i < N; ++i) {
        centered_sum += a[i];
    }
    printf("nowait+barrier: mean=%.1f first=%.1f last=%.1f sum=%.1f\n",
           mean, a[0], a[N - 1], centered_sum);
    return !(mean == 6.5 && a[0] == -5.5 &&
             a[N - 1] == 5.5 && centered_sum == 0.0);
}
