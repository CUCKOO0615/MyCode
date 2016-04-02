#pragma once
#include <io.h>
#include <sys/stat.h>
#include <direct.h>
#include <string>
#include <ctime>

namespace CkCommon
{
	// ��ʽ����ǰʱ��ΪszFormatStrָ����ʽ���ַ���, 
	// ��ʽ����ʽ�μ�C���strftime����
	void FormatTime2Str(char* pBuff, size_t nBuffLen, const char* szFormatStr)
	{
		time_t t;
		::time(&t);
		::strftime(pBuff, nBuffLen, szFormatStr, ::localtime(&t));
	}

	//�ر��ļ�,�ر�ʧ��������5��
	void SafeCloseLogFile(FILE*& pLogFile)
	{
		int i = 0;
		while (i < 5)
		{
			if (0 == ::fclose(pLogFile))
				break;
			else
			{
				Sleep(100);
				++i;
			}
		}
		pLogFile = NULL;
	}

    /*
    ** �����༶Ŀ¼
    ** @Param szDirPath: 
    ** @Ret : �����ɹ�����true,ʧ�ܷ���false
    */
    bool CreateMultiDir(const char* szDirPath)
    {
        if(!szDirPath) return false;
        size_t nLength = strlen(szDirPath);
        if(!nLength) return false;

        for (int i = 0; i != nLength; ++i)
        {
            if ('/' == szDirPath[i] ||
                '\\' == szDirPath[i] ||
                '\0' == szDirPath[i])
            {
                std::string strTmp(szDirPath, szDirPath + i);
                const char* szTmp = strTmp.c_str();
                if (0 == ::_access(szTmp, 0))
                    continue;
                else if (0 != ::_mkdir(szTmp))
                    return false;
            }
        }
        return true;
    }
    
}