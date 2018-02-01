/******************************************
   file name: Blob.cpp
   create time: 2017年12月26日 星期二 20时42分26秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: Base class of object
******************************************/
#include "Core.h"

Blob::Blob(Rect box)
{
    setBBox(box);
    color = Scalar(rand() % 255, rand() % 255, rand() & 255);
}

Blob::Blob(vector<Point> &c)
{
    setBBox(boundingRect(c));
    color = Scalar(rand() % 255, rand() % 255, rand() & 255);
}

Blob::Blob(Rect box, double s)
{
    score = s;
    setBBox(box);
    color = Scalar(rand() % 255, rand() % 255, rand() & 255);
}

void Blob::setBBox(Rect newBox)
{
    bBox = newBox;
    width = bBox.width;
    height = bBox.height;
    area = width * height;
    diagonal = sqrt(pow(width, 2) + pow(height, 2));
    ratio = 1.0 * width / height;
    Point curCenter;
    curCenter.x = (bBox.x * 2 + width) / 2;
    curCenter.y = (bBox.y * 2 + height) / 2;
    center = curCenter;
}
