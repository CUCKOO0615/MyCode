#pragma once
//************************************************
//¡óAuthor: CUCKOO0615
//¡óDate: 2016/03/30
//¡óComment: ×Ö·û´®×ª»»
//*************************************************

namespace StringConvert
{
    // ANSI×Ö·û´®×ªCStringT
    CString StrConv_cstr2CStringT(const char* szStr);

    // TCHAR×Ö·û´®×ªCStringA
    CStringA StrConv_TStr2CStringA(const TCHAR* _tszStr);

    // TCHAR×Ö·û´®×ªCStringW
    CStringW StrConv_TStr2CStringW(const TCHAR* _tszStr);

    // CStringA×ªC·ç¸ñ×Ö·û´®
	void StrConv_CStringA2cstr(CStringA strA, char* pBuff, int nBuffSize);

	//CStringA×Ö·û´®ÄÚ²¿±àÂë×ª»»
	bool StrConv_Utf82A(CStringA& strA, char*& pErr);
	bool StrConv_A2Utf8(CStringA& strA, char*& pErr);
};

