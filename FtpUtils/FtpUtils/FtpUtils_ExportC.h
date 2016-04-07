#pragma once

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
    ** @Ret : ���ӳɹ�����true,ʧ�ܷ���false
    */
    CK_API bool ConnectFtp(FtpConnector* pConnector, 
		const char* szIP, unsigned short usPort = 21,
        const char* szUserName = "anonymous", const char* szPassword = "");
    
    /*
    ** �����趨FTP��������ǰĿ¼
    ** @Param pConnector: ��Ч��FtpConnector����
    ** @Param szRemotePath: Զ��·��,��ΪNULL��""ʱĬ��Ϊ"/"
    ** @Param bUtf8Path: �Ƿ�����UTF8���뷢���ֽ���
    ** @Ret : �μ����·���ֵ
    // 0 Successful
    // 1 UTF8 Convert failed
    // 2 Set FTP Server's current dir failed
    */
    CK_API int FtpTry2SetCurDir(FtpConnector* pConnector,  
		const char* szRemotePath, bool bUtf8Path);
    
    /*
    ** ɾ���������ϵ��ļ�
    ** @Param pConnector: ��Ч��FtpConnector����
    ** @Param szRemotePath: Զ��·��,��ΪNULL��""ʱĬ��Ϊ"/"
    ** @Param szFileName: �ļ���
    ** @Param bUtf8Path: �Ƿ�����UTF8���뷢���ֽ���
    ** @Ret : 
    // 0 Successful
    // 1 UTF8 Convert failed
    // 2 Set FTP Server's current dir failed
    // 3 Remove file failed
    */
    CK_API int FtpRemoveFile(FtpConnector* pConnector, 
        const char* szRemotePath, const char* szFileName, bool bUtf8Path);
}