// CkTreeCtrl.cpp : 实现文件
//
#include "stdafx.h"
#include "CkTreeCtrl.h"


// CkTreeCtrl

IMPLEMENT_DYNAMIC(CkTreeCtrl, CTreeCtrl)

CkTreeCtrl::CkTreeCtrl()
{

}

CkTreeCtrl::~CkTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CkTreeCtrl, CTreeCtrl)
    ON_NOTIFY_REFLECT(NM_CLICK, &CkTreeCtrl::OnNMClick)
    ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CkTreeCtrl::OnTvnItemexpanding)
END_MESSAGE_MAP()



// CkTreeCtrl 消息处理程序

/*
** 鼠标左键单击事件
*/
#define ABORT_THIS_FUNC {*pResult = 1; return;}
void CkTreeCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    CPoint pt;
    GetCursorPos(&pt);
    this->ScreenToClient(&pt);

    UINT uFlag = 0;
    HTREEITEM hSelItem = this->HitTest(pt, &uFlag);
    if (0 == hSelItem) ABORT_THIS_FUNC;
    this->SelectItem(hSelItem);

    switch (uFlag)
    {
        //     case TVHT_NOWHERE:    
        //     case TVHT_ONITEMICON:  
        //     case TVHT_ONITEMLABEL:  
        //     case TVHT_ONITEM: 
        //     case TVHT_ONITEMINDENT: 
        //     case TVHT_ONITEMBUTTON: 
        //     case TVHT_ONITEMRIGHT: 
        //     case TVHT_ONITEMSTATEICON:  //CheckBox
        // 
        //     case TVHT_ABOVE:     
        //     case TVHT_BELOW:         
        //     case TVHT_TORIGHT:      
        //     case TVHT_TOLEFT:
        //default:
        //break;
    } //switch(uFlag)

    *pResult = 0;
}
#undef ABORT_THIS_FUNC

/*
** 节点展开事件
*/
void CkTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    HTREEITEM hSelItem = pNMTreeView->itemNew.hItem;
    this->SelectItem(hSelItem);

    switch (pNMTreeView->action)
    {
        //     case TVE_COLLAPSE:
        //     case TVE_EXPAND:
        //     case TVE_TOGGLE:
        //     case TVE_EXPANDPARTIAL:
        //     case TVE_COLLAPSERESET:
        //default:
        //break;
    }//switch(pNMTreeView->action)

    *pResult = 0;
}
