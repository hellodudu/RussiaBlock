// RussiaBlock.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRussiaBlockApp:
// �йش����ʵ�֣������ RussiaBlock.cpp
//

class CRussiaBlockApp : public CWinApp
{
public:
	CRussiaBlockApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
public:
	HWND m_hwndDlg;
public:
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
};

extern CRussiaBlockApp theApp;