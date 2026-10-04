#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(void)
{
    const int n = 1000000;
    double *a = malloc((size_t)n * sizeof(*a));
    double *b = malloc((size_t)n * sizeof(*b));
    double *c = malloc((size_t)n * sizeof(*c));
    if (a == NULL || b == NULL || c == NULL) {
        fprintf(stderr, "Allocation failed\n");
        free(a); free(b); free(c);
        return 1;
    }
    for (int i = 0; i < n; ++i) {
        a[i] = (double)i;
        b[i] = 2.0 * i;
    }
    #pragma omp parallel for default(none) \
        shared(a, b, c, n) schedule(static)
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
    int errors = 0;
    for (int i = 0; i < n; ++i) {
        if (c[i] != 3.0 * i) {
            ++errors;
        }
    }
    printf("c[0]=%.1f c[n-1]=%.1f errors=%d\n",
           c[0], c[n - 1], errors);
    free(a); free(b); free(c);
    return errors != 0;
}
