//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��8��10��
//��˵�����ļ�·������
//*************************************************
#pragma once

#ifndef CUCKOO0615_UTILS_MACRO
#define CUCKOO0615_UTILS_MACRO

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef ERRMSG_LENGTH
#define ERRMSG_LENGTH 256
#endif

#endif

#include <windows.h>
#include <io.h>

#define CUCKOO0615_USE_STL
#ifdef CUCKOO0615_USE_STL
#include <vector>
#include <string>
using std::vector;
using std::string;
#endif

class PathUtils
{
public:
    //////////////////////////////////////////////////////////////////////////
    //���� STL �ĺ���
#ifdef CUCKOO0615_USE_STL
    /*
    ** ��ȡָ���ļ����µ��������ļ���ȫ·��
    ** @param vecSubDirFullPaths: ���ļ���·������
    ** @param strDir: ���ļ���
    ** @param strWildcard: ͨ���
    ** @param bEnterSubDir: �Ƿ��������ļ���
    ** @param bEnterHiddenSubDir: �Ƿ������������ļ���, ����bEnterSubDirΪtrueʱ��Ч
    ** @param nExceptFileTypes: ����ʱҪ�ų����ļ�������,
    ** - ������ _A_NORMAL|_A_RDONLY|_A_HIDDEN|_A_SYSTEM|_A_ARCH �е�һ������,��"|"����
    */
    bool GetSubDirsInDir(
        vector<string>& vecSubDirFullPaths,
        string strDir,
        string strWildcard = "*",
        bool bEnterSubDir = false,
        bool bEnterHiddenSubDir = false,
        int nExceptFileTypes = -1);

    /*
    ** ��ȡָ���ļ����µ������ļ�ȫ·��
    ** @param vecFileFullPaths: �ļ�·������
    ** @param strDir: ���ļ���
    ** @param strWildcard: ͨ���
    ** @param bEnterSubDir: �Ƿ��������ļ���
    ** @param bEnterHiddenSubDir: �Ƿ������������ļ���, ����bEnterSubDirΪtrueʱ��Ч
    ** @param nExceptFileTypes: ����ʱҪ�ų����ļ�����,
    ** - ������ _A_NORMAL|_A_RDONLY|_A_HIDDEN|_A_SYSTEM|_A_ARCH �е�һ������,��"|"����
    */
    bool GetFilesInDir(
        vector<string>& vecFileFullPaths,
        string strDir,
        string strWildcard = "*.*",
        bool bEnterSubDir = false,
        bool bEnterHiddenSubDir = false,
        int nExceptFileTypes = -1);

    /*
    ** ��ȫ·����ȡ�ļ������ļ�����
    ** @param strFileFullPath: �ļ����ļ��е�ȫ·��
    */
    string GetFileName(string strFileFullPath);
#endif

    /*
    ** ��ȫ·����ȡ�ļ������ļ�����
    ** @param szFullPath: �ļ����ļ��е�ȫ·��,���MAX_PATH
    ** @param szName: �ļ����ļ�����������ָ��
    ** @param nNameBufSize: �ļ����ļ�������������С
    */
    bool GetFileName(const char* szFullPath, char* szName, size_t nNameBufSize);

    /*
    ** ��ȡ�����̷��б�
    ** @param arrLogicalDriveNames[26]: �����̷�����
    ** @return: �����ɹ������̷�����(0 - 26), ����ʧ�ܷ��� -1
    */
    int GetDriveNames(char arrLogicalDriveNames[26]);

    /*
    ** �����̷���ȡ��������
    ** @param chDriveName: �����̷�
    ** @return: δ֪������/��Ч��Ŀ¼/���ƶ��豸/�̶�������/����������/CD-ROM/RAM������/δ֪�豸
    */
    const char* GetDriveType(char chDriveName);

    /*
    ** ���ָ����·���Ƿ����(�������ļ����ļ���,�Ͽ�)
    ** @param szPath: Ҫ����·��
    */
    bool PathIsExist(const char* szPath);

    /*
    ** ���ָ���ļ��Ƿ����
    ** @param szFilePath: Ҫ����·��
    */
    bool FileIsExist(const char* szFilePath);

    /*
    ** ���ָ���ļ����Ƿ����
    ** @param szDirPath: Ҫ����·��
    */
    bool DirIsExist(const char* szDirPath);

    /*
    ** ��ȡ��Ŀ¼
    ** @param szPath: �����·��,�MAX_PATH-1
    ** @param szParentPath: ��Ŀ¼������
    ** @param nParentPathBufSize: ��Ŀ¼����������,��С����Ҫ����szPath�ĳ���
    */
    bool GetParentPath(const char* szPath, char* szParentPath, size_t nParentPathBufSize);

    /*
    ** �����༶Ŀ¼
    ** @param szPath: ָ����Ŀ¼
    ** @return: �����ɹ�����true
    */
    bool CreateMultiDirectory(const char* szPath);

    // ��ȡ������Ϣ
    const char* GetErrMsg() { return m_szErrMsg; }


private:
    char m_szErrMsg[ERRMSG_LENGTH];

#ifdef CUCKOO0615_USE_STL
    bool GetFullPathsInDir(
        vector<string>& vecFullPaths,
        string strDir,
        const string& strWildcard,
        bool bEnterSubDir,
        bool bEnterHiddenSubDir,
        int nExceptFileTypes,
        bool bGetFiles,
        bool bGetDirs);
#endif

    //����ַ����Ƿ�Ϊ��
    //����ֵ:���ַ������ָ�뷵��0,���򷵻��ַ�������
    size_t CheckStringIsNullOrEmpty(const char* szString)
    {
        return ((NULL == szString) || 0 == strlen(szString)) ? 0 : strlen(szString);
    }

public:
    PathUtils() { memset(m_szErrMsg, 0x00, ERRMSG_LENGTH); }
    ~PathUtils() { }
};

