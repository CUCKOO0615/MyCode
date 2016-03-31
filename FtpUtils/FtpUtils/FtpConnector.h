#pragma once
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

    
public:
    /*
    ** FtpConnector�Ĺ��캯��
    ** @Param szSessionName: CInternetSession�Ự����
    ** @Param dwDelay: ���ӳ�ʱʱ��,��λ:����,Ĭ��5000
    */
    FtpConnector(const TCHAR* szSessionName, DWORD dwDelay = 5000);
    ~FtpConnector();

private:
    CInternetSession m_objSession;
    CFtpConnection* m_pConnection;
    CString m_strFtpRootDir;
    CString m_strFtpCurrentDir;
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