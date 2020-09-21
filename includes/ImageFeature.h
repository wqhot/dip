#ifndef IMAGEFEATURE_H
#define IMAGEFEATURE_H
#include <Image.h>
#include <Matrix.h>

namespace dip
{
    class ImageFeature
    {
    public:
        ImageFeature();
        ~ImageFeature();
        // 直方图
        int *getHist(Image src);
    };
}; // namespace dip
#endif
