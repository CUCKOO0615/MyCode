//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2015/04/19
//��˵����ʹ��Critical Sectionʵ�ֵ�ͬ����
//*************************************************

#pragma once
#include <windows.h>

#if _MSC_VER<=1200
#define  _WIN32_WINNT 0x0500
#endif

/*
** �ֶ���
** ��:
** SyncLock sl;
** sl.Lock();
** ..do sth..
** sl.Unlock();
*/
class SyncLock
{
public:
	void Lock()                     { ::EnterCriticalSection(&m_cs); }
	void Unlock()                   { ::LeaveCriticalSection(&m_cs); }
public:
	SyncLock()                      { ::InitializeCriticalSection(&m_cs); }
	SyncLock(DWORD dwSpinCount)     { ::InitializeCriticalSectionAndSpinCount(&m_cs, dwSpinCount); }
	~SyncLock()                     { ::DeleteCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION m_cs;
};

class SyncMutex
{
public:
    void Lock(DWORD dwTimeOut = INFINITE)    { if (MutexValid()) ::WaitForSingleObject(m_hlMutex, dwTimeOut); }
    void Unlock()                            { if (MutexValid()) ::ReleaseMutex(m_hlMutex); }
public:
    SyncMutex(const char* szMutexName = NULL) :
        m_hlMutex(INVALID_HANDLE_VALUE)      {
        m_hlMutex = ::CreateMutexA(NULL, FALSE, szMutexName);
    }
    ~SyncMutex()                             { if (MutexValid()) ::CloseHandle(m_hlMutex); }
private:
    HANDLE m_hlMutex;
    bool MutexValid() { return (m_hlMutex && (INVALID_HANDLE_VALUE != m_hlMutex)); }
};

/*
** �Զ���
** ��:
** SyncLock sl;
** SyncLockGuard slg(&sl);
** ..do sth..
** ���ֶ�����slg.ManualUnlock(), ��ȴ���������ʱ�Զ�����
*/
class SyncLockGuard
{
public:
    SyncLockGuard(SyncLock* pLock):m_pLock(pLock)   { if (m_pLock) m_pLock->Lock(); }
    ~SyncLockGuard()                                { if (m_pLock) m_pLock->Unlock(); m_pLock = NULL; }
    void ManualUnlock()                             { if (m_pLock) m_pLock->Unlock(); m_pLock = NULL; }
private:
    SyncLock* m_pLock;
};

/*
** �Զ���
** ��:
** SyncMutex sm;
** SyncMutexGuard smg(&sm);
** ..do sth..
** ���ֶ�����smg.ManualUnlock(), ��ȴ���������ʱ�Զ�����
*/
class SyncMutexGuard
{
public:
    SyncMutexGuard(SyncMutex* pMutex):m_pMutex(pMutex)  { if (m_pMutex) m_pMutex->Lock(); }
    ~SyncMutexGuard()                                   { if (m_pMutex) m_pMutex->Unlock(); m_pMutex = NULL; }
    void ManualUnlock()                                 { if (m_pMutex) m_pMutex->Unlock(); m_pMutex = NULL; }
private:
    SyncMutex* m_pMutex;
};
