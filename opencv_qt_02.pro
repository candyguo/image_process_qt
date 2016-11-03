#-------------------------------------------------
#
# Project created by QtCreator 2016-07-18T21:39:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencv_qt_02
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    colordetector.cpp \
    histogram1d.cpp \
    threshold_dialog.cpp \
    morphological.cpp \
    watershedsegmenter.cpp \
    edge_detection.cpp \
    laplacian.cpp \
    linefinder.cpp \
    harris_corners_detector.cpp \
    camera_caliabration.cpp \
    roubst_matcher.cpp \
    video_dialog.cpp

HEADERS  += mainwindow.h \
    colordetector.h \
    histogram1d.h \
    threshold_dialog.h \
    morphological.h \
    watershedsegmenter.h \
    edge_detection.h \
    laplacian.h \
    linefinder.h \
    harris_corners_detector.h \
    camera_caliabration.h \
    roubst_matcher.h \
    video_dialog.h

FORMS    += mainwindow.ui \
    threshold_dialog.ui \
    morphological.ui \
    edge_detection.ui \
    camera_caliabration.ui \
    video_dialog.ui

INCLUDEPATH += /usr/local/include/

LIBS += -L/usr/local/lib/

LIBS += -lm -lopencv_core -lopencv_highgui -lopencv_video -lopencv_imgproc -lopencv_calib3d -lopencv_features2d -lopencv_nonfree\
        -lopencv_video
