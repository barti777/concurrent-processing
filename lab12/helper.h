//
// Created by user on 04.06.19.
//

#ifndef LAB12_HELPER_H
#define LAB12_HELPER_H

using namespace std;

void printInfo(image *);

void copyImages(image *, image *);

void printMatrix(uchar **, int, int);

uchar **
matrixMultiplication(uchar **, int, int, uchar **, int, int);

uchar **vectorToMatrix(uchar *vector, int n, int matrix_n, int matrix_m);

uchar *matrixToVector(uchar **matrix, int matrix_n, int matrix_m, int n);

uchar **allocMatrix(int n, int m);


void printInfo(image *img) {
    cout << "Informacje o obrazku:" << endl;
    cout << "width:" << img->width << endl;
    cout << "height:" << img->height << endl;
    cout << "maxValue:" << img->maxValue << endl;
}

void copyImages(image *dst, image *src) {
    dst->width = src->width;
    dst->height = src->height;
    dst->maxValue = src->maxValue;
    memcpy(dst->type, src->type, TYPE_LEN + 1);
    dst->pixel = (uchar *) malloc(sizeof(uchar) * src
            ->height * src->width);
}

uchar **
matrixMultiplication(uchar **matrix_1, const int n1, const int m1, uchar **matrix_2, const int m2, const int p2) {
    auto **matrixResult = (uchar **) malloc(sizeof(uchar *) * n1);
    for (int i = 0; i < n1; i++) {
        matrixResult[i] = (uchar *) malloc(sizeof(char) * p2);
    }

    for (int k = 0; k < n1; k++)
        for (int i = 0; i < p2; i++) {
            matrixResult[i][k] = 0;
            for (int j = 0; j < m1; j++)
                matrixResult[i][k] = matrixResult[i][k] + matrix_1[i][j] * matrix_2[j][k];
        }
    return matrixResult;
}

void printMatrix(uchar **matrix, const int n, const int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

uchar **vectorToMatrix(uchar *vector, const int n, const int matrix_n, const int matrix_m) {
    if (n != matrix_m * matrix_n) {
        cout << "not valid n and N + M" << endl;
        return nullptr;
    }
    uchar **matrix = (uchar **) malloc(sizeof(uchar *) * matrix_n);
    for (int i = 0; i < matrix_n; i++) {
        matrix[i] = (uchar *) malloc(sizeof(unsigned char) * matrix_m);
        for (int j = 0; j < matrix_m; j++) {
            matrix[i][j] = vector[j * matrix_n + i];
        }
    }
    return matrix;
}

uchar *matrixToVector(uchar **matrix, const int matrix_n, const int matrix_m, const int n) {
    if (n != matrix_m * matrix_n) {
        cout << "not valid n and N + M" << endl;
        return nullptr;
    }
    uchar *vector = (uchar *) malloc(sizeof(int) * n);
    for (int i = 0; i < matrix_n; i++) {
        for (int j = 0; j < matrix_m; j++) {
            vector[j* matrix_n + i] = matrix[i][j];
        }
    }
    return vector;
}

uchar **allocMatrix(const int n, const int m) {
    uchar **matrix = (uchar **) malloc(sizeof(uchar *) * n);
    for (int i = 0; i < n; i++) {
        matrix[i] = (uchar *) malloc(sizeof(int) * m);
        for (int j = 0; j < m; j++) {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

#endif //LAB12_HELPER_H
