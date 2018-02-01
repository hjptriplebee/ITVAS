/******************************************
   file name: BlobVehicle.h
   create time: 2017年12月26日 星期二 20时43分36秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: class of vehicle
******************************************/
#ifndef BLOBVEHICLE_H
#define BLOBVEHICLE_H

class BlobVehicle : public Blob
{
private:
    int ID;
    string type;
    //vector<Point> centers;

public:
    /*****************************************
     *  @brief construct Blob with contour c
     *  @param c: contour c
     * ***************************************/
    BlobVehicle(vector<Point> &c);
    /*****************************************
     *  @brief construct Blob with bBox box
     *            usually, we can get its score, if we get its bbox directly.
     *  @param box: boundingbox
     *  @param s: score
     * ***************************************/
    BlobVehicle(Rect box, double s);

    //set function
    inline void setID(int id){ID = id;}
    inline void setType(string t){type = t;}

    //get function
    inline int getID() const {return ID;}
    inline string getType() const {return type;}
    //inline vector<Point> getCenters() const {return }

};

#endif // VEHICLEBLOB_H
