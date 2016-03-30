#pragma once
//************************************************
//◇Author: CUCKOO0615
//◇Date: 2016/03/30
//◇Comment: CString与C风格字符串互转
//*************************************************

namespace CStringUtils
{
    // ANSI字符串转CStringT
    CString StrConv_cstr2CStringT(const char* szStr);

    // TCHAR字符串转CStringA
    CStringA StrConv_TStr2CStringA(const TCHAR* _tszStr);

    // TCHAR字符串转CStringW
    CStringW StrConv_TStr2CStringW(const TCHAR* _tszStr);
};

