#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

double EPS = 0.1e-10;

int N;
int M;
int P;
int Q;

void generate(double **, int, int);
double **allocate(int, int);
void deallocate(double **, int, int);

void exOne(double **, int, int, double **, int, int, double **);
void exTwo(double **, int, int, double **, int, int, double **);
void exThree(double **, int, int, double **, int, int, double **);
void exFour(double **, int, int, double **, int, int, double **);
void exFive(double **, int, int, double **, int, int, double **);
void exSix(double **, int, int, double **, int, int, double **);

int isEqual(double **, double **, int, int);

void fillMatrices(double **, int, int, double **, int, int);
void freeMatrices(double **, int, int, double **, int, int);

void printMatrix(double **, int, int);

void argsHandler(int, char **);

int main(int argc, char **argv)
{
    argsHandler(argc, argv);
    N = M = P = Q = atoi(argv[1]);

    double **firstMatrix = allocate(N, M);
    double **secondMatrix = allocate(P, Q);
    double **taskOneResult = allocate(N, M);
    double **taskTwoResult = allocate(N, M);
    double **taskThreeResult = allocate(N, M);
    double **taskFourResult = allocate(N, M);
    double **taskFiveResult = allocate(N, M);
    double **taskSixResult = allocate(N, M);

    double startTime = 0;
    double endTime = 0;

    fillMatrices(firstMatrix, N, M, secondMatrix, P, Q);
    printMatrix(firstMatrix, N, M);
    printMatrix(secondMatrix, P, Q);

    printf("[T-1] Non-parallel task processing ...\n");
    startTime = omp_get_wtime();
    exOne(firstMatrix, N, M, secondMatrix, P, Q, taskOneResult);
    endTime = omp_get_wtime();
    printf("[T-1] Non-parallel computing consumed %f time\n\n", endTime - startTime);

    printf("[T-2] Paralel task processing ...\n");
    startTime = omp_get_wtime();
    exTwo(firstMatrix, N, M, secondMatrix, P, Q, taskTwoResult);
    endTime = omp_get_wtime();
    printf("[T-2] Parallel computing consumed %f time\n", endTime - startTime);
    printf("Result from T-2 is equal to result from T-1 : %s\n\n",
           isEqual(taskOneResult, taskTwoResult, N, M) ? "true" : "false");

    printf("[T-3] Paralel task processing ...\n");
    startTime = omp_get_wtime();
    exThree(firstMatrix, N, M, secondMatrix, P, Q, taskThreeResult);
    endTime = omp_get_wtime();
    printf("[T-3] Parallel computing consumed %f time\n", endTime - startTime);
    printf("Result from T-3 is equal to result from T-1 : %s\n\n",
           isEqual(taskOneResult, taskThreeResult, N, M) ? "true" : "false");

    printf("[T-4] Paralel task processing ...\n");
    startTime = omp_get_wtime();
    exFour(firstMatrix, N, M, secondMatrix, P, Q, taskFourResult);
    endTime = omp_get_wtime();
    printf("[T-4] Parallel computing consumed %f time\n", endTime - startTime);
    printf("Result from T-4 is equal to result from T-1 : %s\n\n",
           isEqual(taskOneResult, taskFourResult, N, M) ? "true" : "false");

    printf("[T-5] Paralel task processing ...\n");
    startTime = omp_get_wtime();
    exFour(firstMatrix, N, M, secondMatrix, P, Q, taskFiveResult);
    endTime = omp_get_wtime();
    printf("[T-5] Parallel computing consumed %f time\n", endTime - startTime);
    printf("Result from T-5 is equal to result from T-1 : %s\n\n",
           isEqual(taskOneResult, taskFourResult, N, M) ? "true" : "false");

    printf("[T-6] Paralel task processing ...\n");
    startTime = omp_get_wtime();
    exFour(firstMatrix, N, M, secondMatrix, P, Q, taskSixResult);
    endTime = omp_get_wtime();
    printf("[T-6] Parallel computing consumed %f time\n", endTime - startTime);
    printf("Result from T-6 is equal to result from T-1 : %s\n\n",
           isEqual(taskOneResult, taskFourResult, N, M) ? "true" : "false");

    freeMatrices(firstMatrix, N, M, secondMatrix, P, Q);
    deallocate(taskOneResult, N, M);
    deallocate(taskTwoResult, N, M);
    deallocate(taskThreeResult, N, M);
    deallocate(taskFourResult, N, M);
    deallocate(taskFiveResult, N, M);
    deallocate(taskSixResult, N, M);

    return 0;
}

void exOne(double **firstMatrix, int n, int m, double **secondMatrix, int p, int q, double **result)
{
    int i, j, k;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < q; j++)
        {
            result[i][j] = 0;
            for (k = 0; k < p; k++)
            {
                result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
        }
    }
}

void exTwo(double **firstMatrix, int n, int m, double **secondMatrix, int p, int q, double **result)
{
    int i, j, k;
#pragma omp parallel for num_threads(6) private(j, k)
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < q; j++)
        {
            result[i][j] = 0;
            for (k = 0; k < p; k++)
            {
                result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
        }
    }
}

void exThree(double **firstMatrix, int n, int m, double **secondMatrix, int p, int q, double **result)
{
    int i, j, k;
    for (i = 0; i < m; i++)
    {
#pragma omp parallel for num_threads(6) private(k)
        for (j = 0; j < q; j++)
        {
            result[i][j] = 0;
            for (k = 0; k < p; k++)
            {
                result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
        }
    }
}

void exFour(double **firstMatrix, int n, int m, double **secondMatrix, int p, int q, double **result)
{
    int i, j, k;
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < q; j++)
        {
            result[i][j] = 0;
#pragma omp parallel for num_threads(6)
            for (k = 0; k < p; k++)
            {
#pragma omp critical
                result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
        }
    }
}
void exFive(double **firstMatrix, int n, int m, double **secondMatrix, int p, int q, double **result)
{
    int i, j, k;
    for (i = 0; i < m; i++)
    {
#pragma omp parallel for num_threads(6) schedule(dynamic)
        for (j = 0; j < q; j++)
        {
            result[i][j] = 0;
            for (k = 0; k < p; k++)
            {
                result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
        }
    }
}
void exSix(double **firstMatrix, int n, int m, double **secondMatrix, int p, int q, double **result)
{
    int i, j, k;
    for (i = 0; i < m; i++)
    {
#pragma omp parallel for num_threads(6) schedule(static)
        for (j = 0; j < q; j++)
        {
            result[i][j] = 0;
            for (k = 0; k < p; k++)
            {
                result[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
            }
        }
    }
}

void fillMatrices(double **firstMatrix, int n, int m, double **secondMatrix, int p, int q)
{
    generate(firstMatrix, n, m);
    generate(secondMatrix, p, q);
}

void freeMatrices(double **firstMatrix, int n, int m, double **secondMatrix, int p, int q)
{
    deallocate(firstMatrix, N, M);
    deallocate(secondMatrix, P, Q);
}

void generate(double **matrix, int n, int m)
{
    srand(time(NULL));

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            matrix[i][j] = (rand() % 1001) / 1000.;
        }
    }
}

double **allocate(int n, int m)
{
    double **matrix;
    matrix = (double **)malloc(sizeof(double *) * n);
    for (int i = 0; i < n; ++i)
    {
        matrix[i] = malloc(sizeof(double) * m);
    }
    return matrix;
}

void deallocate(double **matrix, int n, int m)
{
    for (int i = 0; i < n; ++i)
    {
        free(matrix[i]);
    }
    free(matrix);
}

int isEqual(double **firstMatrix, double **secondMatrix, int n, int m)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (fabs(firstMatrix[i][j] - secondMatrix[i][j]) >= EPS)
            {
                return 0;
            }
        }
    }
    return 1;
}

void argsHandler(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Invocation: %s <MATRIX_SIZE>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

void printMatrix(double **matrix, int n, int m)
{
    printf("Printing Matrix...\n");
    if (n <= 10 && m <= 10)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                printf("%f ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    else
    {
        printf("Matrix size greater than 10\n");
    }
}