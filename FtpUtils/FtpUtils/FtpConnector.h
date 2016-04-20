#pragma once
#pragma warning(disable:4099)
//************************************************
//◇Author: CUCKOO0615
//◇Date: 2016/03/30
//◇Comment: 创建FTP连接
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
    ** 创建一个FTP连接对象
    ** @Param szIP: IP地址
    ** @Param usPort: 端口号,默认21
    ** @Param szUserName: 用户名,默认anonymous(匿名用户)
    ** @Param szPassword: 默认为空
	** @Param bEnableUtf8: 如果服务器端使用UTF8编码,则该参数应置为true
    ** @Param bPassive: 主动/被动模式，默认使用主动模式
    ** @Ret : 创建成功返回true,失败返回false
    */
    bool CreateFtpConnection
        (LPCSTR szIP, USHORT usPort = 21,
        LPCSTR szUserName = "anonymous", LPCSTR szPassword = "",
        bool bEnableUtf8 = false,
        BOOL bPassive = FALSE);

	// 获取由CreateFtpConnection创建的FTP连接对象指针
	CFtpConnection* GetConnection() const;

    /*
    ** 获取当前会话用户当前目录
    ** @Ret: 操作成功返回服务器端当前目录,操作失败返回"ERROR"
    */
	LPCSTR GetFtpCurrentDir();

    /*
    ** 设置当前会话用户当前目录
    ** @Param strDirPath: 相对路径 
    ** @Ret : 成功返回true,失败返回false
    */
    bool SetFtpCurrentDir(LPCSTR szDirPath);

	/*
	** 服务器文件操作
	** @Param szRemotePath: 远程路径,设为NULL或""时默认为"/"
	** @Param szFileName: 文件名
	** @Ret : 操作成功返回true，失败返回false
	*/
	bool FtpRemoveFile(LPCSTR szRemoteDirPath, LPCSTR szFileName);

	/*
	** 下载文件
	** @Param szRemoteFilePath: 远程文件路径
	** @Param szLocalFilePath: 本地文件路径
	** @Ret: 操作成功返回true，失败返回false
	*/
	bool FtpDownloadFile(
		LPCSTR szRemoteFilePath, 
		LPCSTR szLocalFilePath/*, 
		BOOL bFailIfExist = FALSE, 
		DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL,
		DWORD dwFlags = FTP_TRANSFER_TYPE_BINARY,
		DWORD_PTR dwContext = 1*/);

    /*
    ** 上传文件
    ** @Param szLocalFilePath: 本地文件路径
    ** @Param szRemoteFilePath: 远程文件路径
    ** @Ret: 操作成功返回true，失败返回false
    */
	bool FtpUploadFile(LPCSTR szLocalFilePath, LPCSTR szRemoteFilePath);
	        
	/*
	** 获取远程目录中的文件信息
	** @Param szRemoteDir: 远程目录
    ** @Param szFileName: 文件名，可以使用通配符，默认NULL时使用*.*
	** @Param vecFileInfos: 文件信息集合
	** @Ret : 操作成功返回true，失败返回false
	*/
    bool FtpGetFileInfosInDir(LPCSTR szRemoteDir, LPCSTR szFileName, std::vector<FtpFileInfo>& vecFileInfos);
    
    //获取当前会话用户根目录,以'/'结尾
	LPCSTR GetFtpRootDir() const;

	// 获取错误信息
	LPCSTR GetLastErrMsg() const;
	
public:
    /*
    ** FtpConnector的构造函数
    ** @Param szSessionName: CInternetSession会话名称
    ** @Param dwDelay: 连接超时时间,单位:毫秒,默认5000
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

	void SafeCloseConnection();	//安全关闭m_pConnection并赋值为NULL
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

