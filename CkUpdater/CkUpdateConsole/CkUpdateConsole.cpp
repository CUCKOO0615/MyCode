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

static const int BUFF_LENGTH = 1024/* * 500*/;

char charBuff[BUFF_LENGTH] = { 0 };
void ConsoleWriteLine(const char* szFormat, ...)
{
	va_list argList;
	va_start(argList, szFormat);
	::vsprintf(charBuff, szFormat, argList);
	va_end(argList);
	std::cout << charBuff << std::endl;
}

bool CheckYes(SOCKET s)
{
	int nErrCode = 0;
	int nRecv = 0;
    if (!SocketUtils::RecvFromSocket(s, (char*)&nRecv, sizeof(int), nErrCode))
		return false;
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
	return SocketUtils::SendToSocket(s, (char*)&nQuest, sizeof(int), nErrCode);
}


typedef struct stuFileInfo
{
	size_t nFileLength;
	char szMD5[32];
	char szFileName[256];
}FILE_INFO;

int _tmain(int argc, _TCHAR* argv[])
{
    if (!g_bGlobalInited || argc < 2)
        return 1;

	std::string strAddr(argv[1]), strIP, strPort, strErrMsg;
	
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
	ConsoleWriteLine("====[ Connected to server ]====");

    string strInput;
    FileUtils fu;
	FILE_INFO fileInfo;
	while (true)
	{
		ConsoleWriteLine("====[ <<<< IDLE >>>> ]====");
		::memset(&fileInfo, 0x00, sizeof(FILE_INFO));

		std::getline(std::cin, strInput);
		size_t nPos = strInput.find(' ');
		if (string::npos == nPos)
            continue;

        string strCommand = strInput.substr(0, nPos);
        StringUtils::Trim(strCommand, ' ');

        if (0 == ::stricmp("update", strCommand.c_str()))
        {
            string strFilePath = strInput.substr(nPos + 1);
			::memcpy(fileInfo.szFileName, strFilePath.c_str(), strFilePath.length());
			ConsoleWriteLine(">> Updateing: %s", fileInfo.szFileName);
			
			size_t nFileSize = fu.GetFileSize(fileInfo.szFileName);
			ConsoleWriteLine(">> File size: %d", nFileSize);    

            if (!nFileSize) 
				continue;
			fileInfo.nFileLength = nFileSize;

            if (!QuestReady(sClient))
            {
                sClient = SocketUtils::CreateClientSocket_TCP(nErrCode, szIP, usPort);
				ConsoleWriteLine(">> Reconnect to server %s",
					(INVALID_SOCKET == sClient) ? "failed" : "successful");
				continue;
            }

			if (!CheckYes(sClient))
				continue;
			ConsoleWriteLine("==[ Updater is ready ]==");		
			
			std::ifstream ifs(strFilePath.c_str(), std::ios::binary);
            if (ifs.fail())
            {
				ConsoleWriteLine(">>　Open file failed．");
				continue;
            }

			MD5 md5;
			md5.update(ifs);
			::memcpy(fileInfo.szMD5, md5.toString().c_str(), 32);
			ConsoleWriteLine(">> MD5: %s", fileInfo.szMD5);

            if (!SocketUtils::SendToSocket(sClient, (char*)&fileInfo, sizeof(FILE_INFO), nErrCode))
			{
				ConsoleWriteLine("Send file infos failed");
				continue;
			}
			ConsoleWriteLine("Send file infos successful");
			ConsoleWriteLine("Reading file: %s", strFilePath.c_str());

            char arrBuff[BUFF_LENGTH] = { 0 };
			int nCounter = 0;
            while (EOF != ifs.peek())
            {
                ifs.read(arrBuff, BUFF_LENGTH);
                size_t nRead = ifs.gcount();
                if (!nRead)
                    break;
				if (!SocketUtils::SendToSocket(sClient, arrBuff, nRead, nErrCode))
				{
					ConsoleWriteLine("Send file content failed, err msg: %s",
						SocketUtils::QueryErrMsg(nErrCode));
					break;
				}

				nCounter += nRead;
				if (nCounter >= 1024 * 100)
				{
					std::cout << ">";
					nCounter -= 1024 * 100;
				}
			}
			ifs.close();

			ConsoleWriteLine("File content send completed"); 
			if(CheckYes(sClient))
				ConsoleWriteLine("====[ Update successful ]====");
			else
				ConsoleWriteLine("====[!! Update failed !!]====");
		}
	}
	return 0;
}

