#pragma once
//************************************************
//��Author: CUCKOO0615
//��Date: 2016/03/30
//��Comment: ����FTP����(C�����ӿ�)
//************************************************

#ifdef EXPORT_STDC
#define CK_API extern "C" __declspec(dllexport)
#else
#define CK_API extern "C" __declspec(dllimport)
#endif

struct FtpConnector;

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
};

namespace CkFtpUtils
{
    /*
    ** ����FtpConnector����
    ** @Param szSessionName: �Ự����
    ** @Param nDelay: ���ӳ�ʱʱ��, ��λ:����
    ** @Ret : ����ʧ�ܷ���NULL
    */
    CK_API FtpConnector* CreateFtpConnector(const char* szSessionName, int nDelay = 5000);
    
    /*
    ** �ͷ�FtpConnector����
    ** @Param pFtpConn: FtpConnector����ָ��
    */
    CK_API void ReleaseFtpConnector(FtpConnector* pFtpConn);

	/*
	** ��ȡ������Ϣ
	** @Param pConnector: ��Ч��FtpConnector����
	** @Ret: ����������Ϣ���ַ���
	*/
	CK_API const char* GetLastErrMsg(FtpConnector* pConnector);
	
    /*
    ** ���ӵ�ָ����FTP������
    ** @Param pConnector: ��Ч��FtpConnector����
    ** @Param szIP: ������IP
    ** @Param usPort: �������˿�,��ΪNULLʱĬ��21
    ** @Param szUserName: �û���,��ΪNULLʱĬ��anonymous
    ** @Param szPassword: ����
	** @Param bEnableUtf8: �����������ʹ��UTF8����,��ò���Ӧ��Ϊtrue
    ** @Param bPassive: ����/����ģʽ��Ĭ��ʹ������ģʽ
    ** @Ret : ���ӳɹ�����true,ʧ�ܷ���false
    */
    CK_API bool ConnectFtp(FtpConnector* pConnector, 
		const char* szIP, unsigned short usPort = 21,
        const char* szUserName = "anonymous", const char* szPassword = "",
		bool bEnableUtf8 = false,
        bool bPassive = false);
    
	/*
	** ���õ�ǰ�Ự�û���ǰĿ¼
	** @Param pConnector: ��Ч��FtpConnector����
	** @Param strDirPath: ���·��
	** @Ret : �ɹ�����true,ʧ�ܷ���false
	*/
	CK_API bool SetFtpCurrentDir(FtpConnector* pConnector, const char* szDirPath);
	
	/*
	** ��ȡ��ǰ�Ự�û���ǰĿ¼
	** @Param pConnector: ��Ч��FtpConnector����
	** @Ret: �����ɹ����ط������˵�ǰĿ¼,����ʧ�ܷ���"ERROR"
	*/
	CK_API const char* GetFtpCurrentDir(FtpConnector* pConnector);

	/*
	** ��ȡ��ǰ�Ự�û���Ŀ¼
	** @Param pConnector: ��Ч��FtpConnector����
	** @Ret: �����ɹ����ط������˸�Ŀ¼,,����ʧ�ܷ���"ERROR"
	*/
	CK_API const char* GetFtpRootDir(FtpConnector* pConnector);

    /*
    ** ɾ���������ϵ��ļ�
    ** @Param pConnector: ��Ч��FtpConnector����
    ** @Param szRemotePath: Զ��·��,��ΪNULL��""ʱĬ��Ϊ"/"
    ** @Param szFileName: �ļ���
    ** @Ret : �����ɹ�����true��ʧ�ܷ���false
    */
    CK_API bool FtpRemoveFile(FtpConnector* pConnector, const char* szRemotePath, const char* szFileName);


	/*
	** �����ļ�
	** @Param pConnector: ��Ч��FtpConnector����
	** @Param szRemoteFilePath: Զ���ļ�·��
	** @Param szLocalFilePath: �����ļ�·��
    ** @Ret: �����ɹ�����true��ʧ�ܷ���false
    */
    CK_API bool FtpDownloadFile(FtpConnector* pConnector, 
        const char* szRemoteFilePath, const char* szLocalFilePath);

    /*
    ** �ϴ��ļ�
    ** @Param pConnector: ��Ч��FtpConnector����
    ** @Param szLocalFilePath: �����ļ�·��
    ** @Param szRemoteFilePath: Զ���ļ�·��
    ** @Ret: �����ɹ�����true��ʧ�ܷ���false
    */
    CK_API bool FtpUploadFile(FtpConnector* pConnector,
        const char* szLocalFilePath, const char* szRemoteFilePath);
	
    /*
    ** ��ȡԶ��Ŀ¼�е��ļ���Ϣ
    ** @Param pConnector: ��Ч��FtpConnector����
    ** @Param szRemoteDir: Զ��Ŀ¼
    ** @Param szFileName: �ļ���������ʹ��ͨ�����Ĭ��NULLʱʹ��*.*
    ** @Param arrFileInfos: ָ���ļ���Ϣ�����ָ�룬
    ** ���������ɹ�����Ҫ����ReleaseFileInfos�ֶ��ͷţ����������ڴ�й©
    ** @Ret : �����ɹ�����true��ʧ�ܷ���false
    */
	CK_API bool FtpGetFileInfosInDir(FtpConnector* pConnector, 
		const char* szRemoteDir, const char* szFileName, FtpFileInfo** arrFileInfos, int* nArrSize);

    // �ͷ�FtpFileInfo����ָ��
    CK_API void ReleaseFileInfos(FtpFileInfo** pArrFileInfos);

}

