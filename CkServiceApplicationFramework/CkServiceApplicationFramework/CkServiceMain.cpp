#include "StdAfx.h"
#include "CkMacros.h"
#include "CkServiceMain.h"
#include "CkGlobalManager.h"

SERVICE_STATUS_HANDLE g_hServiceStatus;

void WINAPI CkServiceMain::ServiceCtrl(DWORD dwOpcode)
{
    SERVICE_STATUS tServiceStatus;

    tServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    //tServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    tServiceStatus.dwCurrentState = SERVICE_STOPPED;
    tServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
    tServiceStatus.dwWin32ExitCode = 0;
    tServiceStatus.dwServiceSpecificExitCode = 0;
    tServiceStatus.dwCheckPoint = 0;
    tServiceStatus.dwWaitHint = 0;

    switch (dwOpcode)
    {
    case SERVICE_CONTROL_PAUSE:
        tServiceStatus.dwCurrentState = SERVICE_PAUSED;
        CkLogUtils::RecordingA(gLogUtils, LL_INFO, "PAUSE");
        break;
    case SERVICE_CONTROL_CONTINUE:
        tServiceStatus.dwCurrentState = SERVICE_RUNNING;
        CkLogUtils::RecordingA(gLogUtils, LL_INFO, "CONTINUE");
        break;
    case SERVICE_CONTROL_STOP:
        tServiceStatus.dwCurrentState = SERVICE_STOPPED;
        CkLogUtils::RecordingA(gLogUtils, LL_INFO, "STOP");
        break;
    }
    if (FALSE != SetServiceStatus(g_hServiceStatus, &tServiceStatus))
        return;
}

void WINAPI CkServiceMain::ServiceMain(DWORD argc, LPTSTR * argv)
{
    //×¢²á¿ØÖÆº¯Êý
    g_hServiceStatus = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrl);
    if (g_hServiceStatus == (SERVICE_STATUS_HANDLE)0)
    {
        CkLogUtils::RecordingA(gLogUtils, LL_INFO,
            "RegisterServiceCtrlHandler failed, err code:%d\n", ::GetLastError());
        return;
    }
    ServiceCtrl(SERVICE_CONTROL_CONTINUE);

    //Do something here

    //
}