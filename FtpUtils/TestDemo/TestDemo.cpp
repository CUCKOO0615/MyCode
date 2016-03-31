// TestDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../FtpUtils/FtpUtils_ExportC.h"

#pragma comment(lib, "../Debug/FtpUtils.lib")


int _tmain(int argc, _TCHAR* argv[])
{
    FtpConnector* p = CkFtpUtils::CreateFtpConnector(_T("FtpTest"));
    if (!CkFtpUtils::ConnectFtp(p, "114.215.99.219", 21, "FtpTestUser", "1Q2W3E4R5T"))
    {
        CkFtpUtils::ReleaseFtpConnector(p);
        return 1;
    }

    int n1 = CkFtpUtils::FtpRemoveFile(p, "/", "1.txt",false);

    CkFtpUtils::ReleaseFtpConnector(p);
    //CkFtpUtils::FtpTest("114.215.99.219", 21, "FtpTestUser", "APTX4869", "/����ԭ", true);
	return 0;
}

