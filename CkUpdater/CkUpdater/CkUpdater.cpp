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

char charBuff[1024] = { 0 };
void ConsoleWriteLine(const char* szFormat, ...)
{
    va_list argList;
    va_start(argList, szFormat);
    ::vsprintf(charBuff, szFormat, argList);
    va_end(argList);
    ::strcat(charBuff, "\n");
    ::printf(charBuff);
}

bool ReplayYes(SOCKET s)
{
	int nErrCode = 0;
	int nSend = YES;
	if (SocketUtils::SendToSocket(s, (char*)&nSend, sizeof(int), nErrCode))
		return true;    
    ::printf("%s\n", SocketUtils::QueryErrMsg(nErrCode));
	return false;
}

bool CheckReady(SOCKET s)
{
	int nErrCode = 0;
    int nReply = READY;
	if (!SocketUtils::RecvFromSocket(s, (char*)&nReply, sizeof(int), nErrCode))
	{
        ::printf("%s\n", SocketUtils::QueryErrMsg(nErrCode));
		return false;
	}
	if (READY != nReply)
	{
        ::printf("%s\n", "Command error");
		return false;
	}
	return true;
}

static const int BUFF_LENGTH = 1024;

typedef struct stuFileInfo
{
	size_t nFileLength;
	char szMD5[32];
	char szFileName[256];
}FILE_INFO;

bool GetFiles(SOCKET s)
{
	int nErrCode = 0;
	FILE_INFO fileInfo;
	::memset(&fileInfo, 0x00, sizeof(FILE_INFO));
	if (!SocketUtils::RecvFromSocket(s, (char*)&fileInfo, sizeof(FILE_INFO), nErrCode))
    {
        ::printf("Receive file infos failed. Err msg: %s. \n",
            SocketUtils::QueryErrMsg(nErrCode));
        return false;
    }
    ::printf("%s\n", "File infos received.");
	    
    std::string strNewFileName = 
		std::string(fileInfo.szFileName) + "_temp";
    std::ofstream ofs(strNewFileName.c_str(), std::ios::binary);
    if (ofs.fail())
    {
        ::printf ("Create local file failed.\n");
		return false;
    }

    ::printf("    10   20   30   40   50   60   70   80   90   100\n");
    ::printf("====^====^====^====^====^====^====^====^====^====^ % \n");
    size_t nBlockSize = fileInfo.nFileLength / 50;  

	char arrBuff[BUFF_LENGTH] = { 0 };
	int nCounter = 0;
	int nFileLength = fileInfo.nFileLength;
    char arrRotate[] = { '\\', '|', '/', '-' };
    int nRotate = 0;
	while (nFileLength)
	{
		int nRecv = min(nFileLength, BUFF_LENGTH);
        if (!SocketUtils::RecvFromSocket(s, arrBuff, nRecv, nErrCode))
        {
			::printf("\nErr occured during pulling file content. Err msg: %s\n",
                SocketUtils::QueryErrMsg(nErrCode));
            ofs.close();
            return false;
        }
		ofs.write(arrBuff, nRecv);
        
		nFileLength -= nRecv;
		nCounter += nRecv;
        if (4 == nRotate) 
            nRotate = 0;
        if (nCounter > nBlockSize)
		{
            nCounter -= nBlockSize;
            ::printf("\b>%c", arrRotate[nRotate++]);
		}
        else
            ::printf("\b%c", arrRotate[nRotate++]);
    }
    ofs.close();
    ::printf("\nWrite file completed. Checking md5..\n");
	
    std::ifstream ifs(strNewFileName.c_str(), std::ios::binary);
    if (ifs.fail())
    {
        ::printf("Open file for check md5 failed.\n");
        return false;
    }
    MD5 md5;
    md5.update(ifs);
    ifs.close();
	if (0 != ::memcmp(md5.toString().c_str(), fileInfo.szMD5, 32))
    {
        ::printf("Check md5 failed.\n");
        return false;
    }
    ::printf("Check md5 successful.\n");

	if (!::_access(fileInfo.szFileName, 0) && ::remove(fileInfo.szFileName))
    {
        ::printf("Remove old file failed.\n");
        return false;
    }
	if (::rename(strNewFileName.c_str(), fileInfo.szFileName))
    {
        ::printf("Rename temp file failed.\n");
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

    //std::wstring wstr = argv[1];
	std::string strAddr(argv[1]), strIP, strPort, strErrMsg;
    //StringUtils::StrConv_W2A(wstr.c_str(), strAddr, strErrMsg);

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
            std::cout << "====[ START UPDATE ]====" << std::endl;
            if (!CheckReady(g_sClient))
                break;
            if (!ReplayYes(g_sClient))
                break;
            std::cout << "Receiving file..." << std::endl;
            if (!GetFiles(g_sClient))
                break;
            if (!ReplayYes(g_sClient))
                break;
            std::cout << "====[ UPDATE COMPLETED ]====" << std::endl;
        }
    }
	return 0;
}

