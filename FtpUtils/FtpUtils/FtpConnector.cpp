#include "StdAfx.h"

#pragma warning(disable:4996)
#include "FtpConnector.h"
#include "CStringUtils.h"
#include "StringUtils.h"
#include "PtrUtils.hpp"

#define SET_LAST_ERRMSG(szFormat,...) ::sprintf(m_szLastErrMsg, szFormat, __VA_ARGS__)
#define RESET_ERRMSG                  m_szLastErrMsg[0]='\0';

FtpConnector::FtpConnector(LPCSTR szSessionName, DWORD dwDelay)
:m_objSession(szSessionName), m_pConnection(NULL), m_bEnableUtf8(false)
{
	RESET_ERRMSG;
	m_objSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, dwDelay);
	m_objSession.SetOption(INTERNET_OPTION_SEND_TIMEOUT, dwDelay);
	m_objSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwDelay);
}

FtpConnector::~FtpConnector()
{
	SafeCloseConnection();
    m_objSession.Close();
}

bool FtpConnector::CreateFtpConnection(LPCSTR szIP, USHORT usPort, LPCSTR szUserName, LPCSTR szPassword, bool bEnableUtf8)
{
	RESET_ERRMSG;
	try
	{	//该函数操作失败会直接抛异常,不需要检查返回值
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
		strFtpRootDir += '/';
		strFtpRootDir.Replace('\\', '/');
		strFtpRootDir.Replace("//", "/");
		m_strRootDir = strFtpRootDir.GetBuffer(0);
		return true;
	}
	catch (CInternetException *pEx)
    {
		m_bEnableUtf8 = false;
		SafeCloseConnection();
		InternetExceptionErrorOccured(pEx);       
        return false;
    }
}

LPCSTR FtpConnector::GetFtpCurrentDir()
{
	RESET_ERRMSG;
	LPCTSTR szDefaultRetStr = "ERROR";

    if (!m_pConnection)
    {
        SET_LAST_ERRMSG("m_pConnection is NULL");
		return szDefaultRetStr;
    }

    CString strCurDir;
    try
    {
        if (!m_pConnection->GetCurrentDirectory(strCurDir))
        {
			DWORD dwErrCode = ::GetLastError();
			DWORD dwErrorSize = 256, dwLastErrorMsg;
			CHAR szError[256] = { 0 };
			if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)		
				InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
			SET_LAST_ERRMSG("Get current directory failed, %s, err code: %d", szError, dwErrCode);
			return szDefaultRetStr;
        }    
    }
    catch (CInternetException *pEx)
    {
		InternetExceptionErrorOccured(pEx);
		return szDefaultRetStr;
    }

	if (m_bEnableUtf8)
	{
		char* pErr = NULL;
		char* szAnsi = StringUtils::StrConv_Utf82A(strCurDir.GetBuffer(0), pErr);
		CkPtrUtils::PtrScopeGuard<char> psg(&szAnsi, true);
		if (!szAnsi)
		{
			SET_LAST_ERRMSG("Convert UTF8 to ANSI failed, err msg: %s", pErr);
			return szDefaultRetStr;
		}
		m_strCurrentDir = szAnsi;
	}
	else
		m_strCurrentDir = strCurDir.GetBuffer(0);
	return m_strCurrentDir.c_str();
}

bool FtpConnector::SetFtpCurrentDir(LPCSTR szDirPath)
{
	RESET_ERRMSG;
    if (!m_pConnection)
    {
		SET_LAST_ERRMSG("m_pConnection is NULL");
        return false;
    }
    if (!szDirPath) szDirPath = "/";

	CString strDirPath(m_strRootDir.c_str());
	strDirPath += szDirPath;
	strDirPath.Replace('\\', '/');
	strDirPath.Replace("//", "/");

    if (m_bEnableUtf8)
    {
        char* pErr = NULL;
        char* szUtf8 = StringUtils::StrConv_A2Utf8(szDirPath, pErr);
		CkPtrUtils::PtrScopeGuard<char> psg(&szUtf8, true);
        if (!szUtf8)
        {
            SET_LAST_ERRMSG("Convert ANSI to UTF8 failed, err msg: %s", pErr);
            return false;
        }
        strDirPath = szUtf8;
    }

    try
	{
		if (m_pConnection->SetCurrentDirectory(strDirPath))
			return true;

		DWORD dwErrCode = ::GetLastError();
		DWORD dwErrorSize = 256, dwLastErrorMsg;
		CHAR szError[256] = { 0 };
		if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
			InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
		SET_LAST_ERRMSG("Set current directory failed, %s, err code: %d", szError, dwErrCode);
	}
	catch (CInternetException *pEx)
	{
		InternetExceptionErrorOccured(pEx);
	}
	return false;
}

bool FtpConnector::FtpRemoveFile(LPCSTR szRemoteDirPath, LPCSTR szFileName)
{
	RESET_ERRMSG;
	if (!m_pConnection)
	{
		SET_LAST_ERRMSG("m_pConnection is NULL");
		return false;
	}
	if (!szFileName || !strlen(szFileName))
	{
		SET_LAST_ERRMSG("File name is NULL or empty");
		return false;
	}
	if (!szRemoteDirPath || !strlen(szRemoteDirPath))
		szRemoteDirPath = "/";

	CString strRemoteFilePath = CString(m_strRootDir.c_str()) + szRemoteDirPath + "/" + szFileName;
	strRemoteFilePath.Replace('\\', '/');
	strRemoteFilePath.Replace("//", "/");
	
	if (m_bEnableUtf8)
	{
		char* pszErrMsg = NULL;
		char* szUtf8RemoteFilePath = StringUtils::StrConv_T2UTF8(strRemoteFilePath, pszErrMsg);
		CkPtrUtils::PtrScopeGuard<char> psg(&szUtf8RemoteFilePath, true);
		if (!szUtf8RemoteFilePath)
		{
			SET_LAST_ERRMSG("Convert remote file path to UTF8 failed, err msg: %s", pszErrMsg);
			return false;
		}
		strRemoteFilePath = szUtf8RemoteFilePath;
	}

	try
	{
		if (m_pConnection->Remove(strRemoteFilePath))
			return true;

		DWORD dwErrCode = ::GetLastError();
		DWORD dwErrorSize = 256, dwLastErrorMsg;
		CHAR szError[256] = { 0 };
		if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
			InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
		SET_LAST_ERRMSG("Delete specified file failed, file: %s, err msg: %s, err code: %d",
			szFileName, szError, dwErrCode);
	}
	catch (CInternetException* pEx)
	{
		InternetExceptionErrorOccured(pEx);
	}
	return false;
}

bool FtpConnector::FtpDownloadFile(LPCSTR szRemoteDirPath, LPCSTR szFileName)
{

	return true;
}

bool FtpConnector::FtpUploadFile(LPCSTR szRemoteDirPath, LPCSTR szFileName)
{

	return true;
}


void FtpConnector::InternetExceptionErrorOccured(CInternetException* pEx)
{
	char szError[255] = { 0 };
	if (!(pEx->GetErrorMessage(szError, 255)))
		::strcpy(szError, "FTP unknown exception");
	pEx->Delete();
	SET_LAST_ERRMSG(szError);
}

