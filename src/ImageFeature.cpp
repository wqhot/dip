#include <ImageFeature.h>



ImageFeature::ImageFeature() {
}


ImageFeature::~ImageFeature() {
}


// 直方图
int* ImageFeature::getHist(Image src) {
    int *hist=0;
    if (src.channel == 3) {
        hist = (int*)malloc(sizeof(int) * 256 * 3);
        for (int i = 0; i < 256 * 3; i++) {
            hist[i] = 0;
        }
        //生成直方图
        for (int i = 0; i < src.matrixB.height; i++) {
            for (int j = 0; j < src.matrixB.width; j++) {
                float temp = src.matrixR.getPixel(i + 1, j + 1);
                hist[(int)temp]++;
                temp = src.matrixG.getPixel(i + 1, j + 1);
                hist[(int)temp + 256]++;
                temp = src.matrixB.getPixel(i + 1, j + 1);
                hist[(int)temp + 256 * 2]++;
            }
        }
    }else if (src.channel == 1) {
        hist = (int*)malloc(sizeof(int) * 256);
        for (int i = 0; i < 256; i++) {
            hist[i] = 0;
        }
        //生成直方图
        for (int i = 0; i < src.matrixB.height; i++) {
            for (int j = 0; j < src.matrixB.width; j++) {
                float temp = src.matrixB.getPixel(i + 1, j + 1);
                hist[(int)temp]++;        
            }
        }
    }
    return hist;
}