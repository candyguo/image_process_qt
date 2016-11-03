#include "morphological.h"
#include "ui_morphological.h"

morphological::morphological(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::morphological),
    threshold(-1),
    cross(5,5,CV_8U,cv::Scalar(0)),
    diamond(5,5,CV_8U,cv::Scalar(1)),
    square(5,5,CV_8U,cv::Scalar(1)),
    x(5,5,CV_8U,cv::Scalar(0))
{
    ui->setupUi(this);
    for (int i=0; i<5; i++) {
    cross.at<uchar>(2,i)= 1;
    cross.at<uchar>(i,2)= 1;
    }
    // Creating the diamond-shaped structuring element
    diamond.at<uchar>(0,0)= 0;
    diamond.at<uchar>(0,1)= 0;
    diamond.at<uchar>(1,0)= 0;
    diamond.at<uchar>(4,4)= 0;
    diamond.at<uchar>(3,4)= 0;
    diamond.at<uchar>(4,3)= 0;
    diamond.at<uchar>(4,0)= 0;
    diamond.at<uchar>(4,1)= 0;
    diamond.at<uchar>(3,0)= 0;
    diamond.at<uchar>(0,4)= 0;
    diamond.at<uchar>(0,3)= 0;
    diamond.at<uchar>(1,4)= 0;
    // Creating the x-shaped structuring element
    for (int i=0; i<5; i++) {
    x.at<uchar>(i,i)= 1;
    x.at<uchar>(4-i,i)= 1;
    }//创建structing shape
}

Mat morphological::getCorners(const cv::Mat &image) {
    cv::Mat result;
    // Dilate with a cross
    cv::dilate(image,result,cross);
    // Erode with a diamond
    cv::erode(result,result,diamond);
    cv::Mat result2;
    // Dilate with a X
    cv::dilate(image,result2,x);
    // Erode with a square
    cv::erode(result2,result2,square);
    // Corners are obtained by differencing
    // the two closed images
    cv::absdiff(result2,result,result);
    // Apply threshold to obtain a binary image
    applyThreshold(result);
    return result;
}

void morphological::show_picture()
{
    if(morpho_image.channels()==3)
    {
        QImage img= QImage((const unsigned char*)(morpho_image.data),
        morpho_image.cols,morpho_image.rows,QImage::Format_RGB888);
        // display on label
        ui->label->setPixmap(QPixmap::fromImage(img));
        // resize the label to fit the image
        ui->label->resize(ui->label->pixmap()->size());
    }
    else
    {
        QImage img= QImage((const unsigned char*)(morpho_image.data),
        morpho_image.cols,morpho_image.rows,QImage::Format_Grayscale8);
        // display on label
        ui->label->setPixmap(QPixmap::fromImage(img));
        // resize the label to fit the image
        ui->label->resize(ui->label->pixmap()->size());
    }
}

morphological::~morphological()
{
    delete ui;
}



void morphological::on_comboBox_activated(int index)
{
    Mat corners,morpho_image2,morpho_image3,morpho_image4,morpho_image5,morpho_imagetemp;
    Mat element5(5,5,CV_8U,Scalar(1));//开闭运算结构元素定义
    switch (index) {
    case 0:
        erode(morpho_image,morpho_image,Mat());
        morpho_imagetemp=morpho_image;
        break;
    case 1:
        dilate(morpho_image,morpho_image,Mat());
        morpho_imagetemp=morpho_image;
        break;
    case 2:
        cv::morphologyEx(morpho_image,morpho_image2,MORPH_OPEN,element5);
        morpho_imagetemp=morpho_image2;
        break;
    case 3:
        morphologyEx(morpho_image,morpho_image3,MORPH_CLOSE,element5);
        morpho_imagetemp=morpho_image3;
        break;
    case 4:
        int thres;
        thres=ui->verticalSlider->value();
        set_threshold(thres);//获取边缘提取的二值化阈值
        morpho_image4=getEdges(morpho_image);
        morpho_imagetemp=morpho_image4;
        break;
    case 5:
        morpho_image.copyTo(morpho_image5);
        corners=getCorners(morpho_image5);//得到的角点二值影像
        // Display the corner on the image
        drawOnImage(corners,morpho_image5);
        morpho_imagetemp=morpho_image5;
        break;
    default:
        break;
    }
    QImage img= QImage((const unsigned char*)(morpho_imagetemp.data),
    morpho_imagetemp.cols,morpho_imagetemp.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

Mat morphological::getEdges(Mat morpho_image) {
    // Get the gradient image
    if(morpho_image.channels()==3)
    {
         cvtColor(morpho_image,morpho_image,CV_BGR2GRAY);
    }
    cv::morphologyEx(morpho_image,morpho_image,
                     cv::MORPH_GRADIENT,cv::Mat());//形态梯度提取影像边缘
    // Apply threshold to obtain a binary image
    applyThreshold(morpho_image);
    return morpho_image;
}

void morphological::applyThreshold(cv::Mat& result) {
    // Apply threshold on result
    if (threshold>0)
        cv::threshold(result, result,
                      threshold, 255, cv::THRESH_BINARY);
}

void morphological::set_threshold(int thres)
{
    threshold=thres;
}

void morphological::drawOnImage(const cv::Mat& binary,cv::Mat& image)
{
    cv::Mat_<uchar>::const_iterator it=
            binary.begin<uchar>();
    cv::Mat_<uchar>::const_iterator itend=binary.end<uchar>();
    // for each pixel
    for (int i=0; it!= itend; ++it,++i) {
        if (!*it)
            cv::circle(image,
                       cv::Point(i%image.step,i/image.step),
                       5,cv::Scalar(255,0,0));
}
}
