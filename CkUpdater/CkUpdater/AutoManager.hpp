#pragma once
#include "SocketUtils.h"
#include <iostream>

static bool g_bGlobalInited = false;
static SOCKET g_sClient = INVALID_SOCKET;

class AutoInitialize
{
public:
    AutoInitialize()
    {
        int nErrCode = 0;
        if (!SocketUtils::InitSocketUtils(nErrCode))
        {
            std::cout << SocketUtils::QueryErrMsg(nErrCode) << std::endl;
            system("pause");
        }
        g_bGlobalInited = true;
    }
};


class AutoCleaner
{
public:
    ~AutoCleaner()
    {
        if (g_sClient != INVALID_SOCKET)
        {
            ::closesocket(g_sClient);
            g_sClient = INVALID_SOCKET;
        }
        SocketUtils::ReleaseSocketUtils();
    }
};

static AutoInitialize ai;
static AutoCleaner ac;