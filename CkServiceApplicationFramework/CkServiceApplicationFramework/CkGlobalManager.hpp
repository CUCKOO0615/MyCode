#pragma once

#include "CkMacros.h"

#include "LogUtils_ExportC.h"
#pragma comment(lib, "LogUtils.lib")
LogUtils* g_LogUtils = 0;

class CkGlobalManager
{
public:
    CkGlobalManager()
    {
        g_LogUtils = CkLogUtils::CreateLogUtils();
		CkLogUtils::InitLogUtils(g_LogUtils, LOGFILE_PATH);
    }
    ~CkGlobalManager()
    {
        CkLogUtils::ReleaseLogUtils(g_LogUtils);
    }
};
static CkGlobalManager gm;
