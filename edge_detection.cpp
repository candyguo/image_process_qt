#include "edge_detection.h"
#include "ui_edge_detection.h"
#include "qmessagebox.h"
#include "laplacian.h"

edge_detection::edge_detection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edge_detection)
{
    ui->setupUi(this);
    ui->textEdit->setPlainText("128");
}

edge_detection::~edge_detection()
{
    delete ui;
}

//void edge_detection::on_comboBox_activated(int index)
//{

//}

void edge_detection::on_pushButton_clicked()
{
    Mat imagetemp,sobelX,sobelY,sobel;
    if(ui->comboBox->currentIndex()==0)
    {
        QString str = ui->textEdit->toPlainText();
        int thres=str.toInt();
        Sobel(image,sobelX,CV_16S,1,0);
        Sobel(image,sobelY,CV_16S,0,1);//
        //compute the L1 norm
        sobel= abs(sobelX)+abs(sobelY);
        double sobmin, sobmax;
        cv::minMaxLoc(sobel,&sobmin,&sobmax);//可以使用不同的kernel/mask
        // Conversion to 8-bit image
        // sobelImage = -alpha*sobel + 255
        cv::Mat sobelImage;
        sobel.convertTo(sobelImage,CV_8U,-255./sobmax,255);//获取最大最小值进行灰度的拉伸变换
        imagetemp=sobelImage;
        threshold(imagetemp,imagetemp,thres,255,cv::THRESH_BINARY);
    }
    else if(ui->comboBox->currentIndex()==1)
    {
        QString str = ui->textEdit->toPlainText();
        int aperture=str.toInt();
        laplacian lp;
        lp.set_aperture(aperture);
        lp.computeLaplacian(image);
        imagetemp=lp.getLaplacianImage();
        lp.lapla=imagetemp;
        imagetemp=lp.getZeroCrossings();//对变换的lapla影像获取zerocrossing..
    }
    else if(ui->comboBox->currentIndex()==2)
    {
        QString str = ui->textEdit->toPlainText();
        int thres=str.toInt();
        linefinder lf;
        lf.setLineLengthAndGap(100,30);
        lf.setMinVote(500);
        // Detect lines and draw them
        threshold(image,image,thres,255,cv::THRESH_BINARY);
        lf.findLines(image);
        lf.drawDetectedLines(image);
        imagetemp=image;
    }
    QImage img= QImage((const unsigned char*)(imagetemp.data),
    imagetemp.cols,imagetemp.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}


