/******************************************
   file name: Detection.cpp
   create time: 2017年12月27日 星期三 21时49分13秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: base class of detection algorithm
******************************************/
#include "Basic_algorithm.h"

Detection::Detection(string path)
{
    if(path.empty()) path = "Detection.config";
    maxWidth = 4096, minWidth = 0;
    maxHeight = 4096, minHeight = 0;
    maxArea = 4096 * 4096, minArea = 0;
    maxDiagonal = 1e8, minDiagonal = 0;
    maxRatio = 4096, minRatio = 0;
    freopen(path.c_str(), "r", stdin);
    string name, value;
    while(cin>>name>>value)
    {
        if(name == "maxWidth") maxWidth = stoi(value);
        else if(name == "minWidth") minWidth = stoi(value);
        else if(name == "maxHeight") maxHeight = stoi(value);
        else if(name == "minHeight") minHeight = stoi(value);
        else if(name == "maxArea") maxArea = stoi(value);
        else if(name == "minArea") minArea = stoi(value);
        else if(name == "maxDiagonal") maxDiagonal = stod(value);
        else if(name == "minDiagonal") minDiagonal = stod(value);
        else if(name == "maxRatio") maxRatio = stod(value);
        else if(name == "minRatio") minRatio = stod(value);
    }
    freopen("CON", "r", stdin);
}

void Detection::filter(list<Blob> &blobs) const
{
    for(auto it = blobs.begin(); it != blobs.end();)
    {
        if(
                it->getArea() < minArea || it->getArea() > maxArea ||
                it->getRatio() < minRatio || it->getRatio() > maxRatio ||
                it->getWidth() < minWidth || it->getWidth() > maxWidth ||
                it->getHeight() < minHeight || it->getHeight() > maxHeight ||
                it->getDiagonal() < minDiagonal || it->getDiagonal() > maxDiagonal
          )
            blobs.erase(it);
        else
            it++;
    }
}
