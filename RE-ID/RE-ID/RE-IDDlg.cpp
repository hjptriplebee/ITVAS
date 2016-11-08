// RE-IDDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "RE-ID.h"
#include "RE-IDDlg.h"
#include "afxdialogex.h"

#include "DlgThread.h"
#include "EasySize.h"
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>
#include <ml.h>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;

bool CREIDDlg::m_cameraThreadFlag = false;
bool CREIDDlg::m_fileThreadFlag = false;
/***************************************************************************
函数功能：调整窗口大小,使用easysize
参数说明：
返回：
作者：jipeng huang
时间：2016-3-30
修改：
***************************************************************************/
BEGIN_EASYSIZE_MAP(CREIDDlg)
	//EASYSIZE(Control_ID,left,top,right,bottom,options)
	//EASYSIZE(IDC_COMBO_COMMON, ES_BORDER, ES_BORDER, ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_STATIC_ShowVideo , ES_BORDER, ES_BORDER, ES_BORDER           , ES_BORDER, 0)
	EASYSIZE(IDC_BUTTON_CameraOn, IDC_STATIC_ShowVideo, ES_BORDER, ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_BUTTON_CameraOff, IDC_STATIC_ShowVideo, IDC_BUTTON_CameraOn, ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_BUTTON_FileOn, IDC_STATIC_ShowVideo, IDC_BUTTON_CameraOff, ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_BUTTON_FileOff, IDC_STATIC_ShowVideo, IDC_BUTTON_FileOn, ES_BORDER, ES_KEEPSIZE, 0)
	EASYSIZE(IDC_BUTTON_Exit, IDC_STATIC_ShowVideo, IDC_BUTTON_FileOff, ES_BORDER, ES_KEEPSIZE, 0)
END_EASYSIZE_MAP

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CREIDDlg 对话框
CREIDDlg::CREIDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CREIDDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CREIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CREIDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CameraOn, &CREIDDlg::OnBnClickedButtonCameraon)
	ON_BN_CLICKED(IDC_BUTTON_CameraOff, &CREIDDlg::OnBnClickedButtonCameraoff)
	ON_BN_CLICKED(IDC_BUTTON_FileOn, &CREIDDlg::OnBnClickedButtonFileon)
	ON_BN_CLICKED(IDC_BUTTON_FileOff, &CREIDDlg::OnBnClickedButtonFileoff)
	ON_BN_CLICKED(IDC_BUTTON_Exit, &CREIDDlg::OnBnClickedButtonExit)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CREIDDlg 消息处理程序
BOOL CREIDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	//2016-3-30 hjp 调整窗口大小
	INIT_EASYSIZE;
	//2016-3-14 hjp 将opencv显示框嵌入Pic控件
	Mat BlackScreen(1000, 1000, CV_8UC3, Scalar(0, 0, 0));
	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_ShowVideo)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	imshow("view",BlackScreen);//置背景为黑色

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CREIDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CREIDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CREIDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/***************************************************************************
函数功能：获取摄像头显示线程标识
参数说明：
返回：摄像头显示线程标识
作者：jipeng huang
时间：2016-3-29
修改：
***************************************************************************/
bool CREIDDlg::getCamerathread()
{
	return m_cameraThreadFlag;
}
void CREIDDlg::camerathreadOn()
{
	m_cameraThreadFlag = true;
	return;
}
void CREIDDlg::camerathreadOff()
{
	m_cameraThreadFlag = false;
	return;
}
/***************************************************************************
函数功能：获取文件显示线程标识
参数说明：
返回：文件显示线程标识
作者：jipeng huang
时间：2016-3-29
修改：
***************************************************************************/
bool CREIDDlg::getFilethread()
{
	return m_fileThreadFlag;
}
void CREIDDlg::filethreadOn()
{
	m_fileThreadFlag = true;
}
void CREIDDlg::filethreadOff()
{
	m_fileThreadFlag = false;
}

void CREIDDlg::OnBnClickedButtonCameraon()
{
	// TODO:  在此添加控件通知处理程序代码
	//2016-3-14 hjp 摄像头开按钮控制逻辑
	m_pThreadCamera = ::AfxBeginThread(threadFunCamera, 0, THREAD_PRIORITY_NORMAL, 0, 0);
	GetDlgItem(IDC_BUTTON_CameraOn)->EnableWindow(false);//按钮状态设置
	GetDlgItem(IDC_BUTTON_CameraOff)->EnableWindow(true);
	return;
}

void CREIDDlg::OnBnClickedButtonCameraoff()
{
	// TODO:  在此添加控件通知处理程序代码
	//2016-3-14 hjp 摄像头关按钮控制逻辑
	m_cameraThreadFlag = false;
	GetDlgItem(IDC_BUTTON_CameraOn)->EnableWindow(true);//按钮状态设置
	GetDlgItem(IDC_BUTTON_CameraOff)->EnableWindow(false);
	Mat BlackScreen(1000, 1000, CV_8UC3, Scalar(0, 0, 0));//还原显示区域
	imshow("view", BlackScreen);
	return;
}

void CREIDDlg::OnBnClickedButtonFileon()
{
	// TODO:  在此添加控件通知处理程序代码
	//2016-3-15 hjp 文件开按钮控制逻辑
	m_pThreadFile = ::AfxBeginThread(threadFunFile, 0, THREAD_PRIORITY_NORMAL, 0, 0);
	GetDlgItem(IDC_BUTTON_FileOn)->EnableWindow(false);//按钮状态设置
	GetDlgItem(IDC_BUTTON_FileOff)->EnableWindow(true);
	return;
}


void CREIDDlg::OnBnClickedButtonFileoff()
{
	// TODO:  在此添加控件通知处理程序代码
	//2016-3-15 hjp 文件关按钮控制逻辑
	m_fileThreadFlag = false;
	GetDlgItem(IDC_BUTTON_FileOn)->EnableWindow(true);//按钮状态设置
	GetDlgItem(IDC_BUTTON_FileOff)->EnableWindow(false);
	//Mat BlackScreen(1000, 1000, CV_8UC3, Scalar(0, 0, 0));//还原显示区域
	//imshow("view", BlackScreen);
	return;
}


void CREIDDlg::OnBnClickedButtonExit()
{
	// TODO:  在此添加控件通知处理程序代码
	exit(0);
	return;
}

/***************************************************************************
函数功能：调整窗口大小
参数说明：
返回：
作者：jipeng huang
时间：2016-3-30
修改：
***************************************************************************/
void CREIDDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	UPDATE_EASYSIZE;
}
void CREIDDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO:  在此处添加消息处理程序代码
	//设定最小值
	EASYSIZE_MINSIZE(600, 400, fwSide, pRect);
}

/***************************************************************************
函数功能：关闭程序所有进程，点击关闭窗口按钮或进入该函数
参数说明：
返回：
作者：lei qi
时间：2016-4-7
修改：
***************************************************************************/
void CREIDDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	int a;
	if (m_cameraThreadFlag == true)
	{
		//OnBnClickedButtonFileoff();
		camerathreadOff();
		//a = TerminateThread(m_pThreadCamera->m_hThread, 0);//关闭摄像头进程
	}
	if (m_fileThreadFlag == true)
	{
		TerminateThread(m_pThreadFile->m_hThread, 0);//关闭摄像头进程
	}
	CDialogEx::OnClose();
	//exit(0);//
	return;
}
