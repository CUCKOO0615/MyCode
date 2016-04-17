#include "StdAfx.h"
#include "StringConvert.h"
#include "StringUtils.h"

CString StringConvert::StrConv_cstr2CStringT(const char* szStr)
{
    CString strRet;
#ifdef UNICODE
    char* pszErrMsg = NULL;
    wchar_t* szTmp = StringUtils::StrConv_A2W(szStr, pszErrMsg);
    strRet = szTmp ? szTmp : L"";
    if (szTmp) delete[] szTmp;
#else
    strRet = szStr;
#endif
    return strRet;
}

CStringA StringConvert::StrConv_TStr2CStringA(const TCHAR* _tszStr)
{
    CStringA strRet;
#ifdef UNICODE
    char* pszErrMsg = NULL;
    char* szTmp = StringUtils::StrConv_W2A(_tszStr, pszErrMsg);
    strRet = szTmp ? szTmp : "";
    if (szTmp) delete[] szTmp;
#else
    strRet = _tszStr;
#endif
    return strRet;
}

CStringW StringConvert::StrConv_TStr2CStringW(const TCHAR* _tszStr)
{
    CStringW strRet;
#ifdef UNICODE
    strRet = _tszStr;
#else
    char* pszErrMsg = NULL;
    wchar_t* wszStr = StringUtils::StrConv_A2W(_tszStr, pszErrMsg);
    strRet = wszStr ? wszStr : L"";
    if (wszStr) delete[] wszStr;
#endif
    return strRet;
}

bool StringConvert::StrConv_Utf82A(CStringA& strA, char*& pErr)
{
	char* szAnsi = StringUtils::StrConv_Utf82A(strA.GetBuffer(0), pErr);
	strA.ReleaseBuffer();

	if (!szAnsi)
		return false;	
	strA = szAnsi;
	delete[] szAnsi;
	return true;
}

bool StringConvert::StrConv_A2Utf8(CStringA& strA, char*& pErr)
{
	char* szUtf8 = StringUtils::StrConv_A2Utf8(strA.GetBuffer(0), pErr);
	strA.ReleaseBuffer();

	if (!szUtf8)
		return false;
	strA = szUtf8;
	delete[] szUtf8;
	return true;
}

void StringConvert::StrConv_CStringA2cstr(CStringA strA, char* pBuff, int nBuffSize)
{
	::memset(pBuff, 0, nBuffSize);
	::memcpy(pBuff, strA.GetBuffer(0), min(nBuffSize, strA.GetLength()));
}
