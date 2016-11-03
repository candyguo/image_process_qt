#include "video_dialog.h"
#include "ui_video_dialog.h"
#include "QMessageBox"

video_dialog::video_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::video_dialog)
{
    ui->setupUi(this);
}

video_dialog::~video_dialog()
{
    delete ui;
}

void video_dialog::on_pushButton_clicked()
{
    connect(&theTimer, &QTimer::timeout, this, &video_dialog::updateImage);
        //从摄像头捕获视频
    QString filename = QFileDialog::getOpenFileName(this,
     tr("Open Video"), ".",
      tr("Video Files (*.avi *.mp4 *.rmvb)"));
     // Open the video file
    if(ui->radioButton->close())
    {
        videoCap.open(0);
    }
    else
    {
        videoCap.open(filename.toLatin1().data());
    }
      double rate = videoCap.get(CV_CAP_PROP_FPS);
      int delay = 1000/rate;
      // check if video successfully opened
      if(!videoCap.isOpened())
      {
           QMessageBox::critical(this, tr("Error"), tr("Could not open video"));
           return;//弹出消息框后再次打开当前窗口
      }
      else
        {
            srcImage = Mat::zeros(videoCap.get(CV_CAP_PROP_FRAME_HEIGHT), videoCap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
            theTimer.start();
            //theTimer.interval()=delay;给时钟设定刷新频率以正确的播放视频
        }
}

void video_dialog::paintEvent(QPaintEvent *e)
{
    QImage image2 = QImage((uchar*)(srcImage.data), srcImage.cols, srcImage.rows, QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(image2));
    ui->label->resize(image2.size());
    ui->label->show();
}

void video_dialog::updateImage()
{
    videoCap.read(srcImage);
    if(srcImage.data)
    {
        cvtColor(srcImage, srcImage, CV_BGR2RGB);//Qt中支持的是RGB图像, OpenCV中支持的是BGR
        this->update();  //发送刷新消息
    }
}
