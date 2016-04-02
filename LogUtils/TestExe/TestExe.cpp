// TestExe.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../LogUtils/LogUtils_ExportC.h"

#ifdef _DEBUG
#pragma comment(lib, "../debug/LogUtils.lib")
#else
#pragma comment(lib, "../Release/LogUtils.lib")
#endif


int _tmain(int argc, _TCHAR* argv[])
{

	LogUtils* pLogUtils = CkLogUtils::CreateLogUtils();
	if (!pLogUtils)
		return 1;
	if (!CkLogUtils::InitLogUtils(pLogUtils, ".\\Log\\", "Test"))
		return 2;

	while (1)
	{
		CkLogUtils::Recording(pLogUtils, LL_WARN, "Test formatA:%s%s_%d", "������", "AAATTT", 10086);

		CkLogUtils::RecordingW(pLogUtils, LL_WARN, L"Test formatW:%s%s_%d", L"������", L"AAATTT", 10086);
		
		CkLogUtils::SperateLine(pLogUtils, '=');
	}
	CkLogUtils::ReleaseLogUtils(pLogUtils);


	return 0;
}

