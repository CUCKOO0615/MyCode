
// CUCKOO0615_Utils_MFCDlg.h : 头文件
//

#pragma once
#include "./Utils/CkTreeCtrl.h"
#include "./Utils/CkTrayIcon.h"

// CCUCKOO0615_Utils_MFCDlg 对话框
class CCUCKOO0615_Utils_MFCDlg : public CDialogEx
{
// 构造
public:
	CCUCKOO0615_Utils_MFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CUCKOO0615_UTILS_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
