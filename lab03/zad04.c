#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define WYMIAR 1000
#define ROZMIAR WYMIAR *WYMIAR
double a[ROZMIAR], b[ROZMIAR], c[ROZMIAR];

#define STACK_SIZE 1024
#define THREAD_COUNT 3

#define handle_error_en(en, msg) do { perror(msg); exit(EXIT_FAILURE);} while (0)

double czasozajmowacz() {
    int i, j, k;
    int n = WYMIAR;
    for (i = 0; i < ROZMIAR; i++)
        a[i] = 1.0 * i;
    for (i = 0; i < ROZMIAR; i++)
        b[i] = 1.0 * (ROZMIAR - i);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            c[i + n * j] = 0.0;
            for (k = 0; k < n; k++) {
                c[i + n * j] += a[i + n * k] * b[k + n * j];
            }
        }
    }
    return (c[ROZMIAR - 1]);
}

void* zadanie(void* arg) {
    int numer = *((int *)arg);
    printf("Start watku %d\n", 1);
    czasozajmowacz();
    printf("Koniec watku %d\n", 1);
}
void* zadanie2(void* arg) {
    int numer = *((int *)arg);
    printf("Start watku %d\n", 2);
    czasozajmowacz();
    printf("Koniec watku %d\n", 2);
}
void* zadanie3(void* arg) {
    int numer = *((int *)arg);
    printf("Start watku %d\n", 3);
    czasozajmowacz();
    printf("Koniec watku %d\n", 3);
}

int main() {
    int i;
    int ret;
    struct sched_param param;
    int policy = SCHED_FIFO;

    pthread_t tid[THREAD_COUNT];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_attr_setstacksize(&attr, STACK_SIZE);

    for (i = 0; i < THREAD_COUNT; ++i)
    {
        switch (i)
        {
            case 0:
                ret = pthread_create(&tid[i], &attr, zadanie, &i);
                if (ret == -1) 
                {
                    perror("pthread_create()");
                }
                break;
            case 1:
                ret = pthread_create(&tid[i], &attr, zadanie2, &i);
                if (ret == -1) 
                {
                    perror("pthread_create()");
                }
                break;
            case 2:
                ret = pthread_create(&tid[i], &attr, zadanie3, &i);
                if (ret == -1) 
                {
                    perror("pthread_create()");
                }
                break;
            default:
                break;
        }
        
    }

    param.sched_priority = sched_get_priority_max(policy);
    for (size_t i = 0; i < THREAD_COUNT; ++i)
    {
        ret = pthread_setschedparam(tid[i], policy, &param);
        if (ret == -1)
            handle_error_en(ret, "pthread_setschedparam");
    }

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(2, &cpuset);

    ret = pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset);
    if (ret == -1)
        handle_error_en(ret, "pthread_setaffinity_np");

    for (size_t i = 0; i < THREAD_COUNT; ++i)
    {
        ret = pthread_setaffinity_np(tid[i], sizeof(cpuset), &cpuset);
        if (ret == -1)
            handle_error_en(ret, "pthread_setaffinity_np");
    }

    for (size_t i = 0; i < THREAD_COUNT; ++i)
    {
        ret = pthread_join(tid[i], NULL);
        if (ret == -1)
            handle_error_en(ret, "pthread_join");
    }

    ret = pthread_attr_destroy(&attr);
    if (ret == -1)
        handle_error_en(ret, "pthread_attr_destroy");

    return 0;
}
