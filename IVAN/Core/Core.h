/******************************************
   file name: Core.h
   create time: 2017年12月26日 星期二 20时42分24秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: Header set of Core
******************************************/
#ifndef CORE_H
#define CORE_H
//outer library
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

//c++ library
#include <string>
#include <vector>
using namespace std;

//other inner file


//inner file
//#include "Core_global.h"
#include "Blob.h"
#include "BlobVehicle.h"
#include "BlobPerson.h"

//const
const Scalar BLACK = Scalar(  0,   0,   0);
const Scalar WHITE = Scalar(255, 255, 255);
const Scalar GREEN = Scalar(  0, 255,   0);
const Scalar RED   = Scalar(  0,   0, 255);

#endif // CORE_H
