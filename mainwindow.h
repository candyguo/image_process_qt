#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <QTimer>
using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Mat getcurrent_image();

private slots:
    void on_pushButton_clicked();

    void flip_h();

    void on_actionSalt_pepper_triggered();

    void on_save_imageButton_3_clicked();

    void on_actionReduce_color_triggered();

    void on_actionImage_gray_triggered();

    void on_actionSharpen_triggered();

    void on_actionChannel_split_B_triggered();

    void on_actionChannel_split_G_triggered();

    void on_actionCahnnel_split_R_triggered();

    void on_actionColor_detection_triggered();

    void on_actionImage_histogram_triggered();

    void on_actionThreshold_image_triggered();

    void on_actionHorizontal_flip_triggered();

    void on_actionLookup_inverse_triggered();

    void on_actionStretch_histogram_triggered();

    void on_actionHistogram_equalization_triggered();

    static void MouseCallback(int mouseEvent,int x,int y,int flags,void* param);

    void on_actionRegion_extration_triggered();

    void on_pushButton_2_clicked();

    void on_actionMorphological_image_triggered();

    void on_actionWatershed_segment_triggered();

    void on_actionEdge_detection_triggered();

    void on_actionHarris_corner_triggered();

    void on_actionImage_match_triggered();

    void on_open_video_clicked();

    void updateImage();

    void on_actionOpen_forward_camera_triggered();

    void on_actionOpen_video_triggered();

private:
    Ui::MainWindow *ui;
    Mat image;
    QImage img;
    QString filename;
    QTimer theTimer;
    Mat srcImage;
    VideoCapture videoCap;

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // MAINWINDOW_H
