/******************************************
   file name: Show.cpp
   create time: 2018年1月31日 星期四 13时53分46秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: class of show function
******************************************/
#include "Common.h"

void Show::showContours(const Size size, const vector<vector<Point> > &contours, const string windowName)
{
    Mat image(size, CV_8UC3, BLACK);
    drawContours(image, contours, -1, WHITE, -1);
    imshow(windowName, image);
    return;
}

void Show::showContours(const Size size, const vector<Blob> &blobs, const string windowName)
{
    Mat image(size, CV_8UC3, BLACK);
    vector<vector<Point> > contours;
    for (auto &blob : blobs) contours.push_back(blob.getContour());
    drawContours(image, contours, -1, WHITE, -1);
    imshow(windowName, image);
    return;
}
void Show::showBBox(const Size size, const list<Blob> &blobs, const string windowName)
{
    Mat image(size, CV_8UC3, BLACK);
    vector<vector<Point> > contours;
    for (auto &blob : blobs)
    {
        vector<Point> contour;
        contour.push_back(blob.getBBox().tl());
        contour.push_back(Point(blob.getBBox().tl().x + blob.getBBox().width, blob.getBBox().tl().y));
        contour.push_back(blob.getBBox().br());
        contour.push_back(Point(blob.getBBox().br().x - blob.getBBox().width, blob.getBBox().br().y));
        contours.push_back(contour);
    }
    drawContours(image, contours, -1, WHITE, -1);
    imshow(windowName, image);
    return;
}
