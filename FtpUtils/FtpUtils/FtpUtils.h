// FtpUtils.h : FtpUtils DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <afxinet.h>



// CFtpUtilsApp
// �йش���ʵ�ֵ���Ϣ������� FtpUtils.cpp
//

class CFtpUtilsApp : public CWinApp
{
public:
	CFtpUtilsApp();
    



// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
