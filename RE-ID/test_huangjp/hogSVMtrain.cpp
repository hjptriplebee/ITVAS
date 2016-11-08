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
using namespace ml;


/***************************************************************************
函数功能：训练图像
参数说明：
返回：
作者：jipeng huang
时间：2016-9-29
修改：
***************************************************************************/
void Train()
{
	vector<string> imagePath;
	vector<int> imageClass;
	int numberOfLine = 0;
	string buffer;
	ifstream trainingData(string("E:\\RE-ID\\训练数据\\车辆\\") + "trainFileNameLIst.txt");
	unsigned long n;

	while (!trainingData.eof())
	{
		getline(trainingData, buffer);
		if (!buffer.empty())
		{
			++numberOfLine;
			if (numberOfLine % 2 == 0)
				imageClass.push_back(atoi(buffer.c_str()));//读取样本类别
			else
				imagePath.push_back(buffer);//读取图像路径
		}
	} 
	trainingData.close();
	//样本特征向量矩阵
	int numberOfSample = numberOfLine / 2;
	Mat featureVectorOfSample(numberOfSample, 1764, CV_32FC1);//矩阵中每行为一个样本
	Mat classOfSample(numberOfSample, 1, CV_32SC1);//样本的类别
	Mat convertedImage;
	Mat trainImage;
	// 计算HOG特征
	HOGDescriptor hog(cvSize(64, 64), cvSize(16, 16), cvSize(8, 8), cvSize(8, 8), 9);
	for (vector<string>::size_type i = 0; i <= imagePath.size() - 1; i++)
	{
		Mat src = imread(imagePath[i], -1);
		if (src.empty())
		{
			cout << "can not load the image:" << imagePath[i] << endl;
			continue;
		}
		// 归一化
		resize(src, trainImage, Size(64, 64));
		// 提取HOG特征
		vector<float> descriptors;
		double time1 = getTickCount();
		hog.compute(trainImage, descriptors);//这里可以设置检测窗口步长，如果图片大小超过64×128，可以设置winStride
		double time2 = getTickCount();
		double elapse_ms = (time2 - time1) * 1000 / getTickFrequency();
		//保存到特征向量矩阵中
		for (vector<float>::size_type j = 0; j < descriptors.size(); j++)
		{
			//cout << j << " " << descriptors.size() << endl;
			//cout << descriptors[j] << endl;
			featureVectorOfSample.at<float>(i, j) = descriptors[j];
			
		}
		//保存类别到类别矩阵
		//!!注意类别类型一定要是int 类型的
		classOfSample.at<int>(i, 0) = imageClass[i];
	}    
	Ptr<SVM> svm = SVM::create();
	svm->setType(SVM::C_SVC);
	svm->setKernel(SVM::LINEAR);//注意必须使用线性SVM进行训练，因为HogDescriptor检测函数只支持线性检测！！！
	svm->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER, 1000, FLT_EPSILON));        
	svm->train(featureVectorOfSample, ROW_SAMPLE, classOfSample);
	//保存分类器(里面包括了SVM的参数，支持向量,α和rho)
	svm->save(string("E:\\RE-ID\\训练数据\\车辆\\") + "Classifier3.xml");
	/*
	SVM训练完成后得到的XML文件里面，有一个数组，叫做support vector，还有一个数组，叫做alpha,有一个浮点数，叫做rho;
	将alpha矩阵同support vector相乘，注意，alpha*supportVector,将得到一个行向量，将该向量前面乘以-1。之后，再该行向量的最后添加一个元素rho。
	如此，变得到了一个分类器，利用该分类器，直接替换opencv中行人检测默认的那个分类器（cv::HOGDescriptor::setSVMDetector()），
	*/
	//获取支持向量机：矩阵默认是CV_32F
	Mat supportVector = svm->getSupportVectors();
	//获取alpha和rho
	Mat alpha;//每个支持向量对应的参数α(拉格朗日乘子)，默认alpha是float64的
	Mat svIndex;//支持向量所在的索引
	float rho = svm->getDecisionFunction(0, alpha, svIndex);
	//转换类型:这里一定要注意，需要转换为32的
	Mat alpha2;
	alpha.convertTo(alpha2, CV_32FC1);
	//结果矩阵，两个矩阵相乘
	const int col = 1764;
	Mat result(1, col, CV_32FC1);
	result = alpha2*supportVector;
	//注意因为svm.predict使用的是alpha*sv*another-rho，如果为负的话则认为是正样本，在HOG的检测函数中，使用rho+alpha*sv*another(another为-1)
	for (int i = 0; i < col; i++)
		result.at<float>(0, i) *= -1;
	//将分类器保存到文件，便于HOG识别
	//这个才是真正的判别函数的参数(ω)，HOG可以直接使用该参数进行识别
	FILE *fp = fopen((string("E:\\RE-ID\\训练数据\\车辆\\") + "HOG_SVM3.txt").c_str(), "wb");
	for (int i = 0; i < col; i++)
	{
		fprintf(fp, "%f \n", result.at<float>(0, i));
	}
	fprintf(fp, "%f", rho);
	fclose(fp);
}

void DetectionAndTracking(string path)
{
	Mat dst;
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
	Mat src;
	for (int frameCnt = 1;; frameCnt++)
	{
		cap >> src;
		if (src.empty())break; //处理最后一帧
		resize(src, dst, Size(src.cols, src.rows));//1.5 3.75
		fflush(stdout);
		vector<Rect> found, found_filtered;
		double t = (double)getTickCount();
		//多尺度检测
		hog.detectMultiScale(dst, found, 1.2, Size(16, 16), Size(0, 0), 2, 5);//1.1 1.3 2
		t = (double)getTickCount() - t;
		printf("detection time = %gms\n", t*1000. / cv::getTickFrequency());
		size_t i, j;

		//去掉空间中具有内外包含关系的区域，保留大的
		vector<Rect>::iterator it = found.begin();
		for (; it != found.end();)
		{
			if ((*it).height > 250)
				found.erase(it);
			else
				it++;
		}
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
			//r.x *= 3.75;
			r.width = cvRound(r.width*0.8);
			//r.width *= 1.5;
			r.y += cvRound(r.height*0.07);
			r.height = cvRound(r.height*0.8);
			//cout << r.height << " " << r.width << endl;
			rectangle(dst, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);
			//rectangle(dst, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);
		}
		imshow("car detector", dst);
		waitKey(1);
		//int c = waitKey(0) & 255;
		//if (c == 'q' || c == 'Q' )
			//break;
		//writer << img;
		
	}
}
/*
int main()
{
	//Train();
	//cout << "train finish!" << endl;
	DetectionAndTracking("E:\\RE - ID\\数据集\\车辆\\video\\20160713\\DJI_0003.mov");
	return 0;
}
*/