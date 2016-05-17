#pragma once
//************************************************
//◇作者：CUCKOO0615
//◇日期：2016/04/02
//◇说明：LogUtils日志类 C导出接口
//*************************************************

#ifdef EXPORT_STDC
#define CK_API extern "C" __declspec(dllexport)
#else
#define CK_API extern "C" __declspec(dllimport)
#endif

//日志信息类型
enum LOG_LEVEL
{
    LL_INFO, //普通
	LL_WARN, //警告
	LL_ERROR, //错误
    LL_DEBUG  //调试
};

struct LogUtils;
namespace CkLogUtils
{
    /*
    ** 创建指向LogUtils对象的指针
    ** @Ret: 创建失败返回NULL
    */
    CK_API LogUtils* CreateLogUtils();
	
    /*
    ** 释放由CreateLogUtils()函数创建的LogUtils对象
    ** @Param p: 有效的LogUtils对象指针
    */
    CK_API void ReleaseLogUtils(LogUtils* p);
	
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
	CK_API bool InitLogUtils(LogUtils* p,
		const char* szDirPath = ".\\Log\\", 
		const char* szLogNamePrefix = "Log",
		bool bSwitchFileByDate = true,
		unsigned int nMaxFileSize = 0xA00000 /*10M*/);

    /*
    ** 将指定的字符串写入日志(多字节参数)
    ** @Param p: 有效的LogUtils对象指针
    ** @Param emLL: LOG_LEVEL枚举/LL_INFO/LL_WARN/LL_ERROR
    ** @Param szRec: 输出的日志,支持格式化输出
    */
    CK_API void RecordingA(LogUtils* p, LOG_LEVEL emLL, const char* szRec, ...);
    
	/*
	** 将指定的字符串写入日志(宽字节参数)
	** @Param p: 有效的LogUtils对象指针
	** @Param emLL: LOG_LEVEL枚举/LL_INFO/LL_WARN/LL_ERROR
	** @param wszRec: 输出的日志,支持格式化输出
	*/
    CK_API void RecordingW(LogUtils* p, LOG_LEVEL emLL, const wchar_t* wszRec, ...);

	/*
	** 输出一条60个字符长的分割线
	** @Param p: 有效的LogUtils对象指针
	** @param ch：组成分割线的字符
	*/
    CK_API void SperateLine(LogUtils* p, char ch = '-');
}
