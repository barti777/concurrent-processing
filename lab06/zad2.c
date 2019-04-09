#include <stdio.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
    int a = 7;
#pragma omp parallel for num_threads(7) private(a)
    for (int i = 0; i < 10; i++)
    {
        printf("Thread %d a=%d\n", omp_get_thread_num(), a);
        a++;
    }
    printf("a after =%d\n\n", a);
#pragma omp parallel for num_threads(7) firstprivate(a)
    for (int i = 0; i < 10; i++)
    {
        printf("Thread %d a=%d\n", omp_get_thread_num(), a);
        a++;
    }
    printf("a after =%d\n\n", a);

    a = 7;

    a = 7;
#pragma omp parallel for num_threads(7) shared(a)
    for (int i = 0; i < 10; i++)
    {
        printf("Thread %d a=%d\n", omp_get_thread_num(), a);
        a++;
    }
    printf("a after =%d\n\n", a);
    return 0;
}