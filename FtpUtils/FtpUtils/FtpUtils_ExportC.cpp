#include "stdafx.h"
#define  EXPORT_STDC
#include "FtpUtils_ExportC.h"
#include "LogUtils.hpp"
#include "StringUtils.h"
#include "CStringUtils.h"
#include "FtpConnector.h"
#include "PtrUtils.hpp"

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

CK_API int CkFtpUtils::FtpTry2SetCurDir(FtpConnector* pConnector, const char* szRemotePath, bool bUtf8Path)
{
    if (!pConnector)
        return 1;

    if (!szRemotePath || !strlen(szRemotePath))
        szRemotePath = "/";
    CString strRemotePath = CStringUtils::StrConv_cstr2CStringT(szRemotePath);
    if (_T('/') != strRemotePath[0] && _T('\\') != strRemotePath[0])
        strRemotePath = _T("/") + strRemotePath;
    
    //处理管理员未锁定用户根目录的情况
    CString strFtpRootDir = pConnector->GetFtpRootDir();
    if (_T("\\") != strFtpRootDir && _T("/") != strFtpRootDir)
        strRemotePath = (strFtpRootDir + strRemotePath);
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
            return 2;
        }
        strRemotePath = szUtf8;
        delete[] szUtf8;
    }

    //测试服务器路径
    if (!pConnector->SetCurrentDir(strRemotePath))
        return 3;
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

    //处理管理员未锁定用户根目录的情况
    CString strFtpRootDir = pConnector->GetFtpRootDir();
    if (_T("\\") != strFtpRootDir && _T("/") != strFtpRootDir)
        strRemotePath = strFtpRootDir + strRemotePath;
    strRemotePath.Replace(_T("//"), _T("/"));
    strRemotePath.Replace(_T("\\\\"), _T("\\"));

    //UTF8转码
    if (bUtf8Path)
    {
        char* pszErrMsg = NULL;
        char* szUtf8RemotePath = StringUtils::StrConv_T2UTF8(strRemotePath, pszErrMsg);
        CkPtrUtils::PtrScopeGuard<char> pag1(&szUtf8RemotePath, true);
        if (!szUtf8RemotePath)
        {
            WRITE_LOG_ERROR("Convert remotepath to UTF8 failed, err msg: %s", pszErrMsg);
            return 2;
        }

        char* szUtf8FileName = StringUtils::StrConv_T2UTF8(strFileName, pszErrMsg);
        CkPtrUtils::PtrScopeGuard<char> pag2(&szUtf8FileName, true);
        if (!szUtf8FileName)
        {
            WRITE_LOG_ERROR("Convert filename to UTF8 failed, err msg: %s", pszErrMsg);
            return 2;
        }
        strRemotePath = szUtf8RemotePath;
        strFileName = szUtf8FileName;
    }

    //设置服务器路径
    if (!pConnector->SetCurrentDir(strRemotePath))
        return 3;

    if (0 == pConnector->GetConnection()->Remove(strFileName))
    {
        DWORD dwErrCode = ::GetLastError();
        if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
        {
            TCHAR szError[256] = { 0 };
            DWORD dwLastErrorMsg, dwErrorSize = 256;
            InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
            WRITE_LOG_ERROR(szError);
        }
        WRITE_LOG_ERROR("Delete specified file failed, filename: %s, error code: %d", szFileName, dwErrCode);
        return 4;
    }
    return 0;
}

