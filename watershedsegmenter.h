#ifndef WATERSHEDSEGMENTER_H
#define WATERSHEDSEGMENTER_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;


class WatershedSegmenter
{
public:
    WatershedSegmenter();
    void setMarkers(const cv::Mat& markerImage);
    cv::Mat process(const cv::Mat &image);
private:
    cv::Mat markers;
};

#endif // WATERSHEDSEGMENTER_H
