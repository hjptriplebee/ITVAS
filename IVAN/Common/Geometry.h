/******************************************
   file name: Geometry.h
   create time: 2017年12月28日 星期四 13时53分46秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: class of computational geometry
******************************************/
#ifndef GEOMETRY_H
#define GEOMETRY_H

class Geometry
{
public:
    /*****************************************
     *  @brief get distance between two points
     *  @param A: point A
     *  @param B: point B
     *  @return distance between two points
     * ***************************************/
    static double getDistance(const Point &A, const Point &B);
    /*****************************************
     *  @brief get cross product of two vectors
     *  @param A: point A
     *  @param B: point B
     *  @return product of two vectors
     * ***************************************/
    static int crossProduct(const Point &A, const Point &B);
    /*****************************************
     *  @brief rapid rejection test
     *  @param A1, A2: rectangle with A1 and A2
     *  @param B1, B2: rectangle with B1 and B2
     *  @return true: overlapped
     *  @return false: not overlapped
     * ***************************************/
    static bool rapidRejectionTest(const Point &A1, const Point &A2, const Point &B1, const Point &B2);
    /*****************************************
     *  @brief judge if two segments ara crossed
     *  @param A1, A2: segment A
     *  @param B1, B2: segment B
     *  @return true: crossed
     *  @return false: not crossed
     * ***************************************/
    static bool isSegmentCross(const Point &A1, const Point &A2, const Point &B1, const Point &B2);
    /*****************************************
     *  @brief judge if two Rectangles ara overlapped
     *  @param box1 : Rect A
     *  @param box2: Rect B
     *  @param IOUthreshold: threshold
     *  @return true: overlapped
     *  @return false: not overlapped
     * ***************************************/
    static bool isOverlapped(const Rect &box1, const Rect &box2, double IOUThreshold);
    /*****************************************
     *  @brief non maximum suppression
     *  @param blobs
     *  @param box2: Rect B
     *  @param IOUthreshold: threshold
     * ***************************************/
    static void nonMaximumSuppression(list<Blob> &blobs, double IOUThreshold);
};
#endif // GEOMETRY_H
