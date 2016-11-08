#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> 
#include <iostream>
#include <cstring>
#include <fstream> 
#include <queue>
using namespace std;
using namespace cv;
#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\reid_tracking.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\reid_tracking.lib")
#endif

const double detectionResizeCol = 2; //检测图像列缩放系数
const double detectionResizeRow = 2; //检测图像行缩放系数
const double trackingResizeCol = 5; //跟踪图像列缩放系数
const double trackingResizeRow = 5; //跟踪图像行缩放系数
const int detectionSkip = 30; //检测间隔
const int trackingSkip = 1; //跟踪间隔 
const int lineThickness = 8; //标注框粗细
/***************************************************************************
函数功能：判断两个boundingbox是否为一个物体
参数说明：边界1左上点，边界1右下点，边界2左上点，边界2右下点，交并比阈值
返回：两个boundingbox是否为一个物体
作者：jipeng huang
时间：2016-10-10
***************************************************************************/
bool isOverlap(Point tl1, Point br1, Point tl2, Point br2, double threshold=0.5)
{
	if (tl1.x > br2.x) return false; //四种不相交的情况
	if (tl1.y > br2.y) return false;
	if (tl2.x > br1.x) return false;
	if (tl2.y > br1.y) return false;
	float colInt = (float)min(br1.x, br2.x) - max(tl1.x, tl2.x);
	float rowInt = (float)min(br1.y, br2.y) - max(tl1.y, tl2.y);
	float intersection = colInt * rowInt;
	float area1 = (float)(br1.x - tl1.x)*(br1.y - tl1.y);
	float area2 = (float)(br2.x - tl2.x)*(br2.y - tl2.y);
	if (intersection / (area1 + area2 - intersection) > threshold) return true;
	return false;
}
/***************************************************************************
函数功能：生成可用颜色
参数说明：原色变化步长
返回：可用颜色队列
作者：jipeng huang
时间：2016-10-09
***************************************************************************/
queue<Scalar> generateColor(int step=63)
{
	queue<Scalar> q;
	for (int red = 0; red < 256; red += step)
		for (int green = 0; green < 256; green += step)
			for (int blue = 0; blue < 256; blue += step)
				q.push(Scalar(red,green,blue));
	return q;
}
/***************************************************************************
函数功能：单帧车辆检测
参数说明：检测图像，HOG检测子
返回：车辆boundingbox的容器
作者：jipeng huang
时间：2016-10-08
***************************************************************************/
vector<Rect> detection(Mat &src,HOGDescriptor hog)
{
	Mat dst;
	resize(src, dst, Size((int)(src.cols / detectionResizeCol), (int)(src.rows / detectionResizeRow)));
	fflush(stdout);
	vector<Rect> found, found_filtered;
	double t = (double)getTickCount();
	//多尺度检测
	hog.detectMultiScale(dst, found, 1.3, Size(16, 16), Size(0, 0), 1.3, 5);
	t = (double)getTickCount() - t;
	printf("detection time = %gms\n", t*1000. / cv::getTickFrequency());
	//去掉空间中具有内外包含关系的区域，保留大的
	vector<Rect>::iterator it = found.begin();
	for (; it != found.end();)
	{
		if ((*it).height > 500) //筛掉太大的区域
			found.erase(it);
		else
			it++;
	}
	for (int i = 0, j; i < found.size(); i++)
	{
		for (j = i+1; j < found.size(); j++)
		if (isOverlap(found[i].tl(), found[i].br(), found[j].tl(), found[j].br(), 0.1))
			break;
		if (j == found.size())
			found_filtered.push_back(found[i]);
	}
	/*for (int i = 0, j ; i < found.size(); i++)
	{
		Rect r = found[i];
		for (j = 0; j < found.size(); j++) 
			if (j != i && (r & found[j]) == r) 
				break;
		if (j == found.size())
			found_filtered.push_back(r);
	}*/
	// 适当缩小矩形,并还原成原坐标
	for (int i = 0; i < found_filtered.size(); i++)
	{
		//found_filtered[i].x += cvRound(found_filtered[i].width*0.1);
		found_filtered[i].x = cvRound(found_filtered[i].x*detectionResizeCol);
		//found_filtered[i].width = cvRound(found_filtered[i].width*0.8*detectionResizeCol);
		found_filtered[i].width = cvRound(found_filtered[i].width*detectionResizeCol);
		//found_filtered[i].y += cvRound(found_filtered[i].height*0.07);
		found_filtered[i].y = cvRound(found_filtered[i].y*detectionResizeRow);
		//found_filtered[i].height = cvRound(found_filtered[i].height*0.8*detectionResizeRow);
		found_filtered[i].height = cvRound(found_filtered[i].height*detectionResizeRow);	
	}
	return found_filtered;
}
/***************************************************************************
函数功能：检测+跟踪
参数说明：文件路径
返回：
作者：jipeng huang
时间：2016-10-08
***************************************************************************/
void detection_tracking(string path)
{
	VideoWriter writer("E:\\RE-ID\\数据集\\车辆\\video\\20160719\\demo.avi", ('M', 'J', 'P', 'G'), 29, Size(3840, 2160));

	VideoCapture cap(path);
	//加载训练好的判别函数的参数(注意，与svm->save保存的分类器不同)
	vector<float> detector;
	ifstream fileIn(string("E:\\RE-ID\\训练数据\\车辆\\") + "HOG_SVM.txt", ios::in);
	float val = 0.0f;
	while (!fileIn.eof())
	{
		fileIn >> val;
		detector.push_back(val);
	}
	fileIn.close();
	//设置HOG
	HOGDescriptor hog(cvSize(128, 128), cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 9);
	hog.setSVMDetector(detector);// 使用自己训练的分类器
	namedWindow("car detector", WINDOW_NORMAL);
	Mat src, dst;
	vector<Rect> cars;//检测出的车辆
	vector<Rect2d> preCars; //之前的车辆
	vector<Scalar> nowColor, preColor;//现在的配色，之前的配色
	queue<Scalar> colorQueue = generateColor(127);//生成矩形框颜色
	for (int frameCnt = 0;; frameCnt++)
	{
		cap >> src;
		if (src.empty()) break; //处理最后一帧s
		if (frameCnt%detectionSkip == 0)//检测
		{
			nowColor.clear();
			cars = detection(src, hog);
			for (int i = 0,j; i < cars.size(); i++)
			{
				for (j = 0; j < preCars.size(); j++)
					if (isOverlap(cars[i].tl(), cars[i].br(), preCars[j].tl(), preCars[j].br(), 0.1)) break;
				if (j == preCars.size()) //没有重合的，新车，重新分配颜色
				{
					nowColor.push_back(colorQueue.front());
					colorQueue.pop();
				}
				else//旧车	
					nowColor.push_back(preColor[j]);
				rectangle(src, cars[i], nowColor[i], lineThickness);
			}
			imshow("car detector",src);
			writer << src;
			waitKey(1);
			//释放preColor,preCars
			preCars.clear();
			for (int i = 0, j; i < preColor.size(); i++)
			{
				for (j = 0; j < nowColor.size(); j++)
					if (nowColor[j] == preColor[i]) break;
				if (j == nowColor.size()) //旧车出镜头，回收未用到的颜色
					colorQueue.push(preColor[i]);
			}
			preColor.clear();
		}
		else //跟踪
		{
			vector<Rect2d> temp;
			MultiTracker tracker("KCF");
			resize(src, dst, Size((int)(src.cols / trackingResizeCol), (int)(src.rows / trackingResizeRow)));
			for (int i = 0; i < cars.size(); i++) //boundingbox缩放
			{
				Rect2d now;
				now.x = cvRound(cars[i].x / trackingResizeCol);
				now.width = cvRound(cars[i].width / trackingResizeCol);
				now.y = cvRound(cars[i].y / trackingResizeRow);
				now.height = cvRound(cars[i].height / trackingResizeRow);
				temp.push_back(now);
			}
			tracker.add(dst,temp);//boundingbox初始化
			for (; frameCnt%detectionSkip != detectionSkip - 1; frameCnt++)
			{
				cap >> src;
				if (src.empty()) break;
				if (frameCnt%trackingSkip == 0) //需要tracking的帧
				{
					double t = (double)getTickCount();
					resize(src, dst, Size((int)(src.cols / trackingResizeCol), (int)(src.rows / trackingResizeRow)));
					tracker.update(dst); //KCF
					t = (double)getTickCount() - t;
					printf("tracking time = %gms\n", t*1000. / cv::getTickFrequency());
				}
				preColor = nowColor;
				for (int i = 0; i < tracker.objects.size(); i++)//还原并框选
				{
					Rect2d display;
					display.x = tracker.objects[i].x * trackingResizeCol;
					display.width = tracker.objects[i].width * trackingResizeCol;
					display.y = tracker.objects[i].y * trackingResizeRow;
					display.height = tracker.objects[i].height * trackingResizeRow;
					if (frameCnt%detectionSkip == detectionSkip - 2)//跟踪最后一帧更新preCar
						preCars.push_back(display);
					rectangle(src, display, preColor[i], lineThickness);
				}
				imshow("car detector", src);
				writer << src;
				waitKey(1);
			}
		}
	}
}

int main()
{
	detection_tracking("E:\\RE-ID\\数据集\\车辆\\video\\20160719\\DJI_0006.avi");
	return 0;
}