
//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014/12/26 15:00:24
//��˵��������ͼ����ʾ��
//************************************************
/************************************************************************/
/*                                Demo                                  */
/************************************************************************
HICON arrIcons[] = {AfxGetApp()->LoadIcon(IDR_MAINFRAME),
AfxGetApp()->LoadIcon(IDI_ICON1),
AfxGetApp()->LoadIcon(IDI_ICON2),
AfxGetApp()->LoadIcon(IDI_ICON3)};
m_ckTrayIcon.Init(this->GetSafeHwnd(), arrIcons, 4, _T("��������"));
m_ckTrayIcon.AddIcon();
m_ckTrayIcon.PlayAnimation(2000);
************************************************************************/

#pragma once
#include <windows.h>

#define CKTRAYICONNUM 8

class CkTrayIcon
{
public:
    CkTrayIcon();
    ~CkTrayIcon(void);

    /*
    ** ��ȡ������Ϣ
    */
    const char* GetErrMsg();

    /*
    ** ��ʼ�� CkTrayIcon ��
    ** @param hWnd: ��������
    ** @param arrIcons: ��������������ʾ��ͼ������, ��ͼ��������ֹһ��ʱ��ʵ�ֶ�������
    ** @param nIconNum: arrIcons��Ԫ������, ���֧��8��ͼ��
    ** @param szToolTip[128]: ����ͼ����Ҫ��ʾ��ToolTip�ַ���,���128���ַ�(����\0��β)
    */
    bool Init(HWND hWnd, HICON* arrIcons, size_t nIconNum, TCHAR szToolTip[128]);

    /*
    **���������/�Ƴ�ͼ��
    */
    bool AddIcon();
    bool DelIcon();

    /*
    ** �޸�Ҫ��ʾ��ͼ��
    ** @param nIconIndex: ͼ������
    */
    bool ModifyIcon(size_t nIconIndex);

    /*
    ** ��������ͼ�궯��
    ** @param nMilliseconds: ����֡ʱ����,��λ ����
    */
    bool PlayAnimation(size_t nMilliseconds = 100);

    /*
    ** ֹͣ��������ͼ�궯��
    */
    void StopAnimation();

    /*
    ** ��ʾ����ͼ�����һ֡
    */
    void PlayNextFrame();

    /*
    ** ��ȡ��������״̬
    */
    bool IsAniPlaying();

    /*
    ** ��ȡ����֡ʱ����
    */
    size_t GetAniInterval();

private:
    bool m_bIsInited;
    NOTIFYICONDATA m_tnid;
    UINT m_uTrayIconID;

    HICON m_hIcons[CKTRAYICONNUM];
    size_t m_nIconsNum;

    HANDLE m_hAniThreadHandle;
    unsigned int m_uAniThreadID;
    bool m_bAniPlaying;
    size_t m_nAniCurFrame;
    size_t m_nAniInterval;

    char m_szErrMsg[256];
    void SetErrMsg(const char* szErrMsg);
    bool CheckInit();
};

//////////////////////////////////////////////////////////////////////////
// Inline ʵ��
inline bool CkTrayIcon::IsAniPlaying()
{
    return m_bAniPlaying;
}

inline size_t CkTrayIcon::GetAniInterval()
{
    return m_nAniInterval;
}

inline void CkTrayIcon::SetErrMsg(const char* szErrMsg)
{
    strcpy(m_szErrMsg, szErrMsg);
}

inline const char* CkTrayIcon::GetErrMsg()
{
    return m_szErrMsg;
}
