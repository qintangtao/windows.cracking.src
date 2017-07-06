// VirtualMemory.cpp : implementation file
//

#include "stdafx.h"
#include "EasyDbg.h"
#include "VirtualMemory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVirtualMemory dialog

//声明
extern HANDLE g_hProcess;

CVirtualMemory::CVirtualMemory(CWnd* pParent /*=NULL*/)
	: CDialog(CVirtualMemory::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVirtualMemory)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVirtualMemory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVirtualMemory)
	DDX_Control(pDX, IDC_LIST1, m_MemoryList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVirtualMemory, CDialog)
	//{{AFX_MSG_MAP(CVirtualMemory)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVirtualMemory message handlers

void CVirtualMemory::OnOK() 
{
	// TODO: Add extra validation here
	
   //	CDialog::OnOK();
}

BOOL CVirtualMemory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    m_MemoryList.InsertColumn(0,"首地址",LVCFMT_LEFT,100);
    m_MemoryList.InsertColumn(1,"大小",LVCFMT_LEFT,100);
    m_MemoryList.InsertColumn(2,"保护属性",LVCFMT_LEFT,120);
    m_MemoryList.InsertColumn(3,"状态",LVCFMT_LEFT,80);
    m_MemoryList.InsertColumn(4,"类型",LVCFMT_LEFT,80);
   
    m_MemoryList.SetExtendedStyle(m_MemoryList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

    ListVirtualMemory();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void  CVirtualMemory::ListVirtualMemory()
{
    
     m_MemoryList.DeleteAllItems();

    if (g_hProcess==NULL)
    {
        AfxMessageBox("枚举失败,句柄为空");
        return;
    }
    
    SYSTEM_INFO Sys_info;
   
    GetSystemInfo(&Sys_info);
    
    MEMORY_BASIC_INFORMATION  Meminfo; 
    //最小进程的可以有访问权限的的地址值 通过Sys_info得到
    DWORD theFirstAddress=(DWORD)Sys_info.lpMinimumApplicationAddress; 
    //循环遍历直到到达进程拥有访问权限的最大地址
    CString szText;
    //编号
    static int i=-1;
    while (theFirstAddress<(DWORD)Sys_info.lpMaximumApplicationAddress)
    {
        i++;
        
        if (i==0)
        {
           m_MemoryList.InsertItem(0,"0X00000000");
           m_MemoryList.SetItemText(0,1,"0X00010000");
           m_MemoryList.SetItemText(0,2,"N/A");
           m_MemoryList.SetItemText(0,3,"Free");
           m_MemoryList.SetItemText(0,4,"----");
           continue;
        }
        //查询指定进程的虚拟内存地址
         VirtualQueryEx(g_hProcess,(LPCVOID)theFirstAddress,&Meminfo,sizeof(Meminfo));
         //虚拟地址空间的一个区域的首地址
         szText.Format("0x%08X",Meminfo.BaseAddress);
         m_MemoryList.InsertItem(i,szText);
         //区域的大小
         szText.Format("0x%08X",Meminfo.RegionSize);
         m_MemoryList.SetItemText(i,1,szText);
         //保护属性
         switch (Meminfo.Protect)
         {
         case PAGE_READONLY:
             szText.Format("%s","-R");
             break;
         case PAGE_READWRITE:
             szText.Format("%s","-RW");
             break;
         case PAGE_EXECUTE:
             szText.Format("%s","-E");
             break;
         case PAGE_EXECUTE_READ:
             szText.Format("%s","-ER");
             break;
         case  PAGE_EXECUTE_READWRITE:
             szText.Format("%s","-ERW)");
             break;
         case PAGE_EXECUTE_WRITECOPY:
             szText.Format("%s","-ERWC");
             break;
         case PAGE_GUARD:
             szText.Format("%s","PAGE_GUARD");
             break;
         case PAGE_NOACCESS:
             szText.Format("%s","PAGE_NOACCESS");
             break;

         case PAGE_NOCACHE:
             szText.Format("%s","PAGE_NOCACHE");
             break;
         default:
             szText.Format("%s","-----");
             break;
         }
         m_MemoryList.SetItemText(i,2,szText);
         //当前区域状态
         switch (Meminfo.State)
         {
         case MEM_COMMIT:
             szText.Format("%s","Commmit");
             break;
         case MEM_FREE:
             szText.Format("%s","Free");
             break;
         case MEM_RESERVE:
             szText.Format("%s","Reserve");
             break;
         default:
             szText.Format("%s","-----");
             break;
         }
         m_MemoryList.SetItemText(i,3,szText);

         //区域类型
         switch (Meminfo.Type)
         {
         case MEM_IMAGE:
             szText.Format("%s","Image");
             break;
         case MEM_MAPPED:
             szText.Format("%s","Mapped");
             break;
         case MEM_PRIVATE:
             szText.Format("%s","Privete");
             break;
         default:
             szText.Format("%s","----");
             break;
         }
         m_MemoryList.SetItemText(i,4,szText);

        
       theFirstAddress=(DWORD)theFirstAddress+(DWORD)Meminfo.RegionSize;

    }
    szText.Format("进程虚拟内存空间[%d]",i);
    SetWindowText(szText);
    i=-1;
    theFirstAddress=0;
}

//当窗口最大话时,列表控件也要与窗口相适应
void CVirtualMemory::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    
    if (!m_MemoryList.m_hWnd)
    {
        return;
    }
    
    CRect rc;
    GetClientRect(&rc);
    m_MemoryList.MoveWindow(&rc,TRUE);


	
}
