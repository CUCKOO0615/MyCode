// LogUtils.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"
#include "LogUtils.h"
#include "Common.h"

#define LOGINDEX_MAX 99999999

LogUtils::LogUtils():
m_isInited(false),
m_pLogFile(NULL),
m_nFileMaxSize(0),
m_nLogIndex(0),
m_bSwitchFileByDate(true),
m_bthrUtilsFlag(true),
m_slSyncLock(2000)
{
}
LogUtils::~LogUtils()
{
	if (m_isInited)
	{
		m_bthrUtilsFlag = false;
		::WaitForSingleObject(m_thrUtils.GetThreadHandle(), 5000);
		::CloseHandle(m_thrUtils.GetThreadHandle());

		if (m_pLogFile)
			CkCommon::SafeCloseLogFile(m_pLogFile);
	}
}

bool LogUtils::InitLogUtils(
	const char* szDirPath, const char* szLogNamePrefix,
	bool bSwitchFileByDate, unsigned int nMaxFileSize)
{
	if (m_isInited) 
		return false;
	if (!CkCommon::CreateMultiDir(szDirPath) || 0 != ::_access(szDirPath, 0))
        return false;

    m_strLogPath = szDirPath;
	if ('\\' != *(m_strLogPath.end() - 1) && '/' != *(m_strLogPath.end() - 1))
		m_strLogPath += '\\';

	m_strLogPath += szLogNamePrefix;

	char szTime[256] = { 0 };
	CkCommon::FormatTime2Str(szTime, 256, "_%Y-%m-%d");
	m_strLogPath += szTime;
    
	if (nMaxFileSize)
	{
		m_nFileMaxSize = nMaxFileSize;
		m_strLogPath += "_00000000";
	}
	m_strLogPath += ".txt";

	m_bSwitchFileByDate = bSwitchFileByDate;

	m_thrUtils.SetThreadParams((void*)this);
	m_thrUtils.Run(SwitchLogFile_ThreadEntry);
	if (0xFFFFFFFF == m_thrUtils.GetExitCode())
		return false;

    m_isInited = true;
    return true;
}

void LogUtils::Recording(LOG_LEVEL emLL, const wchar_t* wszRec, ...)
{
    if (!m_isInited) return;
    va_list argsList;
    va_start(argsList, wszRec);
    Recording(emLL, wszRec, argsList);
    va_end(argsList);
}

void LogUtils::Recording(LOG_LEVEL emLL, const wchar_t* wszRec, va_list argsList)
{
    if (!m_isInited) return;
    wchar_t pBuff[512] = { 0 };
    ::vswprintf(pBuff, wszRec, argsList);

    char* pBuffMultiBytes = NULL;
    do
    {
        int nBufLen = ::WideCharToMultiByte(CP_ACP, 0, pBuff, -1, NULL, 0, NULL, NULL);
        pBuffMultiBytes = new char[nBufLen];
        if (!pBuffMultiBytes)
            break;
        ::memset(pBuffMultiBytes, 0, nBufLen);
        if (!::WideCharToMultiByte(CP_ACP, 0, pBuff, -1, pBuffMultiBytes, nBufLen, NULL, NULL))
            break;
        Recording(emLL, pBuffMultiBytes);
    } while (0);
    if (pBuffMultiBytes)
        delete[] pBuffMultiBytes;
    return;
}

void LogUtils::Recording(LOG_LEVEL emLL, const char* szRec, ...)
{
    if (!m_isInited) return;
    va_list argsList;
	va_start(argsList, szRec);
    Recording(emLL, szRec, argsList);
    va_end(argsList);
}

void LogUtils::Recording(LOG_LEVEL emLL, const char* szRec, va_list argsList)
{
    if (!m_isInited) return;
    const char* arrLogLevel[] = { "INFO", "WARN", "ERROR" };
	char szCurTime[100] = { 0 };
	CkCommon::FormatTime2Str(szCurTime, 100, "%Y/%m/%d %H:%M:%S");

	SyncLockGuard slg(&m_slSyncLock);
    m_pLogFile = ::fopen(m_strLogPath.c_str(), "a+");
    if (m_pLogFile)
    {
		::fprintf(m_pLogFile, " [%s %s]: ", szCurTime, arrLogLevel[emLL]);
        ::vfprintf(m_pLogFile, szRec, argsList);
        ::fprintf(m_pLogFile, "\n");
		CkCommon::SafeCloseLogFile(m_pLogFile);
    }
}

void LogUtils::SperateLine(char ch/* = '-'*/)
{
    if (!m_isInited) return;
	char tmpBuf[61] = { 0 };
	::memset(tmpBuf, ch, 60);

	SyncLockGuard slg(&m_slSyncLock);
    m_pLogFile = ::fopen(m_strLogPath.c_str(), "a+");
    if (m_pLogFile)
    {
        ::fprintf(m_pLogFile, "%s\n", tmpBuf);
		CkCommon::SafeCloseLogFile(m_pLogFile);
    }
}


//////////////////////////////////////////////////////////////////////////
// 自动切换新文件

unsigned int __stdcall LogUtils::SwitchLogFile_ThreadEntry(void* pParam)
{
	LogUtils* pThis = (LogUtils*)pParam;
	while (pThis->m_bthrUtilsFlag)
	{
		pThis->SwitchLogFileByFileSize();
		pThis->SwitchLogFileByDate();
		::Sleep(5000);
	}
	return 0;
}

void LogUtils::SwitchLogFileByFileSize()
{
	if (!m_isInited || !m_nFileMaxSize)
		return;

	const char* m_szLogPath = m_strLogPath.c_str();
	if (0 != ::_access(m_szLogPath, 0))
		return;

	while (true)
	{
		int nRet = ::_stat(m_szLogPath, &_stuFileState);
		_off_t nFileSize = (0 == nRet) ? _stuFileState.st_size : 0;

		if (nFileSize < m_nFileMaxSize)
			return;

		if ((++m_nLogIndex) >= LOGINDEX_MAX)
			return;

		char numBuf[16] = { 0 };
		::sprintf(numBuf, "%08d.txt", m_nLogIndex);

		size_t nPos = m_strLogPath.rfind('_');
		if (std::string::npos == nPos)
			return;

		SyncLockGuard slg(&m_slSyncLock);
		m_strLogPath.replace(m_strLogPath.begin() + nPos + 1, m_strLogPath.end(), numBuf);
	}
}

void LogUtils::SwitchLogFileByDate()
{
	if (!m_isInited || !m_bSwitchFileByDate)
 		return;
	
	int nPos = m_strLogPath.rfind('_');
	if (std::string::npos == nPos)
		return;

	int nPosBeg = 0;
	int nPosEnd = nPos--;
	for (; nPos > 0; --nPos)
	{
		if ('_' == m_strLogPath[nPos])
		{
			nPosBeg = nPos;
			break;
		}
	}
	std::string strOldTime(m_strLogPath.begin() + nPosBeg, m_strLogPath.begin() + nPosEnd);

	//判断是否是当天的Log
	char szTime[256] = { 0 };
	CkCommon::FormatTime2Str(szTime, 256, "_%Y-%m-%d");
	if (0 == ::strcmp(szTime, strOldTime.c_str()))
		return;

	SyncLockGuard slg(&m_slSyncLock);
	m_nLogIndex = 0;
	m_strLogPath.replace(m_strLogPath.begin()+nPosBeg, m_strLogPath.end(), szTime);
	if (m_nFileMaxSize)
		m_strLogPath += "_00000000";
	m_strLogPath += ".txt";
}


