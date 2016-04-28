#pragma once
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2016/04/02
//��˵����LogUtils��־��
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
    ** ��ʼ����־��
    ** @Param szDirPath: ��־���Ŀ¼
    ** @Param szLogNamePrefix: ��־�ļ�����ǰ׺
    ** @Param bSwitchFileByDate: �Ƿ�ÿ��0���Զ��л������ļ�
    ** @Param nMaxFileSize: �ļ���С,Ĭ��10M,��λ:�ֽ�
	// ���ָ�����ļ���С,���ļ���С�ﵽָ��ֵʱ�Զ��л������ļ�,ͬһ����ļ�,������Ӵ�0��ʼ����ź�׺.
	// ���Ϊ0,�򲻸����ļ���С�л������ļ�.
    ** @Ret: �����ɹ�����true,ʧ�ܷ���false
    */
	bool InitLogUtils(
		const char* szDirPath = ".\\Log\\", const char* szLogNamePrefix = "Log",
		bool bSwitchFileByDate = true, unsigned int nMaxFileSize = 0xA00000 /*10M*/);
	
    /*
    ** ��ָ�����ַ���д����־
    ** @param wszRec: �������־,֧�ָ�ʽ�����
    */
    void Recording(LOG_LEVEL emLL, const wchar_t* wszRec, ...);
    void Recording(LOG_LEVEL emLL, const wchar_t* wszRec, va_list argsList);

    /*
    ** ��ָ�����ַ���д����־
    ** @param szRec: �������־,֧�ָ�ʽ�����
    */
    void Recording(LOG_LEVEL emLL, const char* szRec, ...);
    void Recording(LOG_LEVEL emLL, const char* szRec, va_list argsList);

    /*
    ** ���һ��60���ַ����ķָ���
    ** @param ch����ɷָ��ߵ��ַ�
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
	void AutoZipOlderLogFiles();	//�Զ�ɾ��,ֻ���������50����־�ļ�
};
