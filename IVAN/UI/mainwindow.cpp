#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SHOW_FLAG false
#define DEBUG_BY_STEP false
#define SAVE_VIDEO false
#define ROI_X 1200
#define ROI_Y 800
#define ROI_WIDTH 640
#define ROI_HEIGHT 360

/****************************************************
 * 函数功能： 构造函数， 一些变量的初始化
 * 参数说明：
 * 返回：
 * 作者： XU SHIJIAN
 * 时间: 2017-9-11
 * 修改：
 ****************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //====some initialization
    pTimer = NULL;
    isDrawLine = false;
    isSettingROI = false;
    pointCnt = 0;
    roiCnt = 0;
    frameCnt = 0;

    mainROI.x = ROI_X;
    mainROI.y = ROI_Y;
    mainROI.width = ROI_WIDTH;
    mainROI.height = ROI_HEIGHT;


    //====tableView====
    pModel = new QStandardItemModel();
    pModel->setHorizontalHeaderItem(0, new QStandardItem(tr("方向")));
    pModel->setHorizontalHeaderItem(1, new QStandardItem(tr("小车")));
    pModel->setHorizontalHeaderItem(2, new QStandardItem(tr("大车")));
    pModel->setHorizontalHeaderItem(3, new QStandardItem(tr("时间")));
    pModel->setHorizontalHeaderItem(3, new QStandardItem(tr("小时流量")));

    ui->tableView->setModel(pModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Fixed);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Fixed);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setStyleSheet("selection-background-color:lightblue;");
    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    ui->tableView->verticalHeader()->setVisible(false);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //====
    pModel->setItem(0,0,new QStandardItem("SouthLeft"));
    pModel->item(0,0)->setBackground(QBrush(QColor(255, 0, 0)));
    pModel->setItem(1,0,new QStandardItem("SouthDirect"));
    pModel->item(1,0)->setBackground(QBrush(QColor(255, 0, 0)));
    pModel->setItem(2,0,new QStandardItem("SouthRight"));
    pModel->item(2,0)->setBackground(QBrush(QColor(255, 0, 0)));
    //====
    pModel->setItem(3,0,new QStandardItem("EastLeft"));
    pModel->item(3,0)->setBackground(QBrush(QColor(0, 255, 0)));
    pModel->setItem(4,0,new QStandardItem("EastDirect"));
    pModel->item(4,0)->setBackground(QBrush(QColor(0, 255, 0)));
    pModel->setItem(5,0,new QStandardItem("EastRight"));
    pModel->item(5,0)->setBackground(QBrush(QColor(0, 255, 0)));
    //====
    pModel->setItem(6,0,new QStandardItem("NorthLeft"));
    pModel->item(6,0)->setBackground(QBrush(QColor(255, 255, 0)));
    pModel->setItem(7,0,new QStandardItem("NorthDirect"));
    pModel->item(7,0)->setBackground(QBrush(QColor(255, 255, 0)));
    pModel->setItem(8,0,new QStandardItem("NorthRight"));
    pModel->item(8,0)->setBackground(QBrush(QColor(255, 255, 0)));
    //====
    pModel->setItem(9,0,new QStandardItem("WestLeft"));
    pModel->item(9,0)->setBackground(QBrush(QColor(0, 255, 255)));
    pModel->setItem(10,0,new QStandardItem("WestDirect"));
    pModel->item(10,0)->setBackground(QBrush(QColor(0, 255, 255)));
    pModel->setItem(11,0,new QStandardItem("WestRight"));
    pModel->item(11,0)->setBackground(QBrush(QColor(0, 255, 255)));
}


/****************************************************
 * 函数功能：析构函数
 * 参数说明：
 * 返回：
 * 作者： XU SHIJIAN
 * 时间: 2017-9-11
 * 修改：
 ****************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}


/****************************************************
 * 函数功能：确定用户标定的ROI点的真实位置
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::setROI()
{
    int pix1x = int((roip1.x() * 1.0 / ui->label->width()) * frameBackup.cols);
    int pix1y = int((roip1.y() * 1.0 / ui->label->height()) * frameBackup.rows);

    int pix2x = int((roip2.x() * 1.0 / ui->label->width()) * frameBackup.cols);
    int pix2y = int((roip2.y() * 1.0 / ui->label->height()) * frameBackup.rows);

    mainROI.x = pix1x;
    mainROI.y = pix1y;
    mainROI.width = abs(pix2x - pix1x);
    mainROI.height = abs(pix2y - pix1y);
}

/****************************************************
 * 函数功能：将Mat转换为QImage，以便于后面的setpixmap()的使用
 * 参数说明：Mat cvImg
 * 返回：QImage
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
QImage MainWindow::convertMat2QImage(Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels() == 3)                         //3 channels color image
    {
        cv::cvtColor(cvImg,cvImg,CV_BGR2RGB);
        qImg = QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols * cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels() == 1)                    //grayscale image
    {
        qImg = QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols * cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg = QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols * cvImg.channels(),
                    QImage::Format_RGB888);
    }
    return qImg;
}

/****************************************************
 * 函数功能：按钮“打开”的槽函数，打开要处理的视频，并显示第一帧
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::on_open_clicked()
{
    QString s = QString::number(ui->label->width());
    QString t = QString::number(ui->label->height());
    sTextBrowser.append(s);
    sTextBrowser.append(t);
    sTextBrowser.append('\n');

    sTextBrowser.append("Opening video...\n");
    ui->textBrowser->setText(sTextBrowser);

    //clear()
    lineStart.clear();
    lineEnd.clear();
    crossingLineEnd.clear();
    crossingLineStart.clear();
    points.clear();
    blobs.clear();
    crossingLineColor.clear();

    //open
    if(pTimer!=NULL)
    {
        delete pTimer;
        pTimer = NULL;
    }
    if(capture.isOpened())
        capture.release();

    sFileName =QFileDialog::getOpenFileName(this,tr("Open Video File"),".",tr("Video Files(*.avi *.mp4 *.flv *.mkv)"));
    capture.open(sFileName.toLocal8Bit().data());
    if(capture.isOpened())
    {
        capture.read(frame);
        frameBackup = frame.clone();

        cv::resize(frame, frame, Size(ui->label->width(), ui->label->height()));

        if(!frame.empty())
        {
            image = convertMat2QImage(frame);
            ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->width(), ui->label->height()));//,Qt::KeepAspectRatio));
        }
    }
    sTextBrowser.append("Open video done.\n");
    ui->textBrowser->setText(sTextBrowser);
}

/****************************************************
 * 函数功能：按钮“确定ROI”的槽函数，将isSettingROI设置为true
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::on_cut_clicked()
{
    isSettingROI = true;
    sTextBrowser.append("Set 2 points to determine ROI.\n");
    ui->textBrowser->setText(sTextBrowser);
}

/****************************************************
 * 函数功能：获取鼠标的点击事件并响应相应的操作
 * 参数说明：QMouseEvent *event
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(isSettingROI)
    {
        roiCnt++;
        if(roiCnt == 1)
        {
            roip1 = event->pos();
            QString s = QString::number(roip1.x());
            s.append(",");
            QString t = QString::number(roip1.y());
            s.append(t);

            sTextBrowser.append(s);
            sTextBrowser.append("\n");
            ui->textBrowser->setText(sTextBrowser);
        }
        else
        {
            roip2 = event->pos();
            QString s = QString::number(roip2.x());
            s.append(",");
            QString t = QString::number(roip2.y());
            s.append(t);

            sTextBrowser.append(s);
            sTextBrowser.append("\n");
            ui->textBrowser->setText(sTextBrowser);

            //------------
            setROI();

            //------------
            Mat temp = frameBackup.clone();        //原尺寸
            Point st = Point(int((roip1.x() * 1.0) / ui->label->width() * frameBackup.cols), int((roip1.y() * 1.0) / ui->label->height() * frameBackup.rows));
            Point en = Point(int((roip2.x() * 1.0) / ui->label->width() * frameBackup.cols), int((roip2.y() * 1.0) / ui->label->height() * frameBackup.rows));
            rectangle(temp, st, en, Scalar(255,0,0), 10);

            image = convertMat2QImage(temp);
            //ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->width(),ui->label->height()));//,Qt::KeepAspectRatio));

            frameBackup = Mat(frameBackup, mainROI);
            cv::resize(frameBackup, frameBackup, Size(ui->label->width(), ui->label->height()));

            //imshow("frame", frameBackup);
            image = convertMat2QImage(frameBackup);
            ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->width(), ui->label->height()));//,Qt::KeepAspectRatio));
        }
    }
    else if(isDrawLine)
    {
        pointCnt++;
        QPoint temp = event->pos();
        points.push_back(temp);

        QString s = QString::number(temp.x() * 1.0 / ui->label->width());
        s.append(",");
        s.append(QString::number(temp.y() * 1.0 / ui->label->height()));

        sTextBrowser.append(s);
        sTextBrowser.append("\n");
        ui->textBrowser->setText(sTextBrowser);

        if(pointCnt == 1)
        {
            crossingLineStart.push_back(pair<double, double>(temp.x() * 1.0 / ui->label->width(), temp.y() * 1.0 / ui->label->height()));
        }
        else
        {
            crossingLineEnd.push_back(pair<double, double>(temp.x() * 1.0 / ui->label->width(), temp.y() * 1.0 / ui->label->height()));
            crossingLineColor.push_back(Scalar(rand() % 255, rand() % 255, rand() & 255));

            Mat temp = frameBackup.clone();

            for(int i = 0; i < points.size(); i += 2)
            {
                QPoint st = points[i];
                Point start = Point(st.x(),st.y());
                QPoint en = points[i + 1];
                Point end = Point(en.x(), en.y());

                line(temp, start, end, Scalar(255, 0, 0));
            }

            image = convertMat2QImage(temp);
            ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
        }
    }
}

/****************************************************
 * 函数功能：按钮“绘制基准线”的槽函数，将isDrawLine设为true
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::on_drawLine_clicked()
{
    isDrawLine = true;
    sTextBrowser.append("Set 2 points to draw a baseline.\n");
    ui->textBrowser->setText(sTextBrowser);
}


/****************************************************
 * 函数功能：获取鼠标释放事件，并作处理
 * 参数说明：QMouseEvent *event
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(isDrawLine && pointCnt == 2)
    {
        isDrawLine = false;
        pointCnt = 0;
    }
    if(isSettingROI && roiCnt == 2)
    {
        isSettingROI = false;
        roiCnt = 0;
    }
}

/****************************************************
 * 函数功能：按钮“撤销基准线”的槽函数，用于删除最近的一条基准线
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::on_deleteLine_clicked()
{
    if(points.size() > 0)
    {
        points.pop_back();
        points.pop_back();
        crossingLineStart.pop_back();
        crossingLineEnd.pop_back();
        crossingLineColor.pop_back();
    }

    Mat temp = frameBackup.clone();
    for(int i = 0; i < points.size(); i += 2)
    {
        QPoint st = points[i];
        Point start = Point(st.x(),st.y());
        QPoint en = points[i + 1];
        Point end = Point(en.x(),en.y());

        line(temp, start, end, Scalar(255, 0, 0));
    }

    image = convertMat2QImage(temp);
    ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
}

/****************************************************
 * 函数功能：按钮“标定参照物”的槽函数，用于在图像上标定参照物
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：该功能已实现，但为整合到界面中来
 ****************************************************/
void MainWindow::on_mark_clicked()
{

}

/****************************************************
 * 函数功能：按钮“处理”的槽函数，在真正进行处理前的一些预备操作
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::on_proc_clicked()
{
    sTextBrowser.append("Processing...\n");
    ui->textBrowser->setText(sTextBrowser);

    cnt.clear();
    for(int i = 0; i < crossingLineStart.size(); i ++)
        cnt.push_back(0);

    for(int i = 0; i < 3; i ++)
        threeDirection.push_back(0);

    total.resize(crossingLineStart.size());
    for(int i = 0; i < total.size(); i ++)
        total[i].resize(3);

    frameCnt = 2;

    //====tableView 清空
    for(int i = 0; i < total.size(); i ++)
    {
        for(int j = 0; j < 3; j ++)
        {
            total[i][j]=0;
        }
    }

    //====定时器检查和capture打开
    if(pTimer != NULL)
    {
        delete pTimer;
        pTimer = NULL;
    }

    if(!capture.isOpened())
    {
        sFileName =QFileDialog::getOpenFileName(this,tr("Open Video File"),".",tr("Video Files(*.avi *.mp4 *.flv *.mkv)"));
        capture.open(sFileName.toLocal8Bit().data());
    }

    if (!capture.isOpened()) //cannot open the video
    {
        cout << "can't open the video!" << endl;
        system("pause");
        return;
    }

    capture.read(frame1);
    capture.read(frame2);
    frame1 = Mat(frame1, mainROI);

    frame2 = Mat(frame2, mainROI);

    tracker = TrackerKCF::create("KCF");
    roi = selectROI("tracker", frame1, true, false);
    roiOrigin = roi;

    if(roi.width ==0 || roi.height==0)
    {
        cout<<"select roi wrong."<<endl;
        system("pause");
    }

    tracker->init(frame1, roi);

//    cv::resize(frame1,frame1,Size(ui->label->width(),ui->label->height()));
//    cv::resize(frame2,frame2,Size(ui->label->width(),ui->label->height()));

    for (int i = 0; i < crossingLineStart.size(); i ++)       //scale to coordinate
    {
        int x = (int)((frame1.cols - 1) * crossingLineStart[i].first);
        int y = (int)((frame1.rows - 1) * crossingLineStart[i].second);
        Point start(x, y);
        lineStart.push_back(start);

        x = (int)((frame1.cols - 1) * crossingLineEnd[i].first);
        y = (int)((frame1.rows - 1) * crossingLineEnd[i].second);
        Point end(x, y);

        lineEnd.push_back(end);
    }

    if(capture.isOpened())
    {
        //double rate = capture.get(CV_CAP_PROP_FPS);

        proc();

        pTimer = new QTimer(this);
        pTimer->setInterval(16);
        connect(pTimer, SIGNAL(timeout()), this, SLOT(updateFrame()));
        pTimer->start();
    }
}

/****************************************************
 * 函数功能：视频帧处理的主函数，帧差法追踪车辆并显示边框
 *          会被updateFrame()反复调用，知道视频播放完毕
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::proc()
{
    if(frame1.empty()) return;
    bool isfound = tracker->update(frame1, roi);
    rectangle(frame1, roi, Scalar(255, 0, 0), 2, 1);

    int xAdd = (roi.tl().x - roiOrigin.tl().x);
    int yAdd = (roi.tl().y - roiOrigin.tl().y);
    for(int i = 0; i < lineStart.size(); i ++)
    {
        lineStart[i].x += xAdd;
        lineStart[i].y += yAdd;
        lineEnd[i].x += xAdd;
        lineEnd[i].y += yAdd;
    }
    roiOrigin = roi;

    vector<Blob> currentBlobs;             //blob in current frame
    vector<Blob> tempBlobs;                //temp blob
    Mat frame1Copy, frame2Copy;            //copy completely
    Mat imgDifference;                     //frame difference
    Mat imgThresh;                         //binarization


    cv::resize(frame1, frame1Copy, Size((int)(frame1.cols / resizeWidthCoefficient), (int)(frame1.rows / resizeHeightCoefficient))); //resize
    cv::resize(frame2, frame2Copy, Size((int)(frame2.cols / resizeWidthCoefficient), (int)(frame2.rows / resizeHeightCoefficient)));
    cvtColor(frame1Copy, frame1Copy, CV_BGR2GRAY);                    //to gray
    cvtColor(frame2Copy, frame2Copy, CV_BGR2GRAY);
    GaussianBlur(frame1Copy, frame1Copy, Size(5, 5), 0);              //GaussianBlur for smoothing
    GaussianBlur(frame2Copy, frame2Copy, Size(5, 5), 0);
    absdiff(frame1Copy, frame2Copy, imgDifference);                   //frame difference
    //GaussianBlur(imgDifference, imgDifference, Size(5, 5), 0);
    threshold(imgDifference, imgThresh, 19, 255.0, CV_THRESH_BINARY); //binarization
    //adaptiveThreshold(imgDifference, imgThresh, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 9);
    if (SHOW_FLAG) imshow("imgThresh", imgThresh);

    Mat structuringElement3x3 = getStructuringElement(MORPH_RECT, Size(3, 3)); //definition of different kernels, big kernel for noisy image
    Mat structuringElement5x5 = getStructuringElement(MORPH_RECT, Size(5, 5));
    //Mat structuringElement7x7   = getStructuringElement(MORPH_RECT, Size(7, 7));
    //Mat structuringElement15x15 = getStructuringElement(MORPH_RECT, Size(15, 15));

    for (int i = 0; i < 3; i ++)
    {
        //dilate(imgThresh, imgThresh, structuringElement3x3, Point(-1, -1), 2);
        //erode(imgThresh, imgThresh, structuringElement3x3);
    }
    dilate(imgThresh, imgThresh, structuringElement3x3, Point(-1, -1), 2);
    dilate(imgThresh, imgThresh, structuringElement3x3, Point(-1, -1), 2);
    //for (int i = 0; i < 1; i++)
    //	erode(imgThresh, imgThresh, structuringElement3x3);

    Mat imgThreshCopy = imgThresh.clone();
    vector<vector<Point> > contours; //contours
    findContours(imgThreshCopy, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    if (SHOW_FLAG) showContours(imgThresh.size(), contours, "imgContours");

    vector<vector<Point> > convexHulls(contours.size());

    for (int i = 0; i < contours.size(); i ++) convexHull(contours[i], convexHulls[i]);

    if (SHOW_FLAG) showContours(imgThresh.size(), convexHulls, "imgConvexHulls");
    for (auto &convexHull : convexHulls) //filter with heuristic knowledge
    {
        Blob possibleBlob(convexHull, (int)lineStart.size());
        Rect possibleBoundingBox = possibleBlob.getBoundingBox();
        if (
            possibleBoundingBox.area() > minBlobArea &&
            possibleBoundingBox.area() < maxBlobArea &&
            possibleBlob.getRatio() > minBlobRatio &&
            possibleBlob.getRatio() < maxBlobRatio &&
            possibleBoundingBox.width > minBlobWidth &&
            possibleBoundingBox.width < maxBlobWidth &&
            possibleBoundingBox.height > minBlobheight &&
            possibleBoundingBox.height < maxBlobheight &&
            possibleBlob.getDiagonalLength() > minBlobDiagonal &&
            possibleBlob.getDiagonalLength() < maxBlobDiagonal &&
            (contourArea(possibleBlob.getContour()) / (double)possibleBoundingBox.area()) > 0.50        //contour area / rect area
                )
            tempBlobs.push_back(possibleBlob);
    }

    for (int i = 0, j ; i < tempBlobs.size(); i ++) //filter with inclusion
    {
        for (j = 0; j < tempBlobs.size(); j ++)
            if (j != i && isOverlapped(tempBlobs[i].getBoundingBox(), tempBlobs[j].getBoundingBox())) break; //is covered
        if (j == tempBlobs.size())
            currentBlobs.push_back(tempBlobs[i]);
    }
    if (SHOW_FLAG) showContours(imgThresh.size(), currentBlobs, "imgcurrentBlobs");

    matchBlobs(blobs, currentBlobs, frame2Copy);
    if (SHOW_FLAG) showContours(imgThresh.size(), blobs, "imgBlobs");

    //== == == == == == == == == == = step2.3 counting and draw blobs == == == == == == == == == == == == == == =/
    frame2Copy = frame2.clone();
    drawBlob(blobs, frame2Copy);
    for (int i = 0; i < lineStart.size(); i ++)
    {
        if (isCrossLine(blobs, lineStart[i], lineEnd[i], cnt[i], i, total)) //some blob has crossed the line
            line(frame2Copy, lineStart[i], lineEnd[i], GREEN, lineThickness);
        else
            line(frame2Copy, lineStart[i], lineEnd[i], crossingLineColor[i], lineThickness);
    }
    if (SHOW_FLAG)
    {
        for (auto t : total)
        {
            cout << "right: " << t[0] << "  straight: " << t[1] << "  left: " << t[2] << endl;
        }
        cout << endl;
    }
    //drawCnt(cnt, frame2Copy, crossingLineColor, total);

    Mat frame2Copy2;
    cv::resize(frame2Copy, frame2Copy2, Size((int)(frame1.cols / 2), (int)(frame1.rows / 2))); //resize
    //imshow("frame2Copy", frame2Copy2);

    Mat temp = frame2Copy.clone();
    QImage image2 = convertMat2QImage(temp);
    ui->label->setPixmap(QPixmap::fromImage(image2).scaled(ui->label->width(),ui->label->height()));//,Qt::KeepAspectRatio));
}

/****************************************************
 * 函数功能：用于更新下一帧的槽函数，由定时器的timeout()信号调用
 *          获取新的一帧，填写tableView中的统计结果，并调用proc()函数处理下一帧
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::updateFrame()
{
    frame1 = frame2.clone();
    if ((capture.get(CV_CAP_PROP_POS_FRAMES) + 1) < capture.get(CV_CAP_PROP_FRAME_COUNT)) //has next frame
    {
        capture.read(frame2);
        frame2 = Mat(frame2, mainROI);
        if(!total.empty())
        {
            for(int i = 0; i<total.size(); i ++)
            {
                int rightNum = 0, leftNum = 0, straightNum = 0;
                rightNum = total[i][0];
                straightNum = total[i][1];
                leftNum = total[i][2];

                QString lefts = QString::number(leftNum);
                QString straights = QString::number(straightNum);
                QString rights = QString::number(rightNum);
                pModel->setItem(i * 3, 1, new QStandardItem(lefts));
//                pModel->item(i * 3, 1)->setBackground(QBrush(QColor(255, 0, 0)));
                pModel->setItem(i * 3 + 1, 1, new QStandardItem(straights));
//                pModel->item(i * 3 + 1,1)->setBackground(QBrush(QColor(255, 0, 0)));
                pModel->setItem(i * 3 + 2, 1, new QStandardItem(rights));
//                pModel->item(i * 3 + 2, 1)->setBackground(QBrush(QColor(255, 0, 0)));
            }
        }
        proc();
    }

    frameCnt ++;
}

/****************************************************
 * 函数功能：在纯粹的一帧一帧播放视频而不处理时用于更新下一帧
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::nextFrame()
{
    capture >> frame;
    frame = Mat(frame,mainROI);
    cv::resize(frame, frame, Size(ui->label->width(), ui->label->height()));

    if (!frame.empty())
    {
        image = convertMat2QImage(frame);
        ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
    }
}

/****************************************************
 * 函数功能：按钮“播放/继续”的槽函数，用于播放视频
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::on_play_clicked()
{
    sTextBrowser.append("Playing...\n");
    ui->textBrowser->setText(sTextBrowser);
    if(pTimer!=NULL)
    {
        delete pTimer;
        pTimer = NULL;
    }

    if(capture.isOpened())
    {
        double rate = capture.get(CV_CAP_PROP_FPS);
        if(!frame.empty())
        {
            //image = Mat2QImage(frame);
            //ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->width(),ui->label->height(),Qt::KeepAspectRatio));
            pTimer = new QTimer(this);
            pTimer->setInterval(1000 / rate);
            connect(pTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
            pTimer->start();
        }
    }
}

/****************************************************
 * 函数功能：按钮“暂停”的槽函数，停止视频的播放和处理
 *          之后视频可以继续播放，但不可以被继续处理
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::on_stop_clicked()
{
    sTextBrowser.append("Stop.\n");
    ui->textBrowser->setText(sTextBrowser);
    if(pTimer != NULL)
    {
        delete pTimer;
        pTimer = NULL;
    }
}

/****************************************************
 * 函数功能：按钮“重放”的槽函数，暂停之后重新播放
 * 参数说明：
 * 返回：
 * 作者：XU SHIJIAN
 * 时间：2017-9-11
 * 修改：
 ****************************************************/
void MainWindow::on_replay_clicked()
{
    sTextBrowser.append("Replay.\n");
    ui->textBrowser->setText(sTextBrowser);

    if(pTimer != NULL)
    {
        delete pTimer;
        pTimer = NULL;
    }
    if(capture.isOpened())
        capture.release();
    capture.open(sFileName.toLocal8Bit().data());
    if(capture.isOpened())
    {
        capture >> frame;
        frame = Mat(frame,mainROI);

        cv::resize(frame, frame, Size(ui->label->width(), ui->label->height()));

        if(!frame.empty())
        {
            image = convertMat2QImage(frame);
            ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->width(), ui->label->height(),Qt::KeepAspectRatio));
        }
    }
}
