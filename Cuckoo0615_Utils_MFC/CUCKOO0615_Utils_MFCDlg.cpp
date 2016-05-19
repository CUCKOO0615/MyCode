
// CUCKOO0615_Utils_MFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CUCKOO0615_Utils_MFC.h"
#include "CUCKOO0615_Utils_MFCDlg.h"
#include "afxdialogex.h"
//#include "Utils/CKDbgConsole.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCUCKOO0615_Utils_MFCDlg �Ի���




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
    while (0 != strRet.Replace("\\\\", "\\")); // �ϲ����������ķ�б��
    strRet.Replace("\\", "\\\\");            // ��\��ȫ���滻Ϊ��\\��
    strRet = "\\\\" + strRet;               // ���ǰ׺��\\��
    return strRet;
}


// CCUCKOO0615_Utils_MFCDlg ��Ϣ�������

BOOL CCUCKOO0615_Utils_MFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������


    CString str = "D:\\tomcat-mds\\webapps\\MDS\\connection\\yb\\\\sz140404.txt";
    CString fileUrl = CutRelativePathByKeyword(str, "connection");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCUCKOO0615_Utils_MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
        
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
//     m_ckTrayIcon.Init(this->GetSafeHwnd(), arrIcons, 4, _T("��������"));
//     m_ckTrayIcon.AddIcon();
//     m_ckTrayIcon.PlayAnimation(2000);


//    m_ckDebugDlg.ShowWindow(SW_SHOW);


    // TODO: �ڴ���ӿؼ�֪ͨ����������
    //CDialogEx::OnOK();
}


int CCUCKOO0615_Utils_MFCDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDialogEx::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  �ڴ������ר�õĴ�������
    return 0;
}


void CCUCKOO0615_Utils_MFCDlg::OnBnClickedCancel()
{

//     m_ckTrayIcon.StopAnimation();
// 
//     // TODO: �ڴ���ӿؼ�֪ͨ����������
//     CDialogEx::OnCancel();
}
