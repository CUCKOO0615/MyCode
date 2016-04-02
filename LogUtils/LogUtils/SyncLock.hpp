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
	SyncLock(SyncLock&){};
};

/*
** �Զ���
** ��:
** SyncLock sl;
** SyncLockGuard slg(&sl);
** ..do sth..
** ���ֶ�����slg.ManualUnlock()
** ���������ʱ�Զ�����
*/
class SyncLockGuard
{
public:
    SyncLockGuard(SyncLock* pLock) :m_pLock(pLock)   { if (m_pLock) m_pLock->Lock(); }
    ~SyncLockGuard()                                 { if (m_pLock) m_pLock->Unlock(); }

    void ManualUnlock()
    {
        if (m_pLock) m_pLock->Unlock();
        m_pLock = NULL;
    }

private:
    SyncLock* m_pLock;
};