
// CUCKOO0615_Utils_MFCDlg.h : ͷ�ļ�
//

#pragma once
#include "./Utils/CkTreeCtrl.h"
#include "./Utils/CkTrayIcon.h"

// CCUCKOO0615_Utils_MFCDlg �Ի���
class CCUCKOO0615_Utils_MFCDlg : public CDialogEx
{
// ����
public:
	CCUCKOO0615_Utils_MFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CUCKOO0615_UTILS_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
    CkTreeCtrl m_ckTreeCtrl;
    CkTrayIcon m_ckTrayIcon;

public:
    afx_msg void OnBnClickedOk();
    CTabCtrl m_tabTest;
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnBnClickedCancel();
};
