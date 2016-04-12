// TestDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../FtpUtils/FtpUtils_ExportC.h"

#ifdef _DEBUG
#pragma comment(lib, "../Bin/FtpUtils.lib")
#else
#pragma comment(lib, "../Bin/FtpUtils.lib")
#endif

#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
    FtpConnector* p = CkFtpUtils::CreateFtpConnector(_T("FtpTest11"));
 //   if (!CkFtpUtils::ConnectFtp(p, "114.215.99.219", 21, "FtpTestUser", "1Q2W3E4R5T",true))
	if (!CkFtpUtils::ConnectFtp(p, "114.215.99.219", 21, "FtpTestUserRW", "1Q2W3E4R5T", true))
	{
        CkFtpUtils::ReleaseFtpConnector(p);
        return 1;
    }

// 	if (!CkFtpUtils::FtpDownloadFile(p, "/123\\123.txt", "Z:\\�½��ļ���123\\111.txt"))
// 		std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;

// 	if (!CkFtpUtils::FtpRemoveFile(p, "����˹\\�½��ļ���", "�½� BMP ͼ��.bmp"))
// 		std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;

	if (!CkFtpUtils::FtpDownloadFile(p, "����˹\\�½��ļ���\\1.txt", "Z:\\�½��ļ���123\\111.txt"))
	 	std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;

//	CkFtpUtils::ReleaseFtpConnector(p);
	return 0;
}

