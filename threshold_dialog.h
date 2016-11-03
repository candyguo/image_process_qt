#ifndef THRESHOLD_DIALOG_H
#define THRESHOLD_DIALOG_H

#include <QDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
namespace Ui {
class threshold_Dialog;
}

class threshold_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit threshold_Dialog(QWidget *parent = 0);
    ~threshold_Dialog();
    Mat threshold_image;

private slots:

    void on_horizontalSlider_sliderReleased();

private:
    Ui::threshold_Dialog *ui;
};

#endif // THRESHOLD_DIALOG_H
