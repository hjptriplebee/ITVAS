#pragma once
// RE-IDDlg.h : 头文件
#include "EasySize.h"

// CREIDDlg 对话框
class CREIDDlg : public CDialogEx
{
private:
	static bool m_cameraThreadFlag;//摄像头显示线程标识
	static bool m_fileThreadFlag;//文件显示线程标识
protected:
	CWinThread* m_pThreadCamera; //摄像头线程指针
	CWinThread* m_pThreadFile;//文件线程指针
	DECLARE_EASYSIZE//调节窗口大小
// 构造
public:
	CREIDDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REID_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButtonCameraon();
	afx_msg void OnBnClickedButtonCameraoff();
	afx_msg void OnBnClickedButtonFileon();
	afx_msg void OnBnClickedButtonFileoff();
	afx_msg void OnBnClickedButtonExit();

	static bool getCamerathread();//获取摄像头显示线程标识
	static void camerathreadOn();//摄像头显示线程标识true
	static void camerathreadOff();//摄像头显示线程标识false
	static bool getFilethread();//获取文件显示线程标识
	static void filethreadOn();//文件显示线程标识true
	static void filethreadOff();//文件显示线程标识false
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnClose();
};
