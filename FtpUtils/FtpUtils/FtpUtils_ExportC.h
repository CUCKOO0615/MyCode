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
	** 下载文件（UTF8模式下，奇数中文路径依旧有问题）
	** @Param pConnector: 有效的FtpConnector对象
	** @Param szRemoteFilePath: 远程文件路径
	** @Param szLocalFilePath: 本地文件路径
	** @Param bFailIfExist: 置true-本地有同名文件则返回false，置false-覆盖本地文件
	** @Param ulAttributes: 参见MSDN FILE_ATTRIBUTE_NORMAL相关
	** @Param ulFlags: 参见MSDN FTP_TRANSFER_TYPE_BINARY相关
	** @Ret: 操作成功返回true，失败返回false
	*/
	CK_API bool FtpDownloadFile(
		FtpConnector* pConnector,
		const char* szRemoteFilePath,
		const char* szLocalFilePath,
		bool bFailIfExist = false,
		unsigned long ulAttributes = 0x00000080, //FILE_ATTRIBUTE_NORMAL
		unsigned long ulFlags = 0x00000002);     //FTP_TRANSFER_TYPE_BINARY
}

