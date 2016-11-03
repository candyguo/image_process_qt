#ifndef HARRIS_CORNERS_DETECTOR_H
#define HARRIS_CORNERS_DETECTOR_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class harris_corners_detector
{
public:
    harris_corners_detector();
    cv::Mat cornerStrength;
    cv::Mat cornerTh;
    // image of local maxima (internal)
    cv::Mat localMax;
    // size of neighborhood for derivatives smoothing
    int neighbourhood;
    // aperture for gradient computation
    int aperture;
    // Harris parameter
    double k;
    // maximum strength for threshold computation
    double maxStrength;
    // calculated threshold (internal)
    double threshold;
    // size of neighborhood for non-max suppression
    int nonMaxSize;
    // kernel for non-max suppression
    cv::Mat kernel;
    void detect(const cv::Mat& image);
    void getCorners(std::vector<cv::Point> &points,
    double qualityLevel);
    Mat getCornerMap(double qualityLevel);
    void getCorners(std::vector<cv::Point> &points,const cv::Mat& cornerMap);
    void drawOnImage(cv::Mat &image,const std::vector<cv::Point> &points,
    cv::Scalar color= cv::Scalar(255,255,255),int radius=3, int thickness=2);
};

#endif // HARRIS_CORNERS_DETECTOR_H
