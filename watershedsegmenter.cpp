#include "watershedsegmenter.h"

WatershedSegmenter::WatershedSegmenter()
{

}

void WatershedSegmenter::setMarkers(const cv::Mat& markerImage) {
// Convert to image of ints
markerImage.convertTo(markers,CV_32S);//设置32位整型的label marker
}

cv::Mat WatershedSegmenter::process(const cv::Mat &image) {
// Apply watershed
image.convertTo(image,CV_8U,255,255);
cv::watershed(image,markers);
return markers;
}
