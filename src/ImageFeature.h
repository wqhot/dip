#ifndef IMAGEFEATURE_H
#define IMAGEFEATURE_H
#include "Image.h"
#include "Matrix.h"
class ImageFeature {
public:
    ImageFeature();
    ~ImageFeature();
    // Ö±·½Í¼
    int* getHist(Image src);
};
#endif
