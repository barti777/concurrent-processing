#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int suma(int number, int iterations)
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
    printf("Suma 500 kwadratow liczby %d wynosi=%d\n", number, suma(number, N));
    endTime = omp_get_wtime();
    printf("time = %f", endTime - startTime);
    return 0;
}