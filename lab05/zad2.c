#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

const int sizeoftab = 10;
int *sorted = NULL;
int *string1 = NULL;
int *string1_copy = NULL;
int *string2 = NULL;
int *string2_copy = NULL;
int can_merge = 0;

void print_string(int *, int);
int *generate_string(int, int, int);
int *generate_empty_string(int);
void insertionSort(int *, int);
void *sortAndMergeArrays(void *);
void *merge_thread(void *);

struct thread_data_merge
{
    int size;
    int size2;
    unsigned int *array;
    unsigned int *array2;
};

struct thread_data_merge thread_data_arrays;

int main(int argc, char const *argv[])
{
    clock_t start, end, total_t;
    pthread_t thread_sort, thread_merge;

    srand(time(NULL));
    string1 = generate_string(sizeoftab, 1, 100);
    string1_copy = generate_empty_string(sizeoftab);
    string2 = generate_string(sizeoftab, 1, 100);
    string2_copy = generate_empty_string(sizeoftab);

    for (int i = 0; i < sizeoftab; i++)
    {
        string1_copy[i] = string1[i];
        string2_copy[i] = string2[i];
    }

    print_string(string1, sizeoftab);
    print_string(string2, sizeoftab);

    start = clock();
    thread_data_arrays.array = string1_copy;
    thread_data_arrays.array2 = string2_copy;
    thread_data_arrays.size = sizeoftab;
    thread_data_arrays.size2 = sizeoftab;

    if (pthread_create(&thread_sort, NULL, sortAndMergeArrays, (void *)&thread_data_arrays))
    {
        perror("pthread_create()");
        exit(-1);
    }

    thread_data_arrays.array = string1_copy;
    thread_data_arrays.array2 = string2_copy;
    thread_data_arrays.size = sizeoftab;
    thread_data_arrays.size2 = sizeoftab;

    pthread_join(thread_sort, NULL);
    while (1)
    {
        if (can_merge == 1)
        {
            if (pthread_create(&thread_merge, NULL, merge_thread, (void *)&thread_data_arrays))
            {
                perror("pthread_create()");
                exit(-1);
            }
            pthread_join(thread_merge, NULL);
            break;
        }
    }
    end = clock();

    total_t = (double)(end - start);
    printf("Total time taken by CPU to sort using threads: %ld\n", total_t);
    free(sorted);
    free(string1);
    free(string1_copy);
    free(string2);
    free(string2_copy);
    pthread_exit(NULL);

    return EXIT_SUCCESS;
}

void print_string(int *string, int string_size)
{
    for (int i = 0; i < string_size; i++)
    {
        printf("%d ", string[i]);
    }
    printf("\n");
}

int *generate_string(int number_of, int minimum_number, int max_number)
{
    int *returned_string = (int *)malloc(sizeof(int) * number_of);
    for (int i = 0; i < number_of; i++)
    {
        returned_string[i] = rand() % (max_number + 1 - minimum_number) + minimum_number;
    }
    return returned_string;
}

int *generate_empty_string(int number_of)
{
    int *returned_string = (int *)malloc(sizeof(int) * number_of);
    for (int i = 0; i < number_of; i++)
    {
        returned_string[i] = 0;
    }
    return returned_string;
}

void insertionSort(int *array, int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = array[i];
        j = i - 1;

        while (j >= 0 && array[j] > key)
        {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }
}

void *sortAndMergeArrays(void *threadarg)
{
    int size, size2;
    unsigned int *array, *array2;
    struct thread_data_merge *my_data;

    my_data = (struct thread_data_merge *)threadarg;
    size = my_data->size;
    array = my_data->array;
    size2 = my_data->size2;
    array2 = my_data->array2;

    insertionSort(array, size);
    insertionSort(array2, size2);

    printf("sorted:\n");
    print_string(array, size);
    print_string(array2, size2);
    can_merge = 1;
    pthread_exit(NULL);
}

void *merge_thread(void *args)
{
    int *a;
    int m;
    int *b;
    int n;
    struct thread_data_merge *my_data;
    my_data = (struct thread_data_merge *)args;
    a = my_data->array;
    b = my_data->array2;
    m = my_data->size;
    n = my_data->size2;

    sorted = generate_empty_string(m + n);

    int i, j, k, newsize = m + n;

    j = k = 0;

    for (i = 0; i < newsize;)
    {
        if (j < m && k < n)
        {
            if (a[j] < b[k])
            {
                sorted[i] = a[j];
                j++;
            }
            else
            {
                sorted[i] = b[k];
                k++;
            }
            i++;
        }
        else if (j == m)
        {
            for (; i < newsize;)
            {
                sorted[i] = b[k];
                k++;
                i++;
            }
        }
        else
        {
            for (; i < newsize;)
            {
                sorted[i] = a[j];
                j++;
                i++;
            }
        }
    }
    print_string(sorted, newsize);
}