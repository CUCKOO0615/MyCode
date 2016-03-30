#pragma once
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
    ** @Ret : 创建成功返回true,失败返回false,错误信息见日志文件
    */
    bool CreateFtpConnection(
        const char* szIP, unsigned short usPort = 21, 
        const char* szUserName = "anonymous", const char* szPassword="");
    
    /*
    ** 获取由CreateFtpConnection创建的FTP连接对象指针
    */
    CFtpConnection* GetConnection();

    /*
    ** 设置服务器当前目录
    ** @Param strDirPath: 相对路径 
    ** @Ret : 成功返回true,失败返回false,错误信息见日志文件
    */
    bool SetCurrentDir(CString strDirPath);

    
public:
    /*
    ** FtpConnector的构造函数
    ** @Param szSessionName: CInternetSession会话名称
    ** @Param dwDelay: 连接超时时间,单位:毫秒,默认5000
    */
    FtpConnector(const TCHAR* szSessionName, DWORD dwDelay = 5000);
    ~FtpConnector();

private:
    CInternetSession m_objSession;
    CFtpConnection* m_pConnection;
};

//////////////////////////////////////////////////////////////////////////
//inline

inline CFtpConnection* FtpConnector::GetConnection()
{
    return m_pConnection;
}
