#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

const int sizeoftab = 10;
int *sorted = NULL;

void print_string(int *, int);
int *generate_string(int, int, int);
int *generate_empty_string(int);
void insertionSort(int *, int);
void *sortAndMergeArrays(void *);
void *merge_thread(void *);

struct thread_data
{
    int size;
    unsigned int *array;
};

struct thread_data_merge
{
    int size;
    int size2;
    unsigned int *array;
    unsigned int *array2;
};

struct thread_data thread_data_array[2];
struct thread_data_merge thread_data_arrays;

int main(int argc, char const *argv[])
{
    clock_t start, end, total_t;
    pthread_t threads[2], thread_merge;

    srand(time(NULL));
    int *string1 = generate_string(sizeoftab, 1, 100);
    int *string1_copy = generate_empty_string(sizeoftab);
    int *string2 = generate_string(sizeoftab, 1, 100);
    int *string2_copy = generate_empty_string(sizeoftab);

    for (int i = 0; i < sizeoftab; i++)
    {
        string1_copy[i] = string1[i];
        string2_copy[i] = string2[i];
    }

    print_string(string1, sizeoftab);
    print_string(string2, sizeoftab);

    start = clock();
    for (int t = 0; t < 2; t++)
    {
        thread_data_array[t].size = sizeoftab;
        if (t == 0)
            thread_data_array[t].array = string1;
        else
            thread_data_array[t].array = string2;

        if (pthread_create(&threads[t], NULL, sortAndMergeArrays, (void *)&thread_data_array[t]))
        {
            perror("pthread_create()");
            exit(-1);
        }
    }

    thread_data_arrays.array = string1_copy;
    thread_data_arrays.array2 = string2_copy;
    thread_data_arrays.size = sizeoftab;
    thread_data_arrays.size2 = sizeoftab;

    if (pthread_create(&thread_merge, NULL, merge_thread, (void *)&thread_data_arrays))
    {
        perror("pthread_create()");
        exit(-1);
    }

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_join(thread_merge, NULL);

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
    static int which = 0;
    int size;
    unsigned int *array;
    struct thread_data *my_data;

    my_data = (struct thread_data *)threadarg;
    size = my_data->size;
    array = my_data->array;

    insertionSort(array, size);
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