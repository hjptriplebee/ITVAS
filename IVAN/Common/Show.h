/******************************************
   file name: Show.h
   create time: 2018年1月31日 星期四 13时53分46秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: class of show function
******************************************/
#ifndef SHOW_H
#define SHOW_H

class Show
{
public:
    /*****************************************
     *  @brief show contours
     *  @param size: image size
     *  @param contours: contours
     *  @param windowName: window name
     * ***************************************/
    static void showContours(const Size size, const vector<vector<Point> > &contours, const string windowName);
    /*****************************************
     *  @brief show contours
     *  @param size: image size
     *  @param blobs: blobs
     *  @param windowName: window name
     * ***************************************/
    static void showContours(const Size size, const vector<Blob> &blobs, const string windowName);
    /*****************************************
     *  @brief show bbox
     *  @param size: image size
     *  @param blobs: blobs
     *  @param windowName: window name
     * ***************************************/
    static void showBBox(const Size size, const list<Blob> &blobs, const string windowName);
};

#endif // SHOW_H
