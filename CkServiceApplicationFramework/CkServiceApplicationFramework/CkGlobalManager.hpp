#pragma once

#include "LogUtils_ExportC.h"
#pragma comment(lib, "LogUtils.lib")
LogUtils* gLogUtils = NULL;

class CkGlobalManager
{
public:
    CkGlobalManager()
    {
        gLogUtils = CkLogUtils::CreateLogUtils();
        CkLogUtils::InitLogUtils(gLogUtils, "..\\..\\CkServiceLog\\");
    }
    ~CkGlobalManager()
    {
        CkLogUtils::ReleaseLogUtils(gLogUtils);
    }
};
static CkGlobalManager gm;