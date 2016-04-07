#pragma once
#pragma warning(disable:4099)
//************************************************
//◇Author: CUCKOO0615
//◇Date: 2016/03/30
//◇Comment: 创建FTP连接
//*************************************************
#include <afxinet.h>

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
    ** @Ret : 创建成功返回true,失败返回false
    */
    bool CreateFtpConnection
        (LPCSTR szIP, USHORT usPort = 21,
        LPCSTR szUserName = "anonymous", LPCSTR szPassword = "",
        bool bEnableUtf8 = false);

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
	** 删除服务器上的文件
	** @Param szRemotePath: 远程路径,设为NULL或""时默认为"/"
	** @Param szFileName: 文件名
	** @Ret : 操作成功返回true，失败返回false
	*/
	bool FtpRemoveFile(LPCSTR szRemoteDirPath, LPCSTR szFileName);
	    
    // 获取由CreateFtpConnection创建的FTP连接对象指针
    CFtpConnection* GetConnection() const;
    
    //获取当前会话用户根目录,以'/'结尾
	CString GetFtpRootDir() const;

	// 获取错误信息
	const char* GetLastErrMsg() const;

public:
    /*
    ** FtpConnector的构造函数
    ** @Param szSessionName: CInternetSession会话名称
    ** @Param dwDelay: 连接超时时间,单位:毫秒,默认5000
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

	//安全关闭m_pConnection并赋值为NULL
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
