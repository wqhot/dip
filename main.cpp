#include <iostream>
#include <dipInclude.h>
#include <tic_toc.h>
#include <opencv2/opencv.hpp>

int main()
{
    std::cout << "hello" << std::endl;
    TicToc tic;
    dip::Image img = dip::open("/home/wangq/zntk/dip/init_img0.bmp");
    tic.toc_print("open");
    tic.tic();
    img = dip::gaussLowPassFilter(img, 30, -1);
    tic.toc_print("gaussLowPassFilter");
    TicToc tic2;
    cv::Mat img2 = cv::imread("/home/wangq/zntk/dip/init_img0.bmp");
    tic2.toc_print("open2");
    std::cout << img.matrixB.height << std::endl;

    return 0;
}