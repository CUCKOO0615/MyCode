// TestDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../FtpUtils/FtpUtils_ExportC.h"

#pragma comment(lib, "../Debug/FtpUtils.lib")

#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
    FtpConnector* p = CkFtpUtils::CreateFtpConnector(_T("FtpTest"));
    if (!CkFtpUtils::ConnectFtp(p, "114.215.99.219", 21, "FtpTestUser", "1Q2W3E4R5T",true))
    {
        CkFtpUtils::ReleaseFtpConnector(p);
        return 1;
    }
	if (!CkFtpUtils::FtpRemoveFile(p, "����ԭ", "�½� BMP ͼ��.bmp"))
		std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;

	CkFtpUtils::ReleaseFtpConnector(p);
	return 0;
}

