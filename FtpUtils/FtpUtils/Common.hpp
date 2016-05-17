//************************************************
//��Author: CUCKOO0615
//��Date: 2016/04/18
//��Comment: ͨ�ú���
//*************************************************
#pragma once
//#include <afx.h>

#ifdef DEBUG
#include "LogUtils_ExportC.h"
extern LogUtils * g_pLogUtils;
#pragma comment(lib, "LogUtils.lib")
#endif // DEBUG

#define IsTRUE(b) (b==1)

namespace CkCommon
{
    /*
    ** ��·���е�'/'ȫ���滻Ϊ'\'�����Һϲ������ġ�\��ֻ��һ��
    ** �������һ��\����β������ĩβ����һ��'\'
    ** @Param strDirPath: Ŀ¼·���ַ���
    */
    void FixBackSlash_DirPath(CString& strDirPath);

    /*
    ** ��·���е�'/'ȫ���滻Ϊ'\'�����Һϲ������ġ�\��ֻ��һ��
    ** @Param strFilePath: �ļ�·���ַ���
    */
    void FixBackSlash_FilePath(CString& strFilePath);

    /*
    ** ��·���е�'\'ȫ���滻Ϊ'/'�����Һϲ������ġ�/��ֻ��һ��
    ** �������һ��/����β������ĩβ����һ��'/'
    ** @Param strFtpRemotePath: ·���ַ���
    */
    void FixSlash_FtpRemoteDirPath(CString& strFtpRemotePath);

    /*
    ** ��·���е�'\'ȫ���滻Ϊ'/'�����Һϲ������ġ�/��ֻ��һ��
    ** @Param strFtpRemotePath: ·���ַ���
    */
    void FixSlash_FtpRemoteFilePath(CString& strFtpRemotePath);
}
//////////////////////////////////////////////////////////////////////////

inline void CkCommon::FixBackSlash_DirPath(CString& strDirPath)
{
    int nLength = strDirPath.GetLength();
    if (0 == nLength)
        return;
    strDirPath.Replace(_T('/'), _T('\\'));
    strDirPath += _T('\\');
    while (0 != strDirPath.Replace(_T("\\\\"), _T("\\")));
}

inline void CkCommon::FixBackSlash_FilePath(CString& strFilePath)
{
    FixBackSlash_DirPath(strFilePath);
    if (0 != strFilePath.GetLength())
        strFilePath.TrimRight(_T('\\'));
}

inline void CkCommon::FixSlash_FtpRemoteDirPath(CString& strFtpRemotePath)
{
    int nLength = strFtpRemotePath.GetLength();
    if (0 == nLength)
        return;
    strFtpRemotePath.Replace(_T('\\'), _T('/'));
    strFtpRemotePath += _T('/');
    while (0 != strFtpRemotePath.Replace(_T("//"), _T("/")));
}

inline void CkCommon::FixSlash_FtpRemoteFilePath(CString& strFtpRemotePath)
{
    FixSlash_FtpRemoteDirPath(strFtpRemotePath);
    if (0 != strFtpRemotePath.GetLength())
        strFtpRemotePath.TrimRight(_T('/'));
}
