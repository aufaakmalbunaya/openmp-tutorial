#include <stdio.h>
#include <omp.h>

int main(void)
{
    int base = 10;
    int scratch = -1;
    #pragma omp parallel default(none) \
        firstprivate(base) private(scratch)
    {
        int tid = omp_get_thread_num();
        scratch = tid * tid;
        /* Initialize before reading. */
        base += tid;
        /* Changes this private copy. */
        #pragma omp critical
        {
            printf("thread=%d base=%d scratch=%d\n",
                   tid, base, scratch);
        }
    }
    printf("Outside: base=%d scratch=%d\n", base, scratch);
    return 0;
}
