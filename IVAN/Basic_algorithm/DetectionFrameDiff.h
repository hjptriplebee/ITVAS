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
    int thresholdValue;
    bool isAdaptive; // is adaptive frame difference?
    Mat smoothKernel; //big kernel for noisy image

public:
    /*****************************************
     *  @brief init parameters from file
     *  @param path: the location of basical config file
     *  @param path2: the location of frame difference config file
     * ***************************************/
    DetectionFrameDiff(string path, string path2);// init parameters from file, path is the location of file
    /*****************************************
     *  @brief detect object in frame
     *  @param pre: pre frame
     *  @param cur: cur frame
     *  @return list<Blob>: contains detected Blob
     * ***************************************/
    list<Blob> detect(const Mat &pre, const Mat &cur) const;
    /*****************************************
     *  @brief friend function: reload << to test
     * ***************************************/
    friend ostream& operator<<(ostream &out, const DetectionFrameDiff &D);
};

#endif // FRAMEDIFFDETECTION_H
