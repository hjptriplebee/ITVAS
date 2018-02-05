/******************************************
   file name: Geometry.cpp
   create time: 2017年12月28日 星期四 13时53分46秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: class of computational geometry
******************************************/
#include "Common.h"

double Geometry::getDistance(const Point &A, const Point &B)
{
    return sqrt((double)(A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}

int Geometry::crossProduct(const Point &A, const Point &B)
{
    return A.x * B.y - A.y * B.x;
}

bool Geometry::rapidRejectionTest(const Point &A1, const Point &A2, const Point &B1, const Point &B2)
{
    return min(A1.x, A2.x) <= max(B1.x, B2.x) && min(B1.x, B2.x) <= max(A1.x, A2.x) &&
        min(A1.y, A2.y) <= max(B1.y, B2.y) && min(B1.y, B2.y) <= max(A1.y, A2.y);
}

bool Geometry::isSegmentCross(const Point &A1, const Point &A2, const Point &B1, const Point &B2)
{
    if (!rapidRejectionTest(A1, A2, B1, B2)) return false;
    Point AB(A2.x - A1.x, A2.y - A1.y);
    Point AC(B1.x - A1.x, B1.y - A1.y);
    Point AD(B2.x - A1.x, B2.y - A1.y);
    Point CA(A1.x - B1.x, A1.y - B1.y);
    Point CB(A2.x - B1.x, A2.y - B1.y);
    Point CD(B2.x - B1.x, B2.y - B1.y);
    if (crossProduct(AB, AC) * crossProduct(AB, AD) < 0 || crossProduct(CD, CA) * crossProduct(CD, CB)) return true;
    return false;
}

bool Geometry::isOverlapped(const Rect &box1, const Rect &box2, double IOUThreshold)
{
    Point tl1 = box1.tl(), br1 = box1.br();
    Point tl2 = box2.tl(), br2 = box2.br();
    if (tl1.x > br2.x) return false; //disjoint from each other
    if (tl1.y > br2.y) return false;
    if (tl2.x > br1.x) return false;
    if (tl2.y > br1.y) return false;
    int colInt = min(br1.x, br2.x) - max(tl1.x, tl2.x);
    int rowInt = min(br1.y, br2.y) - max(tl1.y, tl2.y);
    int intersection = colInt * rowInt;
    if ((float)intersection / box1.area() > IOUThreshold) return true;

    return false;
}

bool cmpNMS(const Blob &a, const Blob &b)
{
    return a.getScore() > b.getScore();
}

bool isBlobScoreDecendingOrder(list<Blob> &blobs)
{
    double preScore = 1e9;
    for (auto blob : blobs)
    {
        if(blob.getScore() > preScore) return false;
        preScore = blob.getScore();
    }
    return true;
}

void Geometry::nonMaximumSuppression(list<Blob> &blobs, double IOUThreshold)
{
    if(!isBlobScoreDecendingOrder(blobs)) blobs.sort(cmpNMS); //not sorted
    for(auto it = blobs.begin(); it != blobs.end();)
    {
        bool eraseFlag = false;
        for(auto it2 = blobs.begin(); it2 != it;)
        {
            if(isOverlapped(it->getBBox(), it2->getBBox(), IOUThreshold))
            {
                blobs.erase(it++);
                eraseFlag = true;
                break;
            }
            else
                it2++;
        }
        if(!eraseFlag) it++;
    }
}
