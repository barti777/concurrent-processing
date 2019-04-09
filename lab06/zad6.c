#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int _reduction(int number, int iterations)
{
    int i = 0;
    int sum = 0;
#pragma omp parallel for reduction(+ \
                                   : sum) num_threads(7)
    for (i = 0; i < iterations; i++)
    {
        sum += number * number;
    }
    return sum;
}

int _critical(int number, int iterations)
{
    int i = 0;
    int sum = 0;
#pragma omp parallel for num_threads(7)
    for (i = 0; i < iterations; i++)
    {
#pragma omp critical
        sum += number * number;
    }
    return sum;
}

int _lock(int number, int iterations)
{
    int i = 0;
    int sum = 0;
    omp_lock_t writelock;
    omp_init_lock(&writelock);

#pragma omp parallel for num_threads(7)
    for (i = 0; i < iterations; i++)
    {
        omp_set_lock(&writelock);
        sum += number * number;
        omp_unset_lock(&writelock);
    }
    return sum;
}

int main(int argc, char const *argv[])
{
    int N = 500;
    int number;
    if (argv[1] != NULL)
        number = atoi(argv[1]);
    else
        number = 2.5;

    double startTime;
    double endTime;

    startTime = omp_get_wtime();
    printf("Suma 500 kwadratow liczby %d wynosi=%d reduction\n", number, _reduction(number, N));
    endTime = omp_get_wtime();
    printf("time = %f\n\n", endTime - startTime);

    startTime = omp_get_wtime();
    printf("Suma 500 kwadratow liczby %d wynosi=%d critical\n", number, _critical(number, N));
    endTime = omp_get_wtime();
    printf("time = %f\n\n", endTime - startTime);

    startTime = omp_get_wtime();
    printf("Suma 500 kwadratow liczby %d wynosi=%d lock\n", number, _lock(number, N));
    endTime = omp_get_wtime();
    printf("time = %f\n\n", endTime - startTime);
    return 0;
}