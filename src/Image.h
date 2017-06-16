#ifndef BITMAPMAT_H
#define BITMAPMAT_H
#include "Matrix.h"
class Image {
public:
    int channel;
    Matrix matrixR;
    Matrix matrixG;
    Matrix matrixB;
    Image();
    Image(Matrix mat);
    ~Image();
    void operator = (Image & img);
    int destory();
    Image copy();
};
#endif