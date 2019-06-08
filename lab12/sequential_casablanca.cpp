#include <iostream>
#include <mpi.h>
#include "pbm.h"
#include "helper.h"

using namespace std;

int LP[][3] = {{1, 1, 1},
               {1, 1, 1},
               {1, 1, 1}};


int main(int argc, char *argv[]) {
    image *image_casablanca = nullptr;
    image *image_casablanca_out = nullptr;
    int width, height;
    double start, end;

    image_casablanca = (image *) malloc(sizeof(image));
    image_casablanca_out = (image *) malloc(sizeof(image));

    cout << "loading image casablanca" << endl;
    readInput("../sources/casablanca.pgm", image_casablanca);
    printInfo(image_casablanca);
    cout << endl;

    width = image_casablanca->width;
    height = image_casablanca->height;
    const int size = width * height;

    uchar *inVector = nullptr;
    uchar *outVector = nullptr;

    copyImages(image_casablanca_out, image_casablanca);
    inVector = allocVector(width * height);
    mempcpy(inVector, image_casablanca->pixel, width * height);
    outVector = allocVector(width * height);

    start = (double) clock()
            / (double) CLOCKS_PER_SEC;

    // warunki początkowe i brzegowe i koncowe
    for (int i = 0; i < size; i++) {
        if (i < width)
            outVector[i] = inVector[i];
        if (i > size - width)
            outVector[i] = inVector[i];
        if (i % width == 0)
            outVector[i] = inVector[i];
        if ((i % width - (width - 1)) == 0)
            outVector[i] = inVector[i];
    }

    for (int i = 1; i < size - 1; i++) {
        if ((i < width) || (i > size - width) || (i % width == 0)) {
//            cout << i << endl;
            continue;
        }
        int average = inVector[i - width - 1] * LP[0][0] + inVector[i - width] * LP[0][1] +
                      inVector[i - width + 1] * LP[0][2] +
                      inVector[i - 1] * LP[1][0] + inVector[i] * LP[1][1] + inVector[i + 1] * LP[1][2] +
                      inVector[i + width - 1] * LP[2][0] + inVector[i + width] * LP[2][1] +
                      inVector[i + width + 1] * LP[2][2];
        int ourPoint = average / (
                LP[0][0] + LP[0][1] + LP[0][2] + LP[1][0] + LP[1][1] + LP[1][2] + LP[2][0] + LP
                [2][1] + LP[2][2]
        );
        outVector[i] = ourPoint;
    }

    end = (double) clock()
          / (double) CLOCKS_PER_SEC;

    mempcpy(image_casablanca_out->pixel, outVector, size);
    writeData("../output/sequential_casablanca.pgm", image_casablanca_out);
    cout << "end of program, working time: " << end - start << endl;
    return 0;
}