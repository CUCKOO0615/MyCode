// CkUpdateProxy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SocketUtils.h"
#include <iostream>
#include "ThreadUtils.hpp"
#include "AutoManager.hpp"

#define BUFF_LENGTH 1024*2048
char g_recvBuff[BUFF_LENGTH] = { 0 };

#define PRINT_SOCKET_ERRMSG \
	std::cout<<SocketUtils::QueryErrMsg(g_nErrCode)<<std::endl

int g_nErrCode = 0;

const int _1M_BYTES_ = 1024 * 1024;
TU_DECLEAR_THREADENTRY(TransmitFunc)
{
	SOCKET* arrSocket = (SOCKET*)pParam;	
    int nCount = 0;
	while (true)
    {
        int nRecv = ::recv(arrSocket[0], g_recvBuff, BUFF_LENGTH, 0);
        if (!nRecv || SOCKET_ERROR == nRecv)
            break;
        if (!SocketUtils::SendToSocket(arrSocket[1], g_recvBuff, nRecv, g_nErrCode))
        {
            PRINT_SOCKET_ERRMSG;
            break;
        }
        nCount += nRecv;
        if (nCount >= _1M_BYTES_)
        {
            std::cout << "*";
            nCount -= _1M_BYTES_;
        }
    }
	::closesocket(arrSocket[0]);
	::closesocket(arrSocket[1]);
    std::cout 
        << std::endl
        << "Transmit thread exit" 
        << std::endl;
	return 0;
}

SOCKET g_sServer4Updater = INVALID_SOCKET;
SOCKET g_sServer4Console = INVALID_SOCKET;
SOCKET g_sUpdater = INVALID_SOCKET;
SOCKET g_sConsole = INVALID_SOCKET;
HANDLE g_hEvent4Updater = ::CreateEvent(NULL, FALSE, FALSE, NULL);
HANDLE g_hEvent4Console = ::CreateEvent(NULL, FALSE, FALSE, NULL);

ThreadUtils tuConsole2Updater;
ThreadUtils tuUpdater2Console;

TU_DECLEAR_THREADENTRY(AcceptingUpdater)
{
    while (true)
    {
        SOCKET sNew = ::accept(g_sServer4Updater, NULL, NULL);
        if (STILL_ACTIVE == tuConsole2Updater.GetExitCode() ||
            STILL_ACTIVE == tuUpdater2Console.GetExitCode())
        {
            ::closesocket(sNew);
            continue;
        }
        if (INVALID_SOCKET == sNew)
            continue;
        g_sUpdater = sNew;
        std::cout << "Updater connected" << std::endl;       
        ::SetEvent(g_hEvent4Updater);
    }
    return 0;
}

TU_DECLEAR_THREADENTRY(AcceptingConsole)
{
    while (true)
    {
        SOCKET sNew = ::accept(g_sServer4Console, NULL, NULL);
        if (STILL_ACTIVE == tuConsole2Updater.GetExitCode() ||
            STILL_ACTIVE == tuUpdater2Console.GetExitCode())
        {
            ::closesocket(sNew);
            continue;
        }
        if (INVALID_SOCKET == sNew)
            continue;
        g_sConsole = sNew;
        std::cout << "Console connected" << std::endl;      
        ::SetEvent(g_hEvent4Console);
    }
    return 0;
}

char charBuff[1024] = { 0 };
void PrintLine(const char* szFormat, ...)
{
    va_list argList;
    va_start(argList, szFormat);
	::vsprintf(charBuff, szFormat, argList);
    va_end(argList);
	std::cout << charBuff << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (!g_bGlobalInited)
        return 0;
    
    PrintLine("Creating server sockets..");
    g_sServer4Updater = SocketUtils::CreateServerSocket_TCP(g_nErrCode, 61500);
    g_sServer4Console = SocketUtils::CreateServerSocket_TCP(g_nErrCode, 61501);
    if (INVALID_SOCKET == g_sServer4Updater ||
        INVALID_SOCKET == g_sServer4Console)
    {
        PrintLine("Create server socket failed");
        return -1;
    }
    PrintLine("Server sockets created successful");

    if (!SocketUtils::ListenAt(g_nErrCode, g_sServer4Updater, 1))
    {
        PRINT_SOCKET_ERRMSG;
        ::closesocket(g_sServer4Updater);
        return 1;
    }
    PrintLine("Server for updater listening at: %d", g_sServer4Updater);

    if (!SocketUtils::ListenAt(g_nErrCode, g_sServer4Console, 1))
    {
        PRINT_SOCKET_ERRMSG;
        ::closesocket(g_sServer4Updater);
        ::closesocket(g_sServer4Console);
        return 1;
    }
    PrintLine("Server for console listening at: %d", g_sServer4Console);
    
    ThreadUtils tuAcceptingUpdater;
    ThreadUtils tuAcceptingConsole;
    if (!tuAcceptingUpdater.Run(AcceptingUpdater) ||
        !tuAcceptingConsole.Run(AcceptingConsole))
    {
        PrintLine("Create accepting thread failed");
        return -2;
    }
    
    const HANDLE arrHandles[2] = { g_hEvent4Console, g_hEvent4Updater };
    while (true)
	{
        std::cout << "=== Idle ===" << std::endl;
        ::WaitForMultipleObjects(2, arrHandles, TRUE, INFINITE);
        std::cout << "=== Transmit start ===" << std::endl;

        SOCKET arrConsoleUpdater[2] = { g_sConsole, g_sUpdater };
		tuConsole2Updater.SetThreadParams(arrConsoleUpdater);
		tuConsole2Updater.Run(TransmitFunc);
		std::cout << "Console to updater tramsmiting.." << std::endl;

        SOCKET arrUpdaterConsole[2] = { g_sUpdater, g_sConsole };
		tuUpdater2Console.SetThreadParams(arrUpdaterConsole);
		tuUpdater2Console.Run(TransmitFunc);
		std::cout << "Updater to console tramsmiting.." << std::endl;

		tuConsole2Updater.Clean();
		tuUpdater2Console.Clean();
		std::cout << "=== Transmit completed ===" << std::endl;
	}
	return 0;
}

