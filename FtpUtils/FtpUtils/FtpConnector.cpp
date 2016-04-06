#include "StdAfx.h"

#pragma warning(disable:4996)
#include "FtpConnector.h"
#include "CStringUtils.h"
#include "StringUtils.h"
//#include "LogUtils.hpp"

FtpConnector::FtpConnector(const TCHAR* szSessionName, DWORD dwDelay)
:m_objSession(szSessionName), m_pConnection(NULL)
{
    SetLastErrMsg();
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


bool FtpConnector::CreateFtpConnection(LPCSTR szIP, USHORT usPort, LPCSTR szUserName, LPCSTR szPassword)
{
    SetLastErrMsg();
    try
    {
        m_pConnection = m_objSession.GetFtpConnection(
            szIP, szUserName, szPassword, (INTERNET_PORT)usPort,
            FALSE); //该函数操作失败会直接抛异常,不需要检查返回值

        if (!m_pConnection->GetCurrentDirectory(m_strFtpRootDir))
        {
            m_pConnection->Close();
            m_pConnection = NULL;
			SetLastErrMsg("Get FTP current dir failed, err code:%d", ::GetLastError());
            return false;
        }
        if ("/" == m_strFtpRootDir || "\\" == m_strFtpRootDir || m_strFtpRootDir.IsEmpty())
        {
            m_strFtpRootDir = "/";
            return true;
        }        
        int nLastIndex = m_strFtpRootDir.GetLength() - 1;
        if ('/' != m_strFtpRootDir[nLastIndex] && 
            '\\' != m_strFtpRootDir[nLastIndex])
            m_strFtpRootDir += "/";
        m_strFtpCurrentDir = m_strFtpRootDir;
        return true;
    }
    catch (CInternetException *pEx)
    {
        char szError[255] = { 0 };
		if (pEx->GetErrorMessage(szError, 255))
			SetLastErrMsg(szError);
        else
			SetLastErrMsg("FTP unknown exception");
        pEx->Delete();
        return false;
    }
}

bool FtpConnector::SetCurrentDirectory(LPCSTR szDirPath)
{
    SetLastErrMsg();
    if (!m_pConnection)
    {
		SetLastErrMsg("m_pConnection is NULL");
        return false;
    }
    if (!szDirPath)
        szDirPath = "/";

    if (!m_pConnection->SetCurrentDirectory(szDirPath))
    {
        DWORD dwErrCode = ::GetLastError();
        TCHAR szError[256] = { 0 };

        if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
        {
            DWORD dwLastErrorMsg;
		DWORD dwErrorSize = 256;
            InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
        }
		SetLastErrMsg("%s, err code: %d", szError, dwErrCode);
        return false;
    }
    m_strFtpCurrentDir = szDirPath;
    return true;
}

//////////////////////////////////////////////////////////////////////////
void FtpConnector::SetLastErrMsg(const char* szFormat, ...)
{
	va_list argList;
	va_start(argList, szFormat);
	::vsprintf(m_szLastErrMsg, szFormat, argList);
	va_end(argList);
}


