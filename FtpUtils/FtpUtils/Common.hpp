//************************************************
//◇Author: CUCKOO0615
//◇Date: 2016/04/18
//◇Comment: 通用函数
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
    ** 将路径中的'/'全部替换为'\'，并且合并连续的‘\’只留一个
    ** 如果不是一‘\’结尾，则在末尾补齐一个'\'
    ** @Param strDirPath: 目录路径字符串
    */
    void FixBackSlash_DirPath(CString& strDirPath);

    /*
    ** 将路径中的'/'全部替换为'\'，并且合并连续的‘\’只留一个
    ** @Param strFilePath: 文件路径字符串
    */
    void FixBackSlash_FilePath(CString& strFilePath);

    /*
    ** 将路径中的'\'全部替换为'/'，并且合并连续的‘/’只留一个
    ** 如果不是一‘/’结尾，则在末尾补齐一个'/'
    ** @Param strFtpRemotePath: 路径字符串
    */
    void FixSlash_FtpRemoteDirPath(CString& strFtpRemotePath);

    /*
    ** 将路径中的'\'全部替换为'/'，并且合并连续的‘/’只留一个
    ** @Param strFtpRemotePath: 路径字符串
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
