// CkUpdater.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AutoManager.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <io.h>
#include "md5.h"
#include "StringUtils.h"

#define YES   1111
#define READY 2222

bool ReplayYes(SOCKET s)
{
	int nErrCode = 0;
    int nSend = YES;
    if (!SocketUtils::SendToSocket(s, (char*)&nSend, sizeof(int), nErrCode))
    {
        std::cout << SocketUtils::QueryErrMsg(nErrCode) << std::endl;
        return false;
	}
	return true;
}

bool CheckReady(SOCKET s)
{
	int nErrCode = 0;
    int nSend = READY;
    if (!SocketUtils::RecvFromSocket(s, (char*)&nSend, sizeof(int), nErrCode))
	{
		std::cout << SocketUtils::QueryErrMsg(nErrCode) << std::endl;
		return false;
	}
    if (READY != nSend)
	{
		std::cout << "Command error" << std::endl;
		return false;
	}
	return true;
}

static const int FILEINFO_LENGTH = 4/*FileLength*/ + 32/*MD5*/ + 256/*FilePath*/;
static const int BUFF_LENGTH = 1024 * 1024;

bool GetFiles(SOCKET s)
{
	int nErrCode = 0;
    char arrFileInfos[FILEINFO_LENGTH] = { 0 };
    if (!SocketUtils::RecvFromSocket(s, arrFileInfos, FILEINFO_LENGTH, nErrCode))
    {
        std::cout
            << "Receive file infos failed. Err msg: "
            << SocketUtils::QueryErrMsg(nErrCode)
            << std::endl;
        return false;
    }
	std::cout << "File infos received" << std::endl;

	size_t nFileLength;
	char szMD5[32] = { 0 };
	char szFileName[256] = { 0 };
	::memcpy(&nFileLength, arrFileInfos, 4);
	::memcpy(szMD5, arrFileInfos + 4, 32);
	::memcpy(szFileName, arrFileInfos + 4 + 32, 256);
    
    std::string strNewFileName = 
        std::string(szFileName) + "_temp";
    std::ofstream ofs(strNewFileName.c_str(), std::ios::binary);
    if (ofs.fail())
    {
        std::cout << "Create local file failed." << std::endl;
		return false;
    }

	std::cout << "Writing file (1MB/*):" << szFileName << std::endl;
	char arrBuff[BUFF_LENGTH] = { 0 };
	while (nFileLength)
	{
		int nRecv = min(nFileLength, BUFF_LENGTH);
        std::cout <<"nRecv："<<nRecv << endl;
        if (!SocketUtils::RecvFromSocket(s, arrBuff, nRecv, nErrCode))
        {
            std::cout
                << "Err occured during pulling file content. Err msg: "
                << SocketUtils::QueryErrMsg(nErrCode)
                << std::endl;
            ofs.close();
            return false;
        }
		ofs.write(arrBuff, nRecv);
		nFileLength -= nRecv;
		std::cout <<"*";
    }
    ofs.close();
    std::cout 
        << std::endl
        << "Write file completed."
        << std::endl;

    std::cout << "Checking md5.." << std::endl;
    std::ifstream ifs(strNewFileName.c_str(), std::ios::binary);
    if (ifs.fail())
    {
        std::cout << "Open file for check md5 failed." << std::endl;
        return false;
    }
    MD5 md5;
    md5.update(ifs);
    ifs.close();
    std::string strMD5 = md5.toString();
    if (0 != ::memcmp(strMD5.c_str(), szMD5, 32))
    {
        std::cout << "Check md5 failed." << std::endl;
        return false;
    }
    std::cout << "Check md5 successful" << std::endl;

    if (!::_access(szFileName, 0) && ::remove(szFileName))
    {
        std::cout << "Remove old file failed" << endl;
        return false;
    }
    if (::rename(strNewFileName.c_str(),szFileName))
    {
        std::cout << "Rename temp file failed" << endl;
        return false;
    }
    return true;
}

bool CreateClientSocket(SOCKET & sClient, const char* szIP, USHORT usPort)
{
    // 清理Socket
    if (INVALID_SOCKET != sClient)
    {
        ::closesocket(sClient);
        sClient = INVALID_SOCKET;
    }

    // 建立（重建）Socket
    int nErrCode = 0;
    sClient = SocketUtils::CreateClientSocket_TCP(nErrCode, szIP, usPort);
    if (INVALID_SOCKET == sClient)
    {
        std::cout << SocketUtils::QueryErrMsg(nErrCode) << std::endl;
        system("pause");
        return false;
    }
    return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (!g_bGlobalInited || argc < 2)
        return 1;

    struct timeval timeout = { 30, 0 };

    std::wstring wstr = argv[1];
    std::string strAddr, strIP, strPort, strErrMsg;
    StringUtils::StrConv_W2A(wstr.c_str(), strAddr, strErrMsg);

	size_t nPos = strAddr.find(':');
	if (std::string::npos == nPos)
	{
		strIP = strAddr;
		strPort = "61500";
	}
	else
	{
		strIP = strAddr.substr(0, nPos);
        strPort = strAddr.substr(nPos + 1);
    }

    USHORT usPort = atoi(strPort.c_str());
    const char* szIP = strIP.c_str();
    int nErrCode = 0;

    while (true)
	{
        if (!CreateClientSocket(g_sClient, szIP, usPort))
            continue;
        std::cout << "*** Connected to server ***" << std::endl;
             
        while (true)
        {
            std::cout << "==== Start update ====" << std::endl;
            if (!CheckReady(g_sClient))
                break;
            if (!ReplayYes(g_sClient))
                break;
            std::cout << "Receiving file..." << std::endl;
            if (!GetFiles(g_sClient))
                break;
            if (!ReplayYes(g_sClient))
                break;
            std::cout << "==== Update completed ====" << std::endl;
        }
    }
	return 0;
}

