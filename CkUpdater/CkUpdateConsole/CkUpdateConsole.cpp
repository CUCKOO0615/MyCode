// CkUpdateConsole.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include "SocketUtils.h"
#include "FileUtils.h"
#include "md5.h"
#include "AutoManager.hpp"
#include "StringUtils.h"

#define YES   1111
#define READY 2222

static const int FILEINFO_LENGTH = 4/*FileLength*/ + 32/*MD5*/ + 256/*FilePath*/;
static const int BUFF_LENGTH = 1024 * 800;

bool CheckYes(SOCKET s)
{
	int nErrCode = 0;
	int nRecv = 0;
    if (!SocketUtils::RecvFromSocket(s, (char*)&nRecv, sizeof(int), nErrCode))
	{
		std::cout << SocketUtils::QueryErrMsg(nErrCode) << std::endl;
		return false;
	}
    if (YES != nRecv)
	{
		std::cout << "Check reply failed" << std::endl;
		return false;
	}
	return true;
}

bool QuestReady(SOCKET s)
{
    int nErrCode = 0;
    int nQuest = READY;
    if (!SocketUtils::SendToSocket(s, (char*)&nQuest, sizeof(int), nErrCode))
    {
        std::cout << SocketUtils::QueryErrMsg(nErrCode) << std::endl;
        return false;
    }
    return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (!g_bGlobalInited || argc < 2)
        return 1;

    std::wstring wstr = argv[1];
    std::string strAddr, strIP, strPort, strErrMsg;
    StringUtils::StrConv_W2A(wstr.c_str(), strAddr, strErrMsg);

    size_t nPos = strAddr.find(':');
    if (std::string::npos == nPos)
    {
        strIP = strAddr;
        strPort = "61501";
    }
    else
    {
        strIP = strAddr.substr(0, nPos);
        strPort = strAddr.substr(nPos + 1);
    }

    USHORT usPort = atoi(strPort.c_str());
    const char* szIP = strIP.c_str();
    
	int nErrCode = 0;
    SOCKET sClient = SocketUtils::CreateClientSocket_TCP(nErrCode, szIP, usPort);
    if (INVALID_SOCKET == sClient)
    {
        std::cout << SocketUtils::QueryErrMsg(nErrCode) << std::endl;
  		return 2;  
    }

    string strInput;
    FileUtils fu;
    while (std::cin >> strInput)
    {
        size_t nPos = strInput.find('_');
        if (string::npos == nPos)
            continue;

        string strCommand = strInput.substr(0, nPos);
        if (0 == ::stricmp("update", strCommand.c_str()))
        {
            string strFilePath = strInput.substr(nPos + 1);
            std::cout << "Updateing: " << strFilePath << std::endl;

            size_t nFileSize = fu.GetFileSize(strFilePath.c_str());
            if (!nFileSize)
                continue;
            std::cout << "File size: " << nFileSize << std::endl;

            if (!QuestReady(sClient))
            {
                sClient = SocketUtils::CreateClientSocket_TCP(nErrCode, szIP, usPort);
                if (INVALID_SOCKET == sClient)
                {
                    std::cout << "Reconnect to server failed." << std::endl;
                    continue;
                }
                else
                {
                    std::cout << "Reconnect to server successful." << std::endl;
                    QuestReady(sClient);
                }
            }

			if (!CheckYes(sClient))
				continue;
			std::cout << "<Updater is ready>" << std::endl;

            char arrFileInfos[FILEINFO_LENGTH] = { 0 };
			::memcpy(arrFileInfos, &nFileSize, 4);
			::memcpy(arrFileInfos + 4 + 32, strFilePath.c_str(), strFilePath.length());

			std::ifstream ifs(strFilePath.c_str(), std::ios::binary);
            if (ifs.fail())
            {
                std::cout << "Open file failed" << std::endl;
				continue;
            }

			MD5 md5;
			md5.update(ifs);
			std::string strMD5 = md5.toString();
			::memcpy(arrFileInfos + 4, strMD5.c_str(), 32);
            std::cout << "MD5: " << strMD5 << std::endl;

            if (!SocketUtils::SendToSocket(sClient, arrFileInfos, FILEINFO_LENGTH, nErrCode))
            {
                std::cout 
                    << "Send file infos failed, err msg: " 
                    << SocketUtils::QueryErrMsg(nErrCode) 
                    << std::endl;
                continue;
            }
            std::cout << "File infos send successful." << std::endl;

            char arrBuff[BUFF_LENGTH] = { 0 };
            std::cout << "Reading file: " << strFilePath.c_str() << std::endl;
            while (EOF != ifs.peek())
            {
                ifs.read(arrBuff, BUFF_LENGTH);
                size_t nRead = ifs.gcount();
                if (!nRead)
                    break;
				if (!SocketUtils::SendToSocket(sClient, arrBuff, nRead, nErrCode))
					break;
                std::cout << "*";
			}
			ifs.close();
            std::cout 
                << std::endl
                << "File content send completed"
                << std::endl;

			if(CheckYes(sClient))
				std::cout << "Update completed." << std::endl;
		}
	}
	return 0;
}

