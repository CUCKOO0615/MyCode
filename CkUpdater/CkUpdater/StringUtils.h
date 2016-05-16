//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��10��
//��˵�����ַ�������
//*************************************************
#pragma once

#ifndef CUCKOO0615_UTILS_MACRO
#define CUCKOO0615_UTILS_MACRO
#endif

#define CUCKOO0615_USE_STL

#include <windows.h>
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////
// ���� STL ʵ�ֵĺ���
#ifdef CUCKOO0615_USE_STL
#include <string>
#include <vector>
using namespace std;

namespace StringUtils
{
    /*
    ** �Ƴ���������ַ�
    ** @param str: Դ�ַ���
    ** @param chSpilter: Ŀ���ַ�
    */
    void TrimLeft(string& str, char chSpilter = ' ');

    /*
    ** �Ƴ��Ҳ������ַ�
    ** @param str: Դ�ַ���
    ** @param chSpilter: Ŀ���ַ�
    */
    void TrimRight(string& str, char chSpilter = ' ');

    /*
    ** �Ƴ���β�����ַ�
    ** @param str: Դ�ַ���
    ** @param chSpilter: Ŀ���ַ�
    */
    void Trim(string& str, char chSpilter = ' ');

    /*
    ** �滻�ַ�����ָ�����Ӵ�Ϊ��һ���Ӵ�
    ** @param str: Դ�ַ���
    ** @param oldStr: Ҫ���滻�ľ��Ӵ�
    ** @param newStr: �����滻�����Ӵ�
    */
    void ReplaceSpecifiedString(string& str, string oldStr, string newStr);

    /*
    ** �з��ַ���������
    ** @param vecOut: ������ַ�������
    ** @param str: ������ַ���
    ** @param chSpliter: �ַ����зַ�
    */
	void SplitStr2Array(vector<string>& vecOut, const string& str, char chSpliter, bool bSkipContinuous = true);

    /*
    ** ���ֽ��ַ�ת���ֽ��ַ�
    ** @param szMultibyteStr: ����Ķ��ֽ��ַ���
    ** @param strWide: ����Ŀ��ֽ��ַ���
    ** @param strErrMsg: ������Ϣ
    */
    bool StrConv_A2W(const char* szMultibyteStr, wstring& strWide, string& strErrMsg);

    /*
    ** ���ֽ��ַ�ת���ֽ��ַ�
    ** @param szWidestr: ����Ŀ��ֽ��ַ���
    ** @param szMultibyteStr: ����Ķ��ֽ��ַ���
    ** @param strErrMsg: ������Ϣ
    */
    bool StrConv_W2A(const wchar_t* szWidestr, string& szMultibyteStr, string& strErrMsg);
 }
#endif
//////////////////////////////////////////////////////////////////////////

namespace StringUtils
{
    /*
    ** �Ƴ���������ַ�
    ** �����ַ��������Ƿ���\0��������ʹ��
    ** @param szStr: Դ�ַ���������,
    ** @param nBufSize: szStr�������ֽڳ���
    ** @param chSpilter: Ŀ���ַ�
    */
    void TrimLeft(char * szStr, size_t nBufSize, char chSpilter = ' ');

    /*
    ** �Ƴ��Ҳ������ַ�
    ** �����ַ��������Ƿ���\0��������ʹ��
    ** @param szStr: Դ�ַ���������
    ** @param nBufSize: szStr�������ֽڳ���
    ** @param chSpilter: Ŀ���ַ�
    */
    void TrimRight(char* szStr, size_t nBufSize, char chSpilter = ' ');

    /*
    ** �Ƴ���β�����ַ�
    ** �����ַ��������Ƿ���\0��������ʹ��
    ** @param szStr: Դ�ַ���������
    ** @param nBufSize: szStr�������ֽڳ���
    ** @param chSpilter: Ŀ���ַ�
    */
    void Trim(char* szStr, size_t nBufSize, char chSpilter = ' ');

    /*
    ** �滻�ַ�����ָ�����Ӵ�Ϊ��һ���Ӵ�
    ** �����ַ�������szStr�Ƿ���\0��������ʹ��
    ** @param szStr: Դ�ַ���������,Ҫȷ���������ռ��㹻,
    ** - ����滻��������ַ�������Ļ��������ȳ���szStr��ָ��Ļ���������,
    ** - �������𱨴���쳣,�滻����ַ�����ռ��Ԥ���Ļ�����,����nBufSize���ض�,
    ** - ��ᵼ��szStrĩβ��\0�ֽڱ����ǵ�,�Ӷ����һ����\0�������ַ�������.
    ** @param nBufSize: Դ�ַ�������������
    ** @param szOldStr: Ҫ���滻���Ӵ�
    ** @param szNewStr: �滻���Ӵ�
    */
    void ReplaceSpecifiedString(char* szStr, size_t nBufSize, const char* szOldStr, const char* szNewStr);

    /*
    ** �ж��ַ����Ƿ��ǺϷ�������\����
    ** @param szStr: ������ַ���
    */
    bool IsDigitStr(const char* szStr);

    /*
    ** �������ֽ���ת��Ϊ�ɶ���16�����ı�
    ** @param inBuff: Դ���������ݻ�����
    ** @param nInBuffSize: Դ���������ݻ�������С
    ** @param outBuff: ����ı�������
    ** @param nOutBuffSize: ����ı���������С
    ** - ����һ���������ֽڶ�Ӧ2��16�����ı��ַ�,
    ** - ��nOutBuffSize����С��nInBuffSize*2, ����������false
    */
    bool Hex2Text(const char* inBuff, size_t nInBuffSize, char* outBuff, size_t nOutBuffSize);
    

    //////////////////////////////////////////////////////////////////////////
    // �ַ�������ת��ϵ��
	//////////////////////////////////////////////////////////////////////////

	/*
	** ���ַ���(Unicode)ת���ֽ��ַ���
	** @Param wszUnicode: ���ַ���
	** @Param uCodePage: ָ��ת��ʹ�õĴ���ҳ
	** @Param pszErrMsg: ָ�����������Ϣ�Ļ�������ָ��,����Ҫ�û�delete
	** @Ret : ת���ɹ�����ָ���������ַ����Ļ�������ָ��, ��Ҫ�û�����delete[], ʧ�ܷ���NULL
	*/
	char* StrConv_W2M(const wchar_t* wszUnicode, UINT uCodePage, char*& pszErrMsg);
	
    /*
    ** ���ַ���(Unicode)ת���ֽ��ַ���(ANSI)
    ** @Param wszUnicode: ���ַ���
    ** @Param pszErrMsg: ָ�����������Ϣ�Ļ�������ָ��,����Ҫ�û�delete
    ** @Ret : ת���ɹ�����ָ���������ַ����Ļ�������ָ��, ��Ҫ�û�����delete[], ʧ�ܷ���NULL
    */
    char* StrConv_W2A(const wchar_t* wszUnicode, char*& pszErrMsg);
    
    /*
    ** ���ַ���(Unicode)תת���ֽ��ַ���(UTF8)
    ** @Param wszUnicode: ���ַ���
    ** @Param pszErrMsg: ָ�����������Ϣ�Ļ�������ָ��,����Ҫ�û�delete
    ** @Ret : ת���ɹ�����ָ���������ַ����Ļ�������ָ��, ��Ҫ�û�����delete[], ʧ�ܷ���NULL
    */
    char* StrConv_W2Utf8(const wchar_t* wszUnicode, char*& pszErrMsg);

	/*
	** ���ֽ��ַ���ת���ַ���(Unicode)
	** @Param szAnsi: ���ֽ��ַ���
	** @Param uCodePage: ָ��ת��ʹ�õĴ���ҳ
	** @Param pszErrMsg: ָ�����������Ϣ�Ļ�������ָ��,����Ҫ�û�delete
	** @Ret : ת���ɹ�����ָ���������ַ����Ļ�������ָ��, ��Ҫ�û�����delete[], ʧ�ܷ���NULL
	*/
	wchar_t* StrConv_M2W(const char* szMultiBytes, UINT uCodePage, char*& pszErrMsg);

    /*
    ** ���ֽ��ַ���(ANSI)ת���ַ���(Unicode)
    ** @Param szAnsi: ���ֽ��ַ���
    ** @Param pszErrMsg: ָ�����������Ϣ�Ļ�������ָ��,����Ҫ�û�delete
    ** @Ret : ת���ɹ�����ָ���������ַ����Ļ�������ָ��, ��Ҫ�û�����delete[], ʧ�ܷ���NULL
    */
    wchar_t* StrConv_A2W(const char* szAnsi, char*& pszErrMsg);
	
    /*
    ** ���ֽ��ַ���(ANSI)תUTF8�ַ���
    ** @Param szAnsi: ���ֽ��ַ���
    ** @Param pszErrMsg: ָ�����������Ϣ�Ļ�������ָ��,����Ҫ�û�delete
    ** @Ret : ת���ɹ�����ָ���������ַ����Ļ�������ָ��, ��Ҫ�û�����delete[], ʧ�ܷ���NULL
    */
    char* StrConv_A2Utf8(const char* szAnsi, char*& pszErrMsg);

    /*
    ** UTF8�ַ���ת���ַ���(Unicode)
    ** @Param szUtf8: UTF8�ַ���
    ** @Param pszErrMsg: ָ�����������Ϣ�Ļ�������ָ��,����Ҫ�û�delete
    ** @Ret : ת���ɹ�����ָ���������ַ����Ļ�������ָ��, ��Ҫ�û�����delete[], ʧ�ܷ���NULL
    */
    wchar_t* StrConv_Utf82W(const char* szUtf8, char*& pszErrMsg);

    /*
    ** UTF8�ַ���ת���ֽ��ַ���(ANSI)
    ** @Param szUtf8: UTF8�ַ���
    ** @Param pszErrMsg: ָ�����������Ϣ�Ļ�������ָ��,����Ҫ�û�delete
    ** @Ret : ת���ɹ�����ָ���������ַ����Ļ�������ָ��, ��Ҫ�û�����delete[], ʧ�ܷ���NULL
    */
    char* StrConv_Utf82A(const char* szUtf8, char*& pszErrMsg);
    
#ifdef UNICODE
#define StrConv_Utf82T  StrConv_Utf82W
#define StrConv_T2UTF8  StrConv_W2Utf8
#else
#define StrConv_Utf82T  StrConv_Utf82A
#define StrConv_T2UTF8  StrConv_A2Utf8
#endif

    //////////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////
// ����inlineʵ��
#ifdef CUCKOO0615_USE_STL

inline void StringUtils::TrimLeft(string& str, char chSpilter /*= ' ' */)
{
    size_t nPos = str.find_first_not_of(chSpilter);
    if (string::npos == nPos)
    {
        str = "";
        return;
    }
    str.erase(0, nPos);
}

inline void StringUtils::TrimRight(string& str, char chSpilter /*= ' ' */)
{
    size_t nPos = str.find_last_not_of(chSpilter);
    if (string::npos == nPos)
    {
        str = "";
        return;
    }
    str.erase(nPos + 1);
}

inline void StringUtils::Trim(string& str, char chSpilter /*= ' ' */)
{
    size_t nPosFirst = str.find_first_not_of(chSpilter);
    if (string::npos != nPosFirst)
        str.erase(0, nPosFirst);
    else
    {
        str = "";
        return;
    }
    size_t nPosLast = str.find_last_not_of(chSpilter);
    if (string::npos != nPosLast)
        str.erase(nPosLast + 1);
}
#endif

