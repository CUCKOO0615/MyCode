#pragma once
#pragma warning(disable:4099)
//************************************************
//��Author: CUCKOO0615
//��Date: 2016/03/30
//��Comment: ����FTP����
//*************************************************
#include <afxinet.h>

class FtpConnector
{
public:
    /*
    ** ����һ��FTP���Ӷ���
    ** @Param szIP: IP��ַ
    ** @Param usPort: �˿ں�,Ĭ��21
    ** @Param szUserName: �û���,Ĭ��anonymous(�����û�)
    ** @Param szPassword: Ĭ��Ϊ��
    ** @Ret : �����ɹ�����true,ʧ�ܷ���false,������Ϣ����־�ļ�
    */
    bool CreateFtpConnection(
        const char* szIP, unsigned short usPort = 21, 
        const char* szUserName = "anonymous", const char* szPassword="");
    
    /*
    ** ��ȡ��CreateFtpConnection������FTP���Ӷ���ָ��
    */
    CFtpConnection* GetConnection();

    CString GetFtpRootDir();
    CString GetFtpCurrentDir();

    /*
    ** ���÷�������ǰĿ¼
    ** @Param strDirPath: ���·�� 
    ** @Ret : �ɹ�����true,ʧ�ܷ���false,������Ϣ����־�ļ�
    */
    bool SetCurrentDir(CString strDirPath);

	// ��ȡ������Ϣ
	const char* GetLastErrMsg();	

public:
    /*
    ** FtpConnector�Ĺ��캯��
    ** @Param szSessionName: CInternetSession�Ự����
    ** @Param dwDelay: ���ӳ�ʱʱ��,��λ:����,Ĭ��5000
    */
    FtpConnector(const TCHAR* szSessionName, DWORD dwDelay = 5000);
    ~FtpConnector();

	void SetLastErrMsg(const char* szFormat = "OK", ...);
	void SetLastErrMsg(const wchar_t* wszFormat = L"OK", ...);

private:
    CInternetSession m_objSession;
    CFtpConnection* m_pConnection;
    CString m_strFtpRootDir;
    CString m_strFtpCurrentDir;

	static const int LASTERRMSG_LENGTH = 1024;
	char m_szLastErrMsg[LASTERRMSG_LENGTH];
};

//////////////////////////////////////////////////////////////////////////
//inline

inline CFtpConnection* FtpConnector::GetConnection()
{
    return m_pConnection;
}

inline CString FtpConnector::GetFtpRootDir()
{
    return m_strFtpRootDir;
}

inline CString FtpConnector::GetFtpCurrentDir()
{
    return m_strFtpCurrentDir;
}

inline const char* FtpConnector::GetLastErrMsg()
{
	return m_szLastErrMsg;
}