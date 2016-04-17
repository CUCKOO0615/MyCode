#include "stdafx.h"
#define  EXPORT_STDC
#include "FtpUtils_ExportC.h"
#include "StringUtils.h"
#include "StringConvert.h"
#include "FtpConnector.h"
#include "PtrUtils.hpp"

CK_API FtpConnector* CkFtpUtils::CreateFtpConnector(const char* szSessionName, int nDelay /*= 5000*/)
{
    FtpConnector* pRet = new FtpConnector(szSessionName, nDelay);
    return pRet;
}

CK_API void CkFtpUtils::ReleaseFtpConnector(FtpConnector* pFtpConn)
{
    if(pFtpConn)
        delete pFtpConn;
}

CK_API const char* CkFtpUtils::GetLastErrMsg(FtpConnector* pConnector)
{
	ASSERT(pConnector);
    return pConnector->GetLastErrMsg();
}

CK_API bool CkFtpUtils::ConnectFtp(FtpConnector* pConnector, 
	const char* szIP, unsigned short usPort /*= 21*/, 
	const char* szUserName /*= "anonymous"*/, const char* szPassword /*= ""*/,
    bool bEnableUtf8/* = false*/, bool bPassive /*= false*/)
{
	ASSERT(pConnector);
    return pConnector->CreateFtpConnection(szIP, usPort, szUserName, szPassword, bEnableUtf8, bPassive);
}


CK_API bool CkFtpUtils::SetFtpCurrentDir(FtpConnector* pConnector, const char* szDirPath)
{
	ASSERT(pConnector);
	return pConnector->SetFtpCurrentDir(szDirPath);
}

CK_API const char* CkFtpUtils::GetFtpCurrentDir(FtpConnector* pConnector)
{
	ASSERT(pConnector);
	return pConnector->GetFtpCurrentDir();
}

CK_API bool CkFtpUtils::FtpRemoveFile(FtpConnector* pConnector, const char* szRemotePath, const char* szFileName)
{
	ASSERT(pConnector);
	return pConnector->FtpRemoveFile(szRemotePath, szFileName);
}

CK_API const char* CkFtpUtils::GetFtpRootDir(FtpConnector* pConnector)
{
	ASSERT(pConnector);
	return pConnector->GetFtpRootDir();
}

CK_API bool CkFtpUtils::FtpDownloadFile(FtpConnector* pConnector,
	const char* szRemoteFilePath, const char* szLocalFilePath)
{
	ASSERT(pConnector);
	return pConnector->FtpDownloadFile(szRemoteFilePath, szLocalFilePath);
}

CK_API bool CkFtpUtils::FtpUploadFile(FtpConnector* pConnector,
    const char* szLocalFilePath, const char* szRemoteFilePath)
{
    ASSERT(pConnector);
    return pConnector->FtpUploadFile(szLocalFilePath, szRemoteFilePath);
}

CK_API bool CkFtpUtils::FtpGetFileInfosInDir(FtpConnector* pConnector, 
	const char* szRemoteDir, FtpFileInfo** arrFileInfos, int* nArrSize)
{
	ASSERT(pConnector);

	std::vector<FtpFileInfo> vecFileInfos;
	bool bRet = pConnector->FtpGetFileInfosInDir(szRemoteDir, vecFileInfos);
	if (!bRet)
		return false;

	int nVecSize = vecFileInfos.size();
	*nArrSize = nVecSize;
	*arrFileInfos = new FtpFileInfo[nVecSize];
	::memset(*arrFileInfos, 0, nVecSize*sizeof(FtpFileInfo));
	::memcpy(*arrFileInfos, &vecFileInfos[0], nVecSize*sizeof(FtpFileInfo));
	return true;
}



