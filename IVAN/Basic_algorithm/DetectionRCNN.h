/******************************************
   file name: DetectionRCNN.h
   create time: 2017年12月27日 星期三 22时32分18秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: class of RCNN detection algorithm
******************************************/
#ifndef DETECTIONRCNN_H
#define DETECTIONRCNN_H

class DetectionRCNN : public Detection
{
private:
    PyObject* pModule; //module object
    PyObject* pDict; //function dict object
    PyObject* pFunction; //function object
    PyObject* pImgArray; //image to array object
    PyObject* pFunctionBack; //function return object
    PyObject* pReturnItem; //return item object

public:
    /*****************************************
     *  @brief init parameters from file
     *  @param path: the location of file
     * ***************************************/
    DetectionRCNN(string path);
    /*****************************************
     *  @brief Destructor function
     * ***************************************/
    ~DetectionRCNN();
    /*****************************************
     *  @brief detect object in frame
     *  @param frame: frame for detect
     *  @return list<Blob>: contains detected Blob
     * ***************************************/
    virtual list<Blob> detect(Mat &frame) override;
    /*****************************************
     *  @brief init numpy
     * ***************************************/
#pragma GCC diagnostic ignored "-Wconversion-null"
#pragma GCC diagnostic ignored "-Wreturn-type"
    inline int init_numpy(){import_array();}
#pragma GCC diagnostic warning "-Wconversion-null"
#pragma GCC diagnostic warning "-Wreturn-type"

};

#endif // RCNNDETECTION_H
