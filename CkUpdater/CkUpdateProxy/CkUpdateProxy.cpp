// CkUpdateProxy.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "SocketUtils.h"
#include <iostream>
#include "ThreadUtils.hpp"
#include "AutoManager.hpp"

#define BUFF_LENGTH 1024*1000
#define PRINT_SOCKET_ERRMSG \
	std::cout<<SocketUtils::QueryErrMsg(g_nErrCode)<<std::endl

int g_nErrCode = 0;

TU_DECLEAR_THREADENTRY(TransmitFunc)
{
	SOCKET* arrSocket = (SOCKET*)pParam;
	char arrBuff[BUFF_LENGTH] = { 0 };
    const int n_1M_Bytes = 1024 * 1024;
    int nCount = 0;
	while (true)
    {
        int nRecv = ::recv(arrSocket[0], arrBuff, BUFF_LENGTH, 0);
        if (!nRecv || SOCKET_ERROR == nRecv)
            break;
        if (!SocketUtils::SendToSocket(arrSocket[1], arrBuff, nRecv, g_nErrCode))
        {
            PRINT_SOCKET_ERRMSG;
            break;
        }

        nCount += nRecv;
        if (nCount >= n_1M_Bytes)
        {
            std::cout << "*";
            nCount -= n_1M_Bytes;
        }
    }
	std::cout << std::endl;
	::closesocket(arrSocket[0]);
	::closesocket(arrSocket[1]);
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
        std::cout << "Updater accepted" << std::endl;
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
        std::cout << "Console accepted" << std::endl;
        ::SetEvent(g_hEvent4Console);
    }
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (!g_bGlobalInited)
        return 0;

    g_sServer4Updater = SocketUtils::CreateServerSocket_TCP(g_nErrCode, 61500);
    g_sServer4Console = SocketUtils::CreateServerSocket_TCP(g_nErrCode, 61501);

    if (!SocketUtils::ListenAt(g_nErrCode, g_sServer4Updater, 1))
    {
        PRINT_SOCKET_ERRMSG;
        ::closesocket(g_sServer4Updater);
        return 1;
    }
    std::cout << "listening at: " << g_sServer4Updater << std::endl;

    if (!SocketUtils::ListenAt(g_nErrCode, g_sServer4Console, 1))
    {
        PRINT_SOCKET_ERRMSG;
        ::closesocket(g_sServer4Updater);
        ::closesocket(g_sServer4Console);
        return 1;
    }
    std::cout << "listening at: " << g_sServer4Console << std::endl;
    
    ThreadUtils tuAcceptingUpdater;
    ThreadUtils tuAcceptingConsole;
    if (!tuAcceptingUpdater.Run(AcceptingUpdater) ||
        !tuAcceptingConsole.Run(AcceptingConsole))
        std::cout << "Create accepting thread failed" << std::endl;

    while (true)
	{
        HANDLE arrHandles[2] = { g_hEvent4Console, g_hEvent4Updater };
        ::WaitForMultipleObjects(2, arrHandles, TRUE, INFINITE);

        std::cout << "=== Transmit start ===" << std::endl;

        SOCKET arrConsoleUpdater[2] = { g_sConsole, g_sUpdater };
		tuConsole2Updater.SetThreadParams(arrConsoleUpdater);
		tuConsole2Updater.Run(TransmitFunc);
		std::cout << "Console to updater tramsmiting" << std::endl;

        SOCKET arrUpdaterConsole[2] = { g_sUpdater, g_sConsole };
		tuUpdater2Console.SetThreadParams(arrUpdaterConsole);
		tuUpdater2Console.Run(TransmitFunc);
		std::cout << "Updater to console tramsmiting" << std::endl;

		tuConsole2Updater.Clean();
		tuUpdater2Console.Clean();
		std::cout << "=== Transmit completed ===" << std::endl;
	}
	return 0;
}
