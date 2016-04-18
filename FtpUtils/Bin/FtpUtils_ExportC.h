#pragma once
//************************************************
//◇Author: CUCKOO0615
//◇Date: 2016/03/30
//◇Comment: 创建FTP连接(C导出接口)
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
    ** 创建FtpConnector对象
    ** @Param szSessionName: 会话名称
    ** @Param nDelay: 连接超时时间, 单位:毫秒
    ** @Ret : 创建失败返回NULL
    */
    CK_API FtpConnector* CreateFtpConnector(const char* szSessionName, int nDelay = 5000);
    
    /*
    ** 释放FtpConnector对象
    ** @Param pFtpConn: FtpConnector对象指针
    */
    CK_API void ReleaseFtpConnector(FtpConnector* pFtpConn);

	/*
	** 获取错误信息
	** @Param pConnector: 有效的FtpConnector对象
	** @Ret: 包含错误信息的字符串
	*/
	CK_API const char* GetLastErrMsg(FtpConnector* pConnector);
	
    /*
    ** 连接到指定的FTP服务器
    ** @Param pConnector: 有效的FtpConnector对象
    ** @Param szIP: 服务器IP
    ** @Param usPort: 服务器端口,设为NULL时默认21
    ** @Param szUserName: 用户名,设为NULL时默认anonymous
    ** @Param szPassword: 密码
	** @Param bEnableUtf8: 如果服务器端使用UTF8编码,则该参数应置为true
    ** @Param bPassive: 主动/被动模式，默认使用主动模式
    ** @Ret : 连接成功返回true,失败返回false
    */
    CK_API bool ConnectFtp(FtpConnector* pConnector, 
		const char* szIP, unsigned short usPort = 21,
        const char* szUserName = "anonymous", const char* szPassword = "",
		bool bEnableUtf8 = false,
        bool bPassive = false);
    
	/*
	** 设置当前会话用户当前目录
	** @Param pConnector: 有效的FtpConnector对象
	** @Param strDirPath: 相对路径
	** @Ret : 成功返回true,失败返回false
	*/
	CK_API bool SetFtpCurrentDir(FtpConnector* pConnector, const char* szDirPath);
	
	/*
	** 获取当前会话用户当前目录
	** @Param pConnector: 有效的FtpConnector对象
	** @Ret: 操作成功返回服务器端当前目录,操作失败返回"ERROR"
	*/
	CK_API const char* GetFtpCurrentDir(FtpConnector* pConnector);

	/*
	** 获取当前会话用户根目录
	** @Param pConnector: 有效的FtpConnector对象
	** @Ret: 操作成功返回服务器端根目录,,操作失败返回"ERROR"
	*/
	CK_API const char* GetFtpRootDir(FtpConnector* pConnector);

    /*
    ** 删除服务器上的文件
    ** @Param pConnector: 有效的FtpConnector对象
    ** @Param szRemotePath: 远程路径,设为NULL或""时默认为"/"
    ** @Param szFileName: 文件名
    ** @Ret : 操作成功返回true，失败返回false
    */
    CK_API bool FtpRemoveFile(FtpConnector* pConnector, const char* szRemotePath, const char* szFileName);


	/*
	** 下载文件
	** @Param pConnector: 有效的FtpConnector对象
	** @Param szRemoteFilePath: 远程文件路径
	** @Param szLocalFilePath: 本地文件路径
    ** @Ret: 操作成功返回true，失败返回false
    */
    CK_API bool FtpDownloadFile(FtpConnector* pConnector, 
        const char* szRemoteFilePath, const char* szLocalFilePath);

    /*
    ** 上传文件
    ** @Param pConnector: 有效的FtpConnector对象
    ** @Param szLocalFilePath: 本地文件路径
    ** @Param szRemoteFilePath: 远程文件路径
    ** @Ret: 操作成功返回true，失败返回false
    */
    CK_API bool FtpUploadFile(FtpConnector* pConnector,
        const char* szLocalFilePath, const char* szRemoteFilePath);
	
    /*
    ** 获取远程目录中的文件信息
    ** @Param pConnector: 有效的FtpConnector对象
    ** @Param szRemoteDir: 远程目录
    ** @Param szFileName: 文件名，可以使用通配符，默认NULL时使用*.*
    ** @Param arrFileInfos: 指向文件信息数组的指针，
    ** 函数操作成功后，需要调用ReleaseFileInfos手动释放，否则会产生内存泄漏
    ** @Ret : 操作成功返回true，失败返回false
    */
	CK_API bool FtpGetFileInfosInDir(FtpConnector* pConnector, 
		const char* szRemoteDir, const char* szFileName, FtpFileInfo** arrFileInfos, int* nArrSize);

    // 释放FtpFileInfo数组指针
    CK_API void ReleaseFileInfos(FtpFileInfo** pArrFileInfos);

}

