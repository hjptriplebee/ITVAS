//2016-3-16 hjp 分离线程函数，便于工程管理

#pragma once

#include "stdafx.h"
#include "afxdialogex.h"


//摄像头显示线程函数
UINT threadFunCamera(LPVOID pParam);

//文件显示线程函数
UINT threadFunFile(LPVOID pParam);