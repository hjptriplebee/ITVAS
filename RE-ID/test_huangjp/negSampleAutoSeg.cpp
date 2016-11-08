/***************************************************************************
功能：小工具-截取图片或视频中的负样本并保存
作者：jipeng huang
时间：2016-7-13
修改：
使用说明：按回车键确认从当前帧中获取负样本；
超过10s不按键或按除回车外任意键跳过此帧；
生成的负样本需要手工剔除少量正样本
***************************************************************************/
#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;
/***************************************************************************
函数功能：自动分割图像
参数说明：分割图像，当前帧在视频中的位置
返回：
作者：jipeng huang
时间：2016-7-13
修改：
***************************************************************************/
void autoSeg(Mat frame,int frameCnt)
{
	int rowSegNum = 16;
	int colSegNum = 30;
	int winHeight = frame.rows / rowSegNum;
	int winWidth = frame.cols / colSegNum;

	for (int i = 0; i < rowSegNum; i++)
	{
		for (int j = 0; j < colSegNum; j++)
		{
			Rect2d window( j*winWidth, i*winHeight, winWidth, winHeight);
			Mat segFrame = frame(window);	
			stringstream sr,sc,sid;
			sr << i;
			sc << j;
			sid << frameCnt;
			string srString = sr.str();
			string scString = sc.str();
			string sidString = sid.str();
			//保存负样本的路径
			imwrite("E:\\RE-ID\\数据集\\车辆\\video\\20160711\\DJI_0001_"+sidString+"_"+srString+"_"+scString+".jpg",segFrame);
		}
	}
}

/*int main(int argc, char** argv)
{
	cout << "按回车键确认从当前帧中获取负样本"<<endl;
	cout << "超过10s不按键或按除回车外任意键跳过此帧" << endl;
	const int fps = 30;
	string videoPath = "E:\\RE-ID\\数据集\\车辆\\video\\20160711\\DJI_0001.MOV";//视频路径
	VideoCapture cap(videoPath);
	Mat frame;
	namedWindow("frame", 0);
	for (int frameCnt=1;;frameCnt++)
	{
		cap >> frame;
		if (frame.empty())break;
		if (frameCnt%fps)continue;
		imshow("frame", frame);
		int key = waitKey(10000);
		if (key == 13) autoSeg(frame,frameCnt);
	}
	return 0;
}
*/