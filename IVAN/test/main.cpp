#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>

#include <Core.h>
#include <Basic_algorithm.h>

#include<Python.h>
//#include</home/jipeng/anaconda3/include/python3.6m/Python.h>
#include</home/jipeng/anaconda3/lib/python3.6/site-packages/numpy/core/include/numpy/arrayobject.h>

using namespace std;
using namespace cv;

int main()
{
    DetectionRCNN R("/data/jipeng/IVAN/IVAN/Basic_algorithm/Detection.config");
    VideoCapture cap("/home/jipeng/9_1_1.mp4");
    Mat img, img2, img3;
    int cnt = 0;
    while(true)
    {
        cout<<cnt++<<endl;
        cap >> img;
        //Rect r1(Point(img3.cols / 2, img3.rows / 2), Point( img3.cols, img3.rows));
        //img = img3(r1);
        list<Blob> blobs = R.detect(img);

        for(auto blob: blobs)
        {
            rectangle(img, blob.getBBox(), Scalar(0, 255, 0), 3);
        }

        resize(img, img2, Size(1080, 720));
        imshow("demo", img2);
        waitKey(1);
    }
    return 0;
}


//#define SHOW_FLAG false
//#define DEBUG_BY_STEP false
//#define SAVE_VIDEO false

//void carCounting(vector<pair<double, double>> &crossingLineStart, vector<pair<double, double>> &crossingLineEnd)
//{
//    VideoCapture capture;
//    Mat frame1, frame2;                                                 //two frames for substract
//    vector<Point> lineStart, lineEnd;                                   //crossing line start points and end points
//    char keyValue = 0;                                                  //key you press
//    capture.open("/home/jipeng/test1.mp4");
//    if (!capture.isOpened()) //cannot open the video
//    {
//        cout << "can't open the video！" << endl;
//        return;
//    }
//    capture.read(frame1);
//    capture.read(frame2);
//    for (int i = 0; i < crossingLineStart.size(); i++)       //scale to coordinate
//    {
//        int x = (int)((frame1.cols - 1) * crossingLineStart[i].first);
//        int y = (int)((frame1.rows - 1) * crossingLineStart[i].second);
//        Point start(x, y);
//        lineStart.push_back(start);

//        x = (int)((frame1.cols - 1) * crossingLineEnd[i].first);
//        y = (int)((frame1.rows - 1) * crossingLineEnd[i].second);
//        Point end(x, y);

//        lineEnd.push_back(end);
//    }
//    //DetectionFrameDiff diffDetector("diffDetectParameters.txt");
//    while (capture.isOpened() && keyValue != 27)
//    {
//        cout << "按任意键退出！" << endl;
//        if (keyValue != 27) waitKey(0);                         //exit without press ESC, wait
//    }
//    return ;
//}
////{
////    /*== == == == == == == == == == = step1 Variable definition and Initialization == == == == == == == == == == == == == == =*/
////
////
////    vector<Blob> blobs;	                                                //blobs

////vector<int> cnt(crossingLineStart.size(), 0);                       //number of cars cross each line
////    int frameCnt = 2;                                                   //number of frames
////



////        vector<Blob> currentBlobs;             //blob in current frame
////        vector<Blob> tempBlobs;                //temp blob
////        Mat frame1Copy, frame2Copy;            //copy completely
////        Mat imgDifference;                     //frame difference
////        Mat imgThresh;                         //binarization

////        /*== == == == == == == == == == = step2.2 denoise and find contours == == == == == == == == == == == == == == =*/
////        resize(frame1, frame1Copy, Size((int)(frame1.cols / resizeWidthCoefficient), (int)(frame1.rows / resizeHeightCoefficient))); //resize
////        resize(frame2, frame2Copy, Size((int)(frame2.cols / resizeWidthCoefficient), (int)(frame2.rows / resizeHeightCoefficient)));
////        cvtColor(frame1Copy, frame1Copy, CV_BGR2GRAY);                    //to gray
////        cvtColor(frame2Copy, frame2Copy, CV_BGR2GRAY);
////        GaussianBlur(frame1Copy, frame1Copy, Size(5, 5), 0);              //GaussianBlur for smoothing
////        GaussianBlur(frame2Copy, frame2Copy, Size(5, 5), 0);
////        absdiff(frame1Copy, frame2Copy, imgDifference);                   //frame difference
////        //GaussianBlur(imgDifference, imgDifference, Size(5, 5), 0);
////        //threshold(imgDifference, imgThresh, 30, 255.0, CV_THRESH_BINARY); //binarization
////        adaptiveThreshold(imgDifference, imgThresh, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 10);
////        if (SHOW_FLAG) imshow("imgThresh", imgThresh);

////        Mat structuringElement3x3   = getStructuringElement(MORPH_RECT, Size(3, 3)); //definition of different kernels, big kernel for noisy image
////        Mat structuringElement5x5   = getStructuringElement(MORPH_RECT, Size(5, 5));
////        //Mat structuringElement7x7   = getStructuringElement(MORPH_RECT, Size(7, 7));
////        //Mat structuringElement15x15 = getStructuringElement(MORPH_RECT, Size(15, 15));

////        for (int i = 0; i < 2; i++)
////        {
////            dilate(imgThresh, imgThresh, structuringElement3x3, Point(-1, -1), 2);
////            erode(imgThresh, imgThresh, structuringElement5x5);
////        }

////        Mat imgThreshCopy = imgThresh.clone();
////        vector<vector<Point> > contours; //contours
////        findContours(imgThreshCopy, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
////        if (SHOW_FLAG) showContours(imgThresh.size(), contours, "imgContours");

////        vector<vector<Point> > convexHulls(contours.size());

////        for (int i = 0; i < contours.size(); i++) convexHull(contours[i], convexHulls[i]);

////        if (SHOW_FLAG) showContours(imgThresh.size(), convexHulls, "imgConvexHulls");
////        for (auto &convexHull : convexHulls) //filter with heuristic knowledge
////        {
////            Blob possibleBlob(convexHull, (int)lineStart.size());
////            Rect possibleBoundingBox = possibleBlob.getBoundingBox();
////            if (
////                possibleBoundingBox.area() > minBlobArea &&
////                possibleBoundingBox.area() < maxBlobArea &&
////                possibleBlob.getRatio() > minBlobRatio &&
////                possibleBlob.getRatio() < maxBlobRatio &&
////                possibleBoundingBox.width > minBlobWidth &&
////                possibleBoundingBox.width < maxBlobWidth &&
////                possibleBoundingBox.height > minBlobheight &&
////                possibleBoundingBox.height < maxBlobheight &&
////                possibleBlob.getDiagonalLength() > minBlobDiagonal &&
////                possibleBlob.getDiagonalLength() < maxBlobDiagonal &&
////                (contourArea(possibleBlob.getContour()) / (double)possibleBoundingBox.area()) > 0.50        //contour area / rect area
////            )
////                tempBlobs.push_back(possibleBlob);
////        }
////        for (int i = 0, j ; i < tempBlobs.size(); i++) //filter with inclusion
////        {
////            for (j = 0; j < tempBlobs.size(); j++)
////            if (j != i && isOverlapped(tempBlobs[i].getBoundingBox(), tempBlobs[j].getBoundingBox())) break; //is covered
////            if (j == tempBlobs.size())
////                currentBlobs.push_back(tempBlobs[i]);
////        }
////        if (SHOW_FLAG) showContours(imgThresh.size(), currentBlobs, "imgcurrentBlobs");

////        matchBlobs(blobs, currentBlobs, frame2Copy);
////        if (SHOW_FLAG) showContours(imgThresh.size(), blobs, "imgBlobs");

////        /*== == == == == == == == == == = step2.3 counting and draw blobs == == == == == == == == == == == == == == =*/
////        frame2Copy = frame2.clone();
////        drawBlob(blobs, frame2Copy);
////        for (int i = 0; i < lineStart.size(); i++)
////        {
////            if (isCrossLine(blobs, lineStart[i], lineEnd[i], cnt[i], i)) //some blob has crossed the line
////                line(frame2Copy, lineStart[i], lineEnd[i], GREEN, 2);
////            else
////                line(frame2Copy, lineStart[i], lineEnd[i], RED, 2);
////        }
////        drawCnt(cnt, frame2Copy);
////        //writer << frame2Copy;
////        imshow("frame2Copy", frame2Copy);

////        /*== == == == == == == == == == = step2.4 prepare for the next iteration == == == == == == == == == == == == == == =*/
////        currentBlobs.clear();
////        frame1 = frame2.clone();
////        if ((capture.get(CV_CAP_PROP_POS_FRAMES) + 1) < capture.get(CV_CAP_PROP_FRAME_COUNT)) //has next frame
////            capture.read(frame2);
////        else
////            break;

////        frameCnt++;
////        keyValue = waitKey(1);
////        if (DEBUG_BY_STEP) waitKey(0);
////    }


//int main()
//{
//    Rect t(1, 1, 2, 2);
//    Blob test(t);
//    cout<<test.getBBox().tl()<<" "<<test.getBBox().br()<<endl;

//    vector<pair<double, double>> crossingLineStart;                      //represent counting line
//    vector<pair<double, double>> crossingLineEnd;
//    pair<double, double> straightStart(0.15, 0.45);
//    pair<double, double> straightEnd(0.9, 0.45);
//    //pair<double, double> leftStart(0.15, 0);
//    //pair<double, double> leftEnd(0.15, 0.45);
//    //pair<double, double> rightStart(0.75, 0);
//    //pair<double, double> rightEnd(0.75, 0.45);

//    crossingLineStart.push_back(straightStart);
//    //crossingLineStart.push_back(leftStart);
//    //crossingLineStart.push_back(rightStart);
//    crossingLineEnd.push_back(straightEnd);
//    //crossingLineEnd.push_back(leftEnd);
//    //crossingLineEnd.push_back(rightEnd);

//    //carCounting(crossingLineStart, crossingLineEnd);
//    return 0;
//}

