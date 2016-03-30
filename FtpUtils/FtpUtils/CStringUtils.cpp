#include "StdAfx.h"
#include "CStringUtils.h"
#include "StringUtils.h"

CString CStringUtils::StrConv_cstr2CStringT(const char* szStr)
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

CStringA CStringUtils::StrConv_TStr2CStringA(const TCHAR* _tszStr)
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

CStringW CStringUtils::StrConv_TStr2CStringW(const TCHAR* _tszStr)
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