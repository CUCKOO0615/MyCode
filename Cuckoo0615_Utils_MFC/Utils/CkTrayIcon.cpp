#include "stdafx.h"
#include "CkTrayIcon.h"

CkTrayIcon::CkTrayIcon() : 
m_bIsInited(false),
m_uTrayIconID(1), m_bAniPlaying(false),
m_hAniThreadHandle(0), m_uAniThreadID(0),
m_nAniCurFrame(0), m_nAniInterval(100)
{
  
}

CkTrayIcon::~CkTrayIcon(void)
{
    m_bAniPlaying = false;
    
    if(m_bIsInited)
        DelIcon();
    m_bIsInited = false;

    if(0 != m_hAniThreadHandle)
    {
        CloseHandle(m_hAniThreadHandle);
        m_hAniThreadHandle = 0;
    }
}

bool CkTrayIcon::Init(HWND hWnd, HICON* arrIcons, size_t nIconNum, TCHAR szToolTip[128])
{
    if(0 == hWnd || 0 == arrIcons || 0 == nIconNum)
    {
        SetErrMsg("Invalid arguments");
        return false;
    }

    if (nIconNum > CKTRAYICONNUM)
    {
        SetErrMsg("Icon num should not be larger than CKTRAYICONNUM");
        return false;
    }
    
    m_nIconsNum = 0;
    for (; m_nIconsNum != nIconNum; ++m_nIconsNum)
    {
        m_hIcons[m_nIconsNum] = arrIcons[m_nIconsNum];
        if (0 == m_hIcons[m_nIconsNum])
        {
            SetErrMsg("Load icons failed.");
            return false;
        }
    }

    m_tnid.cbSize = sizeof(NOTIFYICONDATA);
    m_tnid.hWnd = hWnd;
    m_tnid.uID = m_uTrayIconID;
    m_tnid.uFlags = NIF_ICON | NIF_TIP;
    m_tnid.hIcon = m_hIcons[0];
    _tcscpy(m_tnid.szTip, szToolTip);

    m_bIsInited = true;
    return true;
}

bool CkTrayIcon::AddIcon()
{
    if(!CheckInit())
        return false;

    BOOL bRet = Shell_NotifyIcon(NIM_ADD, &m_tnid);
    if (FALSE == bRet)
    {
        SetErrMsg("Add icon failed.");
        return false;
    }
    return true;
}

bool CkTrayIcon::DelIcon()
{
    if(!CheckInit())
        return false;

    BOOL bRet = Shell_NotifyIcon(NIM_DELETE, &m_tnid);
    if (FALSE == bRet)
    {
        SetErrMsg("Delete icon failed.");
        return false;
    }
    return true;
}

bool CkTrayIcon::ModifyIcon(size_t nIconIndex)
{
    if(!CheckInit())
        return false;

    if (nIconIndex > m_nIconsNum)
    {
        SetErrMsg("nIconIndex should not be larger than icon number.");
        return false;
    }

    m_tnid.hIcon = m_hIcons[nIconIndex];
    BOOL bRet = Shell_NotifyIcon(NIM_MODIFY, &m_tnid);
    if (FALSE == bRet)
    {
        SetErrMsg("Modify icon failed.");
        return false;
    }
    return true;
}

static unsigned int __stdcall AnimationThreadEntry(void* pParam)
{
    CkTrayIcon* pThis = (CkTrayIcon*)pParam;
    while (pThis->IsAniPlaying())
    {
        pThis->PlayNextFrame();
        Sleep(pThis->GetAniInterval());
    }
    return 0;
}

void CkTrayIcon::PlayNextFrame()
{
    if(!CheckInit())
        return;

    ++m_nAniCurFrame;
    if (m_nAniCurFrame >= m_nIconsNum)
        m_nAniCurFrame = 0;
    m_tnid.hIcon = m_hIcons[m_nAniCurFrame];
    Shell_NotifyIcon(NIM_MODIFY, &m_tnid);
}

bool CkTrayIcon::PlayAnimation(size_t nMilliseconds)
{
    m_bAniPlaying = true;
    m_nAniInterval = nMilliseconds;
    m_hAniThreadHandle = (HANDLE)_beginthreadex(NULL, 0, AnimationThreadEntry, this, 0, &m_uAniThreadID);
    if (0 == m_hAniThreadHandle)
    {
        SetErrMsg("Create playing thread failed.");
        return false;
    }
    return true;
}

void CkTrayIcon::StopAnimation()
{
    m_bAniPlaying = false;
    CloseHandle(m_hAniThreadHandle);
    m_hAniThreadHandle = 0;
}

bool CkTrayIcon::CheckInit()
{
    if(m_bIsInited)
        return true;
    SetErrMsg("Uninitialized.");
    return false;
}
