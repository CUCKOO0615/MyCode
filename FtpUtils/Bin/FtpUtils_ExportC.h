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
    ** @Ret : ���ӳɹ�����true,ʧ�ܷ���false
    */
    CK_API bool ConnectFtp(FtpConnector* pConnector, 
		const char* szIP, unsigned short usPort = 21,
        const char* szUserName = "anonymous", const char* szPassword = "",
		bool bEnableUtf8 = false);
    
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
    ** ɾ���������ϵ��ļ�
    ** @Param pConnector: ��Ч��FtpConnector����
    ** @Param szRemotePath: Զ��·��,��ΪNULL��""ʱĬ��Ϊ"/"
    ** @Param szFileName: �ļ���
    ** @Ret : �����ɹ�����true��ʧ�ܷ���false
    */
    CK_API bool FtpRemoveFile(FtpConnector* pConnector, const char* szRemotePath, const char* szFileName);
}
