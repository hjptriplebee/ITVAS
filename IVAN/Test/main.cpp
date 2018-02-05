#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

#include <Core.h>
#include <Basic_algorithm.h>
#include <Common.h>

using namespace std;
using namespace cv;

//----------------------------HOGSVM test-------------------------------------------
int main()
{
    DetectionRCNN R("/data/jipeng/IVAN/IVAN/Basic_algorithm/Detection.config");
    VideoCapture cap("/data/jipeng/9_1_1.mp4");
    Mat img, img2;
    while(true)
    {
        cap >> img;
        list<Blob> blobs = R.detect(img);
        for(auto blob: blobs) rectangle(img, blob.getBBox(), Scalar(0, 255, 0), 3);
        resize(img, img2, Size(1080, 720));
        imshow("demo", img2);
        waitKey(1);
    }
    return 0;
}

//----------------------------DetectionFrameDiff test----------------------------------
//int main()
//{
//    DetectionFrameDiff D("/data/jipeng/IVAN/IVAN/Basic_algorithm/Detection.config", "/data/jipeng/IVAN/IVAN/Basic_algorithm/DetectionFrameDiff.config");
//    VideoCapture cap("/data/jipeng/9_1_1.mp4");
//    Mat frame1, frame2, frame3;
//    cap>>frame1;
//    while(true)
//    {
//        cap>>frame2;
//        list<Blob> blobs = D.detect(frame1, frame2);
//        for(auto blob: blobs) rectangle(frame1, blob.getBBox(), Scalar(0, 255, 0), 3);
//        resize(frame1, frame3, Size(1080, 720));
//        imshow("demo", frame3);
//        waitKey(1);
//        frame1 = frame2.clone();

//    }
//    return 0;
//}

////----------------------------DetectionRCNN test-------------------------------------------
//int main()
//{
//    DetectionRCNN R("/data/jipeng/IVAN/IVAN/Basic_algorithm/Detection.config");
//    VideoCapture cap("/data/jipeng/9_1_1.mp4");
//    Mat img, img2;
//    while(true)
//    {
//        cap >> img;
//        list<Blob> blobs = R.detect(img);
//        for(auto blob: blobs) rectangle(img, blob.getBBox(), Scalar(0, 255, 0), 3);
//        resize(img, img2, Size(1080, 720));
//        imshow("demo", img2);
//        waitKey(1);
//    }
//    return 0;
//}


