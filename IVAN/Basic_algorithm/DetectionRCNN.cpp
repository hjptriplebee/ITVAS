/******************************************
   file name: DetectionRCNN.cpp
   create time: 2017年12月27日 星期三 22时32分18秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: class of RCNN detection algorithm
******************************************/
#include "Basic_algorithm.h"
DetectionRCNN::DetectionRCNN(string path) : Detection(path)
{
    Py_Initialize();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/data/jipeng/IVAN/interface/')");
    PyRun_SimpleString("sys.path.append('/data/jipeng/IVAN/models-master/research/')");
    PyRun_SimpleString("sys.path.append('/data/jipeng/IVAN/models-master/research/slim/')");

    pModule = PyImport_ImportModule("DLDetection");
    // load module
    if (!pModule) cout << "Load detection module failed!" << endl;
    pDict = PyModule_GetDict(pModule);
    // load function
    pFunction = PyDict_GetItemString(pDict, "detect");
    if (!pFunction || !PyCallable_Check(pFunction)) cout << "Load detection function failed!" << endl;
    init_numpy(); //init numpy

}

DetectionRCNN::~DetectionRCNN()
{
    Py_Finalize();
}

list<Blob> DetectionRCNN::detect(const Mat &frame)
{
    list<Blob> result;
    npy_intp Dims[3] = { frame.rows, frame.cols, frame.channels() }; //get size
    pImgArray = PyArray_SimpleNewFromData(3, Dims, NPY_UBYTE, reinterpret_cast<void*>(frame.data)); //create array
    pFunctionBack = PyObject_CallFunction(pFunction, "O", pImgArray); //call python detection

    if (PyList_Check(pFunctionBack)) //result of detection is not empty
    {
        double box[5];
        for(int i = 0; i < PyList_Size(pFunctionBack); i++)
        {
            pReturnItem = PyList_GetItem(pFunctionBack, i); //get i-th item
            box[i % 5] = PyFloat_AsDouble(pReturnItem);
            if(i % 5 == 4) //An object has processed
                result.push_back(Blob(Rect(Point(box[1], box[2]), Point(box[3], box[4])), box[0]));
        }
    }
    filter(result);
    Geometry::nonMaximumSuppression(result, 0.8);
    return  result;
}

ostream& operator<<(ostream &out, const DetectionRCNN &D)
{
    out<<"DetectionRCNN parameters:"<<endl;
    out<<"Module "<<(D.pModule ? "loaded" : "not loaded")<<endl;
    return out;
}
