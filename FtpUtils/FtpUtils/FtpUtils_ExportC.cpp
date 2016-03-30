#include "stdafx.h"
#define  EXPORT_STDC
#include "FtpUtils_ExportC.h"
#include "LogUtils.hpp"
#include "StringUtils.h"
#include "CStringUtils.h"
#include "FtpConnector.h"

CK_API FtpConnector* CkFtpUtils::CreateFtpConnector(const TCHAR* szSessionName, int nDelay /*= 5000*/)
{
    FtpConnector* pRet = new FtpConnector(szSessionName, nDelay);
    return pRet;
}

CK_API void CkFtpUtils::ReleaseFtpConnector(FtpConnector* pFtpConn)
{
    if(pFtpConn)
        delete pFtpConn;
}

CK_API bool CkFtpUtils::ConnectFtp(FtpConnector* pConnector, const char* szIP, unsigned short usPort /*= 21*/, const char* szUserName /*= "anonymous"*/, const char* szPassword /*= ""*/)
{
    if (!pConnector)
    {
        WRITE_LOG_ERROR("Param FtpConnector* is null, func: ConnectFtp");
        return false;
    }
    return pConnector->CreateFtpConnection(szIP, usPort, szUserName, szPassword);
}

CK_API int CkFtpUtils::FtpCheckDirExist(FtpConnector* pConnector, const char* szRemotePath, bool bUtf8Path)
{
    if (!pConnector)
        return 1;

    if (!szRemotePath || !strlen(szRemotePath))
        szRemotePath = "/";
    CString strRemotePath = CStringUtils::StrConv_cstr2CStringT(szRemotePath);
    if (_T('/') != strRemotePath[0] && _T('\\') != strRemotePath[0])
        strRemotePath = _T("/") + strRemotePath;
    
    //获取服务器当前路径
    CString strFtpCurDir;
    if (!pConnector->GetConnection()->GetCurrentDirectory(strFtpCurDir))
    {
        WRITE_LOG_ERROR("Get FTP current dir failed, err code:%d", ::GetLastError());
        return 2;
    }
#ifdef UNICODE
    //UNICODE下该字符串末尾会有两个L'\0',长度检测+1,影响字符串拼加
    strFtpCurDir.Delete(strFtpCurDir.GetLength()-1,1); 
#endif // UNICODE

    //处理管理员未锁定用户根目录的情况
    if (_T("\\") != strFtpCurDir && _T("/") != strFtpCurDir)
        strRemotePath = (strFtpCurDir + strRemotePath);
    CString a = CString(L"/1") + CString(L"/梁中原");
    strRemotePath.Replace(_T("//"), _T("/"));
    strRemotePath.Replace(_T("\\\\"), _T("\\"));

    //UTF8转码
    if (bUtf8Path)
    {
        char* pszErrMsg = NULL;
        char* szUtf8 = StringUtils::StrConv_T2UTF8(strRemotePath, pszErrMsg);
        if (!szUtf8)
        {
            WRITE_LOG_ERROR("Convert to UTF8 failed, err msg: %s", pszErrMsg);
            return 3;
        }
        strRemotePath = szUtf8;
        delete[] szUtf8;
    }

    //测试服务器路径
    if (!pConnector->SetCurrentDir(strRemotePath))
        return 4;
    return 0;
}

CK_API int CkFtpUtils::FtpRemoveFile(FtpConnector* pConnector,
    const char* szRemotePath, const char* szFileName, bool bUtf8Path)
{
    if (!szRemotePath || !strlen(szRemotePath))
        szRemotePath = "/";
    CString strRemotePath = CStringUtils::StrConv_cstr2CStringT(szRemotePath);
    if (_T('/') != strRemotePath[0] && _T('\\') != strRemotePath[0])
        strRemotePath = _T("/") + strRemotePath;
    if (!szFileName)
        szFileName = "";
    CString strFileName = CStringUtils::StrConv_cstr2CStringT(szFileName);

    if (!pConnector)
        return 1;

    //获取服务器当前路径
    CString strFtpCurDir;
    if (!pConnector->GetConnection()->GetCurrentDirectory(strFtpCurDir))
    {
        WRITE_LOG_ERROR("Get FTP current dir failed, err code:%d", ::GetLastError());
        return 2;
    }
#ifdef UNICODE
    //UNICODE下该字符串末尾会有两个L'\0',长度检测+1,影响字符串拼加
    strFtpCurDir.Delete(strFtpCurDir.GetLength() - 1, 1);
#endif // UNICODE

    //处理管理员未锁定用户根目录的情况
    if (_T("\\") != strFtpCurDir && _T("/") != strFtpCurDir)
        strRemotePath = strFtpCurDir + strRemotePath;
    strRemotePath.Replace(_T("//"), _T("/"));
    strRemotePath.Replace(_T("\\\\"), _T("\\"));

    //UTF8转码
    if (bUtf8Path)
    {
        char* pszErrMsg = NULL;
        char* szUtf8 = StringUtils::StrConv_T2UTF8(strRemotePath, pszErrMsg);
        if (!szUtf8)
        {
            WRITE_LOG_ERROR("Convert remotepath to UTF8 failed, err msg: %s", pszErrMsg);
            return 3;
        }
        strRemotePath = szUtf8;
        delete[] szUtf8;
    }

    //设置服务器路径
    if (!pConnector->SetCurrentDir(strRemotePath))
        return 4;

    if (bUtf8Path)
    {
        char* pszErrMsg = NULL;
        char* szUtf8 = StringUtils::StrConv_T2UTF8(strFileName, pszErrMsg);
        if (!szUtf8)
        {
            WRITE_LOG_ERROR("Convert filename to UTF8 failed, err msg: %s", pszErrMsg);
            return 3;
        }
        strFileName = szUtf8;
        delete[] szUtf8;
    }

    if (0 == pConnector->GetConnection()->Remove(strFileName))
    {
        DWORD dwErrCode = ::GetLastError();
        if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
        {
            TCHAR szError[256] = { 0 };
            DWORD dwErrorSize = 256;
            DWORD dwLastErrorMsg;
            InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
            CStringA strErrA = CStringUtils::StrConv_TStr2CStringA(szError);
            WRITE_LOG_ERROR(strErrA.GetBuffer(0));
        }
        WRITE_LOG_ERROR("Delete specified file failed, filename: %s, error code: %d", szFileName, dwErrCode);
        return 5;
    }
    return 0;
}

