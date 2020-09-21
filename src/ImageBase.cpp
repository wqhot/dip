#include <ImageBase.h>

namespace dip
{

    typedef struct Bitmap_File_Header
    {
        char bfType[2]; //文件类型
        int bfSize = 0;    //文件大小
        short bf1, bf2;    //保留
        int bfOffBits = 0; //位图数据偏移量
    } BitmapFileHeader;

    typedef struct Bitmap_Info_Header
    {
        int biSize;          //信息头长度
        int biWidth;         //宽度(px)
        int biHeight;        //高度(px)
        short biPlanes;      //位平面数
        short biBitCount;    //每个像素所占位数
        int biCompression;   //是否压缩
        int biSizeImage;     //图像大小
        int biXPelsPerMeter; //水平分辨率
        int biYPelsPerMeter; //垂直分辨率
        int biClrUsed;       //颜色数
        int biClrImportant;  //重要颜色数
    } BitmapInfoHeader;

    typedef struct BitmapFile
    {

        BitmapFileHeader bitmapheader;
        BitmapInfoHeader bitmapinfoheader;
        float *R;
        float *G;
        float *B;

    } Bitmap;

    Image open(std::string fileName)
    {
        std::ifstream fin(fileName, std::ios::binary);
        unsigned char temp[4];
        Bitmap bitmap;
        Image bitmapMat;

        fin.read((char *)bitmap.bitmapheader.bfType, sizeof(char) * 2);
        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapheader.bfSize = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);
        fin.read((char *)temp, sizeof(short));
        bitmap.bitmapheader.bf1 = *temp + (*(temp + 1) << 8);
        fin.read((char *)temp, sizeof(short));
        bitmap.bitmapheader.bf2 = *temp + (*(temp + 1) << 8);
        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapheader.bfOffBits = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);

        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapinfoheader.biSize = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);

        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapinfoheader.biWidth = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);

        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapinfoheader.biHeight = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);

        fin.read((char *)temp, sizeof(short));
        bitmap.bitmapinfoheader.biPlanes = *temp + (*(temp + 1) << 8);

        fin.read((char *)temp, sizeof(short));
        bitmap.bitmapinfoheader.biBitCount = *temp + (*(temp + 1) << 8);

        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapinfoheader.biCompression = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);

        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapinfoheader.biSizeImage = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);

        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapinfoheader.biXPelsPerMeter = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);

        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapinfoheader.biYPelsPerMeter = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);

        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapinfoheader.biClrUsed = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);

        fin.read((char *)temp, sizeof(int));
        bitmap.bitmapinfoheader.biClrImportant = *temp + (*(temp + 1) << 8) + (*(temp + 2) << 16) + (*(temp + 3) << 24);
        //彩色图
        if (bitmap.bitmapinfoheader.biBitCount / 8 == 3)
        {
            //分配内存空间
            bitmap.R = new float[bitmap.bitmapinfoheader.biHeight * bitmap.bitmapinfoheader.biWidth];
            bitmap.G = new float[bitmap.bitmapinfoheader.biHeight * bitmap.bitmapinfoheader.biWidth];
            bitmap.B = new float[bitmap.bitmapinfoheader.biHeight * bitmap.bitmapinfoheader.biWidth];
            //根据偏移量读文件
            fin.seekg(bitmap.bitmapheader.bfOffBits, std::ios::beg);

            for (int i = 0; i < bitmap.bitmapinfoheader.biHeight; i++)
            {
                for (int j = 0; j < bitmap.bitmapinfoheader.biWidth; j++)
                {
                    fin.read((char *)temp, sizeof(char) * 3);
                    *(bitmap.B + bitmap.bitmapinfoheader.biWidth * (bitmap.bitmapinfoheader.biHeight - 1 - i) + j) = *temp;
                    *(bitmap.G + bitmap.bitmapinfoheader.biWidth * (bitmap.bitmapinfoheader.biHeight - 1 - i) + j) = *(temp + 1);
                    *(bitmap.R + bitmap.bitmapinfoheader.biWidth * (bitmap.bitmapinfoheader.biHeight - 1 - i) + j) = *(temp + 2);
                }
                if (bitmap.bitmapinfoheader.biWidth * 3 % 4 != 0)
                {
                    for (int j = 0; j < 4 - (bitmap.bitmapinfoheader.biWidth * 3 % 4); j++)
                    {
                        fin.read((char *)temp, sizeof(char));
                    }
                }
            }
            bitmapMat.channel = 3;
            bitmapMat.matrixR.setMatrix(bitmap.bitmapinfoheader.biWidth, bitmap.bitmapinfoheader.biHeight, bitmap.R);
            bitmapMat.matrixG.setMatrix(bitmap.bitmapinfoheader.biWidth, bitmap.bitmapinfoheader.biHeight, bitmap.G);
            bitmapMat.matrixB.setMatrix(bitmap.bitmapinfoheader.biWidth, bitmap.bitmapinfoheader.biHeight, bitmap.B);
        }
        else //灰色图
        {
            //分配内存空间
            bitmap.B = new float[bitmap.bitmapinfoheader.biHeight * bitmap.bitmapinfoheader.biWidth];
            //根据偏移量读文件
            fin.seekg(bitmap.bitmapheader.bfOffBits, std::ios::beg);

            for (int i = 0; i < bitmap.bitmapinfoheader.biHeight; i++)
            {
                for (int j = 0; j < bitmap.bitmapinfoheader.biWidth; j++)
                {
                    fin.read((char *)temp, sizeof(char));
                    *(bitmap.B + bitmap.bitmapinfoheader.biWidth * (bitmap.bitmapinfoheader.biHeight - 1 - i) + j) = *temp;
                }
                if (bitmap.bitmapinfoheader.biWidth % 4 != 0)
                {
                    for (int j = 0; j < 4 - (bitmap.bitmapinfoheader.biWidth % 4); j++)
                    {
                        fin.read((char *)temp, sizeof(char));
                    }
                }
            }
            bitmapMat.channel = 1;
            bitmapMat.matrixB.setMatrix(bitmap.bitmapinfoheader.biWidth, bitmap.bitmapinfoheader.biHeight, bitmap.B);
        }
        fin.close();
        return bitmapMat;
    }

    // 保存图片
    int save(std::string filename, Image bitmapMat)
    {
        std::ofstream fout(filename, std::ios::binary);
        unsigned char *temp;
        int p = 0; //指针
        //计算文件大小
        int bfSize = bitmapMat.channel * (bitmapMat.matrixB.width + 4 - (bitmapMat.matrixB.width * bitmapMat.channel % 4)) * bitmapMat.matrixB.height + 54;
        if (bitmapMat.channel == 1)
        {
            bfSize += 1024;
        }
        //计算偏移
        int bfOffset;
        if (bitmapMat.channel == 3)
        {
            bfOffset = 54;
        }
        else
        {
            bfOffset = 1024 + 54;
        }

        //计算每像素大小
        int biBitCount = bitmapMat.channel * 8;
        //计算图像大小
        int biSizeImage = bitmapMat.matrixB.width * bitmapMat.matrixB.height * bitmapMat.channel;
        //初始化temp
        temp = new unsigned char[bfSize];
        //写文件头
        *(temp + p++) = 'B';
        *(temp + p++) = 'M';
        *(temp + p++) = (unsigned char)bfSize;
        *(temp + p++) = (unsigned char)(bfSize >> 8);
        *(temp + p++) = (unsigned char)(bfSize >> 16);
        *(temp + p++) = (unsigned char)(bfSize >> 24);
        *(temp + p++) = 0;
        *(temp + p++) = 0;
        *(temp + p++) = 0;
        *(temp + p++) = 0;
        *(temp + p++) = (unsigned char)bfOffset;
        *(temp + p++) = (unsigned char)(bfOffset >> 8);
        *(temp + p++) = (unsigned char)(bfOffset >> 16);
        *(temp + p++) = (unsigned char)(bfOffset >> 24);
        //写位图信息
        *(temp + p++) = 40;
        *(temp + p++) = 0;
        *(temp + p++) = 0;
        *(temp + p++) = 0;
        *(temp + p++) = (unsigned char)bitmapMat.matrixB.width;
        *(temp + p++) = (unsigned char)(bitmapMat.matrixB.width >> 8);
        *(temp + p++) = (unsigned char)(bitmapMat.matrixB.width >> 16);
        *(temp + p++) = (unsigned char)(bitmapMat.matrixB.width >> 24);
        *(temp + p++) = (unsigned char)bitmapMat.matrixB.height;
        *(temp + p++) = (unsigned char)(bitmapMat.matrixB.height >> 8);
        *(temp + p++) = (unsigned char)(bitmapMat.matrixB.height >> 16);
        *(temp + p++) = (unsigned char)(bitmapMat.matrixB.height >> 24);
        *(temp + p++) = 1;
        *(temp + p++) = 0;
        *(temp + p++) = (unsigned char)biBitCount;
        *(temp + p++) = (unsigned char)(biBitCount >> 8);
        *(temp + p++) = 0;
        *(temp + p++) = 0;
        *(temp + p++) = 0;
        *(temp + p++) = 0;
        *(temp + p++) = (unsigned char)biSizeImage;
        *(temp + p++) = (unsigned char)(biSizeImage >> 8);
        *(temp + p++) = (unsigned char)(biSizeImage >> 16);
        *(temp + p++) = (unsigned char)(biSizeImage >> 24);
        for (int i = 0; i < 16; i++)
        {
            *(temp + p++) = 0;
        }
        //写图像内容
        if (bitmapMat.channel == 3)
        {
            for (int i = 0; i < bitmapMat.matrixB.height; i++)
            {
                for (int j = 0; j < bitmapMat.matrixB.width; j++)
                {
                    *(temp + p++) = (unsigned char)bitmapMat.matrixB.getPixel(bitmapMat.matrixB.height - i, j + 1);
                    *(temp + p++) = (unsigned char)bitmapMat.matrixG.getPixel(bitmapMat.matrixG.height - i, j + 1);
                    *(temp + p++) = (unsigned char)bitmapMat.matrixR.getPixel(bitmapMat.matrixR.height - i, j + 1);
                }
                if (bitmapMat.matrixB.width * 3 % 4 != 0)
                {
                    for (int j = 0; j < 4 - (bitmapMat.matrixB.width * 3 % 4); j++)
                    {
                        *(temp + p++) = 0;
                    }
                }
            }
        }
        else
        {
            //写调色板
            for (int i = 0; i < 256; i++)
            {
                *(temp + p++) = i;
                *(temp + p++) = i;
                *(temp + p++) = i;
                *(temp + p++) = 0;
            }
            for (int i = 0; i < bitmapMat.matrixB.height; i++)
            {
                for (int j = 0; j < bitmapMat.matrixB.width; j++)
                {
                    *(temp + p++) = (unsigned char)bitmapMat.matrixB.getPixel(bitmapMat.matrixB.height - i, j + 1);
                }
                if (bitmapMat.matrixB.width % 4 != 0)
                {
                    for (int j = 0; j < 4 - (bitmapMat.matrixB.width % 4); j++)
                    {
                        *(temp + p++) = 0;
                    }
                }
            }
        }
        fout.write((char *)temp, sizeof(unsigned char) * bfSize);
        delete[] temp;
        fout.close();
        return 0;
    }

    // 销毁
    int destory(Image imageMat)
    {
        if (imageMat.channel == 3)
        {
            imageMat.matrixB.destory();
            imageMat.matrixG.destory();
            imageMat.matrixR.destory();
        }
        else
        {
            imageMat.matrixB.destory();
        }
        return 0;
    }

    Image create(Matrix matrix)
    {
        Image dest;
        dest.channel = 1;
        dest.matrixB.create(matrix.width, matrix.height);
        memcpy(dest.matrixB.data, matrix.data, matrix.width * matrix.height * sizeof(float));
        return dest;
    }

} // namespace dip