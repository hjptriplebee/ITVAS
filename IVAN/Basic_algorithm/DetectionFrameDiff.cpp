/******************************************
   file name: DetectionFrameDiff.cpp
   create time: 2017年12月27日 星期三 21时52分13秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: class of frame difference detection algorithm
******************************************/
#include "Basic_algorithm.h"

DetectionFrameDiff::DetectionFrameDiff(string path, string path2) : Detection(path)
{
    if(path2.empty()) path2 = "DetectionFrameDiff.config";
    thresholdValue = 10;
    isAdaptive = false;
    smoothKernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    freopen(path2.c_str(), "r", stdin);
    string name, value;
    while(cin>>name>>value)
    {
        if(name == "threshold") thresholdValue = stoi(value);
        else if(name == "isAdaptive") isAdaptive = stoi(value);
        else if(name == "smoothKernelSize") smoothKernel = getStructuringElement(MORPH_RECT, Size(stoi(value), stoi(value)));
    }
    freopen("CON", "r", stdin);
}

ostream& operator<<(ostream &out, const DetectionFrameDiff &D)
{
    out<<"DetectionRCNN parameters:"<<endl;
    out<<"isAdaptive: "<<D.isAdaptive<<", smoothKernelSize: "<<D.smoothKernel.size()<<", threshold: "<<D.thresholdValue<<endl;
    return out;
}

list<Blob> DetectionFrameDiff::detect(const Mat &pre, const Mat &cur) const
{
    Mat frame1Copy = pre;
    Mat frame2Copy = cur;
    Mat imgDifference;                     //frame difference
    Mat imgThresh;                         //binarization
    cvtColor(frame1Copy, frame1Copy, CV_BGR2GRAY);                    //to gray
    cvtColor(frame2Copy, frame2Copy, CV_BGR2GRAY);

    GaussianBlur(frame1Copy, frame1Copy, Size(5, 5), 0);              //GaussianBlur for smoothing
    GaussianBlur(frame2Copy, frame2Copy, Size(5, 5), 0);

    absdiff(frame1Copy, frame2Copy, imgDifference);                   //frame difference
    if(!isAdaptive)                                      //binarization
        threshold(imgDifference, imgThresh, thresholdValue, 255.0, CV_THRESH_BINARY);
    else
        adaptiveThreshold(imgDifference, imgThresh, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 25, 0);

    for (int i = 0; i < 2; i ++)                        //dilate and erode
    {
        dilate(imgThresh, imgThresh, smoothKernel, Point(-1, -1), 2);
        erode(imgThresh, imgThresh, smoothKernel);
    }

    vector<vector<Point> > contours; //contours
    findContours(imgThresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > convexHulls(contours.size());
    list<Blob> result;
    for (unsigned int i = 0; i < contours.size(); i ++)
    {
        convexHull(contours[i], convexHulls[i]);
        Blob possibleBlob(convexHulls[i]);
        if(contourArea(convexHulls[i]) / (double)possibleBlob.getArea() > 0.5  && !isFilter(possibleBlob)) //contour area / rect area and size filter
        {
            bool overlappedFlag = false;
            for(auto pre : result) //filter for overlapped blobs
            {
                if(Geometry::isOverlapped(pre.getBBox(), possibleBlob.getBBox(), 0.8))
                {
                    overlappedFlag = true;
                    if(pre.getArea() < possibleBlob.getArea()) pre = possibleBlob;
                    break;
                }
            }
            if(!overlappedFlag) result.push_back(possibleBlob);
        }
    }

#ifdef SHOW_DETECTIONFRAMEDIFF
        imshow("imgThresh", imgThresh);
        Show::showContours(imgThresh.size(), contours, "imgContours");
        Show::showContours(imgThresh.size(), convexHulls, "imgConvexHulls");
        Show::showBBox(imgThresh.size(), result, "imgCurrentBlobs");
#endif

    return result;
}
