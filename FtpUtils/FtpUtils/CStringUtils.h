#pragma once
//************************************************
//��Author: CUCKOO0615
//��Date: 2016/03/30
//��Comment: CString��C����ַ�����ת
//*************************************************

namespace CStringUtils
{
    // ANSI�ַ���תCStringT
    CString StrConv_cstr2CStringT(const char* szStr);

    // TCHAR�ַ���תCStringA
    CStringA StrConv_TStr2CStringA(const TCHAR* _tszStr);

    // TCHAR�ַ���תCStringW
    CStringW StrConv_TStr2CStringW(const TCHAR* _tszStr);
};

