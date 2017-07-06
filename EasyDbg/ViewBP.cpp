// ViewBP.cpp : implementation file
//

#include "stdafx.h"
#include "EasyDbg.h"
#include "ViewBP.h"
#include <afxtempl.h>
#include "DebugData.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DebugData.h"
/////////////////////////////////////////////////////////////////////////////
// CViewBP dialog
//全局INT3链表
extern CList<INT3_BP,INT3_BP&> g_Int3BpList;
//全局内存断点链表
extern CList<MEM_BP,MEM_BP&> g_MemBpList;

extern HANDLE g_hThread;

CViewBP::CViewBP(CWnd* pParent /*=NULL*/)
	: CDialog(CViewBP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewBP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CViewBP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewBP)
	DDX_Control(pDX, IDC_BPLIST, m_bpList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewBP, CDialog)
	//{{AFX_MSG_MAP(CViewBP)
	ON_BN_CLICKED(BTN_INT3, OnInt3)
	ON_BN_CLICKED(BTN_HARD, OnHard)
	ON_BN_CLICKED(BTN_MEMORY, OnMemory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewBP message handlers

void CViewBP::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

BOOL CViewBP::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//     m_bpList.InsertColumn(0,TEXT("断点地址"),LVCFMT_LEFT,100);
//     m_bpList.InsertColumn(1,TEXT("断点类型"),LVCFMT_LEFT,100);
//     m_bpList.InsertColumn(2,TEXT("断点原数据"),LVCFMT_LEFT,100);
//     m_bpList.InsertColumn(3,TEXT("是否是永久断点"),LVCFMT_LEFT,150);
//     
//     m_bpList.SetExtendedStyle(m_bpList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
 


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//DEL void CViewBP::OnSize(UINT nType, int cx, int cy) 
//DEL {
//DEL 	CDialog::OnSize(nType, cx, cy);
//DEL 	
//DEL 	// TODO: Add your message handler code here
//DEL     if (!m_bpList.m_hWnd)
//DEL     {
//DEL         return;
//DEL     }
//DEL     
//DEL     CRect rc;
//DEL     GetClientRect(&rc);
//DEL     m_bpList.MoveWindow(&rc,TRUE);
//DEL 
//DEL 	
//DEL }

//枚举当前INT3断点
void CViewBP::ListBp()
{

    Int3UIinit();
    //每次都要先清空所有
    m_bpList.DeleteAllItems();

    CString szText;
    POSITION pos=NULL;
    pos=g_Int3BpList.GetHeadPosition();
    int i=0;
    while (pos!=NULL)
    {
        
        INT3_BP bp=g_Int3BpList.GetNext(pos);
        szText.Format("%08X",bp.dwAddress);
        m_bpList.InsertItem(i,szText);
        szText.Format("INT3断点");
        m_bpList.SetItemText(i,1,szText);
        szText.Format("%02X",bp.bOriginalCode);
        m_bpList.SetItemText(i,2,szText);
        szText.Format("%d",bp.isForever);
        m_bpList.SetItemText(i,3,szText);
        i++;

    } 

}


//查看INT3断点
void CViewBP::OnInt3() 
{
	// TODO: Add your control notification handler code here
    ListBp();

	
}


//INT3断点界面的初始化
void CViewBP::Int3UIinit()
{
    //先删除所有的列
    for (int i=0;i<5;i++)
    {
        
        m_bpList.DeleteColumn(0);
    }
    m_bpList.InsertColumn(0,TEXT("断点地址"),LVCFMT_LEFT,100);
    m_bpList.InsertColumn(1,TEXT("断点类型"),LVCFMT_LEFT,100);
    m_bpList.InsertColumn(2,TEXT("断点原数据"),LVCFMT_LEFT,100);
    m_bpList.InsertColumn(3,TEXT("是否是永久断点"),LVCFMT_LEFT,150);
    
    m_bpList.SetExtendedStyle(m_bpList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    //设置标题
    SetWindowText(TEXT("当前INT3断点如下"));
}

//硬件断点界面的初始化
void CViewBP::HardUIinit()
{
    //先删除所有的列
    for (int i=0;i<5;i++)
    {
        
        m_bpList.DeleteColumn(0);
    }
    m_bpList.InsertColumn(0,TEXT("断点地址"),LVCFMT_LEFT,100);
    m_bpList.InsertColumn(1,TEXT("断点类型"),LVCFMT_LEFT,70);
    m_bpList.InsertColumn(2,TEXT("断点长度"),LVCFMT_LEFT,70);
    m_bpList.InsertColumn(3,TEXT("附注"),LVCFMT_LEFT,300);
    
    m_bpList.SetExtendedStyle(m_bpList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    //设置标题
    SetWindowText(TEXT("当前硬件断点如下"));

}

void CViewBP::ListHardBp()
{
    HardUIinit();
    m_bpList.DeleteAllItems();
    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(g_hThread,&ct))
    {
        OutputDebugString("ViewBP.cpp 183行出错");
        
        return;
    }
    DR7 tagDr7={0};
    tagDr7.dwDr7=ct.Dr7;
    CString szText;
    int i=0;
    if(ct.Dr0)
    {
        //断点地址
        szText.Format("%08X",ct.Dr0);
        m_bpList.InsertItem(i,szText);
        //断点类型
        szText.Format("%d",tagDr7.DRFlag.rw0);
        m_bpList.SetItemText(i,1,szText);
        //断点长度
        szText.Format("%d",tagDr7.DRFlag.len0+1);
        m_bpList.SetItemText(i,2,szText);
        m_bpList.SetItemText(i,3,TEXT("0:执行断点 3:访问断点 1:写入断点"));
        i++;
    }
    
    if(ct.Dr1)
    {
        
        szText.Format("%08X",ct.Dr1);
        m_bpList.InsertItem(i,szText);
        
        szText.Format("%d",tagDr7.DRFlag.rw1);
        m_bpList.SetItemText(i,1,szText);
        
        szText.Format("%d",tagDr7.DRFlag.len1+1);
        m_bpList.SetItemText(i,2,szText);
        m_bpList.SetItemText(i,3,TEXT("0:执行断点 3:访问断点 1:写入断点"));
        i++;
    }

    if(ct.Dr2)
    {
        
        szText.Format("%08X",ct.Dr2);
        m_bpList.InsertItem(i,szText);
        
        szText.Format("%d",tagDr7.DRFlag.rw2);
        m_bpList.SetItemText(i,1,szText);
        
        szText.Format("%d",tagDr7.DRFlag.len2+1);
        m_bpList.SetItemText(i,2,szText);
        m_bpList.SetItemText(i,3,TEXT("0:执行断点 3:访问断点 1:写入断点"));
        i++;
    }

    if(ct.Dr3)
    {
        
        szText.Format("%08X",ct.Dr3);
        m_bpList.InsertItem(i,szText);
        
        szText.Format("%d",tagDr7.DRFlag.rw3);
        m_bpList.SetItemText(i,1,szText);
        
        szText.Format("%d",tagDr7.DRFlag.len3+1);
        m_bpList.SetItemText(i,2,szText);
        m_bpList.SetItemText(i,3,TEXT("0:执行断点 3:访问断点 1:写入断点"));
        i++;
    }


}

//查看硬件断点
void CViewBP::OnHard() 
{
	// TODO: Add your control notification handler code here
    ListHardBp();

	
}

//查看内存断点
void CViewBP::OnMemory() 
{
	// TODO: Add your control notification handler code here
   ListMemBp();
	
}

//内存断点界面初始化
void CViewBP::MemUIinit()
{
    //先删除所有的列
    for (int i=0;i<5;i++)
    {
        
        m_bpList.DeleteColumn(0);
    }
    m_bpList.InsertColumn(0,TEXT("断点地址"),LVCFMT_LEFT,70);
    m_bpList.InsertColumn(1,TEXT("断点类型"),LVCFMT_LEFT,70);
    m_bpList.InsertColumn(2,TEXT("断点长度"),LVCFMT_LEFT,70);
    m_bpList.InsertColumn(3,TEXT("所跨内存分页"),LVCFMT_LEFT,100);
    m_bpList.InsertColumn(4,TEXT("附注"),LVCFMT_LEFT,300);
    
    m_bpList.SetExtendedStyle(m_bpList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    //设置标题
    SetWindowText(TEXT("当前内存断点如下"));


}


//枚举当前内存断点
void CViewBP::ListMemBp()
{
    //初始化界面
    MemUIinit();
    m_bpList.DeleteAllItems();

    CString szText;
    POSITION pos=NULL;
    pos=g_MemBpList.GetHeadPosition();
    MEM_BP mBP={0};
    int i=0;
    while (pos!=NULL)
    {
        mBP=g_MemBpList.GetNext(pos);
        szText.Format("%08X",mBP.dwBpAddress);
        m_bpList.InsertItem(i,szText);
        szText.Format("%d",mBP.dwAttribute);
        m_bpList.SetItemText(i,1,szText);
        szText.Format("%d",mBP.dwLength);
        m_bpList.SetItemText(i,2,szText);
        switch (mBP.dwNumPage)
        {
        case 1:
            szText.Format("%08X",mBP.dwMemPage[0]);
            
            break;
        case 2:
            szText.Format("%08X %08X",mBP.dwMemPage[0],mBP.dwMemPage[1]);
            break;
        case 3:
            szText.Format("%08X %08X %08X",mBP.dwMemPage[0],mBP.dwMemPage[1],mBP.dwMemPage[2]);
            break;
        case 4:
            szText.Format("%08X %08X %08X %08X",mBP.dwMemPage[0],mBP.dwMemPage[1],mBP.dwMemPage[2],mBP.dwMemPage[3]);
            break;
        case 5:
            szText.Format("%08X %08X %08X %08X %08X",mBP.dwMemPage[0],mBP.dwMemPage[1],
                mBP.dwMemPage[2],mBP.dwMemPage[3],mBP.dwMemPage[4]);
            break;
        }
        m_bpList.SetItemText(i,3,szText);
        szText.Format("1:写入断点 3: 访问断点");
        m_bpList.SetItemText(i,4,szText);
        i++;

        
    }




}
