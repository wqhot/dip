////////////////////////////////////////////////////////////////
//FileName:ImageBase.h//Creator:WQ
//Date:2016年10月29日20:18:51
//Comment:bmp图像格式文件的打开
//
///////////////////////////////////////////////////////////////
#ifndef IMAGBASE_H
#define IMAGBASE_H
#include <iostream>
#include <fstream>
#include <string>
#include <malloc.h>
#include "Matrix.h"
#include "Image.h"
//typedef struct Bitmap_Mat {
//    int channel;
//    Matrix matrixR;
//    Matrix matrixG;
//    Matrix matrixB;
//} Image;

typedef struct Bitmap_File_Header {
    char* bfType;//文件类型
    int bfSize = 0;//文件大小
    short bf1, bf2;//保留
    int bfOffBits = 0;//位图数据偏移量
} BitmapFileHeader;

typedef struct Bitmap_Info_Header {
    int biSize; //信息头长度
    int biWidth;//宽度(px)
    int biHeight;//高度(px)
    short biPlanes;//位平面数
    short biBitCount;//每个像素所占位数
    int biCompression;//是否压缩
    int biSizeImage;//图像大小
    int biXPelsPerMeter;//水平分辨率
    int biYPelsPerMeter;//垂直分辨率
    int biClrUsed;//颜色数
    int biClrImportant;//重要颜色数
} BitmapInfoHeader;

typedef struct BitmapFile {

    BitmapFileHeader bitmapheader;
    BitmapInfoHeader bitmapinfoheader;
    float* R;
    float* G;
    float* B;

} Bitmap;


class ImageBase {
public:
    ImageBase();
    ~ImageBase();
    Image open(std::string fileName);
    // 保存图片
    int save(std::string filename, Image bitmapMat);
    // 销毁
    int destory(Image imageMat);
	Image create(Matrix matrix);
};
#endif
