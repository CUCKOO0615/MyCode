
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��10��
//��˵�����ļ�·������
//*************************************************
#include "StdAfx.h"
#include "PathUtils.h"
#include <direct.h>
#include <assert.h>

#define SET_ERROR_MSG(szErrMsg)  strcpy(m_szErrMsg, szErrMsg)

#ifdef CUCKOO0615_USE_STL
bool PathUtils::GetFilesInDir(vector<string>& vecFileFullPaths, string strDir, string strWildcard, bool bEnterSubDir, bool bEnterHiddenSubDir, int nExceptFileTypes)
{
    return GetFullPathsInDir(vecFileFullPaths, strDir, strWildcard, bEnterSubDir, bEnterHiddenSubDir, nExceptFileTypes, true, false);
}

bool PathUtils::GetSubDirsInDir(vector<string>& vecSubDirFullPaths, string strDir, string strWildcard, bool bEnterSubDir, bool bEnterHiddenSubDir, int nExceptFileTypes)
{
    return GetFullPathsInDir(vecSubDirFullPaths, strDir, strWildcard, bEnterSubDir, bEnterHiddenSubDir, nExceptFileTypes, false, true);
}

bool PathUtils::GetFullPathsInDir(vector<string>& vecFullPaths, string strDir, const string& strWildcard, bool bEnterSubDir, bool bEnterHiddenSubDir, int nExceptFileTypes, bool bGetFiles, bool bGetDirs)
{
    char chLastChar = strDir[strDir.length() - 1];
    if ('\\' != chLastChar && '/' != chLastChar)
        strDir += "\\";

    string strSearchPath = strDir + strWildcard;

    intptr_t handle = NULL;
    _finddata_t fileinfo;
    handle = _findfirst(strSearchPath.c_str(), &fileinfo);
    if (-1 == handle)
    {
        SET_ERROR_MSG("Search files failed");
        _findclose(handle);
        return false;
    }

    do
    {
        if ((0 == strcmp(fileinfo.name, ".")) || (0 == strcmp(fileinfo.name, "..")))
            continue;

        bool bIsDir = !(0 == (fileinfo.attrib & _A_SUBDIR));
        bool bIsFile = !bIsDir;
        bool bAtrribIsQualified = true;
        if (-1 != nExceptFileTypes)
            bAtrribIsQualified = !(fileinfo.attrib & nExceptFileTypes);

        if (bGetFiles && bIsFile && bAtrribIsQualified)
            vecFullPaths.push_back(strDir + string(fileinfo.name));

        if (bGetDirs && bIsDir && bAtrribIsQualified)
            vecFullPaths.push_back(strDir + string(fileinfo.name));

        if (bEnterSubDir && bIsDir)
        {
            if ((fileinfo.attrib & _A_HIDDEN) && !bEnterHiddenSubDir)
                continue;

            string strSubPath = strDir + fileinfo.name;
            if (!GetFullPathsInDir(vecFullPaths, strSubPath, strWildcard, true, bEnterHiddenSubDir, nExceptFileTypes, bGetFiles, bGetDirs))
            {
                SET_ERROR_MSG("Search files failed when recursing");
                _findclose(handle);
                return false;
            }
        }
    } while (!_findnext(handle, &fileinfo));

    _findclose(handle);
    return true;
}


std::string PathUtils::GetFileName(string strFileFullPath)
{
    char szFileName[MAX_PATH] = { 0 };
    if (GetFileName(strFileFullPath.c_str(), szFileName, MAX_PATH))
        return string(szFileName);
    else
        return "";
}

#endif

bool PathUtils::GetFileName(const char* szFullPath, char* szName, size_t nNameBufSize)
{
	assert(szFullPath);
	assert(szName);
	assert(strlen(szFullPath) > 1);

    if (MAX_PATH <= strlen(szFullPath))
    {
        SET_ERROR_MSG("Input path length is too long");
        return false;
    }

    if (nNameBufSize <= strlen(szFullPath))
    {
        SET_ERROR_MSG("Name buffer size is too small");
        return false;
    }

    char szBuf[MAX_PATH] = { 0 };
    strcpy(szBuf, szFullPath);
    size_t nLength = strlen(szBuf);

    size_t i = nLength - 1;
    if ('/' == szBuf[i] || '\\' == szBuf[i])
        szBuf[i] = 0x00;
    for (; i != -1; --i)
    {
        if ('/' == szBuf[i] || '\\' == szBuf[i])
        {
            size_t nOffset = i + 1;
            memcpy(szName, szBuf + nOffset, nLength - nOffset);
            break;
        }
    }
    if (-1 == i)
    {
        SET_ERROR_MSG("Get name failed");
        return false;
    }
    return true;
}

int PathUtils::GetDriveNames(char arrLogicalDriveNames[26])
{
    const int nBufSize = 26 * 4;
    char szBuf[nBufSize] = { 0 };
    DWORD dwLength = GetLogicalDriveStringsA(nBufSize, szBuf);

    if ((0 == dwLength) || (nBufSize <= dwLength))
    {
        SET_ERROR_MSG("Get logical drive names failed");
        return -1;
    }

    int i = 0;
    for (char * s = szBuf; *s; s += (strlen(s) + 1))
    {
        arrLogicalDriveNames[i] = s[0];
        ++i;
    }
    return i;
}

const char* PathUtils::GetDriveType(char chDriveName)
{
    char szDriveName[] = "X:\\";
    szDriveName[0] = chDriveName;
    UINT uDriveType = ::GetDriveTypeA(szDriveName);

    switch (uDriveType)
    {
    case DRIVE_UNKNOWN:
        return "δ֪������";
    case DRIVE_NO_ROOT_DIR:
        return "��Ч��Ŀ¼";
    case DRIVE_REMOVABLE:
        return "���ƶ��豸";
    case DRIVE_FIXED:
        return "�̶�������";
    case DRIVE_REMOTE:
        return "����������";
    case DRIVE_CDROM:
        return "CD-ROM";
    case DRIVE_RAMDISK:
        return "RAM������";
    default:
        return "δ֪�豸";
    }
}

bool PathUtils::PathIsExist(const char* szPath)
{
    size_t nStrLength = CheckStringIsNullOrEmpty(szPath);
    if (0 == nStrLength)
    {
        SET_ERROR_MSG("Input path can't be NULL or empty");
        return false;
    }

    if (MAX_PATH <= nStrLength)
    {
        SET_ERROR_MSG("Input path length is too long");
        return false;
    }

    return (0 == _access(szPath, 0));
}

bool PathUtils::FileIsExist(const char* szFilePath)
{
    size_t nStrLength = CheckStringIsNullOrEmpty(szFilePath);
    if (0 == nStrLength)
    {
        SET_ERROR_MSG("Input path can't be NULL or empty");
        return false;
    }

    if (MAX_PATH <= nStrLength)
    {
        SET_ERROR_MSG("Input path length is too long");
        return false;
    }

    _finddata_t fileinfo;

#if _MSC_VER >= 1400	// intptr_t/long
    intptr_t handle = _findfirst(szFilePath, &fileinfo);
#else
    long handle = _findfirst( szFilePath, &fileinfo);
#endif

    bool bIsDir = (0 != (fileinfo.attrib & _A_SUBDIR));
    bool ret = true;
    if ((-1 == handle) || bIsDir)
        ret = false;

    _findclose(handle);
    return ret;
}

bool PathUtils::DirIsExist(const char* szDirPath)
{
    size_t nStrLength = CheckStringIsNullOrEmpty(szDirPath);
    if (0 == nStrLength)
    {
        SET_ERROR_MSG("Input path can't be NULL or empty");
        return false;
    }

    if (MAX_PATH <= nStrLength)
    {
        SET_ERROR_MSG("Input path length is too long");
        return false;
    }

    _finddata_t fileinfo;

#if _MSC_VER >= 1400	// intptr_t/long
    intptr_t handle = _findfirst(szDirPath, &fileinfo);
#else
    long handle = _findfirst( szDirPath, &fileinfo);
#endif

    bool bIsDir = (0 != (fileinfo.attrib & _A_SUBDIR));
    bool ret = true;
    if ((-1 == handle) || !bIsDir)
        ret = false;

    _findclose(handle);
    return ret;
}

bool PathUtils::GetParentPath(const char* szPath, char* szParentPath, size_t nParentPathBufSize)
{
    size_t nStrLength = CheckStringIsNullOrEmpty(szPath);
    if (0 == nStrLength)
    {
        SET_ERROR_MSG("Input path can't be NULL or empty");
        return false;
    }

    if (MAX_PATH <= nStrLength)
    {
        SET_ERROR_MSG("Input path length is too long");
        return false;
    }

    if (nParentPathBufSize <= nStrLength)
    {
        SET_ERROR_MSG("Parent path buffer size is too small");
        return false;
    }

    char szBuf[MAX_PATH] = { 0 };
    strcpy(szBuf, szPath);
    size_t nLength = strlen(szBuf);
    if ('\\' == szBuf[nLength - 1] || '/' == szBuf[nLength - 1])
        szBuf[nLength - 1] = 0x00;

    int i = nLength - 1;
    for (; i != -1; --i)
    {
        if ('\\' == szBuf[i] || '/' == szBuf[i])
        {
            memcpy(szParentPath, szBuf, i);
            break;
        }
    }
    if (-1 == i)
    {
        SET_ERROR_MSG("Can't find parent path");
        return false;
    }
    return true;
}

bool PathUtils::CreateMultiDirectory(const char* szPath)
{
    size_t nStrLength = CheckStringIsNullOrEmpty(szPath);
    if (0 == nStrLength)
    {
        SET_ERROR_MSG("Input path can't be NULL or empty");
        return false;
    }

    if (MAX_PATH <= nStrLength)
    {
        SET_ERROR_MSG("Input path length is too long");
        return false;
    }

    char szTmp[MAX_PATH] = { 0 };
    for (size_t i = 0; i != nStrLength + 1; ++i)
    {
        if ('/' == szPath[i] || '\\' == szPath[i] || 0x00 == szPath[i])
        {
            memcpy(szTmp, szPath, i);
            if (0 == _access(szTmp, 0))
                continue;

            if (0 != _mkdir(szTmp))
            {
                SET_ERROR_MSG("Create directory failed");
                return false;
            }
        }
    }
    return true;
}

