#pragma once
#include <windows.h>

class CkServiceUtils
{
public:
	/*
	** 安装服务
	** @Param szSrvName: 服务名
	** @Param szSrvAppPath: 服务程序文件路径
	** @Ret: 操作成功返回true, 失败返回false
	*/
	static bool InstallService(LPCTSTR szSrvName, LPCTSTR szSrvAppPath);
	/*
	** 卸载服务
	** @Param szSrvName: 服务名
	** @Ret: 操作成功返回true, 失败返回false
	*/
	static bool UninstallService(LPCTSTR szSrvName);

    //获取错误信息
    static const char* GetLastErrMsg();

private:
	static bool OpenServiceManager(SC_HANDLE& hScManager);
	static bool CreateService(SC_HANDLE hScManager, LPCTSTR szPath, LPCTSTR szServiceName);
	static bool DeleteService(SC_HANDLE hScManager, LPCTSTR szSrvName);
    
    static char m_szLastErrMsg[512];
};

