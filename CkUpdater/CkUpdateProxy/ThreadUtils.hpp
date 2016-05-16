//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014��12��12��
//��˵������װ���̹߳�����
//*************************************************
#pragma once
#include <windows.h>
#include <process.h>

//�߳���ں�����������
#define TU_DECLEAR_THREADENTRY(func_name) \
	unsigned int __stdcall func_name(void* pParam)

typedef unsigned int(_stdcall *TU_LPTHREADENTRY)(void*);

class ThreadUtils
{
	//////////////////////////////////////////////////////////////////////////
public:
    /*
    ** ����Ҫ���ݸ��̵߳Ĳ����б�
    ** @param pParams: ָ��Ҫ���ݸ��̵߳� �����б��ָ��
    */
    void SetThreadParams(void* pParams);

    /*
    ** �����߳�
    ** @param pThreadEntry: ָ���߳���ں�����ָ��
    ** @return: �����ɹ�����true, ����ʧ�ܷ���false
	*/
	bool Run(TU_LPTHREADENTRY pThreadEntry);

	/*
	** ��ȡ�߳��˳���
	** �߳�δ�������޷���ȡ�˳���: ����0xFFFFFFFF
	** �߳���������: ����STILL_ACTIVE (259)
	** �߳����˳�: �����̺߳����ķ���ֵ, ����ExitThread��TerminateThreadָ���Ĵ�����
	*/
	DWORD GetExitCode() const;

    // ��ȡ�߳̾��
	HANDLE GetThreadHandle() const;

    // ��ȡ�߳�ID
    unsigned int GetThreadID() const;

    // ������Դ�����³�ʼ��
    void Clean();
	
	//////////////////////////////////////////////////////////////////////////

public:
	ThreadUtils() : m_uThreadID(0), m_hThreadHandle(0), m_pArgList(NULL)	{	}
	~ThreadUtils(){}

private:
	unsigned int m_uThreadID;
	HANDLE m_hThreadHandle;
	void* m_pArgList;

	ThreadUtils(ThreadUtils& t){}
};

//////////////////////////////////////////////////////////////////////////
// inline
inline void ThreadUtils::SetThreadParams(void* pParams)
{
	m_pArgList = pParams;
}

inline bool ThreadUtils::Run(TU_LPTHREADENTRY pThreadEntry)
{
	m_hThreadHandle = (HANDLE)::_beginthreadex(NULL, 0, pThreadEntry, m_pArgList, 0, &m_uThreadID);
	return (m_hThreadHandle&&m_uThreadID);
}

inline DWORD ThreadUtils::GetExitCode() const
{
	if (!m_hThreadHandle || !m_uThreadID) return 0xFFFFFFFF;
	DWORD dwExitCode = 0;
	BOOL bRet = ::GetExitCodeThread(m_hThreadHandle, &dwExitCode);
	return bRet? dwExitCode : 0xFFFFFFFF;
}

inline HANDLE ThreadUtils::GetThreadHandle() const
{
    return m_hThreadHandle;
}

inline unsigned int ThreadUtils::GetThreadID() const
{
    return m_uThreadID;
}

inline void ThreadUtils::Clean()
{
    ::WaitForSingleObject(m_hThreadHandle, INFINITE);
    ::CloseHandle(m_hThreadHandle);
    m_hThreadHandle = 0;

    m_uThreadID = 0;

    m_pArgList = NULL;
}
