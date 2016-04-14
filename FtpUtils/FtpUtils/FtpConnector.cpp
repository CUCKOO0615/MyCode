#include "StdAfx.h"

#pragma warning(disable:4996)
#include "FtpConnector.h"
#include "StringConvert.h"
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

bool FtpConnector::CreateFtpConnection(LPCSTR szIP, USHORT usPort, LPCSTR szUserName, LPCSTR szPassword, bool bEnableUtf8, BOOL bPassive)
{
	RESET_ERRMSG;
	try
	{	//该函数操作失败会直接抛异常,不需要检查返回值
		m_pConnection = m_objSession.GetFtpConnection(		
            szIP, szUserName, szPassword, (INTERNET_PORT)usPort, bPassive);
        
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
			std::string strErrMsg = GetFtpApiFailedErrMsg(dwErrCode);
			SET_LAST_ERRMSG("GetCurrentDirectory failed, %s, err code: %d", strErrMsg.c_str(), dwErrCode);
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
		if (!StringConvert::StrConv_Utf82A(strCurDir, pErr))
		{
			SET_LAST_ERRMSG("Convert UTF8 to ANSI failed, err msg: %s", pErr);
			return szDefaultRetStr;
		}
	}
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
		if (!StringConvert::StrConv_A2Utf8(strDirPath, pErr))
		{
			SET_LAST_ERRMSG("Convert ANSI to UTF8 failed, err msg: %s", pErr);
			return false;
		}
    }

    try
	{
		if (m_pConnection->SetCurrentDirectory(strDirPath))
			return true;

		DWORD dwErrCode = ::GetLastError();
		std::string strErrMsg = GetFtpApiFailedErrMsg(dwErrCode);
		SET_LAST_ERRMSG("SetCurrentDirectory failed, %s, err code: %d", strErrMsg.c_str(), dwErrCode);
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
		char* pErr = NULL;
		if (!StringConvert::StrConv_A2Utf8(strRemoteFilePath, pErr))
		{
			SET_LAST_ERRMSG("Convert remote file path to UTF8 failed, err msg: %s", pErr);
			return false;
		}
	}

	try
	{
		if (m_pConnection->Remove(strRemoteFilePath))
			return true;

		DWORD dwErrCode = ::GetLastError();
		std::string strErrMsg = GetFtpApiFailedErrMsg(dwErrCode);		
		SET_LAST_ERRMSG("Remove failed, file: %s, err msg: %s, err code: %d",
			szFileName, strErrMsg.c_str(), dwErrCode);
	}
	catch (CInternetException* pEx)
	{
		InternetExceptionErrorOccured(pEx);
	}
	return false;
}

bool FtpConnector::FtpDownloadFile(
	LPCSTR szRemoteFilePath, LPCSTR szLocalFilePath, 
	BOOL bFailIfExist, DWORD dwAttributes, DWORD dwFlags, DWORD_PTR dwContext)
{
	RESET_ERRMSG;
	if (!m_pConnection)
	{
		SET_LAST_ERRMSG("m_pConnection is NULL");
		return false;
	}
	if (!szLocalFilePath || !strlen(szLocalFilePath) ||	!szRemoteFilePath || !strlen(szRemoteFilePath))
	{
		SET_LAST_ERRMSG("File path is NULL or empty");
		return false;
	}

	CString strRemoteFilePath = CString(m_strRootDir.c_str()) + szRemoteFilePath;
	strRemoteFilePath.Replace('\\', '/');
	strRemoteFilePath.Replace("//", "/");

	if (m_bEnableUtf8)
	{
		char* pErr = NULL;
		if (!StringConvert::StrConv_A2Utf8(strRemoteFilePath, pErr))
		{
			SET_LAST_ERRMSG("Convert remote file path to UTF8 failed, err msg: %s", pErr);
			return false;
		}
	}
	
	try
	{
		if (m_pConnection->GetFile(
			strRemoteFilePath.GetBuffer(0), szLocalFilePath,
			bFailIfExist, dwAttributes, dwFlags, dwContext))
			return true;

		DWORD dwErrCode = ::GetLastError();
		std::string strErrMsg = GetFtpApiFailedErrMsg(dwErrCode);		
		SET_LAST_ERRMSG("Download failed, file: %s, err msg: %s, err code: %d", 
			szRemoteFilePath, strErrMsg.c_str(), dwErrCode);
	}
	catch (CInternetException *pEx)
	{
		InternetExceptionErrorOccured(pEx);
	}
	return false;
}

bool FtpConnector::FtpUploadFile(LPCSTR szLocalFilePath, LPCSTR szRemoteFilePath)
{
	RESET_ERRMSG;
	if (!m_pConnection)
	{
		SET_LAST_ERRMSG("m_pConnection is NULL");
		return false;
	}
	if (!szLocalFilePath || !strlen(szLocalFilePath) || 
		!szRemoteFilePath || !strlen(szRemoteFilePath))
	{
		SET_LAST_ERRMSG("File path is NULL or empty");
		return false;
	}

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

std::string FtpConnector::GetFtpApiFailedErrMsg(DWORD dwErrCode)
{
	DWORD dwErrorSize = 512, dwLastErrorMsg;
	char buff[512] = { 0 };
	if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
		InternetGetLastResponseInfo(&dwLastErrorMsg, buff, &dwErrorSize);

	std::string strRet(buff);
	if (m_bEnableUtf8)
	{
		char* pErr = NULL;
		char* szAnsi = StringUtils::StrConv_Utf82A(buff, pErr);
		strRet = szAnsi;
		delete[] szAnsi;
	}
	return strRet;
}

