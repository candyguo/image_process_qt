#ifndef EDGE_DETECTION_H
#define EDGE_DETECTION_H

#include <QDialog>
#include "linefinder.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

namespace Ui {
class edge_detection;
}

class edge_detection : public QDialog
{
    Q_OBJECT

public:
    explicit edge_detection(QWidget *parent = 0);
    ~edge_detection();
    Mat image;

private slots:
    //void on_comboBox_activated(int index);
    void on_pushButton_clicked();

private:
    Ui::edge_detection *ui;
};

#endif // EDGE_DETECTION_H
