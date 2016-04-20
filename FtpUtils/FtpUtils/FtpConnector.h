#pragma once
#pragma warning(disable:4099)
//************************************************
//��Author: CUCKOO0615
//��Date: 2016/03/30
//��Comment: ����FTP����
//*************************************************
#include <afxinet.h>
#include <string>
#include <vector>

#ifndef EXPORT_STDC
static const int PATHBUFFER_LENGTH = 1024;
struct FtpFileInfo
{
	unsigned __int64 ullLength;

	time_t tCreationTime;
	time_t tLastAccessTime;
	time_t tLastWriteTime;

	char szFileName[PATHBUFFER_LENGTH];
	char szFilePath[PATHBUFFER_LENGTH];
	char szFileTitle[PATHBUFFER_LENGTH];
	char szFileUrl[PATHBUFFER_LENGTH];
	char szRoot[PATHBUFFER_LENGTH];

	bool bIsArchived;
	bool bIsCompressed;
	bool bIsDirectory;
	bool bIsDots;
	bool bIsHidden;
	bool bIsNormal;
	bool bIsReadOnly;
	bool bIsSerializable;
	bool bIsSystem;
	bool bIsTemporary;

    bool bIsOK;
    char szErrMsg[1024];
};
#endif

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
    ** @Param bPassive: ����/����ģʽ��Ĭ��ʹ������ģʽ
    ** @Ret : �����ɹ�����true,ʧ�ܷ���false
    */
    bool CreateFtpConnection
        (LPCSTR szIP, USHORT usPort = 21,
        LPCSTR szUserName = "anonymous", LPCSTR szPassword = "",
        bool bEnableUtf8 = false,
        BOOL bPassive = FALSE);

	// ��ȡ��CreateFtpConnection������FTP���Ӷ���ָ��
	CFtpConnection* GetConnection() const;

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
	** �������ļ�����
	** @Param szRemotePath: Զ��·��,��ΪNULL��""ʱĬ��Ϊ"/"
	** @Param szFileName: �ļ���
	** @Ret : �����ɹ�����true��ʧ�ܷ���false
	*/
	bool FtpRemoveFile(LPCSTR szRemoteDirPath, LPCSTR szFileName);

	/*
	** �����ļ�
	** @Param szRemoteFilePath: Զ���ļ�·��
	** @Param szLocalFilePath: �����ļ�·��
	** @Ret: �����ɹ�����true��ʧ�ܷ���false
	*/
	bool FtpDownloadFile(
		LPCSTR szRemoteFilePath, 
		LPCSTR szLocalFilePath/*, 
		BOOL bFailIfExist = FALSE, 
		DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL,
		DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY,
		DWORD_PTR dwContext = 1*/);

    /*
    ** �ϴ��ļ�
    ** @Param szLocalFilePath: �����ļ�·��
    ** @Param szRemoteFilePath: Զ���ļ�·��
    ** @Ret: �����ɹ�����true��ʧ�ܷ���false
    */
	bool FtpUploadFile(LPCSTR szLocalFilePath, LPCSTR szRemoteFilePath);
	        
	/*
	** ��ȡԶ��Ŀ¼�е��ļ���Ϣ
	** @Param szRemoteDir: Զ��Ŀ¼
    ** @Param szFileName: �ļ���������ʹ��ͨ�����Ĭ��NULLʱʹ��*.*
	** @Param vecFileInfos: �ļ���Ϣ����
	** @Ret : �����ɹ�����true��ʧ�ܷ���false
	*/
    bool FtpGetFileInfosInDir(LPCSTR szRemoteDir, LPCSTR szFileName, std::vector<FtpFileInfo>& vecFileInfos);
    
    //��ȡ��ǰ�Ự�û���Ŀ¼,��'/'��β
	LPCSTR GetFtpRootDir() const;

	// ��ȡ������Ϣ
	LPCSTR GetLastErrMsg() const;
	
public:
    /*
    ** FtpConnector�Ĺ��캯��
    ** @Param szSessionName: CInternetSession�Ự����
    ** @Param dwDelay: ���ӳ�ʱʱ��,��λ:����,Ĭ��5000
    */
    FtpConnector(LPCSTR szSessionName, DWORD dwDelay = 5000);
    ~FtpConnector();
	
private:
    CInternetSession m_objSession;
    CFtpConnection* m_pConnection;
    std::string m_strRootDir;
    bool m_bEnableUtf8;

	std::string m_strCurrentDir;
	char m_szLastErrMsg[1024];

	void SafeCloseConnection();	//��ȫ�ر�m_pConnection����ֵΪNULL
	std::string GetFtpApiFailedErrMsg(DWORD dwErrCode);
	void InternetExceptionErrorOccured(CInternetException* pEx);
    time_t FileTime2Timet(const FILETIME& ft);
};

//////////////////////////////////////////////////////////////////////////
//inline

inline CFtpConnection* FtpConnector::GetConnection() const
{
    return m_pConnection;
}

inline LPCSTR FtpConnector::GetFtpRootDir() const
{
	if (!m_pConnection)
		return "ERROR";
    return m_strRootDir.c_str();
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

