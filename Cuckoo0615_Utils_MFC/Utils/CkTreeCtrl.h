#pragma once

//************************************************
//�����ߣ�CUCKOO0615
//�����ڣ�2014/12/24 10:06:43
//��˵��������ͼTreeView�ؼ�
//************************************************

// ��ӵ�Ŀ�괰�ڵ� DoDataExchange ������
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


