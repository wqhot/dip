#include "Image.h"
Image::Image() {
    this->channel = 1;
}
Image::Image(Matrix mat) {
    this->channel = 1;
    this->matrixB = mat;
}

Image::~Image() {
}

void Image::operator=(Image& img) {
    this->channel = img.channel;
    this->matrixB = img.matrixB;
    this->matrixG = img.matrixG;
    this->matrixR = img.matrixR;
}


int Image::destory() {
    if (channel == 3) {
        matrixB.destory();
        matrixG.destory();
        matrixR.destory();
    }
    else {
        matrixB.destory();
    }
    channel = 0;
    return 0;
}


Image Image::copy() {
    Image dst;
    dst.channel = channel;
    dst.matrixB = matrixB.copy();
    dst.matrixG = matrixG.copy();
    dst.matrixR = matrixR.copy();
    return dst;
}
