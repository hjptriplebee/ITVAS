/******************************************
   file name: Blob.h
   create time: 2017年12月26日 星期二 20时42分26秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: Base class of object
******************************************/
#ifndef BLOB_H
#define BLOB_H

class Blob
{
private:
    vector<Point> contour;
    Rect bBox;
    Point center;
    int width;
    int height;
    int area;
    double diagonal;
    double ratio;
    double score;
    Scalar color;

public:
    /*****************************************
     *  @brief construct Blob with contour c
     *  @param c: contour c
     * ***************************************/
    Blob(vector<Point> &c);
    /*****************************************
     *  @brief construct Blob with bBox box
     *  @param box: boundingbox box
     * ***************************************/
    Blob(Rect box);

    /*****************************************
     *  @brief construct Blob with bBox box and score
     *  @param box: boundingbox
     *  @param s: score
     * ***************************************/
    Blob(Rect box, double s);

    //set function
    void setBBox(Rect newBox);
    inline void setColor(Scalar c){color = c;}
    inline void setScore(double s){score = s;}

    //get function -----inline-----
    inline vector<Point> getContour() const {return contour;}
    inline Rect getBBox() const {return bBox;}
    inline Point getCenter() const {return center;}
    inline int getWidth() const {return width;}
    inline int getHeight() const {return height;}
    inline int getArea() const {return area;}
    inline double getDiagonal() const {return diagonal;}
    inline double getRatio() const {return ratio;}
    inline double getScore() const {return score;}
    inline Scalar getColor() const {return color;}

};

#endif // BLOB_H
