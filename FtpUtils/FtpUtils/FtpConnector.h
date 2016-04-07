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
	** @Param bEnableUtf8: �����������ʹ��UTF8����,��ò���Ӧ��Ϊtrue
    ** @Ret : �����ɹ�����true,ʧ�ܷ���false
    */
    bool CreateFtpConnection
        (LPCSTR szIP, USHORT usPort = 21,
        LPCSTR szUserName = "anonymous", LPCSTR szPassword = "",
        bool bEnableUtf8 = false);

    /*
    ** ��ȡ��ǰ�Ự�û���ǰĿ¼
    ** @Ret: �����ɹ����ط������˵�ǰĿ¼,����ʧ�ܷ���"ERROR"
    */
	LPCSTR GetFtpCurrentDir();

    /*
    ** ���õ�ǰ�Ự�û���ǰĿ¼
    ** @Param strDirPath: ���·�� 
    ** @Ret : �ɹ�����true,ʧ�ܷ���false
    */
    bool SetFtpCurrentDir(LPCSTR szDirPath);

	/*
	** ɾ���������ϵ��ļ�
	** @Param szRemotePath: Զ��·��,��ΪNULL��""ʱĬ��Ϊ"/"
	** @Param szFileName: �ļ���
	** @Ret : �����ɹ�����true��ʧ�ܷ���false
	*/
	bool FtpRemoveFile(LPCSTR szRemoteDirPath, LPCSTR szFileName);
	    
    // ��ȡ��CreateFtpConnection������FTP���Ӷ���ָ��
    CFtpConnection* GetConnection() const;
    
    //��ȡ��ǰ�Ự�û���Ŀ¼,��'/'��β
	CString GetFtpRootDir() const;

	// ��ȡ������Ϣ
	const char* GetLastErrMsg() const;

public:
    /*
    ** FtpConnector�Ĺ��캯��
    ** @Param szSessionName: CInternetSession�Ự����
    ** @Param dwDelay: ���ӳ�ʱʱ��,��λ:����,Ĭ��5000
    */
    FtpConnector(LPCSTR szSessionName, DWORD dwDelay = 5000);
    ~FtpConnector();

	void SetLastErrMsg(const char* szFormat = "OK", ...);

private:
    CInternetSession m_objSession;
    CFtpConnection* m_pConnection;
    CString m_strRootDir;
    bool m_bEnableUtf8;

	static const int CURRENTDIR_LENGTH = 1024;
	char m_szCurrentDir[CURRENTDIR_LENGTH];

	static const int LASTERRMSG_LENGTH = 1024;
	char m_szLastErrMsg[LASTERRMSG_LENGTH];

	//��ȫ�ر�m_pConnection����ֵΪNULL
	void SafeCloseConnection();
};

//////////////////////////////////////////////////////////////////////////
//inline

inline CFtpConnection* FtpConnector::GetConnection() const
{
    return m_pConnection;
}

inline CString FtpConnector::GetFtpRootDir() const
{
    return m_strRootDir;
}

inline const char* FtpConnector::GetLastErrMsg() const
{
	return m_szLastErrMsg;
}

inline void FtpConnector::SafeCloseConnection()
{
	if (m_pConnection)
		m_pConnection->Close();
	m_pConnection = NULL;
}
