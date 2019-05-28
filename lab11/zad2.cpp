#include <cstdlib>
#include <cstdio>
#include <mpi.h>
#include <cmath>

int main(int argc, char **argv) {

    int rank, size, number, division, from, to, i, k;
    const double GAMMA = 0.5772156649015328606065120900824024310421;

    double mySum, sum, myGamma, gamma;
    mySum = 0;
    sum = 0;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Input amount of iterations: ");
        std::cin >> number;
    }

    // rozsyłanie komunikatu number typu int jeden raz root 0 w obrębie komunikatora MPI_COMM_WORLD
    MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // podzielenie problemu na podproblemy
    if ((number % size) == 0) {
        division = (number / size);
    } else {
        division = (number / size) + 1;
    }

    // każdy proces ma osobny zakres pracy
    from = division * rank;
    to = from + division - 1;

    // jeżeli wartość końcowa jest większa niż zadeklarowany blok to przypisujemy koniec bloku (naszego podanego
    // problemu)
    if (to > number) {
        to = number;
    }

    // obliczamy 1/n dla każdego procesu jeżeli k = 0 to nie dzielimy przez zero, dodajemy do sumy
    for (i = from; i <= to; i++) {
        k = i;
        if (k == 0) continue;
        sum += 1.0 / k;
    }
    // obliczanie
    if (rank != 0) {
        MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        for (i = 1; i <= size - 1; ++i) {
            MPI_Recv(&mySum, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            sum += mySum;
        }
        myGamma = sum;
    }
    MPI_Reduce(&myGamma, &gamma, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {

        std::cout << "γ (Euler's gamma) is approximately: " << gamma - log(number) << ", Error is: "
                  << fabs((gamma - log(number)) - GAMMA) << std::endl;
    }
    MPI_Finalize();
    return (0);
}


