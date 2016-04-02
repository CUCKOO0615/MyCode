#include "stdafx.h"

#define  EXPORT_STDC

#include "LogUtils_ExportC.h"
#include "LogUtils.h"

CK_API LogUtils* CkLogUtils::CreateLogUtils()
{
    LogUtils* pRet = new LogUtils();
    return pRet;
}

CK_API void CkLogUtils::ReleaseLogUtils(LogUtils* p)
{
    if (p)
        delete p;
}

CK_API bool InitLogUtils(LogUtils* p,
	const char* szDirPath = ".\\Log\\",
	const char* szLogNamePrefix = "Log",
	bool bSwitchFileByDate = true,
	unsigned int nMaxFileSize = 0xA00000 /*10M*/)
{
    if (!p) return false;
	return p->InitLogUtils(szDirPath, szLogNamePrefix, bSwitchFileByDate, nMaxFileSize);
}

CK_API void CkLogUtils::RecordingA(LogUtils* p, LOG_LEVEL emLL, const char* szRec, ...)
{
    if (!p) return;
    va_list argsList;
    va_start(argsList, szRec);
    p->Recording(emLL, szRec, argsList);
    va_end(argsList);
}

CK_API void CkLogUtils::RecordingW(LogUtils* p, LOG_LEVEL emLL, const wchar_t* wszRec, ...)
{
    if (!p) return;
    va_list argsList;
    va_start(argsList, wszRec);
    p->Recording(emLL, wszRec, argsList);
    va_end(argsList);
}

CK_API void CkLogUtils::SperateLine(LogUtils* p, char ch /*= '-'*/)
{
    if (!p) return;
    p->SperateLine(ch);
}


