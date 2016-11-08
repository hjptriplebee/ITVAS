/***************************************************************************
功能：小工具-截取图片或视频中的对象区域并保存
作者：jipeng huang
时间：2016-7-12
修改：
使用说明：
首先选择分割模式，输入1为截取图像，输入2为截取视频；
按住鼠标左键并拖动选择截取区域，不满意可重复选择；
按回车键确认截取，该区域保存为.jpg格式的图像；
完成一张图像的截取后，按ESC键进入下一张图像或后续视频帧；
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

class BoxExtractor 
{
public:
	int key;
	Rect2d extract(const std::string& windowName, Mat img);
	struct handlerT
	{
		bool isDrawing;
		Rect2d box;
		Mat image;
		handlerT() : isDrawing(false) {};
	}params;

private:
	static void mouseHandler(int event, int x, int y, int flags, void *param);
	void opencv_mouse_callback(int event, int x, int y, void *param);
};
/***************************************************************************
函数功能：鼠标事件初始化
参数说明：时间，横坐标，纵坐标，flags(并未使用，只是满足opencv的调用形式)，this指针
返回：
作者：jipeng huang
时间：2016-7-12
修改：
***************************************************************************/
void BoxExtractor::mouseHandler(int event, int x, int y, int flags, void *param)
{
	BoxExtractor *self = static_cast<BoxExtractor*>(param);
	self->opencv_mouse_callback(event, x, y, param);
}
/***************************************************************************
函数功能：鼠标事件响应
参数说明：事件，横坐标，纵坐标，this指针
返回：
作者：jipeng huang
时间：2016-7-12
修改：
***************************************************************************/
void BoxExtractor::opencv_mouse_callback(int event, int x, int y, void *param)
{
	handlerT * data = (handlerT*)param;
	switch (event)
	{
		//移动-更新域
	case EVENT_MOUSEMOVE:
		if (data->isDrawing)
		{
			data->box.width = x - data->box.x;
			data->box.height = y - data->box.y;
		}
		break;
		//左键按下
	case EVENT_LBUTTONDOWN:
		data->isDrawing = true;
		data->box = cvRect(x, y, 0, 0);
		break;
		//左键松开
	case EVENT_LBUTTONUP:
		data->isDrawing = false;
		if (data->box.width < 0)
		{
			data->box.x += data->box.width;
			data->box.width *= -1;
		}
		if (data->box.height < 0)
		{
			data->box.y += data->box.height;
			data->box.height *= -1;
		}
		break;
	}
}
/***************************************************************************
函数功能：提取图片区域
参数说明：窗口名称，输入图像
返回：提取区域的boundingbox
作者：jipeng huang
时间：2016-7-12
修改：
***************************************************************************/
Rect2d BoxExtractor::extract(const std::string& windowName, Mat img)
{
	imshow(windowName, img);
	params.image = img.clone();
	setMouseCallback(windowName, mouseHandler, (void *)&params);
	while (!(key == 27 || key == 13))
	{
		rectangle(params.image, params.box, Scalar(0, 255, 0), 2, 1);
		imshow(windowName, params.image);
		params.image = img.clone();
		key = waitKey(1);
	}
	return params.box;
}

void segImage()
{
	BoxExtractor box;
	freopen("E:\\RE-ID\\训练数据\\车辆\\标记数据\\FileNameList.txt","r",stdin);//批处理文件生成的文件路径列表
	string fileNameList;
	while (cin >> fileNameList)
	{
		string fileName = fileNameList.substr(fileNameList.length()-14,10);
		//ofstream boundingBoxFile("E:\\RE-ID\\训练数据\\车辆\\标记数据\\"+fileName+".txt");//保存boundingbox的文件路径
		Mat frame=imread(fileNameList);
		//imwrite("E:\\RE-ID\\训练数据\\车辆\\标记数据\\" + fileName + ".jpg", frame);
		for (int objectCnt = 1;; objectCnt++)
		{
			box.key = -1;
			Rect2d roi = box.extract("Origin", frame);
			if (roi.width == 0 || roi.height == 0) 
			{
				objectCnt--;
				continue;
			}
			Mat saveRoi = frame(roi);
			imshow("Extract", saveRoi);

			stringstream ss;
			ss << objectCnt;
			string objectId = ss.str();
			imwrite("E:\\RE-ID\\训练数据\\车辆\\标记数据\\"+fileName+"_"+objectId+".jpg",saveRoi);//保存提取图像的路径
			//boundingBoxFile << roi.x << " " << roi.y << " " << roi.height << " " << roi.width << endl;
			box.params.isDrawing = false;//清除前一张图像选取的边框
			box.params.box.height = 0;
			box.params.box.width = 0;
			if (box.key == 27) break;
		}
		//boundingBoxFile.close();
	}
}
void segVideo()
{
	const int fps = 50;
	BoxExtractor box;
	string videoPath = "E:\\RE-ID\\数据集\\车辆\\video\\20160711\\";
	string videoName = "DJI_0005.MOV";
	VideoCapture cap(videoPath+videoName);
	Mat frame;
	for (int frameCnt = 1;; frameCnt++)
	{
		cap >> frame;
		if (frame.empty()) break;
		if (frameCnt%fps) continue;
		imshow("Origin",frame);
		if (waitKey(1000000) != 13) continue;
		for (int objectCnt = 1;; objectCnt++)
		{
			box.key = -1;
			Rect2d roi = box.extract("Origin", frame);
			if (roi.width == 0 || roi.height == 0)
			{
				objectCnt--;
				continue;
			}
			Mat saveRoi = frame(roi);
			imshow("Extract", saveRoi);

			stringstream ss;
			ss << objectCnt;
			string objectId = ss.str();
			ss.str("");
			ss << frameCnt;
			string frameId = ss.str();
			//cout << (videoPath + videoName + "_pos_" + frameId + string("_") + objectId + ".jpg") << endl;
			imwrite(videoPath + videoName + "_pos_" + frameId + string("_") + objectId + ".jpg", saveRoi);//保存提取图像的路径
			box.params.isDrawing = false;//清除前一张图像选取的边框
			box.params.box.height = 0;
			box.params.box.width = 0;
			if (box.key == 27) break;
		}
	}
}
/*
int main(int argc, char** argv)
{
	cout << "截取图片请输入1" << endl << "截取视频请输入2" << endl;
	int choice;
	cin >> choice;
	cout << "按回车确认选取目标" << endl << "按ESC进入下一张图片或后续视频帧" << endl;
	namedWindow("Origin", WINDOW_KEEPRATIO);
	switch (choice)
	{
	case 1: segImage(); break;
	case 2: segVideo(); break;
	}
	return 0;
}
*/