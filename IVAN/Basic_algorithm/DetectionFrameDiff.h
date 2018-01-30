/******************************************
   file name: DetectionFrameDiff.h
   create time: 2017年12月27日 星期三 21时52分13秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: class of frame difference detection algorithm
******************************************/
#ifndef DETECTIONFRAMEDIFF_H
#define DETECTIONFRAMEDIFF_H

class DetectionFrameDiff : public Detection
{
private:
    int threshold;
    bool isAdaptive; // is adaptive frame difference?
    Mat smoothKernel; //big kernel for noisy image

public:
    DetectionFrameDiff(string path);// init parameters from file, path is the location of file
    /*****************************************
     *  @brief detect object in frame
     *  @param pre: pre frame
     *  @param cur: cur frame
     *  @return list<Blob>: contains detected Blob
     * ***************************************/
    list<Blob> detect(Mat &pre, Mat &cur) const;
};

#endif // FRAMEDIFFDETECTION_H
