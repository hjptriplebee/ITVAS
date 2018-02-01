/******************************************
   file name: Basic_algorithm.h
   create time: 2017年12月27日 星期三 21时47分13秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: Header set of basic algorithm
******************************************/
#ifndef BASIC_ALGORITHM_H
#define BASIC_ALGORITHM_H

//outer library
#include <opencv2/core.hpp>
using namespace cv;
#include <Python.h>
#include </home/jipeng/anaconda3/lib/python3.6/site-packages/numpy/core/include/numpy/arrayobject.h>

//c++ library
#include <list>
#include <iostream>
using namespace std;

//other inner file
#include "Core.h"
//#include "Common.h"

//inner file
//#include "Basic_algorithm_global.h"
#include "Detection.h"
#include "DetectionFrameDiff.h"
#include "DetectionRCNN.h"
#include "Match.h"

//define
#define SHOW_DETECTIONFRAMEDIFF true

#endif // BASIC_ALGORITHM_H
