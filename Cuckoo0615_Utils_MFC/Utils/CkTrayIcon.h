
//************************************************
//◇作者：CUCKOO0615
//◇日期：2014/12/26 15:00:24
//◇说明：托盘图标显示类
//************************************************
/************************************************************************/
/*                                Demo                                  */
/************************************************************************
HICON arrIcons[] = {AfxGetApp()->LoadIcon(IDR_MAINFRAME),
AfxGetApp()->LoadIcon(IDI_ICON1),
AfxGetApp()->LoadIcon(IDI_ICON2),
AfxGetApp()->LoadIcon(IDI_ICON3)};
m_ckTrayIcon.Init(this->GetSafeHwnd(), arrIcons, 4, _T("测试托盘"));
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
    ** 获取错误信息
    */
    const char* GetErrMsg();

    /*
    ** 初始化 CkTrayIcon 类
    ** @param hWnd: 父窗体句柄
    ** @param arrIcons: 用于在托盘区显示的图标数组, 当图标数量不止一个时可实现动画播放
    ** @param nIconNum: arrIcons的元素数量, 最多支持8个图标
    ** @param szToolTip[128]: 托盘图标上要显示的ToolTip字符串,最多128个字符(包括\0结尾)
    */
    bool Init(HWND hWnd, HICON* arrIcons, size_t nIconNum, TCHAR szToolTip[128]);

    /*
    **托盘区添加/移除图标
    */
    bool AddIcon();
    bool DelIcon();

    /*
    ** 修改要显示的图标
    ** @param nIconIndex: 图标索引
    */
    bool ModifyIcon(size_t nIconIndex);

    /*
    ** 播放托盘图标动画
    ** @param nMilliseconds: 动画帧时间间隔,单位 毫秒
    */
    bool PlayAnimation(size_t nMilliseconds = 100);

    /*
    ** 停止播放托盘图标动画
    */
    void StopAnimation();

    /*
    ** 显示托盘图标的下一帧
    */
    void PlayNextFrame();

    /*
    ** 获取动画播放状态
    */
    bool IsAniPlaying();

    /*
    ** 获取动画帧时间间隔
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
// Inline 实现
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
