#ifndef HISTOGRAM1D_H
#define HISTOGRAM1D_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class Histogram1D
{
private:
    int histSize[1]; // number of bins
    float hranges[2]; // min and max pixel value
    const float* ranges[1];
    int channels[1]; // only 1 channel used here
public:
    Histogram1D();
    MatND getHistogram(const cv::Mat &image);//通过成员变量参数的赋值，获取影像直方图
    Mat getHistogramImage(const cv::Mat &image);//获取直方图后，line画线绘制直方图
    Mat applyLookUp(const Mat& image,const Mat& lookup);
};

#endif // HISTOGRAM1D_H
