#ifndef VIDEO_DIALOG_H
#define VIDEO_DIALOG_H

#include <QDialog>
#include <QTimer>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
using namespace cv;
using namespace std;


namespace Ui {
class video_dialog;
}

class video_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit video_dialog(QWidget *parent = 0);
    ~video_dialog();

private slots:
    void on_pushButton_clicked();

    void updateImage();



private:
    Ui::video_dialog *ui;
    QTimer theTimer;
    Mat srcImage;
    VideoCapture videoCap;

protected:
    void paintEvent(QPaintEvent *e);
};

#endif // VIDEO_DIALOG_H
