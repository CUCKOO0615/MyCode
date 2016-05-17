// TestDemo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../FtpUtils/FtpUtils_ExportC.h"
#include <array>
#include <time.h>

#ifdef _DEBUG
#pragma comment(lib, "../Bin/FtpUtils.lib")
#else
#pragma comment(lib, "../Bin/FtpUtils.lib")
#endif

#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
    FtpConnector* p = CkFtpUtils::CreateFtpConnector(_T("FtpTest11"));
    if (!CkFtpUtils::ConnectFtp(p, "114.215.99.219", 21, "FtpTestUser", "1Q2W3E4R5T", true))
	//if (!CkFtpUtils::ConnectFtp(p, "114.215.99.219", 21, "FtpTestUserRW", "1Q2W3E4R5T", true))
    //if (!CkFtpUtils::ConnectFtp(p, "114.215.99.219", 21, "Administrator", "Cuckoo0615", true))
	{
        CkFtpUtils::ReleaseFtpConnector(p);
        return 1;
    }

    CkFtpUtils::GetFtpCurrentDir(p);

	FtpFileInfo* pffi = NULL;
	int nArrSize = 0;
	CkFtpUtils::FtpGetFileInfosInDir(p, "CUCKOO0615_MUSIC_FTP\\����˹\\�½��ļ���\\", NULL ,&pffi, &nArrSize);


    for (int i = 0; i != nArrSize; ++i)
    {
        time_t t = pffi[i].tLastWriteTime;
        char buff[30] = { 0 };
        ::strftime(buff, 30, "%Y/%m/%d %H:%M:%S", localtime(&t));
        std::cout << pffi[i].szFileName << ":" << buff << std::endl;
    }

    CkFtpUtils::ReleaseFileInfos(&pffi);

/*
    if (!CkFtpUtils::FtpUploadFile(p, "X:\\flashplayer.zip", "����˹\\�½��ļ���\\flashplayer.zip"))
        std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;

	if (!CkFtpUtils::FtpDownloadFile(p, "����˹\\�½��ļ���\\1.txt", "Z:\\�½��ļ���123\\111.txt"))
	 	std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;

	if (!CkFtpUtils::FtpRemoveFile(p, "����˹\\�½��ļ���", "�½� BMP ͼ��.bmp"))
		std::cout << CkFtpUtils::GetLastErrMsg(p) << std::endl;
*/
	CkFtpUtils::ReleaseFtpConnector(p);
	return 0;
}

