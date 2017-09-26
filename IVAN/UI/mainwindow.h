/******************************************************************
 * FILE NAME: MAINWINDOW.H
 * AUTHOR: XU SHIJIAN
 * DISCRIPTION: This file defines a class MainWindow.
 *              Used to create the UI of this application and
 *              it defines some functions used to process a frame.
 * ****************************************************************/




#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "blob.h"
#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QStandardItemModel>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>
#include <QMessageBox>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/tracking.hpp"
//#include <opencv2/videoio.hpp>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateFrame();
    void nextFrame();
    //以下为自动命名的槽函数
    void on_open_clicked();
    void on_cut_clicked();
    void on_drawLine_clicked();
    void on_deleteLine_clicked();
    void on_mark_clicked();
    void on_proc_clicked();
    void on_play_clicked();
    void on_stop_clicked();
    void on_replay_clicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QImage convertMat2QImage(Mat cvImg);
    void setROI();
    void proc();

    Ui::MainWindow *ui;

    QTimer *pTimer;                 //定时器
    QImage image;                   //mat2image
    QString sFileName;               //the path of video opened
    QStandardItemModel *pModel;     //显示车辆统计结果的tableView
    bool isDrawLine;                //is drawing line?
    bool isSettingROI;              //is setting roi?
    int roiCnt;                     //标定roi的两个点计数
    QPoint roip1;
    QPoint roip2;                   //roi标定的两个点
    int pointCnt;                   //画线的点数计数
    vector<QPoint> points;          //用户绘制的线的点集合
    Rect mainROI;                   //用户标定的roi区域和大小
    QString sTextBrowser;           //显示在TextBrowser上的字符串


    Mat frame;
    Mat frameBackup;
    //=====
    vector<pair<double, double>> crossingLineStart;                      //represent counting line
    vector<pair<double, double>> crossingLineEnd;
    vector<Scalar> crossingLineColor;
    vector<int> threeDirection;
    vector<vector<int>> total;

    VideoCapture capture;
    Mat frame1, frame2;                                                 //two frames for substract
    vector<Blob> blobs;	                                                //blobs

    vector<Point> lineStart, lineEnd;                                   //crossing line start points and end points
    vector<int> cnt;
    int frameCnt;
    Ptr<Tracker> tracker;
    Rect2d roi;                 //用于跟踪的标识物
    Rect2d roiOrigin;
};

#endif // MAINWINDOW_H
