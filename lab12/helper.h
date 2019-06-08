//
// Created by user on 04.06.19.
//

#ifndef LAB12_HELPER_H
#define LAB12_HELPER_H

using namespace std;

void printInfo(image *);

void printInfo(image);

void copyImages(image *, image *);

uchar *allocVector(int n);

void printInfo(image *img) {
    cout << "Informacje o obrazku:" << endl;
    cout << "width:" << img->width << endl;
    cout << "height:" << img->height << endl;
    cout << "maxValue:" << img->maxValue << endl;
}

void printInfo(image img) {
    cout << "Informacje o obrazku:" << endl;
    cout << "width:" << img.width << endl;
    cout << "height:" << img.height << endl;
    cout << "maxValue:" << img.maxValue << endl;
}


void copyImages(image *dst, image *src) {
    dst->width = src->width;
    dst->height = src->height;
    dst->maxValue = src->maxValue;
    memcpy(dst->type, src->type, TYPE_LEN + 1);
    dst->pixel = (uchar *) malloc(sizeof(uchar) * src
            ->height * src->width);
}

uchar *allocVector(const int n) {
    auto *vector = (uchar *) malloc(sizeof(uchar) * n);
    for (int i = 0; i < n; i++) {
        vector[i] = 0;
    }
    return vector;
}

#endif //LAB12_HELPER_H
