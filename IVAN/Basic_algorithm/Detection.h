/******************************************
   file name: Detection.h
   create time: 2017年12月27日 星期三 21时49分13秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: base class of detection algorithm
******************************************/
#ifndef DETECTION_H
#define DETECTION_H

class Detection
{
private:
    //some parameters
    int maxWidth, minWidth;
    int maxHeight, minHeight;
    int maxArea, minArea;
    double maxDiagonal, minDiagonal;
    double maxRatio, minRatio;
    //......

public:
    /*****************************************
     *  @brief init parameters from file
     *  @param path: the location of file
     * ***************************************/
    Detection(string path);
    /*****************************************
     *  @brief detect object in frame, virtual but not pure virtual
     *  @param frame: frame for detect
     *  @return list<Blob>: contains detected Blob
     * ***************************************/
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
    virtual list<Blob> detect(Mat &frame) {}
#pragma GCC diagnostic warning "-Wreturn-type"
#pragma GCC diagnostic warning "-Wunused-parameter"
    /*****************************************
     *  @brief filter impossible blobs with parameters
     *  @param blobs: blobs for filter
     *  @return none
     * ***************************************/
    void filter(list<Blob> &blobs) const;
};

#endif // DETECTION_H
