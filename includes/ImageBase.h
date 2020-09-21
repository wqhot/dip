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
#include <Matrix.h>
#include <Image.h>
#include <memory>

namespace dip {
    Image open(std::string fileName);
    // 保存图片
    int save(std::string filename, Image bitmapMat);
    // 销毁
    int destory(Image imageMat);
	Image create(Matrix matrix);
};
#endif