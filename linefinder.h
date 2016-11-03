#ifndef LINEFINDER_H
#define LINEFINDER_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class linefinder
{
public:
    linefinder();
    void setAccResolution(double dRho, double dTheta);
    void setMinVote(int minv);
    void setLineLengthAndGap(double length, double gap);
    void drawDetectedLines(cv::Mat &image,cv::Scalar color=cv::Scalar(255,255,255));
    vector<cv::Vec4i>findLines(cv::Mat& binary);
private:
    Mat img;
    vector<cv::Vec4i> lines;
    double deltaRho;
    double deltaTheta;
    // minimum number of votes that a line
    // must receive before being considered
    int minVote;
    // min length for a line
    double minLength;
    double maxGap;
};

#endif // LINEFINDER_H
