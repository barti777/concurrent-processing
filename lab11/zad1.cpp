#include <cstdlib>
#include <cstdio>
#include <mpi.h>
#include <cmath>
#include <iostream>

int main(int argc, char *argv[]) {
    int n, rank, numProc, i, nelem;
    double PI25DT = 3.141592653589793238462643;
    double myPi, pi, sum;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // dla procesu macierzystego pobieramy ilość elementów i wysyłamy do komunikatora jeżeli jesteśmy potomkami to
    // odbieramy tą informacje
    if (rank == 0) {
        fprintf(stdout, "Input amount of elements in series: ");
        std::cin >> nelem;
        for (i = 1; i < numProc; i++) {
            MPI_Send(&nelem, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&nelem, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, nullptr);
    }
    // nowe n będzie * numer obecnego potomka i liczymy sumę elementów
    n = nelem * numProc;
    sum = 0.0;
    for (i = rank; i < n; i += numProc) {
        sum += pow((-1), i) * (4.0 / (2 * i + 1));
    }
    myPi = sum;

    //redukujemy do pi

    MPI_Reduce(&myPi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "π is approximately " << pi << ", Error is " << fabs(pi - PI25DT) << std::endl;
    }

    MPI_Finalize();
    return 0;
}
