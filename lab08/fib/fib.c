#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <string.h>

double noParallelization(int);
double parallelization(int);

int main()
{
    double wtime;

    printf("[T-1] Non-parallel task processing ...\n");
    wtime = omp_get_wtime();
    noParallelization(38);
    printf("[T-1] Non-parallel computing consumed %f time\n\n", omp_get_wtime() - wtime);

    printf("[T-2] Paralel task processing ...\n");
    wtime = omp_get_wtime();
    parallelization(38);
    printf("[T-2] Parallel computing consumed %f time\n", omp_get_wtime() - wtime);

    return 0;
}

double noParallelization(int n)
{
    if (n < 2)
    {
        return n;
    }
    return noParallelization(n - 1) + noParallelization(n - 2);
}

double parallelization(int n)
{
    double x, y;

    if (n < 2)
    {
        return n;
    }
#pragma omp task shared(x) if (n > 20)
    x = parallelization(n - 1);

#pragma omp task shared(y) if (n > 20)
    y = parallelization(n - 2);

#pragma omp taskwait
    return x + y;
}
