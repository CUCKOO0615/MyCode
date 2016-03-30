// TestDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../FtpUtils/FtpUtils_ExportC.h"

#pragma comment(lib, "../Debug/FtpUtils.lib")


int _tmain(int argc, _TCHAR* argv[])
{
    FtpConnector* p = CkFtpUtils::CreateFtpConnector(_T("FtpTest"));
    if (!CkFtpUtils::ConnectFtp(p, "192.168.0.118", 21, "Administrator", "SHIELD084"))
    {
        CkFtpUtils::ReleaseFtpConnector(p);
        return 1;
    }

    int n1 = CkFtpUtils::FtpCheckDirExist(p, "/1", false);
    int n2 = CkFtpUtils::FtpCheckDirExist(p, "/����ԭ", false);
    int n3 = CkFtpUtils::FtpRemoveFile(p, "/", "1.txt", false);

    CkFtpUtils::ReleaseFtpConnector(p);
    //CkFtpUtils::FtpTest("114.215.99.219", 21, "FtpTestUser", "APTX4869", "/����ԭ", true);
	return 0;
}

