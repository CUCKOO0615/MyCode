// TestDemo.cpp : 定义控制台应用程序的入口点。
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
	if (!CkFtpUtils::FtpRemoveFile(p, "梁中原", "新建 BMP 图像.bmp"))
		std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;

	CkFtpUtils::ReleaseFtpConnector(p);
	return 0;
}

