
// CUCKOO0615_Utils_MFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CUCKOO0615_Utils_MFC.h"
#include "CUCKOO0615_Utils_MFCDlg.h"
#include "afxdialogex.h"
//#include "Utils/CKDbgConsole.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCUCKOO0615_Utils_MFCDlg 对话框




CCUCKOO0615_Utils_MFCDlg::CCUCKOO0615_Utils_MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCUCKOO0615_Utils_MFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCUCKOO0615_Utils_MFCDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    CKTREECTRL_BIND(pDX, IDC_TREE_CTRL, m_ckTreeCtrl);
    DDX_Control(pDX, IDC_TAB1, m_tabTest);
}

BEGIN_MESSAGE_MAP(CCUCKOO0615_Utils_MFCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDOK, &CCUCKOO0615_Utils_MFCDlg::OnBnClickedOk)
    ON_WM_CREATE()
    ON_BN_CLICKED(IDCANCEL, &CCUCKOO0615_Utils_MFCDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


CString CutRelativePathByKeyword(const CString &strFullPath, const char* szKeyword)
{
    CString strRet = strFullPath.Mid(strFullPath.Find("connection"));
    while (0 != strRet.Replace("\\\\", "\\")); // 合并所有连续的反斜线
    strRet.Replace("\\", "\\\\");            // ‘\’全部替换为‘\\’
    strRet = "\\\\" + strRet;               // 添加前缀‘\\’
    return strRet;
}


// CCUCKOO0615_Utils_MFCDlg 消息处理程序

BOOL CCUCKOO0615_Utils_MFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


    CString str = "D:\\tomcat-mds\\webapps\\MDS\\connection\\yb\\\\sz140404.txt";
    CString fileUrl = CutRelativePathByKeyword(str, "connection");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCUCKOO0615_Utils_MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
        
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCUCKOO0615_Utils_MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT AFX_CDECL ThreadEntry(LPVOID lpVoid)
{
	CCUCKOO0615_Utils_MFCDlg* pDlg = (CCUCKOO0615_Utils_MFCDlg*)lpVoid;


	return 0;
}

void CCUCKOO0615_Utils_MFCDlg::OnBnClickedOk()
{
	//AfxBeginThread(ThreadEntry, this);


//     HICON arrIcons[] = {AfxGetApp()->LoadIcon(IDR_MAINFRAME),
//     AfxGetApp()->LoadIcon(IDI_ICON1),
//     AfxGetApp()->LoadIcon(IDI_ICON2),
//     AfxGetApp()->LoadIcon(IDI_ICON3)};
//     m_ckTrayIcon.Init(this->GetSafeHwnd(), arrIcons, 4, _T("测试托盘"));
//     m_ckTrayIcon.AddIcon();
//     m_ckTrayIcon.PlayAnimation(2000);


//    m_ckDebugDlg.ShowWindow(SW_SHOW);


    // TODO: 在此添加控件通知处理程序代码
    //CDialogEx::OnOK();
}


int CCUCKOO0615_Utils_MFCDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialogEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  在此添加您专用的创建代码
    return 0;
}


void CCUCKOO0615_Utils_MFCDlg::OnBnClickedCancel()
{

//     m_ckTrayIcon.StopAnimation();
// 
//     // TODO: 在此添加控件通知处理程序代码
//     CDialogEx::OnCancel();
}
