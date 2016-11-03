#ifndef ROUBST_MATCHER_H
#define ROUBST_MATCHER_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
using namespace cv;
using namespace std;

class roubst_matcher
{
public:
    roubst_matcher();
    void setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect);
    void setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc);
    Mat match(cv::Mat& image1,
    cv::Mat& image2, // input images
    // output matches and keypoints
    std::vector<cv::DMatch>& matches,
    std::vector<cv::KeyPoint>& keypoints1,
    std::vector<cv::KeyPoint>& keypoints2);//双像匹配，输出匹配结果以及key point
    int ratioTest(std::vector<std::vector<cv::DMatch>>
    &matches);
    void symmetryTest(
    const std::vector<std::vector<cv::DMatch>>& matches1,
    const std::vector<std::vector<cv::DMatch>>& matches2,
    std::vector<cv::DMatch>& symMatches);
    cv::Mat ransacTest(
    const std::vector<cv::DMatch>& matches,
    const std::vector<cv::KeyPoint>& keypoints1,
    const std::vector<cv::KeyPoint>& keypoints2,
    std::vector<cv::DMatch>& outMatches);
private:
    // pointer to the feature point detector object
    cv::Ptr<cv::FeatureDetector> detector;
    // pointer to the feature descriptor extractor object
    cv::Ptr<cv::DescriptorExtractor> extractor;
    float ratio; // max ratio between 1st and 2nd NN
    bool refineF; // if true will refine the F matrix
    double distance; // min distance to epipolar
    double confidence; // confidence level (probability)
};

#endif // ROUBST_MATCHER_H
