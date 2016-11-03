#include "threshold_dialog.h"
#include "ui_threshold_dialog.h"
#include "QMessageBox"
#include "mainwindow.h"

threshold_Dialog::threshold_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::threshold_Dialog)
{
    ui->setupUi(this);
}

threshold_Dialog::~threshold_Dialog()
{
    delete ui;
}


void threshold_Dialog::on_horizontalSlider_sliderReleased()
{
    //QMessageBox::critical(this,tr("ss"),tr("aa"));滑条释放
    Mat thres_image;
    int thres=ui->horizontalSlider->value();
    if(threshold_image.channels()==3)
    {
        QMessageBox::critical(this,tr("warninfg"),tr("binary result image's source image must be gray image"));
        return;
    }
    threshold(threshold_image,thres_image,thres,255,cv::THRESH_BINARY);
    QImage img= QImage((const unsigned char*)(thres_image.data),
    thres_image.cols,thres_image.rows,QImage::Format_Grayscale8);
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}
