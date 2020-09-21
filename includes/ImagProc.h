#ifndef IMAGPROC_H
#define IMAGPROC_H
#include <Matrix.h>
#include <ImageBase.h>
#include <math.h>
#include <Utils.h>
#include <Image.h>
#include <FreCal.h>
#include <ImageFeature.h>

#define DIP_PI       3.14159265358979323846f
#define DIP_E        2.718281828459f
class ImagProc {
public:
    ImagProc();
    ~ImagProc();
    // 彩色图像灰度化
    Image rgbToGray(Image imageMat);
    // 图像镜像,mode=0表示水平镜像，=1表示垂直镜像
    Image mirror(Image imageMat, int mode=0);
    // 图像旋转
    Image rotate(Image imageMat, float degree, float fillcolor=0, int mode=1);
    // 平移
    Image translation(Image imageMat, int dx, int dy, float fillcolor=0);
    Image cut(Image imageMat, int startrow, int startcol, int endrow, int endcol);
    // 缩放,mode=1：最近邻，mode=2：双线性插值，mode=3：三次内插值
    Image zoom(Image imageMat, float scalex, float scaley, int mode=1);
private:
    // 近似sin(pi*x)/(pi*x)
    float sinpixdivpix(float x);
public:
	// 滤波
	// --平滑--
	// 1.均值
	// 2.梯度倒数加权平滑
	// 3.超限邻域平均法
	// 4.噪声门限法
	// 5.中值滤波
	// 6.超限中值滤波
	// --锐化--
	// 7.Roberts
	// 8.Prewitt
	// 9.Sobel
	// 10.Laplace
	Image filter(Image imageMat, int type = 1, int size = 3, int T = 0);
	// 返回一个滤波窗口的值
	// --平滑--
	// 1.均值
	// 2.梯度倒数加权平滑
	// 3.超限邻域平均法
	// 4.噪声门限法
	// 5.中值滤波
	// 6.超限中值滤波
	// --锐化--
	// 7.Roberts
	// 8.Prewitt
	// 9.Sobel
	// 10.Laplace
	float getFilterWindow(int type, Matrix info, int size=3, int T=0);
    // 理想低通滤波
    Image idealLowPassFilter(Image src, float D0, int n);
    // 巴特沃斯低通滤波
    Image butterworthFilter(Image src, float D0, int n);
    // 高斯低通滤波
    Image gaussLowPassFilter(Image src, float D0, int n);
    // 指数滤波,n>0低通,n<0高通
    Image expFilter(Image src, float D0, int n);
    // 梯形滤波    
    Image trapeziumFilter(Image src, float D0, float D1, int n);
    // 图像二值化
    Image im2bw(Image src, float t);
    // 图像二值化（最大类间方差法）
    Image im2bw(Image src);
    int ostu(Matrix src);
    // 直方图均衡化
    Image histEqualization(Image src);
};
#endif

