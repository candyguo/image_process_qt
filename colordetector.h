#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class ColorDetector
{
public:
    ColorDetector();
    void setColorDistanceThreshold(int distance);
    int getColorDistanceThreshold() const;
    void setTargetColor(unsigned char blue,unsigned char green,unsigned char red);
    Vec3b getTargetColor() const;
    int getDistance(const Vec3b& color);
    Mat process(const cv::Mat &image);
    // image containing resulting binary map
    Mat result,converted;
private:
// minimum acceptable distance
int minDist;
// target color
Vec3b target;
};

#endif // COLORDETECTOR_H
