// FtpUtils.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "FtpUtils.h"
#include "Common.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CFtpUtilsApp

BEGIN_MESSAGE_MAP(CFtpUtilsApp, CWinApp)
END_MESSAGE_MAP()


// CFtpUtilsApp ����

CFtpUtilsApp::CFtpUtilsApp()
{
    // TODO:  �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFtpUtilsApp ����

CFtpUtilsApp theApp;


// CFtpUtilsApp ��ʼ��

BOOL CFtpUtilsApp::InitInstance()
{
    CWinApp::InitInstance();
#ifdef DEBUG
    CkLogUtils::InitLogUtils(g_pLogUtils, ".\\Log\\", "FtpUtilsDebug");
    //CkLogUtils::ReleaseLogUtils(g_pLogUtils);
#endif
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////


