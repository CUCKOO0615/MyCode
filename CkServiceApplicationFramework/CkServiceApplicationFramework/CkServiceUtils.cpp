#include "stdafx.h"
#include "CkServiceUtils.h"

#define SET_LASTERRMSG(strFormat, ...)   ::sprintf(m_szLastErrMsg, strFormat, __VA_ARGS__)
#define RESET_LASTERRMSG                 m_szLastErrMsg[0]='\0'

char CkServiceUtils::m_szLastErrMsg[512] = { 0 };

bool CkServiceUtils::OpenServiceManager(SC_HANDLE& hScManager)
{
	hScManager = ::OpenSCManager(
		NULL,                    //Local machine
		NULL,                    //SerivceActive database
		SC_MANAGER_ALL_ACCESS);  //full access rights

    bool bRet = (NULL != hScManager);
    if (!bRet)
        SET_LASTERRMSG("OpenSCManager failed, err code:%d", ::GetLastError());
    return bRet;
}

bool CkServiceUtils::CreateService(SC_HANDLE hScManager, LPCTSTR szPath, LPCTSTR szServiceName)
{
    SC_HANDLE hService = ::CreateService(
        hScManager,                            //SCM句柄
        szServiceName,                         //服务名
        szServiceName,                         //显示的服务名
        SERVICE_ALL_ACCESS,                    //存取权限
        SERVICE_WIN32_OWN_PROCESS,             //服务类别
        SERVICE_DEMAND_START,                  //启动类别:手动 (SERVICE_AUTO_START自动)
        SERVICE_ERROR_NORMAL,                  //错误控制类别
        szPath,                                //服务的可执行文件路径
        NULL,                                  //no load ordering group
        NULL,                                  //no tag indentifier
        NULL,                                  //no dependencies
        NULL,                                  //LocalSystem account
        NULL);                                 //no password

    bool bRet = (NULL != hService);
    if (!bRet)
        SET_LASTERRMSG("CreateService failed, err code:%d", ::GetLastError());
    else
        ::CloseServiceHandle(hService);
    return bRet;
}

bool CkServiceUtils::DeleteService(SC_HANDLE hScManager, LPCTSTR szSrvName)
{
    SC_HANDLE hService = OpenService(
        hScManager,
        szSrvName,
        DELETE
        );

    if (NULL == hService)
    {
        SET_LASTERRMSG("OpenService failed in DeleteService, err code:%d", ::GetLastError());
        return false;
    }

    bool bRet = (FALSE != ::DeleteService(hService));
    if (!bRet)
        SET_LASTERRMSG("::DeleteService failed in DeleteService, err code:%d", ::GetLastError());
    ::CloseServiceHandle(hService);
    return bRet;
}

bool CkServiceUtils::InstallService(LPCTSTR szSrvName, LPCTSTR szSrvAppPath)
{
    RESET_LASTERRMSG;
	SC_HANDLE hSrvManager;
    if (!OpenServiceManager(hSrvManager))
        return false;
    
    bool bRet = CreateService(hSrvManager, szSrvAppPath, szSrvName);
    ::CloseServiceHandle(hSrvManager);
    return bRet;
}

bool CkServiceUtils::UninstallService(LPCTSTR szSrvName)
{
    RESET_LASTERRMSG;
	SC_HANDLE hSrvManager;
    if (!OpenServiceManager(hSrvManager))
        return false;
    
    bool bRet = DeleteService(hSrvManager, szSrvName);	
	::CloseServiceHandle(hSrvManager);
    return bRet;
}

const char* CkServiceUtils::GetLastErrMsg()
{
    return m_szLastErrMsg;
}
