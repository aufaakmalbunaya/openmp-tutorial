#include <stdio.h>
#include <omp.h>

int main(void)
{
    enum { N = 8 };
    int result[N];
    int owner[N];

    #pragma omp parallel default(none) shared(result, owner)
    {
        #pragma omp single
        {
            for (int i = 0; i < N; ++i) {
                #pragma omp task default(none) \
                    firstprivate(i) shared(result, owner)
                {
                    result[i] = i * i;
                    owner[i] = omp_get_thread_num();
                }
            }
            #pragma omp taskwait
            for (int i = 0; i < N; ++i) {
                printf("task=%d result=%d executed_by=%d\n",
                       i, result[i], owner[i]);
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        if (result[i] != i * i) {
            return 1;
        }
    }
    return 0;
}
