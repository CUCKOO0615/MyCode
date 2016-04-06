#include "StdAfx.h"

#pragma warning(disable:4996)
#include "FtpConnector.h"
#include "CStringUtils.h"
#include "StringUtils.h"
//#include "LogUtils.hpp"

FtpConnector::FtpConnector(const TCHAR* szSessionName, DWORD dwDelay)
:m_objSession(szSessionName), m_pConnection(NULL)
{
	::memset(m_szLastErrMsg, 0, LASTERRMSG_LENGTH);
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
    CString strIP = CStringUtils::StrConv_cstr2CStringT(szIP);
    CString strUserName = CStringUtils::StrConv_cstr2CStringT(szUserName);
    CString strPassword = CStringUtils::StrConv_cstr2CStringT(szPassword);

    //连接FTP服务器
    try
    {
        m_pConnection = m_objSession.GetFtpConnection(
            strIP, strUserName, strPassword, (INTERNET_PORT)usPort,
            FALSE); //该函数操作失败会直接抛异常,不需要检查返回值

        if (!m_pConnection->GetCurrentDirectory(m_strFtpRootDir))
        {
            m_pConnection->Close();
            m_pConnection = NULL;
			SetLastErrMsg("Get FTP current dir failed, err code:%d", ::GetLastError());
            return false;
        }
#ifdef UNICODE
        //UNICODE下该字符串末尾会有两个L'\0',导致长度检测+1,影响字符串拼加
        m_strFtpRootDir.Delete(m_strFtpRootDir.GetLength() - 1, 1);
#endif // UNICODE
        if (_T('/') != m_strFtpRootDir[m_strFtpRootDir.GetLength() - 1] &&
            _T('\\') != m_strFtpRootDir[m_strFtpRootDir.GetLength() - 1])
            m_strFtpRootDir += _T("/");
        m_strFtpCurrentDir = m_strFtpRootDir;
        return true;
    }
    catch (CInternetException *pEx)
    {
        TCHAR szError[255] = { 0 };
		if (pEx->GetErrorMessage(szError, 255))
			SetLastErrMsg(szError);
        else
			SetLastErrMsg("FTP unknown exception");
        pEx->Delete();
        return false;
    }
}

bool FtpConnector::SetCurrentDir(CString strDirPath)
{
    if (!m_pConnection)
    {
		SetLastErrMsg("m_pConnection is NULL");
        return false;
    }

    BOOL bRet = m_pConnection->SetCurrentDirectory(strDirPath);
    if (0 == bRet)
    {
        DWORD dwErrCode = ::GetLastError();
        TCHAR szError[256] = { 0 };
		DWORD dwErrorSize = 256;
        if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
        {
            DWORD dwLastErrorMsg;
            InternetGetLastResponseInfo(&dwLastErrorMsg, szError, &dwErrorSize);
        }
		SetLastErrMsg(_T("Set current dir failed, err code: %d, err msg: %s"), dwErrCode, szError);
        return false;
    }
    m_strFtpCurrentDir = strDirPath;
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

void FtpConnector::SetLastErrMsg(const wchar_t* wszFormat /*= L"OK"*/, ...)
{
	wchar_t wcBuff[1024] = { 0 };
	va_list argList;
	va_start(argList, wszFormat);	
	::vswprintf(wcBuff, wszFormat, argList);
	va_end(argList);

	char* pszErrMsg = NULL;
	char* pBuff = StringUtils::StrConv_W2A(wszFormat, pszErrMsg);
	SetLastErrMsg(pBuff);
	delete[] pBuff;
}


