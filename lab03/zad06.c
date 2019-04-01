#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define handle_error_en(en, msg) do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define THREAD_COUNT 10

static void
display_pthread_attr(pthread_attr_t *attr, char *prefix)
{
    int s, i;
    size_t v;
    void *stkaddr;
    struct sched_param sp;

    s = pthread_attr_getdetachstate(attr, &i);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getdetachstate");
    printf("%sDetach state        = %s\n", prefix,
           (i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" : (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" : "???");

    s = pthread_attr_getscope(attr, &i);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getscope");
    printf("%sScope               = %s\n", prefix,
           (i == PTHREAD_SCOPE_SYSTEM) ? "PTHREAD_SCOPE_SYSTEM" : (i == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS" : "???");

    s = pthread_attr_getinheritsched(attr, &i);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getinheritsched");
    printf("%sInherit scheduler   = %s\n", prefix,
           (i == PTHREAD_INHERIT_SCHED) ? "PTHREAD_INHERIT_SCHED" : (i == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" : "???");

    s = pthread_attr_getschedpolicy(attr, &i);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getschedpolicy");
    printf("%sScheduling policy   = %s\n", prefix,
           (i == SCHED_OTHER) ? "SCHED_OTHER" : (i == SCHED_FIFO) ? "SCHED_FIFO" : (i == SCHED_RR) ? "SCHED_RR" : "???");

    s = pthread_attr_getschedparam(attr, &sp);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getschedparam");
    printf("%sScheduling priority = %d\n", prefix, sp.sched_priority);

    s = pthread_attr_getguardsize(attr, &v);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getguardsize");
    printf("%sGuard size          = %ld bytes\n", prefix, v);

    s = pthread_attr_getstack(attr, &stkaddr, &v);
    if (s != 0)
        handle_error_en(s, "pthread_attr_getstack");
    printf("%sStack address       = %p\n", prefix, stkaddr);
    printf("%sStack size          = 0x%zx bytes\n", prefix, v);
}

static void *
thread_start(void *arg)
{
    int s;
    pthread_attr_t gattr;

    s = pthread_getattr_np(pthread_self(), &gattr);
    if (s != 0)
        handle_error_en(s, "pthread_getattr_np");

    printf("Thread attributes:\n");
    display_pthread_attr(&gattr, "\t");
}

pthread_attr_t getAttr() {
    int retval = 0;
    pthread_attr_t attr;
    int policy;
    retval = pthread_attr_init(&attr);
    if (retval != 0)
        handle_error_en(retval, "pthread_attr_init");

    do
    {
        retval = pthread_attr_setdetachstate(&attr, rand() % 2);
    } while (retval == EINVAL || retval == -1);

    do {
        retval = pthread_attr_setinheritsched(&attr, rand() % 3);
    } while (retval == EINVAL || retval == -1);

    do
    {
        policy = rand() % 30;
        retval = pthread_attr_setschedpolicy(&attr, policy);
    } while (retval == EINVAL || retval == -1);
    do
    {
        struct sched_param param;
        param.sched_priority = policy;
        retval = pthread_attr_setschedparam(&attr, &param);
    } while (retval == EINVAL || retval == -1);
    do
    {
        retval = pthread_attr_setschedpolicy(&attr, rand()%30);
    } while (retval == EINVAL || retval == -1);
    do
    {
        retval = pthread_attr_setscope(&attr, rand() % 4);
    } while (retval == EINVAL || retval == -1);

    return attr;
}


int main(int argc, char *argv[])
{
    pthread_t thr;
    pthread_attr_t attr;
    pthread_attr_t *attrp;
    pthread_attr_t *attrp_array[THREAD_COUNT];
    int s;
    int i;
    int retval = 0;

    srand(time(NULL));

    attrp = NULL;

    for (i = 0; i < THREAD_COUNT;) {
        attr = getAttr();
        attrp = &attr;
        s = pthread_create(&thr, attrp, &thread_start, NULL);
        if (s == 0) {
            ++i;
            attrp_array[i] = attrp;
            sleep(1);
        }
    }

    for (i = 0; i < THREAD_COUNT; ++i) {
        if (attrp_array[i] != NULL) {
            s = pthread_attr_destroy(attrp_array[i]);
            if (s != 0)
                handle_error_en(s, "pthread_attr_destroy");
        }
    }

    // pause();
    exit(EXIT_SUCCESS);
}
