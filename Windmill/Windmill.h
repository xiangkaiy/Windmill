
// Windmill.h : Windmill Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CWindmillApp:
// �йش����ʵ�֣������ Windmill.cpp
//

class CWindmillApp : public CWinApp
{
public:
	CWindmillApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnExit();
};

extern CWindmillApp theApp;
