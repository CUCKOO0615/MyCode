#pragma once
#include <Windows.h>

namespace CkServiceMain
{
    void WINAPI ServiceCtrl(DWORD dwOpcode);
    void WINAPI ServiceMain(DWORD argc, LPTSTR * argv);
};

