
// CUCKOO0615_Utils_MFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCUCKOO0615_Utils_MFCApp:
// �йش����ʵ�֣������ CUCKOO0615_Utils_MFC.cpp
//

class CCUCKOO0615_Utils_MFCApp : public CWinApp
{
public:
	CCUCKOO0615_Utils_MFCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCUCKOO0615_Utils_MFCApp theApp;