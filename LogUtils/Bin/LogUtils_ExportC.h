#pragma once
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2016/04/02
//��˵����LogUtils��־�� C�����ӿ�
//*************************************************

#ifdef EXPORT_STDC
#define CK_API extern "C" __declspec(dllexport)
#else
#define CK_API extern "C" __declspec(dllimport)
#endif

//��־��Ϣ����
enum LOG_LEVEL
{
    LL_INFO, //��ͨ
	LL_WARN, //����
	LL_ERROR, //����
    LL_DEBUG  //����
};

struct LogUtils;
namespace CkLogUtils
{
    /*
    ** ����ָ��LogUtils�����ָ��
    ** @Ret: ����ʧ�ܷ���NULL
    */
    CK_API LogUtils* CreateLogUtils();
	
    /*
    ** �ͷ���CreateLogUtils()����������LogUtils����
    ** @Param p: ��Ч��LogUtils����ָ��
    */
    CK_API void ReleaseLogUtils(LogUtils* p);
	
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
	CK_API bool InitLogUtils(LogUtils* p,
		const char* szDirPath = ".\\Log\\", 
		const char* szLogNamePrefix = "Log",
		bool bSwitchFileByDate = true,
		unsigned int nMaxFileSize = 0xA00000 /*10M*/);

    /*
    ** ��ָ�����ַ���д����־(���ֽڲ���)
    ** @Param p: ��Ч��LogUtils����ָ��
    ** @Param emLL: LOG_LEVELö��/LL_INFO/LL_WARN/LL_ERROR
    ** @Param szRec: �������־,֧�ָ�ʽ�����
    */
    CK_API void RecordingA(LogUtils* p, LOG_LEVEL emLL, const char* szRec, ...);
    
	/*
	** ��ָ�����ַ���д����־(���ֽڲ���)
	** @Param p: ��Ч��LogUtils����ָ��
	** @Param emLL: LOG_LEVELö��/LL_INFO/LL_WARN/LL_ERROR
	** @param wszRec: �������־,֧�ָ�ʽ�����
	*/
    CK_API void RecordingW(LogUtils* p, LOG_LEVEL emLL, const wchar_t* wszRec, ...);

	/*
	** ���һ��60���ַ����ķָ���
	** @Param p: ��Ч��LogUtils����ָ��
	** @param ch����ɷָ��ߵ��ַ�
	*/
    CK_API void SperateLine(LogUtils* p, char ch = '-');
}
