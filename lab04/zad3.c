#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#define true 1
#define handle_error_en(en, msg) \
    do                           \
    {                            \
        errno = en;              \
        perror(msg);             \
        exit(EXIT_FAILURE);      \
    } while (0)

#define DRINK_COUNT 2

void *pub_simulator(void *);
int get_random_number(int);

struct pub_structure
{
    pthread_mutex_t *mugs;
    int mug_count;
    int max_time_drink;
} pub;

int main(int argc, char *argv[])
{
    int retval;

    if (argc != 4)
    {
        fprintf(stderr, "Składnia: %s <ILOŚĆ KLIENTÓW> <ILOŚĆ KUFLI> <MAKSYMALNY CZAS PICIA>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int client_count = atoi(argv[1]);
    int mug_count = atoi(argv[2]);
    pub.mug_count = mug_count;
    pub.mugs = malloc(sizeof(pthread_mutex_t) * mug_count);
    memset(pub.mugs, 0, (sizeof(pthread_mutex_t) * mug_count));
    for (int i = 0; i < pub.mug_count; ++i)
    {
        pthread_mutex_init(&pub.mugs[i], NULL);
    }

    pub.max_time_drink = atoi(argv[3]);

    pthread_t *tids = malloc(sizeof(pthread_t) * client_count);
    memset(tids, 0, sizeof(pthread_t) * client_count);

    pthread_attr_t *attr = malloc(sizeof(pthread_attr_t) * client_count);
    memset(attr, 0, sizeof(pthread_attr_t) * client_count);

    int *client_indexes = malloc(sizeof(int) * client_count);

    for (int i = 0; i < client_count; ++i)
    {
        client_indexes[i] = i;

        retval = pthread_attr_init(&attr[i]);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_attr_init");
        }

        retval = pthread_create(&tids[i], &attr[i], pub_simulator, (void *)&client_indexes[i]);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_create");
        }
    }

    for (int i = 0; i < client_count; ++i)
    {
        retval = pthread_join(tids[i], NULL);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_join");
        }
    }

    free(tids);
    free(attr);
    for (int i = 0; i < pub.mug_count; ++i)
    {
        pthread_mutex_destroy(&pub.mugs[i]);
    }
    free(pub.mugs);
}

void *pub_simulator(void *args)
{
    int retval;
    int client_id = (*(int *)args);
    printf("Klient numer \033[1;34m%d\033[0m przychodzi do pubu\n", client_id);

    for (int i = 0; i < DRINK_COUNT; ++i)
    {
        int mug_id = -1;
        while (true)
        {
            ++mug_id;
            retval = pthread_mutex_trylock(&pub.mugs[mug_id % pub.mug_count]);
            if (retval == 0)
            {
                mug_id = mug_id % pub.mug_count;
                break;
            }
            else if (retval != EBUSY)
            {
                handle_error_en(retval, "pthread_mutex_trylock");
            }
        }

        printf("Klient numer \033[1;31m%d\033[0m zamawia piwo w kuflu o numerze \033[1;32m%d\033[0m\n", client_id, mug_id);
        int sleep_time = get_random_number(pub.max_time_drink);
        sleep(sleep_time);
        printf("Klient numer \033[1;31m%d\033[0m oddaje kufel o numerze \033[1;32m%d\033[0m po \033[1;33m%d\033[0m sekundach\n", client_id, mug_id, sleep_time);
        retval = pthread_mutex_unlock(&pub.mugs[mug_id]);
        if (retval != 0)
        {
            handle_error_en(retval, "pthread_mutex_unlock");
        }
    }

    return (void *)NULL;
}

int get_random_number(int max_number)
{
    srand(time(NULL) - getpid());
    return rand() % max_number;
}
