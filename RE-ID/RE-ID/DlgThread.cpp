//2016-3-16 hjp 分离线程函数，便于工程管理
#include "stdafx.h"
#include "DlgThread.h"
#include "RE-ID.h"
#include "RE-IDDlg.h"
 
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <ml.h>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;

/***************************************************************************
函数功能：摄像头显示线程函数
参数说明：

返回：
作者：jipeng huang
时间：2016-3-14
修改：
***************************************************************************/
UINT threadFunCamera(LPVOID pParam)
{
	Mat Temp;
	VideoCapture CaptureCamera(0);
	//VideoWriter Writer("E://VideoTest.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(640, 480));
	if (!CaptureCamera.isOpened())//异常处理
	{
		AfxMessageBox(_T("摄像头开启失败"));
		return -1;
	}
	CREIDDlg::camerathreadOn();
	while (CaptureCamera.isOpened() && CREIDDlg::getCamerathread())
	//while (CaptureCamera.isOpened())
	{
		CaptureCamera >> Temp;//摄像头-内存-硬盘
		imshow("view", Temp);
	//	Writer << Temp;
		Sleep(20);
	}
	return 0;
}

/***************************************************************************
函数功能：文件显示线程函数
参数说明：
返回：
作者：jipeng huang
时间：2016-3-15
修改：
***************************************************************************/
UINT threadFunFile(LPVOID pParam)
{
	String Path;
	CFileDialog Openfiledlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Video Files (*.rmvb;*.avi)|*.rmvb;*.avi||"));
	if (Openfiledlg.DoModal() == IDOK) //弹出模态对话框  
	{
		//CString类型转换为string类型  
		CString  filepath;
		filepath = Openfiledlg.GetPathName();
		CStringA temp(filepath.GetBuffer(0));
		filepath.ReleaseBuffer();
		Path = temp.GetBuffer(0);
		temp.ReleaseBuffer();
	}
	else
	{
		return 0;
	}
		
	Mat Temp;
	/*VideoCapture CaptureFile(Path);
	if (!CaptureFile.isOpened())//异常处理
	{
		AfxMessageBox(_T("文件打开失败"));
		return -1;
	}
	CREIDDlg::filethreadOn();
	while (CaptureFile.isOpened() && CREIDDlg::getFilethread())
	{
		CaptureFile >> Temp;//摄像头-内存-硬盘
		if (Temp.empty())
		{
			CaptureFile.release();
			return 0;
		}
		imshow("view", Temp);
		Sleep(50);
	}*/
	return 0;
}