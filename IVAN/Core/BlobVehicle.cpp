/******************************************
   file name: BlobVehicle.cpp
   create time: 2017年12月26日 星期二 20时43分36秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: class of vehicle
******************************************/
#include "Core.h"
BlobVehicle::BlobVehicle(vector<Point> &c) : Blob(c)
{

}

BlobVehicle::BlobVehicle(Rect box, double s) : Blob(box, s)
{

}
