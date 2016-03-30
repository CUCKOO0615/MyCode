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
    ** 创建FtpConnector对象
    ** @Param szSessionName: 会话名称
    ** @Param nDelay: 连接超时时间, 单位:毫秒
    ** @Ret : 创建失败返回NULL
    */
    CK_API FtpConnector* CreateFtpConnector(const TCHAR* szSessionName, int nDelay = 5000);
    
    /*
    ** 释放FtpConnector对象
    ** @Param pFtpConn: FtpConnector对象指针
    */
    CK_API void ReleaseFtpConnector(FtpConnector* pFtpConn);

    /*
    ** 连接到指定的FTP服务器
    ** @Param pConnector: 有效的FtpConnector对象
    ** @Param szIP: 服务器IP
    ** @Param usPort: 服务器端口,设为NULL时默认21
    ** @Param szUserName: 用户名,设为NULL时默认anonymous
    ** @Param szPassword: 密码
    ** @Ret : 连接成功返回true,失败返回false,错误信息见日志
    */
    CK_API bool ConnectFtp(FtpConnector* pConnector, const char* szIP, unsigned short usPort = 21,
        const char* szUserName = "anonymous", const char* szPassword = "");
    
    /*
    ** 测试FTP服务器指定目录是否存在
    ** @Param pConnector: 有效的FtpConnector对象
    ** @Param szRemotePath: 远程路径,设为NULL或""时默认为"/"
    ** @Param bUtf8Path: 是否启用UTF8编码发送字节流
    ** @Ret : 参见以下返回值
    // 0 Successful
    // 1 FtpConnector is NULL
    // 2 Get FTP Server's current dir failed
    // 3 UTF8 Convert failed
    // 4 Set FTP Server's current dir failed
    */
    CK_API int FtpCheckDirExist(FtpConnector* pConnector,  const char* szRemotePath, bool bUtf8Path);
    
    /*
    ** 删除服务器上的文件
    ** @Param pConnector: 有效的FtpConnector对象
    ** @Param szRemotePath: 远程路径,设为NULL或""时默认为"/"
    ** @Param szFileName: 文件名
    ** @Param bUtf8Path: 是否启用UTF8编码发送字节流
    ** @Ret : 
    // 0 Successful
    // 1 FtpConnector is NULL
    // 2 Get FTP Server's current dir failed
    // 3 UTF8 Convert failed
    // 4 Set FTP Server's current dir failed
    // 5 Remove file failed
    */
    CK_API int FtpRemoveFile(FtpConnector* pConnector, 
        const char* szRemotePath, const char* szFileName, bool bUtf8Path);
}