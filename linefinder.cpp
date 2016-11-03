#include "linefinder.h"
#define PI 3.1415926

linefinder::linefinder():deltaRho(1), deltaTheta(PI/180),
    minVote(10), minLength(0.), maxGap(0.)
{

}

void linefinder::setAccResolution(double dRho, double dTheta) {
deltaRho= dRho;
deltaTheta= dTheta;
}//设置step

void linefinder::setMinVote(int minv) {
minVote= minv;
}

void linefinder::setLineLengthAndGap(double length, double gap) {
minLength= length;
maxGap= gap;
}

vector<cv::Vec4i> linefinder::findLines(cv::Mat& binary) {
    lines.clear();
    cv::HoughLinesP(binary,lines,
                    deltaRho, deltaTheta, minVote,
                    minLength, maxGap);
    return lines;
}

void linefinder::drawDetectedLines(cv::Mat &image,cv::Scalar color)
{
    std::vector<cv::Vec4i>::const_iterator it2=
    lines.begin();
    while (it2!=lines.end()) {
    cv::Point pt1((*it2)[0],(*it2)[1]);
    cv::Point pt2((*it2)[2],(*it2)[3]);
    cv::line( image, pt1, pt2, color);
    ++it2;
    }
}
