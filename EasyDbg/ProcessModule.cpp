// ProcessModule.cpp : implementation file
//

#include "stdafx.h"
#include "EasyDbg.h"
#include "ProcessModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Psapi.h"
#pragma comment(lib,"Psapi.Lib")

/////////////////////////////////////////////////////////////////////////////
// CProcessModule dialog


extern HANDLE g_hProcess;

CProcessModule::CProcessModule(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessModule::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcessModule)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProcessModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessModule)
	DDX_Control(pDX, IDC_LIST1, m_ModuleList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessModule, CDialog)
	//{{AFX_MSG_MAP(CProcessModule)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessModule message handlers

void CProcessModule::OnOK() 
{
	// TODO: Add extra validation here
	
	
}

BOOL CProcessModule::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
   
    m_ModuleList.InsertColumn(0,TEXT("模块名"));
    m_ModuleList.InsertColumn(1,TEXT("模块基址"));
    m_ModuleList.InsertColumn(2,TEXT("模块大小"));
    m_ModuleList.InsertColumn(3,TEXT("模块入口点"));
    m_ModuleList.InsertColumn(4,TEXT("模块路径"));
    
    
    m_ModuleList.SetExtendedStyle(m_ModuleList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    
    ListModules();

    m_ModuleList.SetColumnWidth(0,LVSCW_AUTOSIZE_USEHEADER);
    m_ModuleList.SetColumnWidth(1,LVSCW_AUTOSIZE_USEHEADER);
    m_ModuleList.SetColumnWidth(2,LVSCW_AUTOSIZE_USEHEADER);
    m_ModuleList.SetColumnWidth(3,LVSCW_AUTOSIZE_USEHEADER);
    m_ModuleList.SetColumnWidth(4,LVSCW_AUTOSIZE_USEHEADER);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//列举被调试进程的模块
void  CProcessModule::ListModules()
{
    m_ModuleList.DeleteAllItems();
   
    HMODULE  hModule[500];
    //接收返回的字节数
    DWORD nRetrun=0;
    //枚举
    BOOL isSuccess=EnumProcessModules(g_hProcess,hModule,sizeof(hModule),&nRetrun);
    if (isSuccess==0)
    {
        OutputDebugString(TEXT("枚举模块失败"));
        return;
        
    }
    
    TCHAR ModuleName[500];
    TCHAR ModuleFilePath[MAX_PATH];
    CString szText;
   
    MODULEINFO minfo;
    //开始添加
    for (DWORD i=0;i<(nRetrun/sizeof(HMODULE));i++)
    {
        //获取模块名
        DWORD nLength=GetModuleBaseName(g_hProcess,hModule[i],ModuleName,sizeof(ModuleName));
        if (nLength==0)
        {
            OutputDebugString(TEXT("获取模块名失败"));
        }
        //添加到列表
        m_ModuleList.InsertItem(i,ModuleName);
        //格式化模块基址
        szText.Format("0x%08X",hModule[i]);
        m_ModuleList.SetItemText(i,1,szText);
        //获取模块信息
        nLength=GetModuleInformation(g_hProcess,hModule[i],&minfo,sizeof(minfo));
        if (nLength==0)
        {
            OutputDebugString(TEXT("获取模块信息失败"));
            return;
        }
        //格式化模块大小
        szText.Format("0x%08X",minfo.SizeOfImage);
        m_ModuleList.SetItemText(i,2,szText);
        //格式化模块入口点
        szText.Format("0x%08X",minfo.EntryPoint);
        m_ModuleList.SetItemText(i,3,szText);
        //获取模块主路径
        nLength=GetModuleFileNameEx(g_hProcess,hModule[i],ModuleFilePath,MAX_PATH);
        if (nLength==0)
        {
            OutputDebugString(TEXT("获取模块路径失败"));
        }
        m_ModuleList.SetItemText(i,4,ModuleFilePath);
        
    }
    


}

void CProcessModule::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
   
    if (!m_ModuleList.m_hWnd)
    {
        return;
    }
    
    CRect rc;
    GetClientRect(&rc);
    m_ModuleList.MoveWindow(&rc,TRUE);
	
}
