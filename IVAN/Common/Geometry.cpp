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
