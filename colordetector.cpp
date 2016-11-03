#include "colordetector.h"

ColorDetector::ColorDetector():minDist(100)
{
   target[0]=target[1]=target[2]=0;
}

void ColorDetector::setColorDistanceThreshold(int distance) {
if (distance<0)
{
    distance=0;
}
minDist= distance;
}

int ColorDetector::getColorDistanceThreshold() const {
return minDist;
}

void ColorDetector::setTargetColor(unsigned char blue, unsigned char green, unsigned char red) {
    // BGR order
    cv::Mat tmp(1,1,CV_8UC3);
    tmp.at<cv::Vec3b>(0,0)[0]= blue;
    tmp.at<cv::Vec3b>(0,0)[1]= green;
    tmp.at<cv::Vec3b>(0,0)[2]= red;
    // Converting the target to Lab color space
    cv::cvtColor(tmp, tmp, CV_BGR2Lab);
    target= tmp.at<cv::Vec3b>(0,0);//一个像素的颜色空间转化赋给目标像素
}

Vec3b ColorDetector::getTargetColor() const {
return target;
}

int ColorDetector::getDistance(const Vec3b& color){
    return abs(color[0]-target[0])+abs(color[1]-target[1])+abs(color[2]-target[2]);
   // return static_cast<int>(norm<int,3>(Vec3i(color[0]-target[0],color[1]-target[1],color[2]-target[2])));
}

Mat ColorDetector::process(const cv::Mat &image) {
    // re-allocate binary map if necessary
    // same size as input image, but 1-channel
    result.create(image.rows,image.cols,CV_8U);
    converted.create(image.rows,image.cols,image.type());
    cvtColor(image,converted,CV_BGR2Lab);
    cv::Mat_<cv::Vec3b>::const_iterator it=
            converted.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::const_iterator itend=
            converted.end<cv::Vec3b>();
    cv::Mat_<uchar>::iterator itout=
            result.begin<uchar>();
    // for each pixel
    for ( ; it!= itend; ++it, ++itout) {
        // process each pixel ---------------------
        // compute distance from target color
        if (getDistance(*it)<minDist) {
            *itout= 255;//小于tolerance的为白色即图像中的目标颜色
        } else {
            *itout= 0;
        }
        // end of pixel processing ----------------
    }
    return result;
}
