#include <stdio.h>
#include <omp.h>

int main(void)
{
    enum { N = 32 };
    int owner[N];
    double value[N];
    int used = 0;
    double start = omp_get_wtime();

    #pragma omp parallel default(none) shared(owner, value, used)
    {
        #pragma omp single nowait
        used = omp_get_num_threads();

        #pragma omp for schedule(runtime)
        for (int i = 0; i < N; ++i) {
            int work = 100000 * (i + 1);
            double local = 0.0;
            for (int k = 1; k <= work; ++k) {
                local += 1.0 / k;
            }
            value[i] = local;
            owner[i] = omp_get_thread_num();
        }
    }
    double elapsed = omp_get_wtime() - start;

    double checksum = 0.0;
    for (int i = 0; i < N; ++i) {
        checksum += value[i];
        printf("iteration=%2d thread=%d\n", i, owner[i]);
    }
    printf("threads=%d checksum=%.12f seconds=%.6f\n",
           used, checksum, elapsed);
    return 0;
}
