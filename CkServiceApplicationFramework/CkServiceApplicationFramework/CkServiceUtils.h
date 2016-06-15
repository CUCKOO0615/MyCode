#pragma once
#include <windows.h>

class CkServiceUtils
{
public:
	/*
	** ��װ����
	** @Param szSrvName: ������
	** @Param szSrvAppPath: ��������ļ�·��
	** @Ret: �����ɹ�����true, ʧ�ܷ���false
	*/
	static bool InstallService(LPCTSTR szSrvName, LPCTSTR szSrvAppPath);
	/*
	** ж�ط���
	** @Param szSrvName: ������
	** @Ret: �����ɹ�����true, ʧ�ܷ���false
	*/
	static bool UninstallService(LPCTSTR szSrvName);

    //��ȡ������Ϣ
    static const char* GetLastErrMsg();

private:
	static bool OpenServiceManager(SC_HANDLE& hScManager);
	static bool CreateService(SC_HANDLE hScManager, LPCTSTR szPath, LPCTSTR szServiceName);
	static bool DeleteService(SC_HANDLE hScManager, LPCTSTR szSrvName);
    
    static char m_szLastErrMsg[512];
};

