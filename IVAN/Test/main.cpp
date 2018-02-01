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

    //DetectionFrameDiff D("/data/jipeng/IVAN/IVAN/Basic_algorithm/Detection.config", "/data/jipeng/IVAN/IVAN/Basic_algorithm/DetectionFrameDiff.config");

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

////        vector<Blob> currentBlobs;             //blob in current frame
////        vector<Blob> tempBlobs;                //temp blob

////        Mat imgThreshCopy = imgThresh.clone();

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

////        if (DEBUG_BY_STEP) waitKey(0);
////    }



