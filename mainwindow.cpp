#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "colordetector.h"
#include "threshold_dialog.h"
#include <QMessageBox>
#include <QColorDialog>
#include "histogram1d.h"
#include "morphological.h"
#include "watershedsegmenter.h"
#include "edge_detection.h"
#include "harris_corners_detector.h"
#include "camera_caliabration.h"
#include "video_dialog.h"

struct drawbox
{
    Point2d point1;
    Point2d point2;
    Mat image;
    Mat temp;
    bool isDraw;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->save_imageButton_3->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this,
    tr("Open Image"), ".",
    tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if(filename.isEmpty())
    {
         QMessageBox::critical(this, tr("Error"), tr("Could not open image"));
         return;//弹出消息框后再次打开当前窗口
    }
    image=imread(filename.toLatin1().data(),1);
    if(image.data)
    {
        ui->save_imageButton_3->setEnabled(true);
    }
    cvtColor(image,image,CV_BGR2RGB);//转成qimage的rgb图像
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::flip_h()
{

    flip(image,image,1);
    if(image.channels()==3)
    {
        img= QImage((const unsigned char*)(image.data),
        image.cols,image.rows,QImage::Format_RGB888);
    }
    else
    {
        img= QImage((const unsigned char*)(image.data),
        image.cols,image.rows,QImage::Format_Grayscale8);
    }
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());//转化成qimage在控件里进行显示
}

void MainWindow::on_actionSalt_pepper_triggered()//增加0.05比列的椒盐
{
    for (int k=0; k<image.total()*0.05; k++)
    {
    // rand() is the MFC random number generator
    // try qrand() with Qt
    int i= qrand()%image.cols;
    int j= qrand()%image.rows;
    if (image.channels() == 1)
    { // gray-level image
    image.at<uchar>(j,i)= 255;//先行在列
    }
    else if (image.channels() == 3)
    {
    image.at<cv::Vec3b>(j,i)[0]= 255;
    image.at<cv::Vec3b>(j,i)[1]= 255;
    image.at<cv::Vec3b>(j,i)[2]= 255;
    }
    }
    if(image.channels()==3)
    {
        img= QImage((const unsigned char*)(image.data),
        image.cols,image.rows,QImage::Format_RGB888);
    }
    else
    {
        img= QImage((const unsigned char*)(image.data),
        image.cols,image.rows,QImage::Format_Grayscale8);
    }
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_save_imageButton_3_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), QString(),
         tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));//获取图像文件保存路径
    Mat save_image;
    image.copyTo(save_image);//保存影像是成员变量影像的副本
    cvtColor(save_image,save_image,CV_BGR2RGB);
    if (!fileName.isEmpty())
    {
       imwrite(fileName.toLatin1().data(),save_image);
    }
    else{
        QMessageBox::critical(this,tr("save image"),tr("please input the path to save"));
        return;
    }

}

void MainWindow::on_actionReduce_color_triggered()
{
    int div=32;
    int nl= image.rows; // number of lines
    // total number of elements per line
    int nc= image.cols * image.channels();
    for (int j=0; j<nl; j++) {
    // get the address of row j
    uchar* data= image.ptr<uchar>(j);//data是指向每一行的元素数组名，指针
    for (int i=0; i<nc; i++) {
    data[i]= data[i]/div*div + div/2;//利用除法的取整来降低颜色数
    }
    }//对效率要求较高时，可以采用掩膜位运算的方法
//    Mat image(256,256,CV_8U,Scalar(0,0,0));
//    int nl= image.rows; // number of lines
//    int nc= image.cols;
//    for (int j=0; j<nc; j++)
//    {
//    for (int i=0; i<nl; i++) {
//     image.at<uchar>(i,j)=j;//利用除法的取整来降低颜色数
//    }
//    }
    if(image.channels()==3)
    {
        img= QImage((const unsigned char*)(image.data),
        image.cols,image.rows,QImage::Format_RGB888);
    }
    else
    {
        img= QImage((const unsigned char*)(image.data),
        image.cols,image.rows,QImage::Format_Grayscale8);
    }
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
 }
/*遍历图像的最快方式
 * void colorReduce(cv::Mat &image, int div=64) {
int nl= image.rows; // number of lines
int nc= image.cols ; // number of columns
// is it a continous image?
if (image.isContinuous()) {
// then no padded pixels
nc= nc*nl;
nl= 1; // it is now a 1D array
}
int n= static_cast<int>(
log(static_cast<double>(div))/log(2.0));
// mask used to round the pixel value
uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
// for all pixels
for (int j=0; j<nl; j++) {
// pointer to first column of line j
uchar* data= image.ptr<uchar>(j);
for (int i=0; i<nc; i++) {
*data++= *data&mask + div/2;
*data++= *data&mask + div/2;
*data++= *data&mask + div/2;
// end of pixel processing ----------------
} // end of line
}
}*/



void MainWindow::on_actionImage_gray_triggered()
{
//    Mat gray_image(image.rows,image.cols,CV_8UC1);
//    for(int i=0;i<image.rows;i++)
//    {
//        for(int j=0;j<image.cols;j++)
//        {
//            gray_image.at<uchar>(i,j)=image.at<cv::Vec3b>(i,j)[0];
//        }
//    }
//    image=gray_image;
    cvtColor(image,image,CV_RGB2GRAY);
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_actionSharpen_triggered()//针对灰度图像的影像锐化
{
    if(image.channels()==3)
    {
        QMessageBox::critical(this,tr("warning"),tr("image's channel must be 1"));
    }
    Mat result;
    result.create(image.size(), image.type());
    for (int j= 1; j<image.rows-1; j++) { // for all rows
    // (except first and last)
    const uchar* previous=image.ptr<const uchar>(j-1); // previous row
    const uchar* current=image.ptr<const uchar>(j); // current row
    const uchar* next=image.ptr<const uchar>(j+1); // next row
    uchar* output= result.ptr<uchar>(j); // output row
    for (int i=1; i<image.cols-1; i++) {
    *output++= cv::saturate_cast<uchar>(5*current[i]-current[i-1]-current[i+1]-previous[i]-next[i]);
    }
    }
    // Set the unprocess pixels to 0
    result.row(0).setTo(cv::Scalar(0));
    result.row(result.rows-1).setTo(cv::Scalar(0));
    result.col(0).setTo(cv::Scalar(0));
    result.col(result.cols-1).setTo(cv::Scalar(0));
    image=result;
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
    /*锐化的卷积实现，核矩阵定义与影像滤波
     * void sharpen2D(const cv::Mat &image, cv::Mat &result) {
// Construct kernel (all entries initialized to 0)
cv::Mat kernel(3,3,CV_32F,cv::Scalar(0));
// assigns kernel values
kernel.at<float>(1,1)= 5.0;
kernel.at<float>(0,1)= -1.0;
kernel.at<float>(2,1)= -1.0;
kernel.at<float>(1,0)= -1.0;
kernel.at<float>(1,2)= -1.0;
//filter the image
cv::filter2D(image,result,image.depth(),kernel);
} */
}




void MainWindow::on_actionChannel_split_B_triggered()
{
    vector<Mat> planes;
    if(image.channels()==1)
    {
        QMessageBox::critical(this,tr("warning"),tr("current gray image can't be splited"));
        return;
    }
    split(image,planes);
    image=planes[0];
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_actionChannel_split_G_triggered()
{
    vector<Mat> planes;
    if(image.channels()==1)
    {
        QMessageBox::critical(this,tr("warning"),tr("current gray image can't be splited"));
        return;
    }
    split(image,planes);
    image=planes[1];
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_actionCahnnel_split_R_triggered()
{
    vector<Mat> planes;
    if(image.channels()==1)
    {
        QMessageBox::critical(this,tr("warning"),tr("current gray image can't be splited"));
        return;
    }
    split(image,planes);
    image=planes[2];
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_actionColor_detection_triggered()
{
    if(image.channels()==1)
    {
        return;
    }
    ColorDetector cd;
    QColor color = QColorDialog::getColor(Qt::green, this);
    if (color.isValid()) {
//    ColorDetectController::getInstance()
//    ->setTargetColor(color.red(),color.green(),color.blue());
      cd.setTargetColor(color.red(),color.green(),color.blue());
    }
    //cd.setTargetColor(130,190,230);
    cd.process(image);
    image=cd.result;
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_actionImage_histogram_triggered()//灰度影像直方图
{
    if(image.channels()==3)
    {
        return;
    }
    Mat histimage;
    Histogram1D image_hist;
    histimage=image_hist.getHistogramImage(image);
    imshow("影像直方图",histimage);
}

void MainWindow::on_actionThreshold_image_triggered()
{
    threshold_Dialog thd;
    thd.threshold_image=image;//构造子窗口类，将主窗口的成员变量传给子对话框的成员变量
    thd.exec();
}

Mat MainWindow::getcurrent_image()
{
    return image;
}

void MainWindow::on_actionHorizontal_flip_triggered()
{
    flip_h();
}


void MainWindow::on_actionLookup_inverse_triggered()
{
    int dim(256);
    cv::Mat lut(1, // 1 dimension
    &dim, // 256 entries
    CV_8U); // uchar
    for (int i=0; i<256; i++) {
    lut.at<uchar>(i)= 255-i;
    }
    Histogram1D hg;
    image=hg.applyLookUp(image,lut);
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_actionStretch_histogram_triggered()
{
    Histogram1D hg;
    MatND hist= hg.getHistogram(image);
    int imin= 0,minValue=10;
    for( ; imin < 256; imin++ ) {
    if (hist.at<float>(imin) > minValue){
        break;//直方图某一灰度值的个数
    }
    }
    int imax= 255;
    for( ; imax >= 0; imax-- ) {
    if (hist.at<float>(imax) > minValue){ break;}
    }
    int dim(256);
    cv::Mat lookup(1,&dim,CV_8U); // uchar
    for (int i=0; i<256; i++) {
    // stretch between imin and imax
    if (i < imin) { lookup.at<uchar>(i)= 0;}
    else if (i > imax) {
        lookup.at<uchar>(i)= 255;}
    else {
        lookup.at<uchar>(i)= static_cast<uchar>(
    255.0*(i-imin)/(imax-imin)+0.5);}//像素拉伸
    }
    // Apply lookup table
    image= hg.applyLookUp(image,lookup);
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_actionHistogram_equalization_triggered()
{
    equalizeHist(image,image);
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_Grayscale8);
    // display on label
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::MouseCallback(int mouseEvent,int x,int y,int flags,void* param)
{
    drawbox* box = (drawbox*)param;
    switch(mouseEvent)
    {
    case CV_EVENT_LBUTTONDOWN:
        box->point1 = Point(x,y);
        box->point2 = Point(x,y);
        box->isDraw = true;
        break;
    case CV_EVENT_MOUSEMOVE:
        box->point2 = Point(x,y);
        break;
    case CV_EVENT_LBUTTONUP:
        //char tmp[20];
        box->point2 = Point(x,y);
        if(box->point1==box->point2)
            box->isDraw=false;
        else
        {
        rectangle(box->image,box->point1,box->point2,Scalar(255,255,255));
        Mat imageROI=box->image(Rect(box->point1,box->point2));//感兴趣区域绘制并提取
        //pyrUp(imageROI,imageROI,Size(imageROI.cols*2,imageROI.rows*2));
//        sprintf(tmp,"(%.2f,%.2f)",box->point2.x,box->point2.y);
//        Point org(box->point2.x,box->point2.y);
//        putText(box->image,tmp,org,CV_FONT_HERSHEY_COMPLEX_SMALL,0.9,Scalar(235,206,135));
        }
        box->isDraw = false;
        break;
    }
}


void MainWindow::on_actionRegion_extration_triggered()
{

    namedWindow("区域提取窗口",0);
    //imshow("目标点识别窗口",scrimage);//图像载入处理
    drawbox box;
    box.point1 = Point(0,0);
    box.point2 = Point(0,0);

    box.image = image;
    box.temp = box.image.clone();
    box.isDraw = false;
    Mat result; // segmentation (4 possible values)
    Mat bgModel,fgModel; // the models (internally used)
    // GrabCut segmentation
    cvSetMouseCallback("区域提取窗口",MouseCallback,&box); //设置鼠标响应以及响应的实现
    Mat foreground(image.size(),CV_8UC3, cv::Scalar(255,255,255));

//    Rect rect1(10,100,380,180);
//    cv::grabCut(image, result,rect1, bgModel,fgModel,5, cv::GC_INIT_WITH_RECT);
//    compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
//    image.copyTo(foreground,result);
//    result= result&1;
//    imshow("grab cut",foreground);
    Rect rect1;
    while(1)
    {
        //box.temp = box.image;
        box.temp = box.image.clone();
        if(box.isDraw)
        {
            rectangle(box.temp,box.point1,box.point2,Scalar(0,255,0));
            rect1=Rect(box.point1.x,box.point1.y,box.point2.x,box.point2.y);
            cv::grabCut(image, result,rect1, bgModel,fgModel,5, cv::GC_INIT_WITH_RECT);
            compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
            image.copyTo(foreground,result);
            result= result&1;
            imshow("grab cut",foreground);
            if(cvWaitKey(30) == 27)  break;
        }
        imshow("区域提取窗口",box.temp);
        if(cvWaitKey(30) == 27)  break; //按下esc退出程序
    }
}

void MainWindow::on_pushButton_2_clicked()//reload image
{
    image=imread(filename.toLatin1().data(),1);
    if(image.data)
    {
        ui->save_imageButton_3->setEnabled(true);
    }
    cvtColor(image,image,CV_BGR2RGB);//转成qimage的rgb图像
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_RGB888);
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_actionMorphological_image_triggered()
{
    morphological mp;
    mp.morpho_image=image;
    mp.show_picture();
    mp.exec();
}

void MainWindow::on_actionWatershed_segment_triggered()
{
    Mat binary,marker;
    threshold(image,binary,128,256,cv::THRESH_BINARY_INV);
    cv::Mat fg;
    cv::erode(binary,fg,cv::Mat(),cv::Point(-1,-1),6);
    // Identify image pixels without objects
    cv::Mat bg;
    cv::dilate(binary,bg,cv::Mat(),cv::Point(-1,-1),6);
    cv::threshold(bg,bg,1,128,cv::THRESH_BINARY_INV);
    cv::Mat markers(binary.size(),CV_8U,cv::Scalar(0));
    markers= fg+bg;//行成marker image
    WatershedSegmenter wss;
    wss.setMarkers(markers);
    image.convertTo(image,CV_32S);
    marker=wss.process(image);
    marker.convertTo(marker,CV_8U,255,255);
    imshow("watershed",marker);
    cvWaitKey(0);
//    img= QImage((const unsigned char*)(wss.markers.data),
//    wss.markers.cols,wss.markers.rows,QImage::Format_Grayscale8);
//    ui->label->setPixmap(QPixmap::fromImage(img));
//    // resize the label to fit the image
//    ui->label->resize(ui->label->pixmap()->size());
}

void MainWindow::on_actionEdge_detection_triggered()
{
    edge_detection ed;
    ed.image=image;
    ed.exec();
}

void MainWindow::on_actionHarris_corner_triggered()
{
    harris_corners_detector harris;
    // Compute Harris values
    harris.detect(image);
    // Detect Harris corners
    std::vector<cv::Point> pts;
    harris.getCorners(pts,0.01);
    // Draw Harris corners
    harris.drawOnImage(image,pts);
    img= QImage((const unsigned char*)(image.data),
    image.cols,image.rows,QImage::Format_Grayscale8);
    ui->label->setPixmap(QPixmap::fromImage(img));
    // resize the label to fit the image
    ui->label->resize(ui->label->pixmap()->size());

//    std::vector<cv::KeyPoint> keypoints;
//    // Construction of the Good Feature to Track detector
//    cv::GoodFeaturesToTrackDetector gftt(//    500, // maximum number of corners to be returned［   就纠结纠结纠结角jjjjjjjjjjjjjjjj
//    0.01, // quality level
//    10); // minimum allowed distance between points
//    // point detection using FeatureDetector method
//    gftt.detect(image,keypoints);
}

void MainWindow::on_actionImage_match_triggered()
{
    camera_caliabration calib;
    calib.exec();
}

void MainWindow::on_open_video_clicked()
{
  filename = QFileDialog::getOpenFileName(this,
   tr("Open Video"), ".",
    tr("Video Files (*.avi *.mp3 *.rmvb)"));
   // Open the video file
    cv::VideoCapture capture;
    capture.open(filename.toLatin1().data());
    // check if video successfully opened
    if(!capture.isOpened())
    {
         QMessageBox::critical(this, tr("Error"), tr("Could not open video"));
         return;//弹出消息框后再次打开当前窗口
    }

    long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
        //设置开始帧()
        long frameToStart =1;
        capture.set( CV_CAP_PROP_POS_FRAMES,frameToStart);


        //设置结束帧
        int frameToStop =totalFrameNumber;

        double rate = capture.get(CV_CAP_PROP_FPS);

        //定义一个用来控制读取视频循环结束的变量
        bool stop = false;
        //承载每一帧的图像
        Mat frame;
        //显示每一帧的窗口
        namedWindow("Extracted frame",CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
        //两帧间的间隔时间:
        //int delay = 1000/rate;
        int delay = 1000/rate;


        //利用while循环读取帧
        //currentFrame是在循环体中控制读取到指定的帧后循环结束的变量
        long currentFrame = frameToStart;

        while(!stop)
        {
            //读取下一帧
            if(!capture.read(frame))
            {
                break;
            }

            //这里加滤波程序
            imshow("Extracted frame",frame);
            int c = waitKey(delay);
            //按下ESC或者到达指定的结束帧后退出读取视频
            if((char) c == 27 || currentFrame > frameToStop)
            {
                stop = true;
            }
            //按下按键后会停留在当前帧，等待下一次按键
            if( c >= 0)
            {
                waitKey(0);
            }
            currentFrame++;

        }
        //关闭视频文件
        capture.release();
        waitKey(0);
}

void MainWindow::on_actionOpen_forward_camera_triggered()
{
    connect(&theTimer, &QTimer::timeout, this, &MainWindow::updateImage);
        //从摄像头捕获视频
        if(videoCap.open(0))
        {
            srcImage = Mat::zeros(videoCap.get(CV_CAP_PROP_FRAME_HEIGHT), videoCap.get(CV_CAP_PROP_FRAME_WIDTH), CV_8UC3);
            theTimer.start();
        }

}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QImage image2 = QImage((uchar*)(srcImage.data), srcImage.cols, srcImage.rows, QImage::Format_RGB888);
    ui->label2->setPixmap(QPixmap::fromImage(image2));
    ui->label2->resize(image2.size());
    ui->label2->show();
}

void MainWindow::updateImage()
{
    videoCap>>srcImage;
    if(srcImage.data)
    {
        cvtColor(srcImage, srcImage, CV_BGR2RGB);//Qt中支持的是RGB图像, OpenCV中支持的是BGR
        this->update();  //发送刷新消息
    }
}

void MainWindow::on_actionOpen_video_triggered()
{
    video_dialog vd;
    vd.exec();
    vd.show();
}
