#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
    int a = 7;
    int N;
    if (argv[1] != NULL)
        N = atoi(argv[1]);
    else
        N = 10;

#pragma omp parallel for num_threads(7) schedule(static, 3)
    for (int i = 0; i < N; i++)
    {
        printf("Thread %d a=%d\n", omp_get_thread_num(), a);
        a++;
    }
    printf("a after =%d\n\n", a);

    a = 7;
#pragma omp parallel for num_threads(7) schedule(static)
    for (int i = 0; i < N; i++)
    {
        printf("Thread %d a=%d\n", omp_get_thread_num(), a);
        a++;
    }
    printf("a after =%d\n\n", a);

    a = 7;
#pragma omp parallel for num_threads(7) schedule(dynamic, 3)
    for (int i = 0; i < N; i++)
    {
        printf("Thread %d a=%d\n", omp_get_thread_num(), a);
        a++;
    }

    printf("a after =%d\n\n", a);
    a = 7;
#pragma omp parallel for num_threads(7) schedule(dynamic)
    for (int i = 0; i < N; i++)
    {
        printf("Thread %d a=%d\n", omp_get_thread_num(), a);
        a++;
    }

    printf("a after =%d\n\n", a);
    return 0;
}