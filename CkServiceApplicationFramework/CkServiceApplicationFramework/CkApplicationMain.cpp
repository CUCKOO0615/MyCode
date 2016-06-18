// CkServiceApplicationFramework.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <windows.h>

#include "CkMacros.h"
#include "CkServiceUtils.h"
#include "CkServiceMain.h"

//��������
void CommandParams(const TCHAR* szServerName, const TCHAR* szSrvAppPath, const TCHAR* szCmd)
{
    //��װ����
    if (0 == _tcsicmp(szCmd, CMD_INSTALL))
    {
        if (CkServiceUtils::InstallService(szServerName, szSrvAppPath))
            ::printf("Service installed successful.\n");
        else
            ::printf("Install service failed, err msg:%s\n", 
			CkServiceUtils::GetLastErrMsg());
    }
    //ж�ط���
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
        //����ָ����SERVICE_WIN32_OWN_PROCESS, lpServiceName������
        tDispatchTable[0].lpServiceName = _T("");
        tDispatchTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)CkServiceMain::ServiceMain;
        tDispatchTable[1].lpServiceName = NULL;   //�̶�
        tDispatchTable[1].lpServiceProc = NULL;   //�̶�

        if (!StartServiceCtrlDispatcher(tDispatchTable))
        {
            if (ERROR_SERVICE_ALREADY_RUNNING == ::GetLastError())
                ::printf("Service is already running.\n");
            return -1;
        }
    }
    return 0;
}

