#include "StdAfx.h"
#include "FtpConnector.h"
#include "CStringUtils.h"
#include "LogUtils.hpp"


FtpConnector::FtpConnector(const TCHAR* szSessionName, DWORD dwDelay)
:m_objSession(szSessionName), m_pConnection(NULL)
{
    m_objSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_SEND_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwDelay);
}

FtpConnector::~FtpConnector()
{
    if (m_pConnection)
        m_pConnection->Close();
    m_pConnection = NULL;
    m_objSession.Close();
}

bool FtpConnector::CreateFtpConnection(
    const char* szIP, unsigned short usPort,
    const char* szUserName, const char* szPassword)
{
    if (!szIP || !strlen(szIP))             szIP = "127.0.0.1";
    if (!usPort)                            usPort = 21;
    if (!szUserName || !strlen(szUserName)) szUserName = "anonymous";
    if (!szPassword || !strlen(szPassword)) szPassword = "";

    CString strIP = CStringUtils::StrConv_cstr2CStringT(szIP);
    CString strUserName = CStringUtils::StrConv_cstr2CStringT(szUserName);
    CString strPassword = CStringUtils::StrConv_cstr2CStringT(szPassword);

    //连接FTP服务器
    try
    {
        m_pConnection = m_objSession.GetFtpConnection(
            strIP, strUserName, strPassword, (INTERNET_PORT)usPort,
            FALSE); //该函数操作失败会直接抛异常,不需要检查返回值
        return true;
    }
    catch (CInternetException *pEx)
    {
        TCHAR szError[255] = { 0 };
        CStringA strErrA;
        if (pEx->GetErrorMessage(szError, 255))
            strErrA = CStringUtils::StrConv_TStr2CStringA(szError);
        else
            strErrA = "FTP unknown exception";
        WRITE_LOG_ERROR(strErrA.GetBuffer(0));
        pEx->Delete();
        return false;
    }
}

bool FtpConnector::SetCurrentDir(CString strDirPath)
{
    if (!m_pConnection)
    {
        WRITE_LOG_ERROR("m_pConnection is NULL");
        return false;
    }

    BOOL bRet = m_pConnection->SetCurrentDirectory(strDirPath);
    if (0 == bRet)
    {
        DWORD dwErrCode = ::GetLastError();
        WRITE_LOG_ERROR("Set current dir failed, err code: %d", dwErrCode);
        if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
        {
            TCHAR szError[256] = { 0 };
            DWORD dwErrorSize = 256;
            DWORD dwLastErrorMsg;
            InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
            CStringA strErrA = CStringUtils::StrConv_TStr2CStringA(szError);
            WRITE_LOG_ERROR(strErrA.GetBuffer(0));
        }
        return false;
    }
    return true;
}
