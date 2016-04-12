// TestDemo.cpp : 定义控制台应用程序的入口点。
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

// 	if (!CkFtpUtils::FtpDownloadFile(p, "/123\\123.txt", "Z:\\新建文件夹123\\111.txt"))
// 		std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;

// 	if (!CkFtpUtils::FtpRemoveFile(p, "喵帕斯\\新建文件夹", "新建 BMP 图像.bmp"))
// 		std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;

	if (!CkFtpUtils::FtpDownloadFile(p, "喵帕斯\\新建文件夹\\1.txt", "Z:\\新建文件夹123\\111.txt"))
	 	std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;

//	CkFtpUtils::ReleaseFtpConnector(p);
	return 0;
}

