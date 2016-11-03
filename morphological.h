#ifndef MORPHOLOGICAL_H
#define MORPHOLOGICAL_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

#include <QDialog>

namespace Ui {
class morphological;
}

class morphological : public QDialog
{
    Q_OBJECT

public:
    explicit morphological(QWidget *parent = 0);
    ~morphological();
    Mat morpho_image;
    morphological* get_currentui();
    void show_picture();
    Mat getEdges(Mat morpho_image);
    void applyThreshold(cv::Mat& result);
    void set_threshold(int thres);
    Mat getCorners(const cv::Mat &image);
    void drawOnImage(const cv::Mat& binary,cv::Mat& image);
private slots:


    void on_comboBox_activated(int index);

private:
    Ui::morphological *ui;
    // threshold to produce binary image
    int threshold;
    // structuring elements used in corner detection
    Mat cross;
    Mat diamond;
    Mat square;
    Mat x;
};

#endif // MORPHOLOGICAL_H
