#ifndef BITMAPMAT_H
#define BITMAPMAT_H
#include <Matrix.h>
namespace dip
{
    class Image
    {
    public:
        int channel;
        Matrix matrixR;
        Matrix matrixG;
        Matrix matrixB;
        Image();
        Image(Matrix mat);
        ~Image();
        void operator=(const Image &img);
        int destory();
        Image copy();
    };
}; // namespace dip

#endif