#include "StdAfx.h"

#pragma warning(disable:4996)
#include "FtpConnector.h"
#include "StringConvert.h"
#include "StringUtils.h"
#include "PtrUtils.hpp"
#include <fstream>
#include "Common.hpp"
#include "LogUtils_ExportC.h"

//extern LogUtils * g_pLogUtils;


#define SET_LAST_ERRMSG(szFormat,...) ::sprintf(m_szLastErrMsg, szFormat, __VA_ARGS__)
#define RESET_ERRMSG                  m_szLastErrMsg[0]='\0';

FtpConnector::FtpConnector(LPCSTR szSessionName, DWORD dwDelay)
:m_objSession(szSessionName), m_pConnection(NULL), m_bEnableUtf8(false)
{
    RESET_ERRMSG;
    m_objSession.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_SEND_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, dwDelay);
    m_objSession.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, dwDelay);
}

FtpConnector::~FtpConnector()
{
    SafeCloseConnection();
    m_objSession.Close();
}

bool FtpConnector::CreateFtpConnection(LPCSTR szIP, USHORT usPort, LPCSTR szUserName, LPCSTR szPassword, bool bEnableUtf8, BOOL bPassive)
{
    RESET_ERRMSG;
    try
    {	//该函数操作失败会直接抛异常,不需要检查返回值
        m_pConnection = m_objSession.GetFtpConnection(
            szIP, szUserName, szPassword, (INTERNET_PORT)usPort, bPassive);

        m_bEnableUtf8 = bEnableUtf8; //GetCurrentDirectory()正确执行的前提
        CString strFtpRootDir = GetFtpCurrentDir();
        if ("ERROR" == strFtpRootDir)
        {
            m_bEnableUtf8 = false;
            SafeCloseConnection();
            return false;
        }
        CkCommon::FixSlash_FtpRemoteDirPath(strFtpRootDir);
        m_strRootDir = strFtpRootDir.GetBuffer(0);
        return true;
    }
    catch (CInternetException *pEx)
    {
        m_bEnableUtf8 = false;
        SafeCloseConnection();
        InternetExceptionErrorOccured(pEx);
        return false;
    }
}

LPCSTR FtpConnector::GetFtpCurrentDir()
{
    RESET_ERRMSG;
    LPCTSTR szDefaultRetStr = "ERROR";

    if (!m_pConnection)
    {
        SET_LAST_ERRMSG("m_pConnection is NULL");
        return szDefaultRetStr;
    }

    CString strCurDir;
    try
    {
        if (!m_pConnection->GetCurrentDirectory(strCurDir))
        {
            DWORD dwErrCode = ::GetLastError();
            std::string strErrMsg = GetFtpApiFailedErrMsg(dwErrCode);
            SET_LAST_ERRMSG("GetCurrentDirectory failed, %s, err code: %d", strErrMsg.c_str(), dwErrCode);
            return szDefaultRetStr;
        }
    }
    catch (CInternetException *pEx)
    {
        InternetExceptionErrorOccured(pEx);
        return szDefaultRetStr;
    }

    if (m_bEnableUtf8)
    {
        char* pErr = NULL;
        if (!StringConvert::StrConv_Utf82A(strCurDir, pErr))
        {
            SET_LAST_ERRMSG("Convert UTF8 to ANSI failed, err msg: %s", pErr);
            return szDefaultRetStr;
        }
    }
    m_strCurrentDir = strCurDir.GetBuffer(0);
    return m_strCurrentDir.c_str();
}

bool FtpConnector::SetFtpCurrentDir(LPCSTR szDirPath)
{
    RESET_ERRMSG;
    if (!m_pConnection)
    {
        SET_LAST_ERRMSG("m_pConnection is NULL");
        return false;
    }
    if (!szDirPath) szDirPath = "/";

    CString strDirPath(m_strRootDir.c_str());
    strDirPath += szDirPath;
    CkCommon::FixSlash_FtpRemoteDirPath(strDirPath);
    strDirPath.TrimRight('/');

    if (m_bEnableUtf8)
    {
        char* pErr = NULL;
        if (!StringConvert::StrConv_A2Utf8(strDirPath, pErr))
        {
            SET_LAST_ERRMSG("Convert ANSI to UTF8 failed, err msg: %s", pErr);
            return false;
        }
    }

    try
    {
        if (m_pConnection->SetCurrentDirectory(strDirPath))
            return true;

        DWORD dwErrCode = ::GetLastError();
        std::string strErrMsg = GetFtpApiFailedErrMsg(dwErrCode);
        SET_LAST_ERRMSG("SetCurrentDirectory failed, %s, err code: %d", strErrMsg.c_str(), dwErrCode);
    }
    catch (CInternetException *pEx)
    {
        InternetExceptionErrorOccured(pEx);
    }
    return false;
}

bool FtpConnector::FtpRemoveFile(LPCSTR szRemoteDirPath, LPCSTR szFileName)
{
    RESET_ERRMSG;
    if (!m_pConnection)
    {
        SET_LAST_ERRMSG("m_pConnection is NULL");
        return false;
    }
    if (!szFileName || !strlen(szFileName))
    {
        SET_LAST_ERRMSG("File name is NULL or empty");
        return false;
    }
    if (!szRemoteDirPath || !strlen(szRemoteDirPath))
        szRemoteDirPath = "/";

    CString strRemoteFilePath = CString(m_strRootDir.c_str()) + szRemoteDirPath + "/" + szFileName;
    CkCommon::FixSlash_FtpRemoteFilePath(strRemoteFilePath);

    if (m_bEnableUtf8)
    {
        char* pErr = NULL;
        if (!StringConvert::StrConv_A2Utf8(strRemoteFilePath, pErr))
        {
            SET_LAST_ERRMSG("Convert remote file path to UTF8 failed, err msg: %s", pErr);
            return false;
        }
    }

    try
    {
        if (m_pConnection->Remove(strRemoteFilePath))
            return true;

        DWORD dwErrCode = ::GetLastError();
        std::string strErrMsg = GetFtpApiFailedErrMsg(dwErrCode);
        SET_LAST_ERRMSG("Remove failed, file: %s, err msg: %s, err code: %d",
            szFileName, strErrMsg.c_str(), dwErrCode);
    }
    catch (CInternetException* pEx)
    {
        InternetExceptionErrorOccured(pEx);
    }
    return false;
}

bool FtpConnector::FtpDownloadFile(LPCSTR szRemoteFilePath, LPCSTR szLocalFilePath/*,
    BOOL bFailIfExist, DWORD dwAttributes, DWORD dwFlags, DWORD_PTR dwContext*/)
{
    RESET_ERRMSG;
    if (!m_pConnection)
    {
        SET_LAST_ERRMSG("m_pConnection is NULL");
        return false;
    }
    if (!szLocalFilePath || !strlen(szLocalFilePath) || !szRemoteFilePath || !strlen(szRemoteFilePath))
    {
        SET_LAST_ERRMSG("File path is NULL or empty");
        return false;
    }

    CString strRemoteFilePath = CString(m_strRootDir.c_str()) + szRemoteFilePath;
    CkCommon::FixSlash_FtpRemoteFilePath(strRemoteFilePath);

    if (m_bEnableUtf8)
    {
        char* pErr = NULL;
        if (!StringConvert::StrConv_A2Utf8(strRemoteFilePath, pErr))
        {
            SET_LAST_ERRMSG("Convert remote file path to UTF8 failed, err msg: %s", pErr);
            return false;
        }
    }

    try
    {
        //文件打开失败会直接抛异常，不需要检查返回值
        CInternetFile* pFile = m_pConnection->OpenFile(strRemoteFilePath.GetBuffer(0), GENERIC_READ);

        ofstream ofs(szLocalFilePath, ios::binary | ios::out);
        //打开本地文件的操作不能放在m_pConnection->OpenFile之前，
        //否则当远程文件打开失败时，本地就已经创建了一个空文件
        if (ofs.fail())
        {
            pFile->Close();
            SET_LAST_ERRMSG("Create local file failed, file path: %s,", szLocalFilePath);
            return false;
        }

        const int nBuffSize = 102400;
        char buff[nBuffSize] = { 0 };
        while (true)
        {
            //文件Read失败会直接抛异常
            UINT nRet = pFile->Read(buff, nBuffSize);
            if (0 == nRet)
                break; //文件尾

            ofs.write(buff, nRet);
            if (nBuffSize != nRet)
                break; //文件尾
        }

        ofs.close();
        pFile->Close();
        return true;

        /* GetFile的远端路径转成中文UTF8后，奇数个汉字服务器乱码
                if (m_pConnection->GetFile(
                strRemoteFilePath.GetBuffer(0), szLocalFilePath,
                bFailIfExist, dwAttributes, dwFlags, dwContext))
                return true;

                DWORD dwErrCode = ::GetLastError();
                std::string strErrMsg = GetFtpApiFailedErrMsg(dwErrCode);
                SET_LAST_ERRMSG("Download failed, file: %s, err msg: %s, err code: %d",
                szRemoteFilePath, strErrMsg.c_str(), dwErrCode);*/
    }
    catch (CInternetException *pEx)
    {
        InternetExceptionErrorOccured(pEx);
    }
    return false;
}

bool FtpConnector::FtpUploadFile(LPCSTR szLocalFilePath, LPCSTR szRemoteFilePath)
{
    RESET_ERRMSG;
    if (!m_pConnection)
    {
        SET_LAST_ERRMSG("m_pConnection is NULL");
        return false;
    }
    if (!szLocalFilePath || !strlen(szLocalFilePath) || !szRemoteFilePath || !strlen(szRemoteFilePath))
    {
        SET_LAST_ERRMSG("File path is NULL or empty");
        return false;
    }

    CString strRemoteFilePath = CString(m_strRootDir.c_str()) + szRemoteFilePath;
    CkCommon::FixSlash_FtpRemoteFilePath(strRemoteFilePath);

    if (m_bEnableUtf8)
    {
        char* pErr = NULL;
        if (!StringConvert::StrConv_A2Utf8(strRemoteFilePath, pErr))
        {
            SET_LAST_ERRMSG("Convert remote file path to UTF8 failed, err msg: %s", pErr);
            return false;
        }
    }

    ifstream ifs(szLocalFilePath, ios::binary | ios::in); 
    //打开本地文件的操作不能放在m_pConnection->OpenFile之后，
    //否则当本地文件打开失败时，远程就已经创建了一个空文件
    if (ifs.fail())
    {
        SET_LAST_ERRMSG("Read local file failed, file path: %s,", szLocalFilePath);
        return false;
    }

    try
    {
        //文件打开失败会直接抛异常，不需要检查返回值
        CInternetFile* pFile = m_pConnection->OpenFile(strRemoteFilePath.GetBuffer(0), GENERIC_WRITE);

        const int nBuffSize = 102400;
        char buff[nBuffSize] = { 0 };
        while (true)
        {
            ifs.read(buff, nBuffSize);
            std::streamsize nRet = ifs.gcount();
            if (0 == nRet)
                break;  //文件尾
            pFile->Write(buff, (UINT)nRet);
            if (nRet != nBuffSize)
                break;  //文件尾
        }
        pFile->Close();
        ifs.close();
        return true;
    }
    catch (CInternetException *pEx)
    {
        InternetExceptionErrorOccured(pEx);
    }
    ifs.close();
    return false;
}

void FtpConnector::InternetExceptionErrorOccured(CInternetException* pEx)
{
    char szError[255] = { 0 };
    if (!(pEx->GetErrorMessage(szError, 255)))
        ::strcpy(szError, "FTP unknown exception");
    pEx->Delete();
    SET_LAST_ERRMSG(szError);
}

std::string FtpConnector::GetFtpApiFailedErrMsg(DWORD dwErrCode)
{
    DWORD dwErrorSize = 512, dwLastErrorMsg;
    char buff[512] = { 0 };
    if (ERROR_INTERNET_EXTENDED_ERROR == dwErrCode)
        InternetGetLastResponseInfo(&dwLastErrorMsg, buff, &dwErrorSize);

    std::string strRet(buff);
    if (m_bEnableUtf8)
    {
        char* pErr = NULL;
        char* szAnsi = StringUtils::StrConv_Utf82A(buff, pErr);
        strRet = szAnsi;
        delete[] szAnsi;
    }
    return strRet;
}

time_t FtpConnector::FileTime2Timet(const FILETIME& ft)
{
    SYSTEMTIME _LOCALTIME;
    ::FileTimeToSystemTime(&ft, &_LOCALTIME);
    return CTime(_LOCALTIME).GetTime();
}

bool FtpConnector::FtpGetFileInfosInDir(LPCSTR szRemoteDir, LPCSTR szFileName, std::vector<FtpFileInfo>& vecFileInfos)
{
	RESET_ERRMSG;
	if (!m_pConnection)
	{
		SET_LAST_ERRMSG("m_pConnection is NULL");
		return false;
	}
	if (!szRemoteDir || !strlen(szRemoteDir))
	{
		SET_LAST_ERRMSG("File path is NULL or empty");
		return false;
	}
    if (!szFileName)
        szFileName = "*.*";
//    CkLogUtils::RecordingA(g_pLogUtils, LL_INFO, "4");

    CString strRemoteFilePath = 
        CString(m_strRootDir.c_str()) + CString(szRemoteDir) + "/" + CString(szFileName);
//    CkLogUtils::RecordingA(g_pLogUtils, LL_INFO, strRemoteFilePath);

    CkCommon::FixSlash_FtpRemoteFilePath(strRemoteFilePath);


//    CkLogUtils::RecordingA(g_pLogUtils, LL_INFO, strRemoteFilePath);

    if (m_bEnableUtf8)
    {
        char* pErr = NULL;
        if (!StringConvert::StrConv_A2Utf8(strRemoteFilePath, pErr))
        {
            SET_LAST_ERRMSG("Convert remote file path to UTF8 failed, err msg: %s", pErr);
            return false;
        }
//        CkLogUtils::RecordingA(g_pLogUtils, LL_INFO, "Path convert 2 utf8");
    }

    CFtpFileFind fileFinder(m_pConnection);
    FILETIME _UTCTIME;
	FtpFileInfo fileInfo;
	try
	{
        BOOL bFind = fileFinder.FindFile(strRemoteFilePath.GetBuffer(0));
//        CkLogUtils::RecordingA(g_pLogUtils, LL_INFO, "Find file ret %d", bFind);

		while (bFind)
		{
			bFind = fileFinder.FindNextFile();
            
			fileInfo.ullLength = fileFinder.GetLength();
            
            fileFinder.GetCreationTime(&_UTCTIME);
            fileInfo.tCreationTime = FileTime2Timet(_UTCTIME);

            fileFinder.GetLastAccessTime(&_UTCTIME);
            fileInfo.tLastAccessTime = FileTime2Timet(_UTCTIME);

            fileFinder.GetLastWriteTime(&_UTCTIME);
            fileInfo.tLastWriteTime = FileTime2Timet(_UTCTIME);
           			
			CString strFileName = fileFinder.GetFileName();
			CString strFilePath = fileFinder.GetFilePath();
			CString strFileTitle = fileFinder.GetFileTitle();
			CString strFileUrl = fileFinder.GetFileURL();
            CString strRoot = fileFinder.GetRoot();

            bool bRet = true;
            fileInfo.szErrMsg[0] = '\0';
            using namespace StringConvert;
            if (m_bEnableUtf8)
            {
                char* pErr = NULL;
                const char* szFormat = "Convert remote file %s %s to UTF8 failed, err msg: %s";
                do 
                {
                    bRet = StrConv_Utf82A(strFileName, pErr);
                    if (!bRet)
                    {
                        sprintf(fileInfo.szErrMsg, szFormat, "name", strFileName, pErr);
                        break;
                    }
                    bRet = StrConv_Utf82A(strFilePath, pErr);
                    if (!bRet)
                    {
                        sprintf(fileInfo.szErrMsg, szFormat, "path", strFilePath, pErr);
                        break;
                    }
                    bRet = StrConv_Utf82A(strFileTitle, pErr);
                    if (!bRet)
                    {
                        sprintf(fileInfo.szErrMsg, szFormat, "title", strFileTitle, pErr);
                        break;
                    }
                    bRet = StrConv_Utf82A(strFileUrl, pErr);
                    if (!bRet)
                    {
                        sprintf(fileInfo.szErrMsg, szFormat, "url", strFileUrl, pErr);
                        break;
                    }
                    bRet = StrConv_Utf82A(strRoot, pErr);
                    if (!bRet)
                    {
                        sprintf(fileInfo.szErrMsg, szFormat, "root", strRoot, pErr);
                        break;
                    }
                } while (0);                
            }
            fileInfo.bIsOK = bRet;

			StrConv_CStringA2cstr(strFileName, fileInfo.szFileName, PATHBUFFER_LENGTH);
			StrConv_CStringA2cstr(strFilePath, fileInfo.szFilePath, PATHBUFFER_LENGTH);
			StrConv_CStringA2cstr(strFileTitle, fileInfo.szFileTitle, PATHBUFFER_LENGTH);
			StrConv_CStringA2cstr(strFileUrl, fileInfo.szFileUrl, PATHBUFFER_LENGTH);
			StrConv_CStringA2cstr(strRoot, fileInfo.szRoot, PATHBUFFER_LENGTH);

            fileInfo.bIsArchived = IsTRUE(fileFinder.IsArchived());
            fileInfo.bIsCompressed = IsTRUE(fileFinder.IsCompressed());
            fileInfo.bIsDirectory = IsTRUE(fileFinder.IsDirectory());
            fileInfo.bIsDots = IsTRUE(fileFinder.IsDots());
            fileInfo.bIsHidden = IsTRUE(fileFinder.IsHidden());
            fileInfo.bIsNormal = IsTRUE(fileFinder.IsNormal());
            fileInfo.bIsReadOnly = IsTRUE(fileFinder.IsReadOnly());
            fileInfo.bIsSerializable = IsTRUE(fileFinder.IsSerializable());
            fileInfo.bIsSystem = IsTRUE(fileFinder.IsSystem());
            fileInfo.bIsTemporary = IsTRUE(fileFinder.IsTemporary());

            vecFileInfos.push_back(fileInfo);
		}       

        if (!vecFileInfos.size())
        {
            SET_LAST_ERRMSG("Cannot find file %s", szFileName);
            return false;
        }
	    fileFinder.Close();
	    return true;
	}
	catch (CInternetException *pEx)
	{
		InternetExceptionErrorOccured(pEx);
        fileFinder.Close();
        return false;
	}
}

