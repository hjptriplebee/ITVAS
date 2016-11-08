// test.cpp : 定义控制台应用程序的入口点。
//
/************************DLL_test*********************************/
/*#include "stdafx.h"
#include "reid_persondetection.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 1;
	int b = 2;
	cout << add(a, b) << endl;
	return 0;
}

*/

/************************opencv_test******************************/
/*#include <stdafx.h>
#include <opencv2\opencv.hpp>  
#include <iostream>  
#include <string>  
using namespace cv;
using namespace std;
int main()
{    
	Mat img = imread("C://Users//admin//Desktop//背景.jpg");    
	if (img.empty())    
	{        
		cout << "error";        
		return -1;    
	}    
	imshow("mypic", img);    
	waitKey();    
	return 0;
} 
*/

#include "stdafx.h"  
#include "cv.h"  
#include <opencv2\opencv.hpp>  
#include "highgui.h"  
#include <ml.h>  
#include <iostream>  
#include <fstream>  
#include <string>  
#include <vector> 
#include <windows.h>
using namespace cv;
using namespace std;

/*
void Train()
{
	string FILEPATH = "";
	////////////////////////////////读入训练样本图片路径和类别///////////////////////////////////////////////////
	//图像路径和类别
	vector<string> imagePath;
	vector<int> imageClass;
	int numberOfLine = 0;
	string buffer;
	ifstream trainingData(string(FILEPATH) + "TrainData.txt");
	unsigned long n;

	while (!trainingData.eof())
	{
		getline(trainingData, buffer);
		if (!buffer.empty())
		{
			++numberOfLine;
			if (numberOfLine % 2 == 0)
			{
				//读取样本类别
				imageClass.push_back(atoi(buffer.c_str()));
			}
			else
			{
				//读取图像路径
				imagePath.push_back(buffer);
			}
		}
	}

	//关闭文件  
	trainingData.close();


	////////////////////////////////获取样本的HOG特征///////////////////////////////////////////////////
	//样本特征向量矩阵
	int numberOfSample = numberOfLine / 2;
	Mat featureVectorOfSample(numberOfSample, 3780, CV_32FC1);//矩阵中每行为一个样本

	//样本的类别
	Mat classOfSample(numberOfSample, 1, CV_32SC1);

	Mat convertedImage;
	Mat trainImage;

	// 计算HOG特征
	for (vector<string>::size_type i = 0; i <= imagePath.size() - 1; ++i)
	{
		//读入图片
		Mat src = imread(imagePath[i], -1);
		if (src.empty())
		{
			cout << "can not load the image:" << imagePath[i] << endl;
			continue;
		}
		//cout << "processing:" << imagePath[i] << endl;

		// 归一化
		resize(src, trainImage, Size(64, 128));

		// 提取HOG特征
		HOGDescriptor hog(cvSize(64, 128), cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 9);
		vector<float> descriptors;
		double time1 = getTickCount();
		hog.compute(trainImage, descriptors);//这里可以设置检测窗口步长，如果图片大小超过64×128，可以设置winStride
		double time2 = getTickCount();
		double elapse_ms = (time2 - time1) * 1000 / getTickFrequency();
		//cout << "HOG dimensions:" << descriptors.size() << endl;
		//cout << "Compute time:" << elapse_ms << endl;


		//保存到特征向量矩阵中
		for (vector<float>::size_type j = 0; j <= descriptors.size() - 1; ++j)
		{
			featureVectorOfSample.at<float>(i, j) = descriptors[j];
		}

		//保存类别到类别矩阵
		//!!注意类别类型一定要是int 类型的
		classOfSample.at<int>(i, 0) = imageClass[i];
	}


	///////////////////////////////////使用SVM分类器训练///////////////////////////////////////////////////    
	//设置参数，注意Ptr的使用
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);//注意必须使用线性SVM进行训练，因为HogDescriptor检测函数只支持线性检测！！！
	svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER, 1000, FLT_EPSILON));

	//使用SVM学习         
	svm->train(featureVectorOfSample, ROW_SAMPLE, classOfSample);

	//保存分类器(里面包括了SVM的参数，支持向量,α和rho)
	svm->save(string(FILEPATH) + "Classifier.xml");

	
	//SVM训练完成后得到的XML文件里面，有一个数组，叫做support vector，还有一个数组，叫做alpha,有一个浮点数，叫做rho;
	//将alpha矩阵同support vector相乘，注意，alpha*supportVector,将得到一个行向量，将该向量前面乘以-1。之后，再该行向量的最后添加一个元素rho。
	//如此，变得到了一个分类器，利用该分类器，直接替换opencv中行人检测默认的那个分类器（cv::HOGDescriptor::setSVMDetector()），
	
	//获取支持向量机：矩阵默认是CV_32F
	Mat supportVector = svm->getSupportVectors();//

	//获取alpha和rho
	Mat alpha;//每个支持向量对应的参数α(拉格朗日乘子)，默认alpha是float64的
	Mat svIndex;//支持向量所在的索引
	float rho = svm->getDecisionFunction(0, alpha, svIndex);

	//转换类型:这里一定要注意，需要转换为32的
	Mat alpha2;
	alpha.convertTo(alpha2, CV_32FC1);

	//结果矩阵，两个矩阵相乘
	Mat result(1, 3780, CV_32FC1);
	result = alpha2*supportVector;

	//乘以-1，这里为什么会乘以-1？
	//注意因为svm.predict使用的是alpha*sv*another-rho，如果为负的话则认为是正样本，在HOG的检测函数中，使用rho+alpha*sv*another(another为-1)
	for (int i = 0; i < 3780; ++i)
		result.at<float>(0, i) *= -1;

	//将分类器保存到文件，便于HOG识别
	//这个才是真正的判别函数的参数(ω)，HOG可以直接使用该参数进行识别
	FILE *fp = fopen((string(FILEPATH) + "HOG_SVM.txt").c_str(), "wb");
	for (int i = 0; i<3780; i++)
	{
		fprintf(fp, "%f \n", result.at<float>(0, i));
	}
	fprintf(fp, "%f", rho);

	fclose(fp);

}
*/
// 使用训练好的分类器识别
void Detect()
{
	string FILEPATH = "";
	Mat img;
	FILE* f = 0;
	char _filename[1024];

	// 获取测试图片文件路径
	/*f = fopen((string(FILEPATH) + "TestData.txt").c_str(), "rt");
	if (!f)
	{
		fprintf(stderr, "ERROR: the specified file could not be loaded\n");
		return;
	}

	//加载训练好的判别函数的参数(注意，与svm->save保存的分类器不同)
	vector<float> detector;
	ifstream fileIn(string(FILEPATH) + "HOG_SVM.txt", ios::in);
	float val = 0.0f;
	while (!fileIn.eof())
	{
		fileIn >> val;
		detector.push_back(val);
	}
	fileIn.close();
	*/
	HOGDescriptor hog;
	//hog.setSVMDetector(detector);// 使用自己训练的分类器
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());//可以直接使用05 CVPR已训练好的分类器,这样就不用Train()这个步骤了
	namedWindow("people detector", 1);

	// 检测图片
	for (;;)
	{
		// 读取文件名
		char* filename = _filename;
		if (f)
		{
			if (!fgets(filename, (int)sizeof(_filename)-2, f))
				break;
			//while(*filename && isspace(*filename))
			//  ++filename;
			if (filename[0] == '#')
				continue;

			//去掉空格
			int l = (int)strlen(filename);
			while (l > 0 && isspace(filename[l - 1]))
				--l;
			filename[l] = '\0';
			img = imread(filename);
		}
		printf("%s:\n", filename);
		if (!img.data)
			continue;

		fflush(stdout);
		vector<Rect> found, found_filtered;
		double t = (double)getTickCount();
		// run the detector with default parameters. to get a higher hit-rate
		// (and more false alarms, respectively), decrease the hitThreshold and
		// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
		//多尺度检测
		hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
		t = (double)getTickCount() - t;
		printf("detection time = %gms\n", t*1000. / cv::getTickFrequency());
		size_t i, j;

		//去掉空间中具有内外包含关系的区域，保留大的
		for (i = 0; i < found.size(); i++)
		{
			Rect r = found[i];
			for (j = 0; j < found.size(); j++)
			if (j != i && (r & found[j]) == r)
				break;
			if (j == found.size())
				found_filtered.push_back(r);
		}

		// 适当缩小矩形
		for (i = 0; i < found_filtered.size(); i++)
		{
			Rect r = found_filtered[i];
			r.x += cvRound(r.width*0.1);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.07);
			r.height = cvRound(r.height*0.8);
			rectangle(img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);
		}
		imshow("people detector", img);
		int c = waitKey(0) & 255;
		if (c == 'q' || c == 'Q' || !f)
			break;
	}
	if (f)
		fclose(f);
	return;
}
/*
int main()
{
	//HOG_SVM2();
	Mat img;
//<<<<<<< .mine
//	//img = imread("C:\\Users\\admin\\Desktop\\QQ截图20160414130948.png");
//	img = imread("E:\\RE-ID\\数据集\\USCPedestrianSetA\\9Cup10.bmp");
//||||||| .r24
//	img = imread("C:\\Users\\admin\\Desktop\\QQ截图20160414130948.png");
//=======
//	//img = imread("E:\\RE-ID\\pedestrians128x64\\pedestrians128x64\\per00099.ppm");
//	//img = imread("C:\\Users\\admin\\Desktop\\QQ截图20160414130948.png");
//>>>>>>> .r25
	HOGDescriptor hog; 
	//hog.setSVMDetector(detector);// 使用自己训练的分类器
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());//可以直接使用05 CVPR已训练好的分类器,这样就不用Train()这个步骤了
	namedWindow("people detector", 1);
	fflush(stdout);
	vector<Rect> found, found_filtered;
	double t = (double)getTickCount();
	// run the detector with default parameters. to get a higher hit-rate
	// (and more false alarms, respectively), decrease the hitThreshold and
	// groupThreshold (set groupThreshold to 0 to turn off the grouping completely).
	//多尺度检测
	/*
	hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2);
	t = (double)getTickCount() - t;
	printf("detection time = %gms\n", t*1000. / cv::getTickFrequency());
	size_t i, j;

	//去掉空间中具有内外包含关系的区域，保留大的
	for (i = 0; i < found.size(); i++)
	{
		Rect r = found[i];
		for (j = 0; j < found.size(); j++)
		if (j != i && (r & found[j]) == r)
			break;
		if (j == found.size())
			found_filtered.push_back(r);
	}
	// 适当缩小矩形
	for (i = 0; i < found_filtered.size(); i++)
	{
		Rect r = found_filtered[i];
		r.x += cvRound(r.width*0.1);
		r.width = cvRound(r.width*0.8);
		r.y += cvRound(r.height*0.07);
		r.height = cvRound(r.height*0.8);
		rectangle(img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);
	}
	imshow("people detector", img);
	waitKey(30000);*/
/*
	//视频
	VideoCapture CaptureFile("E:\\RE-ID\\dataset\\AVSS_AB_Easy_Divx.mp4");
	//VideoWriter Writer("E:\\RE-ID\\dataset\\result_AVSS_AB_Easy_Divx.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(720, 576));
	int cnt = 0;
	while (CaptureFile.isOpened())
	{
		cnt++;
		CaptureFile >> img;
		if (img.empty())
		{
			CaptureFile.release();
			return 0;
		}
		fflush(stdout);
		vector<Rect> found, found_filtered;
		double t = (double)getTickCount();
		hog.detectMultiScale(img, found, 0.47, Size(8, 8), Size(32, 32), 1.1, 2);
		t = (double)getTickCount() - t;
		printf("detection time = %gms\n", t*1000. / cv::getTickFrequency());
		size_t i, j;

		//去掉空间中具有内外包含关系的区域，保留大的
		for (i = 0; i < found.size(); i++)
		{
			Rect r = found[i];
			for (j = 0; j < found.size(); j++)
			if (j != i && (r & found[j]) == r)
				break;
			if (j == found.size())
				found_filtered.push_back(r);
		}

		// 适当缩小矩形
		for (i = 0; i < found_filtered.size(); i++)
		{
			Rect r = found_filtered[i];
			r.x += cvRound(r.width*0.1);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.07);
			r.height = cvRound(r.height*0.8);
			rectangle(img, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);
		}
		imshow("people detector", img);
		//Writer << img;
		waitKey(10);
	}
	
	return 0;
}
*/