#include "camera_caliabration.h"
#include "ui_camera_caliabration.h"

camera_caliabration::camera_caliabration(QWidget *parent) :
    QDialog(parent),flag(0),mustInitUndistort(true),
    ui(new Ui::camera_caliabration)
{
    ui->setupUi(this);
}

camera_caliabration::~camera_caliabration()
{
    delete ui;
}

/*int camera_caliabration::addChessboardPoints(const std::vector<std::string>& filelist,cv::Size & boardSize) {
    // the points on the chessboard
    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;
    // 3D Scene Points:
    // Initialize the chessboard corners
    // in the chessboard reference frame
    // The corners are at 3D location (X,Y,Z)= (i,j,0)
    for (int i=0; i<boardSize.height; i++) {
        for (int j=0; j<boardSize.width; j++) {
            objectCorners.push_back(cv::Point3f(i, j, 0.0f));
        }
    }
    cv::Mat image; // to contain chessboard image
    int successes = 0;
    // for all viewpoints
    for (int i=0; i<filelist.size(); i++) {
        // Open the image
        image = cv::imread(filelist[i],0);
        // Get the chessboard corners
        bool found = cv::findChessboardCorners(
                    image, boardSize, imageCorners);
        // Get subpixel accuracy on the corners
        cv::cornerSubPix(image, imageCorners,
                         cv::Size(5,5),
                         cv::Size(-1,-1),
                         cv::TermCriteria(cv::TermCriteria::MAX_ITER +
                                          cv::TermCriteria::EPS,
                                          30, // max number of iterations
                                          0.1)); // min accuracy
        //If we have a good board, add it to our data
        if (imageCorners.size() == boardSize.area()) {
            // Add image and scene points from one view
            addPoints(imageCorners, objectCorners);
            successes++;
        }
    }
    return successes;
}*/

/*void camera_caliabration::addPoints(const std::vector<cv::Point2f>&
imageCorners, const std::vector<cv::Point3f>& objectCorners) {
// 2D image points from one view
imagePoints.push_back(imageCorners);
// corresponding 3D scene points
objectPoints.push_back(objectCorners);
}

double camera_caliabration::calibrate(cv::Size &imageSize)
{
// undistorter must be reinitialized
mustInitUndistort= true;
//Output rotations and translations
std::vector<cv::Mat> rvecs, tvecs;
// start calibration
return calibrateCamera(objectPoints, // the 3D points
                imagePoints, // the image points
                imageSize, // image size
                cameraMatrix, // output camera matrix
                distCoeffs, // output distortion matrix
                rvecs, tvecs, // Rs, Ts
                flag); // set options
                }*/

cv::Mat camera_caliabration::remap(const cv::Mat &image) {
    cv::Mat undistorted;
    if (mustInitUndistort) { // called once per calibration
        cv::initUndistortRectifyMap(
                    cameraMatrix, // computed camera matrix
                    distCoeffs, // computed distortion matrix
                    cv::Mat(), // optional rectification (none)
                    cv::Mat(), // camera matrix to generate undistorted
                    image.size(), // size of undistorted
                    CV_32FC1, // type of output map
                    map1, map2); // the x and y mapping functions
        mustInitUndistort= false;
    }
    cv::remap(image, undistorted, map1, map2,
              cv::INTER_LINEAR); // interpolation type
    return undistorted;
}

