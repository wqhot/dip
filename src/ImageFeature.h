#ifndef IMAGEFEATURE_H
#define IMAGEFEATURE_H
#include "Image.h"
#include "Matrix.h"
class ImageFeature {
public:
    ImageFeature();
    ~ImageFeature();
    // ֱ��ͼ
    int* getHist(Image src);
};
#endif
