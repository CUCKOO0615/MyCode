// CkServiceApplicationFramework.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <windows.h>

#include "CkMacros.h"
#include "CkServiceUtils.h"
#include "CkServiceMain.h"

//启动参数
void CommandParams(const TCHAR* szServerName, const TCHAR* szSrvAppPath, const TCHAR* szCmd)
{
    //安装服务
    if (0 == _tcsicmp(szCmd, CMD_INSTALL))
    {
        if (CkServiceUtils::InstallService(szServerName, szSrvAppPath))
            ::printf("Service installed successful.\n");
        else
            ::printf("Install service failed, err msg:%s\n", 
			CkServiceUtils::GetLastErrMsg());
    }
    //卸载服务
    else if (0 == _tcsicmp(szCmd, CMD_UNINSTALL))
    {
        if (CkServiceUtils::UninstallService(szServerName))
            ::printf("Service uninstalled successful.\n");
        else
            ::printf("Uninstall service failed, err msg:%s\n", 
			CkServiceUtils::GetLastErrMsg());
    }
}

//main
int _tmain(int argc, _TCHAR* argv[])
{
    if (argc > 1)
        CommandParams(SERVICE_NAME, argv[0], argv[1]);
    else
    {
        SERVICE_TABLE_ENTRY tDispatchTable[2] = { 0 };
        //由于指定了SERVICE_WIN32_OWN_PROCESS, lpServiceName被忽略
        tDispatchTable[0].lpServiceName = _T("");
        tDispatchTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)CkServiceMain::ServiceMain;
        tDispatchTable[1].lpServiceName = NULL;   //固定
        tDispatchTable[1].lpServiceProc = NULL;   //固定

        if (!StartServiceCtrlDispatcher(tDispatchTable))
        {
            if (ERROR_SERVICE_ALREADY_RUNNING == ::GetLastError())
                ::printf("Service is already running.\n");
            return -1;
        }
    }
    return 0;
}

