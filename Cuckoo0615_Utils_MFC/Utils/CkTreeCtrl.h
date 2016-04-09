#pragma once

//************************************************
//◇作者：CUCKOO0615
//◇日期：2014/12/24 10:06:43
//◇说明：树视图TreeView控件
//************************************************

// 添加到目标窗口的 DoDataExchange 函数中
#define CKTREECTRL_BIND(pDx, nID, mCkTreeCtrl) (DDX_Control(pDX, nID, mCkTreeCtrl))

// CkTreeCtrl

class CkTreeCtrl : public CTreeCtrl
{
    DECLARE_DYNAMIC(CkTreeCtrl)

public:
    CkTreeCtrl();
    virtual ~CkTreeCtrl();

protected:
    DECLARE_MESSAGE_MAP()


public:
    afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
};


