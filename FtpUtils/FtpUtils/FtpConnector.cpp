#include "StdAfx.h"

#pragma warning(disable:4996)
#include "FtpConnector.h"
#include "CStringUtils.h"
#include "StringUtils.h"
#include "PtrUtils.hpp"

FtpConnector::FtpConnector(LPCSTR szSessionName, DWORD dwDelay)
:m_objSession(szSessionName), m_pConnection(NULL), m_bEnableUtf8(false)
{
    SetLastErrMsg();
    m_objSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_SEND_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwDelay);
	::memset(m_szCurrentDir, 0, CURRENTDIR_LENGTH);
}

FtpConnector::~FtpConnector()
{
	SafeCloseConnection();
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

		m_bEnableUtf8 = bEnableUtf8; //GetCurrentDirectory()正确执行的前提
		CString strFtpRootDir = GetFtpCurrentDir();
		if ("ERROR" == strFtpRootDir)
		{
			m_bEnableUtf8 = false;
			SafeCloseConnection();
			return false;
		}
		strFtpRootDir.Replace('\\', '/');
		strFtpRootDir += '/';
		strFtpRootDir.Replace("//", "/");

		m_strRootDir = strFtpRootDir;
		return true;
	}
	catch (CInternetException *pEx)
    {
		m_bEnableUtf8 = false;
		SafeCloseConnection();

        char szError[255] = { 0 };
		if (pEx->GetErrorMessage(szError, 255))
			SetLastErrMsg(szError);
        else
			SetLastErrMsg("FTP unknown exception");
        pEx->Delete();
        return false;
    }
}

LPCSTR FtpConnector::GetFtpCurrentDir()
{
    SetLastErrMsg();
	::strcpy(m_szCurrentDir, "ERROR");
    if (!m_pConnection)
    {
        SetLastErrMsg("m_pConnection is NULL");
		return m_szCurrentDir;
    }

    CString strCurDir;
    try
    {
        if (!m_pConnection->GetCurrentDirectory(strCurDir))
        {
			DWORD dwErrCode = ::GetLastError();
			TCHAR szError[256] = { 0 };
			if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
			{
				DWORD dwLastErrorMsg;
				DWORD dwErrorSize = 256;
				InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
			}
			SetLastErrMsg("Get current directory failed, %s, err code: %d", szError, dwErrCode);
			return m_szCurrentDir;
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
		return m_szCurrentDir;
    }

	if (m_bEnableUtf8)
	{
		char* pErr = NULL;
		char* szAnsi = StringUtils::StrConv_Utf82A(strCurDir.GetBuffer(0), pErr);
		if (!szAnsi)
		{
			SetLastErrMsg("Convert UTF8 to ANSI failed, err msg: %s", pErr);
			return m_szCurrentDir;
		}
		::strcpy(m_szCurrentDir, szAnsi);
		delete[] szAnsi;
	}
	else
		::strcpy(m_szCurrentDir, strCurDir.GetBuffer(0));
	return m_szCurrentDir;
}

bool FtpConnector::SetFtpCurrentDir(LPCSTR szDirPath)
{
    SetLastErrMsg();
    if (!m_pConnection)
    {
		SetLastErrMsg("m_pConnection is NULL");
        return false;
    }
    if (!szDirPath) szDirPath = "/";

	CString strDirPath(szDirPath);
	strDirPath = m_strRootDir + strDirPath;
	strDirPath.Replace('\\', '/');
	strDirPath.Replace("//", "/");

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

    try
    {
        if (!m_pConnection->SetCurrentDirectory(strDirPath))
        {
            DWORD dwErrCode = ::GetLastError();
            TCHAR szError[256] = { 0 };
            if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
            {
                DWORD dwLastErrorMsg;
                DWORD dwErrorSize = 256;
                InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
            }
            SetLastErrMsg("Set current directory failed, %s, err code: %d", szError, dwErrCode);
            return false;
        }
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

bool FtpConnector::FtpRemoveFile(LPCSTR szRemoteDirPath, LPCSTR szFileName)
{
	SetLastErrMsg();
	if (!m_pConnection)
	{
		SetLastErrMsg("m_pConnection is NULL");
		return false;
	}
	if (!szFileName || !strlen(szFileName))
	{
		SetLastErrMsg("File name is NULL or empty");
		return false;
	}

	if (!szRemoteDirPath || !strlen(szRemoteDirPath))
		szRemoteDirPath = "/";

	CString strRemoteFilePath = m_strRootDir + szRemoteDirPath + "/" + szFileName;
	strRemoteFilePath.Replace('\\', '/');
	strRemoteFilePath.Replace("//", "/");
	
	if (m_bEnableUtf8)
	{
		char* pszErrMsg = NULL;
		char* szUtf8RemoteFilePath = StringUtils::StrConv_T2UTF8(strRemoteFilePath, pszErrMsg);
		if (!szUtf8RemoteFilePath)
		{
			SetLastErrMsg("Convert remote file path to UTF8 failed, err msg: %s", pszErrMsg);
			return false;
		}
		strRemoteFilePath = szUtf8RemoteFilePath;
		delete[] szUtf8RemoteFilePath;
	}

	if (0 == m_pConnection->Remove(strRemoteFilePath))
	{
		DWORD dwErrCode = ::GetLastError();
		SetLastErrMsg("Delete specified file failed, file: %s, errcode: %d", szFileName, dwErrCode);

		TCHAR szError[256] = { 0 };
		DWORD dwErrorSize = 256;
		if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
		{
			DWORD dwLastErrorMsg;
			InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
			SetLastErrMsg("Delete specified file failed, err msg: %s", szError);
		}
		return false;
	}
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



