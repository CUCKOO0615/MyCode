#include "StdAfx.h"

#pragma warning(disable:4996)
#include "FtpConnector.h"
#include "CStringUtils.h"
#include "StringUtils.h"

FtpConnector::FtpConnector(LPCSTR szSessionName, DWORD dwDelay)
:m_objSession(szSessionName), m_pConnection(NULL), m_bEnableUtf8(false)
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

bool FtpConnector::CreateFtpConnection(LPCSTR szIP, USHORT usPort, LPCSTR szUserName, LPCSTR szPassword, bool bEnableUtf8)
{
    SetLastErrMsg();
    try
    {
        //该函数操作失败会直接抛异常,不需要检查返回值
        m_pConnection = m_objSession.GetFtpConnection(
            szIP, szUserName, szPassword, (INTERNET_PORT)usPort, FALSE); 
        
        CString strFtpRootDir = FtpConnector::GetCurrentDirectory();
        if ("ERROR" == strFtpRootDir)
        {
            m_pConnection->Close();
            m_pConnection = NULL;
            return false;
        }



        if (!m_pConnection->GetCurrentDirectory(strFtpRootDir))
        {
            SetLastErrMsg("Get FTP current directory failed, err code: %d", ::GetLastError());
            m_pConnection->Close();
            m_pConnection = NULL;
            return false;
        }
        if (bEnableUtf8)
        {
            char* pErr = NULL;
            char* szAnsi = StringUtils::StrConv_Utf82A(strFtpRootDir.GetBuffer(0), pErr);
            if (!szAnsi)
            {
                SetLastErrMsg("Convert UTF8 to ANSI failed, err msg: %s", pErr);
                m_pConnection->Close();
                m_pConnection = NULL;
                return false;
            }
            m_strRootDir = szAnsi;
            delete[] szAnsi;
        }
        else
            m_strRootDir = strFtpRootDir;

        if ("/" == m_strRootDir || "\\" == m_strRootDir || m_strRootDir.IsEmpty())
            m_strRootDir = "/";
        else
        {
            int nLastIndex = m_strRootDir.GetLength() - 1;
            if ('/' != m_strRootDir[nLastIndex] && '\\' != m_strRootDir[nLastIndex])
                m_strRootDir += "/";
        }
        m_bEnableUtf8 = bEnableUtf8;
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
        if (m_pConnection)
            m_pConnection->Close();
        m_pConnection = NULL;
        return false;
    }
}

CString FtpConnector::GetCurrentDirectory()
{
    SetLastErrMsg();
    if (!m_pConnection)
    {
        SetLastErrMsg("m_pConnection is NULL");
        return "ERROR";
    }

    try
    {
        CString strCurDir;
        if (!m_pConnection->GetCurrentDirectory(strCurDir))
        {
            SetLastErrMsg("Get FTP current directory failed, err code: %d", ::GetLastError());
            return "ERROR";
        }
        if (m_bEnableUtf8)
        {
            char* pErr = NULL;
            char* szAnsi = StringUtils::StrConv_Utf82A(strCurDir.GetBuffer(0), pErr);
            if (!szAnsi)
            {
                SetLastErrMsg("Convert UTF8 to ANSI failed, err msg: %s", pErr);
                return "ERROR";
            }
            strCurDir.ReleaseBuffer();
            strCurDir = szAnsi;
            delete[] szAnsi;
        }
        return strCurDir;
    }
    catch (CInternetException *pEx)
    {
        char szError[255] = { 0 };
        if (pEx->GetErrorMessage(szError, 255))
            SetLastErrMsg(szError);
        else
            SetLastErrMsg("FTP unknown exception");
        pEx->Delete();
        return "ERROR";
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

    CString strDirPath;
    if (m_bEnableUtf8)
    {
        char* pErr = NULL;
        char* szUtf8 = StringUtils::StrConv_A2Utf8(szDirPath, pErr);
        if (!szUtf8)
        {
            SetLastErrMsg("Convert ANSI to UTF8 failed, err msg: %s", pErr);
            return false;
        }
        strDirPath = szUtf8;
        delete[] szUtf8;
    }
    else
        strDirPath = szDirPath;

    try
    {
        if (!m_pConnection->SetCurrentDirectory(strDirPath.GetBuffer(0)))
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
    m_pConnection->GetCurrentDirectory(m_strRootDir);
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


