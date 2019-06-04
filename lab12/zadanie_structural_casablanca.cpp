#include <iostream>
#include <mpi.h>
#include "pbm.h"
#include "helper.h"

using namespace std;

int LP[][3] = {{1, 1,  1},
                {1, 12, 1},
                {1, 1,  1}};


int main(int argc, char *argv[]) {
    image *obrazek_casablanca;
    image *obrazek_casablanca_out;
    int width, height;
    double start, end;

    obrazek_casablanca = (image *) malloc(sizeof(image));
    obrazek_casablanca_out = (image *) malloc(sizeof(image));

    cout << "wczytywanie obrazka casablanca" << endl;
    readInput("../sources/casablanca.pgm", obrazek_casablanca);
    printInfo(obrazek_casablanca);
    cout << endl;

    width = obrazek_casablanca->width;
    height = obrazek_casablanca->height;

    copyImages(obrazek_casablanca_out, obrazek_casablanca);

    uchar **inMatrix = vectorToMatrix(obrazek_casablanca->pixel, width * height, width, height);
    uchar **outMatrix = allocMatrix(width, height);

    start = (double) clock()
            / (double) CLOCKS_PER_SEC;

    // warunki początkowe i brzegowe i koncowe
    for (int i = 0; i < width; i++) {
        outMatrix[i][0] = inMatrix[i][0];
        outMatrix[i][height - 1] = inMatrix[i][height - 1];
    }
    for (int j = 0; j < height; j++) {
        outMatrix[0][j] = inMatrix[0][j];
        outMatrix[width - 1][j] = inMatrix[width - 1][j];
    }

    for (int i = 1; i < width - 1; i++) {
        for (int j = 1; j < height - 1; j++) {
            int average = inMatrix[i - 1][j - 1] * LP[0][0] + inMatrix[i - 1][j] * LP[0][1] +
                          inMatrix[i - 1][j + 1] * LP[0][2] +
                          inMatrix[i][j - 1] * LP[1][0] + inMatrix[i][j] * LP[1][1] + inMatrix[i][j + 1] * LP[1][2] +
                          inMatrix[i + 1][j - 1] * LP[2][0] + inMatrix[i + 1][j] * LP[2][1] +
                          inMatrix[i + 1][j + 1] * LP[2][2];
            int ourPoint = average / (
                    LP[0][0] + LP[0][1] + LP[0][2] + LP[1][0] + LP[1][1] + LP[1][2] + LP[2][0] + LP
                    [2][1] + LP[2][2]
            );
            outMatrix[i][j] = ourPoint;
        }
    }

    end = (double) clock()
          / (double) CLOCKS_PER_SEC;

    obrazek_casablanca_out->pixel = matrixToVector(outMatrix, width, height, width * height);
    writeData("new_casablanca_image.pgm", obrazek_casablanca_out);
    cout << "koniec programu, czas działania: " << end - start << endl;
    return 0;
}

