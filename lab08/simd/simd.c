#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <string.h>

#define SIZE 250000000

void seed(double *, int);
double *allocate(int);

void noParallelization(double *, double *, double *);
void parallelization(double *, double *, double *);
void __simd(double *, double *, double *);

int main()
{
    int i;
    srand(time(NULL));
    double wtime, wtimenp, wtimep, wtimesimd;

    double *v1 = allocate(SIZE);
    double *v2 = allocate(SIZE);

    double *result = allocate(SIZE);
    memset(result, 0, SIZE);

    seed(v1, SIZE);
    seed(v2, SIZE);

    wtime = omp_get_wtime();
    noParallelization(result, v1, v2);
    wtimenp = omp_get_wtime() - wtime;

    wtime = omp_get_wtime();
    parallelization(result, v1, v2);
    wtimep = omp_get_wtime() - wtime;

    wtime = omp_get_wtime();
    __simd(result, v1, v2);
    wtimesimd = omp_get_wtime() - wtime;

    printf("Non-parallel computing consumed %lf time\n", wtimenp);
    printf("Parallel computing consumed %lf time\n", wtimep);
    printf("Simd computing consumed: %lf time\n", wtimesimd);

    free(v1);
    free(v2);
    free(result);

    return 0;
}

void noParallelization(double *result, double *v1, double *v2)
{
    for (int i = 0; i < SIZE; i++)
    {
        result[i] = v1[i] * v2[i];
    }
}

void parallelization(double *result, double *v1, double *v2)
{
    int i;
#pragma omp parallel for
    for (i = 0; i < SIZE; i++)
    {
        result[i] = v1[i] * v2[i];
    }
}

void __simd(double *result, double *v1, double *v2)
{
    int i;
#pragma omp for simd
    for (i = 0; i < SIZE; i++)
    {
        result[i] = v1[i] * v2[i];
    }
}

void seed(double *vector, int size)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = (double)rand() / RAND_MAX;
    }
}

double *allocate(int size)
{
    return malloc(size * sizeof(double *));
}
