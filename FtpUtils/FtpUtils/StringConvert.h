#pragma once
//************************************************
//��Author: CUCKOO0615
//��Date: 2016/03/30
//��Comment: �ַ���ת��
//*************************************************

namespace StringConvert
{
    // ANSI�ַ���תCStringT
    CString StrConv_cstr2CStringT(const char* szStr);

    // TCHAR�ַ���תCStringA
    CStringA StrConv_TStr2CStringA(const TCHAR* _tszStr);

    // TCHAR�ַ���תCStringW
    CStringW StrConv_TStr2CStringW(const TCHAR* _tszStr);

    // CStringAתC����ַ���
	void StrConv_CStringA2cstr(CStringA strA, char* pBuff, int nBuffSize);

	//CStringA�ַ����ڲ�����ת��
	bool StrConv_Utf82A(CStringA& strA, char*& pErr);
	bool StrConv_A2Utf8(CStringA& strA, char*& pErr);
};

