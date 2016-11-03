#ifndef LAPLACIAN_H
#define LAPLACIAN_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class laplacian
{
public:
    laplacian();
    void set_aperture(int a);
    Mat computeLaplacian(const cv::Mat& image);
    Mat getLaplacianImage(double scale=-1.0);
    Mat getZeroCrossings(float threshold=1.0);
    Mat lapla;//变换影像
private:
    Mat image_lp;
    int aperture;

};

#endif // LAPLACIAN_H
