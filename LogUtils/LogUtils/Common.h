#pragma once
#include <io.h>
#include <sys/stat.h>
#include <direct.h>
#include <string>
#include <ctime>

namespace CkCommon
{
	// 格式化当前时间为szFormatStr指定格式的字符串, 
	// 格式化方式参见C库的strftime函数
	void FormatTime2Str(char* pBuff, size_t nBuffLen, const char* szFormatStr)
	{
		time_t t;
		::time(&t);
		::strftime(pBuff, nBuffLen, szFormatStr, ::localtime(&t));
	}

	//关闭文件,关闭失败则重试5次
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
    ** 创建多级目录
    ** @Param szDirPath: 
    ** @Ret : 创建成功返回true,失败返回false
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