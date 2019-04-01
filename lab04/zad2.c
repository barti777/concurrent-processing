#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define THREAD_COUNT 2
#define handle_error_en(en, msg) \
    do                           \
    {                            \
        errno = en;              \
        perror(msg);             \
        exit(EXIT_FAILURE);      \
    } while (0)

int shared_variable = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void *task_1()
{
    int retval;
    for (int i = 0; i < 100000; ++i)
    {
        retval = pthread_mutex_lock(&mutex);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_mutex_lock");
        }

        ++shared_variable;

        retval = pthread_mutex_unlock(&mutex);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_mutex_unlock");
        }
    }
}

static void *task_2()
{
    int retval;
    for (int i = 0; i < 100000; ++i)
    {
        retval = pthread_mutex_lock(&mutex);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_attr_destroy");
        }

        --shared_variable;

        retval = pthread_mutex_unlock(&mutex);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_attr_destroy");
        }
    }
}

void *get_task(int i)
{
    if (i == 0)
    {
        return task_1;
    }
    if (i == 1)
    {
        return task_2;
    }
}

int main()
{
    int retval;
    pthread_t tids[THREAD_COUNT];
    pthread_attr_t attr[THREAD_COUNT];
    pthread_attr_t *attrp[THREAD_COUNT] = {NULL};

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        retval = pthread_attr_init(&attr[i]);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_attr_init");
        }
        attrp[i] = &attr[i];
        retval = pthread_create(&tids[i], attrp[i], get_task(i), NULL);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_create");
        }
    }

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        retval = pthread_join(tids[i], NULL);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_join");
        }
    }

    printf("Wartość pamięci współdzielonej: %d\n", shared_variable);

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        retval = pthread_attr_destroy(attrp[i]);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_attr_destroy");
        }
    }
    retval = pthread_mutex_destroy(&mutex);
    if (retval != 0)
    {
        handle_error_en(retval, "pthread_mutex_destroy");
    }
    return 0;
}
