#include <stdio.h>
#include <omp.h>

int main(void)
{
    printf("Before: team size = %d\n", omp_get_num_threads());
    #pragma omp parallel default(none)
    {
        int tid = omp_get_thread_num();
        int team_size = omp_get_num_threads();
        #pragma omp critical
        {
            printf("Hello from thread %d of %d\n", tid, team_size);
        }
    }
    printf("After: team size = %d\n", omp_get_num_threads());
    return 0;
}
