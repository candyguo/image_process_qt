#ifndef CAMERA_CALIABRATION_H
#define CAMERA_CALIABRATION_H

#include <QDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

namespace Ui {
class camera_caliabration;
}

class camera_caliabration : public QDialog
{
    Q_OBJECT

public:
    explicit camera_caliabration(QWidget *parent = 0);
    ~camera_caliabration();
    vector<std::vector<cv::Point3f>> objectPoints;
    // the point positions in pixels
    vector<std::vector<cv::Point2f>> imagePoints;
    // output Matrices
    Mat cameraMatrix;
    Mat distCoeffs;
    // flag to specify how calibration is done
    int flag;
    // used in image undistortion
    Mat map1,map2;
    bool mustInitUndistort;
    int addChessboardPoints(const vector<std::string>& filelist,Size & boardSize);
    void addPoints(const std::vector<cv::Point2f>&imageCorners, const std::vector<cv::Point3f>& objectCorners);
    double calibrate(cv::Size &imageSize);
    Mat remap(const cv::Mat &image);
    Mat leftimage,rightimage;

private:
    Ui::camera_caliabration *ui;
};

#endif // CAMERA_CALIABRATION_H
