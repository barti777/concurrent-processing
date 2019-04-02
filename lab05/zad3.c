#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

int SIZE = 50;

int *generate_integer_list(int);
int *generate_boolean_list(int);
void print_list_on_true(int *, int *, int);
void *erastotenes_sieve(void *);
int power(int, int);

struct function_args
{
    int *list;
    int *boolean_list;
    int size;
};

int main(int argc, char const *argv[])
{

    if (argv[1] != NULL)
    {
        SIZE = atoi(argv[1]);
    }

    int *integer_list = generate_integer_list(SIZE);
    int *boolean_list = generate_boolean_list(SIZE);
    struct function_args args;
    args.list = integer_list;
    args.boolean_list = boolean_list;
    args.size = SIZE;
    pthread_t tid;
    pthread_create(&tid, NULL, erastotenes_sieve, (void *)&args);
    // erastotenes_sieve((void *)&args);
    pthread_join(tid, NULL);
    pthread_exit(NULL);
    return 0;
}

int *generate_integer_list(int size)
{
    int *retval = (int *)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        retval[i] = i;
    }
    return retval;
}
int *generate_boolean_list(int size)
{
    int *retval = (int *)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        retval[i] = 1;
    }
    return retval;
}

void print_list_on_true(int *string, int *boolean, int string_size)
{
    for (int i = 0; i < string_size; i++)
    {
        if (boolean[i])
            printf("%d ", string[i]);
    }
    printf("\n");
}

void *erastotenes_sieve(void *args)
{
    int *list, *boolean_list, size;
    struct function_args *data;
    data = (struct function_args *)args;
    list = data->list;
    boolean_list = data->boolean_list;
    size = data->size;
    for (int i = 2; i < sqrt(SIZE); i++)
    {
        if (boolean_list[i])
            for (int j = i * i; j < SIZE; j = j + i)
                boolean_list[j] = 0;
    }
    print_list_on_true(list + 2, boolean_list + 2, size - 2);
}

int power(int a, int b)
{
    int sum = 1;
    for (int i = 0; i < b; i++)
    {
        sum = sum * a;
    }
    return sum;
}