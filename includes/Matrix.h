////////////////////////////////////////////////////////////////
//FileName:Matrix.h//Creator:WQ
//Date:2016年10月30日00:33:21
//Comment:矩阵的基本操作
//
///////////////////////////////////////////////////////////////
#ifndef MATRIX_H
#define MATRIX_H
#include <string.h>
#include <cfloat>

namespace dip
{
    class Matrix
    {
    public:
        Matrix();
        Matrix(int rows, int cols, float *data);
        ~Matrix();
        int width;
        int height;
        float *data;
        // 设置矩阵
        int setMatrix(int width, int height, float *data);
        // 得到某个像素的值
        float getPixel(int row, int col);
        // 设置某个像素的值
        int setPixel(int row, int col, float data);
        // 获取某一行，需要释放内存
        float *getRows(int row);
        // 获取一列，需要释放内存，慢
        float *getCols(int col);
        // 设置某一行
        int setRows(int row, float *data);
        // 设置某一列
        int setCols(int col, float *data);
        // 转置
        Matrix transpose();
        // 矩阵相加
        Matrix add(Matrix matrix);
        // 点乘
        Matrix dot(Matrix matrix);
        // 点乘
        Matrix dot(float k);
        // 相乘
        Matrix mul(Matrix matrix);
        // 矩阵相减
        Matrix sub(Matrix matrix);
        // 创建矩阵，分配内存
        int create(int width, int height);
        // 创建矩阵，分配内存
        int create(int width, int height, float fillcolor);
        // 复制矩阵
        Matrix copy();
        // 销毁
        int destory();
        // 获取某一行
        int getRows(int row, float *data);
        // 获取某行一定范围的值
        int getRowsPart(int row, float *data, int start, int end);
        // Strassen算法相乘
        Matrix mulFast(Matrix matrix);
        Matrix cut(int startrow, int endrow, int startcol, int endcol);
        //Matrix& operator=(Matrix mat);
        void operator=(const Matrix &mat);
        Matrix expand(int newWidth, int newHeight);
        Matrix normalization(int max, int min);
    };
}; // namespace dip
#endif
