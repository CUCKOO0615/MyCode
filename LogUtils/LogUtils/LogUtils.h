#pragma once
//************************************************
//◇作者：CUCKOO0615
//◇日期：2016/04/02
//◇说明：LogUtils日志类
//*************************************************
#pragma warning(disable:4099)
#pragma warning(disable:4996)

#include <string>
#include "LogUtils_ExportC.h"
#include "ThreadUtils.hpp"
#include "SyncLock.hpp"

#ifndef CK_API
typedef enum emLogLevel
{
    LL_INFO, LL_WARN, LL_ERROR
}LOG_LEVEL;
#endif

class LogUtils
{
public:   
    /*
    ** 初始化日志类
    ** @Param szDirPath: 日志存放目录
    ** @Param szLogNamePrefix: 日志文件名的前缀
    ** @Param bSwitchFileByDate: 是否每天0点自动切换到新文件
    ** @Param nMaxFileSize: 文件大小,默认10M,单位:字节
	// 如果指定了文件大小,则文件大小达到指定值时自动切换到新文件,同一天的文件,依次添加从0开始的序号后缀.
	// 如果为0,则不根据文件大小切换到新文件.
    ** @Ret: 操作成功返回true,失败返回false
    */
	bool InitLogUtils(
		const char* szDirPath = ".\\Log\\", const char* szLogNamePrefix = "Log",
		bool bSwitchFileByDate = true, unsigned int nMaxFileSize = 0xA00000 /*10M*/);
	
    /*
    ** 将指定的字符串写入日志
    ** @param wszRec: 输出的日志,支持格式化输出
    */
    void Recording(LOG_LEVEL emLL, const wchar_t* wszRec, ...);
    void Recording(LOG_LEVEL emLL, const wchar_t* wszRec, va_list argsList);

    /*
    ** 将指定的字符串写入日志
    ** @param szRec: 输出的日志,支持格式化输出
    */
    void Recording(LOG_LEVEL emLL, const char* szRec, ...);
    void Recording(LOG_LEVEL emLL, const char* szRec, va_list argsList);

    /*
    ** 输出一条60个字符长的分割线
    ** @param ch：组成分割线的字符
    */
    void SperateLine(char ch = '-');
	
public:
	LogUtils();
	~LogUtils();

private:
	bool m_isInited;

	FILE * m_pLogFile;
	std::string m_strLogFilePath;
	std::string m_strLogDirPath;
	unsigned int m_nFileMaxSize;
	bool m_bSwitchFileByDate;

	SyncLock m_slSyncLock;
	ThreadUtils m_thrUtils;
	bool m_bthrUtilsFlag;
	
	LogUtils(const LogUtils& lu){}	
	static unsigned int __stdcall SwitchLogFile_ThreadEntry(void* pParam);
	unsigned int m_nLogIndex;
	struct _stat _stuFileState;
	void SwitchLogFileByFileSize();
	void SwitchLogFileByDate();
	void AutoZipOlderLogFiles();	//自动删除,只保留最近的50个日志文件
};
