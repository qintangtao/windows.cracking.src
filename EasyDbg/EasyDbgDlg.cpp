// EasyDbgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EasyDbg.h"
#include "EasyDbgDlg.h"
#include <stdio.h>

#include "Psapi.h"

#include<Tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Decode2Asm.h"
#include "resource.h"

#define BUFSIZE 512
#pragma comment(lib,"Psapi.Lib")


extern CList<INT3_BP,INT3_BP&> g_Int3BpList(100);

extern CList<MEM_BP,MEM_BP&> g_MemBpList(100);
//保存映射的基址
extern char* pFile=NULL;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


extern HANDLE g_hProcess=NULL;

extern HANDLE g_hThread=NULL;


HANDLE hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
//线程回调函数声明
DWORD WINAPI DebugThreadProc(
                        LPVOID lpParameter   // thread data
                        );

//OpenThread指针声明
typedef HANDLE (__stdcall *OpenThread_)(
                                       DWORD dwDesiredAccess,  // access right
                                       BOOL bInheritHandle,    // handle inheritance option
                                       DWORD dwThreadId        // thread identifier
                                       );
//初始化
OpenThread_ pfnOpenThread=NULL;


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyDbgDlg dialog

CEasyDbgDlg::CEasyDbgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEasyDbgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEasyDbgDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
    
}

void CEasyDbgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEasyDbgDlg)
	DDX_Control(pDX, IDC_LIST5, m_Stack);
	DDX_Control(pDX, IDC_LIST3, m_AsmList);
	DDX_Control(pDX, IDC_LIST2, m_DataList);
	DDX_Control(pDX, IDC_LIST4, m_Result);
	DDX_Control(pDX, IDC_EDIT1, m_command);
	DDX_Control(pDX, IDC_LIST1, m_reglist);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEasyDbgDlg, CDialog)
	//{{AFX_MSG_MAP(CEasyDbgDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDM_OPEN, OnOpen)
    ON_WM_DROPFILES()
	ON_COMMAND(IDM_STEPINTO, OnStepinto)
	ON_COMMAND(IDM_STEPOVER, OnStepover)
	ON_BN_CLICKED(BTN_SHOWDATA, OnShowdata)
	ON_COMMAND(IDM_MEMORY, OnMemory)
	ON_COMMAND(IDM_MODULE, OnModule)
	ON_COMMAND(IDM_RUN, OnRun)
	ON_COMMAND(IDM_QUIT, OnQuit)
	ON_COMMAND(IDM_SETBP, OnSetbp)
	ON_COMMAND(IDM_BREAKPOINT, OnBreakpoint)
	ON_COMMAND(IDM_VIEWPE, OnViewpe)
	ON_COMMAND(IDM_AUTOSTEPOUT, OnAutostepout)
	ON_COMMAND(IDM_AUTOSTEPINTO, OnAutostepinto)
	ON_COMMAND(IDM_OUTFUN, OnOutfun)
	ON_COMMAND(IDM_DUMP, OnDump)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST3, OnDblclkList3)
	ON_NOTIFY(NM_RCLICK, IDC_LIST3, OnRclickList3)
	ON_COMMAND(IDM_INT3, OnInt3)
	ON_COMMAND(IDM_EXECUTE, OnExecute)
	ON_COMMAND(IDM_HARD, OnHard)
	ON_BN_CLICKED(BTN_API, OnApi)
	ON_COMMAND(IDM_HELP, OnHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyDbgDlg message handlers

BOOL CEasyDbgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
    //初始化界面值
    UIinit();
    //初始化相关数据
    memset(m_SzFilePath,0,sizeof(m_SzFilePath));
    memset(&m_tpInfo,0,sizeof(m_tpInfo));
    memset(&m_Recover_BP,0,sizeof(m_Recover_BP));
    memset(&m_Dr_Use,0,sizeof(m_Dr_Use));
    m_tpInfo.bCC=0xCC;
    m_isDebuging=FALSE;
    m_IsOepBP=TRUE;
    m_IsF8=FALSE;
    m_IsGo=FALSE;
    m_isDelete=FALSE;
    m_Uaddress=0;
    m_Recover_HBP.dwIndex=-1;

    m_Attribute[0]=0;//做占位用 实际有用的是 1 3
    m_Attribute[1]=PAGE_EXECUTE_READ;
    m_Attribute[2]=0;
    m_Attribute[3]=PAGE_NOACCESS;

    memset(&m_Recover_Mpage,0,sizeof(m_Recover_Mpage));
    m_isMoreMem=FALSE;

    m_IsAutoF8=FALSE;

    m_IsAutoF7=FALSE;
    m_hFile=INVALID_HANDLE_VALUE;
    memset(&m_AsmAddress,0,sizeof(m_AsmAddress));
    m_GetModule=FALSE;    
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEasyDbgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEasyDbgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEasyDbgDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEasyDbgDlg::OnOK()
{
    
}

//得到可执行文件的路径
void CEasyDbgDlg::GetExeFilePath(char* szFilePath)
{
    OPENFILENAME file = {0} ;
    file.lpstrFile = szFilePath ;
    file.lStructSize = sizeof(OPENFILENAME) ;
    file.nMaxFile = 256 ;
    file.lpstrFilter = "Executables\0*.exe\0All Files\0*.*\0\0" ;
    file.nFilterIndex = 1 ;
    
    if(!::GetOpenFileName(&file))
    {
        //点了取消按钮就退出函数
        return;
    }
}




void CEasyDbgDlg::OnOpen() 
{
	// TODO: Add your command handler code here
   
    if (m_isDebuging==TRUE)
    {
        AfxMessageBox("调试器正在调试中!不能在调试另一个程序");
        return;
    }
    GetExeFilePath(m_SzFilePath);
   //如果用户点击了关闭按钮 m_SzFilePath没有值
    if (m_SzFilePath[0]==0x00)
    {
       
        return;

    }
//      if (!MapPEFile())
//      {
//          return;
//         
// 
//      }
     m_isDebuging=TRUE;

   
    CreateThread(NULL,0,DebugThreadProc,this,NULL,NULL);

}

//调试线程函数
DWORD WINAPI DebugThreadProc(
                         LPVOID lpParameter   // thread data
                        )
{
    STARTUPINFO si={0};
    //要初始化此成员
    si.cb = sizeof(si);
    PROCESS_INFORMATION pi={0};
    char szFilePath[256]={0};
    CEasyDbgDlg* pDebug=(CEasyDbgDlg*)lpParameter;
    //要用工作线程 创建调试进程
    if (!CreateProcess(pDebug->m_SzFilePath,NULL,NULL,NULL,FALSE,DEBUG_ONLY_THIS_PROCESS,NULL,NULL,&si,&pi))
   {
        OutputDebugString("EasyDbgDlg.cpp中第337行代码出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //获得出错信息并输出
        pDebug->GetErrorMessage(dwErrorCode);
        return FALSE;
   }
    
    BOOL isExit=FALSE;//被调试进程是否退出的标志
    //调试事件
    DEBUG_EVENT de={0};
    //作为系统第一次断点的标志
    BOOL bFirstBp=FALSE;
    //标志 被调试线程以怎样的方式恢复
    DWORD  dwContinueStatus=DBG_CONTINUE;
    //调试循环
  while (!isExit&&WaitForDebugEvent(&de,INFINITE))//如果不加上isExit则被调试进程退出时,调试器还会一直等待它
  {  
      switch (de.dwDebugEventCode)
      {
      case EXCEPTION_DEBUG_EVENT:
          switch (de.u.Exception.ExceptionRecord.ExceptionCode)
          {
          case EXCEPTION_ACCESS_VIOLATION:
              {
              DWORD dwAccessAddress=0;
              //异常访问的地址
              dwAccessAddress=de.u.Exception.ExceptionRecord.ExceptionInformation[1];
              dwContinueStatus=pDebug->ON_EXCEPTION_ACCESS_VIOLATION(
                  (DWORD)de.u.Exception.ExceptionRecord.ExceptionAddress,
                  dwAccessAddress
                                                             );
              break;
              }
          case EXCEPTION_BREAKPOINT:
              if (bFirstBp)
              {
                 dwContinueStatus = pDebug->ON_EXCEPTION_BREAKPOINT(
                     (DWORD)de.u.Exception.ExceptionRecord.ExceptionAddress
                                                                   );
                
              }
              else
              {
                  //处理系统第一次断点

                  bFirstBp=TRUE;
              }

              break;
          case EXCEPTION_SINGLE_STEP:

              dwContinueStatus = pDebug->ON_EXCEPTION_SINGLE_STEP(
                  (DWORD)de.u.Exception.ExceptionRecord.ExceptionAddress
                                                                 );
              
              break;


          }
          
          break;
      case CREATE_THREAD_DEBUG_EVENT:
        
          
          //主线程创建不会有此事件
         // AfxMessageBox("线程创建");
          break;
      case CREATE_PROCESS_DEBUG_EVENT:

          //主线程创建
          //AfxMessageBox("进程创建");
          dwContinueStatus=pDebug->ON_CREATE_PROCESS_DEBUG_EVENT(de.dwProcessId,
                                                                 de.dwThreadId,
                                                                 de.u.CreateProcessInfo.lpStartAddress);
          break;  
          
      case EXIT_THREAD_DEBUG_EVENT:
          //主线程退出不会产生此事件
          //AfxMessageBox("线程退出");
          break;
      case EXIT_PROCESS_DEBUG_EVENT:
          //主线程退出
          //AfxMessageBox("进程退出");

          isExit=TRUE;
        
          AfxMessageBox("被调试进程退出");
          
          break;
      case LOAD_DLL_DEBUG_EVENT:
          pDebug->ON_LOAD_DLL_DEBUG_EVENT(de.u.LoadDll.hFile,de.u.LoadDll.lpBaseOfDll);
          
          
          break;
      case UNLOAD_DLL_DEBUG_EVENT:
          
          break;
      case OUTPUT_DEBUG_STRING_EVENT:
          break;
      }

      //恢复被调试线程的运行
      if (!ContinueDebugEvent(de.dwProcessId,de.dwThreadId,dwContinueStatus ))
      {
          OutputDebugString("EasyDbgDlg.cpp 442行出错");
          DWORD dwErrorCode=0;
          dwErrorCode=GetLastError();
          pDebug->GetErrorMessage(dwErrorCode);
          
          return DBG_EXCEPTION_NOT_HANDLED ;
          
          
      }
      //重置此标志
      dwContinueStatus=DBG_CONTINUE;
  
      
  }
    

    
    return 0;
}

//显示寄存器
void CEasyDbgDlg::ShowReg(HANDLE hThread)
{
    CONTEXT lpContext={0};
    lpContext.ContextFlags=CONTEXT_FULL;
    GetThreadContext(hThread,&lpContext);
    CString szText;
    szText.Format("%08X",lpContext.Eax);
    m_reglist.SetItemText(0,1,szText);
    szText.Format("%08X",lpContext.Ebx);
    m_reglist.SetItemText(1,1,szText);
    szText.Format("%08X",lpContext.Ecx);
    m_reglist.SetItemText(2,1,szText);
    szText.Format("%08X",lpContext.Edx);
    m_reglist.SetItemText(3,1,szText);
    szText.Format("%08X",lpContext.Ebp);
    m_reglist.SetItemText(4,1,szText);
    szText.Format("%08X",lpContext.Esp);
    m_reglist.SetItemText(5,1,szText);
    szText.Format("%08X",lpContext.Esi);
    m_reglist.SetItemText(6,1,szText);
    szText.Format("%08X",lpContext.Edi);
    m_reglist.SetItemText(7,1,szText);
    szText.Format("%08X",lpContext.Eip);
    
    m_reglist.SetItemText(8,1,szText);
    szText.Format("%04X",lpContext.SegCs);
    m_reglist.SetItemText(9,1,szText);
    szText.Format("%04X",lpContext.SegSs);
    m_reglist.SetItemText(10,1,szText);
    szText.Format("%04X",lpContext.SegDs);
    m_reglist.SetItemText(11,1,szText);
    szText.Format("%04X",lpContext.SegEs);
    m_reglist.SetItemText(12,1,szText);
    szText.Format("%04X",lpContext.SegGs);
    m_reglist.SetItemText(13,1,szText);
    szText.Format("%04X",lpContext.SegFs);
    m_reglist.SetItemText(14,1,szText);
    szText.Format("%d",(bool)(lpContext.EFlags & 0x0001));
    m_reglist.SetItemText(15,1,szText);
    szText.Format("%d",(bool)(lpContext.EFlags & 0x0004));
    m_reglist.SetItemText(16,1,szText);
    szText.Format("%d",(bool)(lpContext.EFlags & 0x0010));
    m_reglist.SetItemText(17,1,szText);
    szText.Format("%d",(bool)(lpContext.EFlags & 0x0040));
    m_reglist.SetItemText(18,1,szText);
    szText.Format("%d",(bool)(lpContext.EFlags & 0x0080));
    m_reglist.SetItemText(19,1,szText);
    szText.Format("%d",(bool)(lpContext.EFlags & 0x0800));
    m_reglist.SetItemText(20,1,szText);

    

    

}

//截获消息
BOOL CEasyDbgDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
      
        //处理 手工输入命令的消息
        if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
        {
             
            if (m_command.GetFocus()->GetDlgCtrlID()==IDC_EDIT1)
            {
                
                char buffer[100]={0};
                m_command.GetWindowText(buffer,200);

                //处理命令
                Handle_User_Command(buffer);


            }
           
           
        }
         //处理F7快捷键
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F7)
        {
            
             ON_VK_F7();
        }
        //处理F8快捷键
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F8)
        {
            ON_VK_F8();
        }
        //处理F9快捷键
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F9)
        {
            OnRun();
        }
        //处理F6快捷键  自动步过
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F6)
        {
            OnAutostepout();
        }
        //处理F5快捷键 自动步入
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F5)
        {
            OnAutostepinto();
        }
    
        return CDialog::PreTranslateMessage(pMsg);



}

//显示反汇编代码 并将断点在缓冲区里还原
void CEasyDbgDlg::ShowAsm(DWORD dwStartAddress)
{

    ShowAsmInWindow(dwStartAddress);

    //显示堆栈 默认自动单步模式下不显示堆栈以增加速度
    if (!m_IsAutoF7 && !m_IsAutoF8)
    {
            ShowStack();
    }

   BYTE pCode[40]={0};

   DWORD dwOldProtect=0;
   DWORD dwRet=0;
   VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwStartAddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);
   if (!ReadProcessMemory(m_tpInfo.hProcess,(LPCVOID)dwStartAddress,pCode,sizeof(pCode),NULL))
   {
       OutputDebugString("EasyDbgDlg.cpp 594行出错");
       DWORD dwErrorCode=0;
       dwErrorCode=GetLastError();
       //向用户输出错误信息
       GetErrorMessage(dwErrorCode);
       VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwStartAddress,4,dwOldProtect,&dwRet);
       return;
   }
    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwStartAddress,4,dwOldProtect,&dwRet);
   //判断是否有断点命中在断点链表中 若命中则就在缓冲区中恢复 
   for (int i=0;i<16;i++)
   {

       POSITION pos=NULL;
       pos = m_Int3BpList.GetHeadPosition();
       while(pos!=NULL)
       {
           INT3_BP bp=m_Int3BpList.GetNext(pos);
           //判断断点地址是否命名在这段缓冲区中
           //还原永久断点的东西
         
               if (bp.dwAddress==dwStartAddress+i)
               {
                   //如果命中 则说明此为用户断点则把原字节还原
                   pCode[i]=bp.bOriginalCode;
               }
           
          

        }


   }
   
   char szAsm[120]={0};
   char szOpCode[120]={0};
   UINT CodeSize=0;

    Decode2AsmOpcode(pCode,szAsm,szOpCode,&CodeSize,dwStartAddress);
    EXPORT_FUN_INFO expFun={0};
    //如果找到改变显示方式
    if (IsExportFun(szAsm,expFun))
    {
        //显示在列表框控件内
        char szResult[200]={0};
        sprintf(szResult,"%08X    %s        %s <%s.%s>",dwStartAddress,szOpCode,szAsm,expFun.szDLLName,expFun.szFunName);
        m_Result.AddString(szResult);
 
        m_Result.SetTopIndex(m_Result.GetCount()-1);

        
        //如果在自动F8模式
        if (m_IsAutoF8)
        {
            OPCODE_RECORD op={0};
            //如果该指令在映射表中已存在 就不再写文件 (判断地址)
            if (m_Opcode.Lookup(dwStartAddress,op))
            {
                return;
            }
            //如果没有就加入映射表并写文件
            op.dwAddress=dwStartAddress;
            m_Opcode.SetAt(dwStartAddress,op);
            //此时也要改变显示方式
            char szNowShow[100]={0};
            sprintf(szNowShow,"%s <%s.%s>",szAsm,expFun.szDLLName,expFun.szFunName);
            WriteOpcodeToFile(dwStartAddress,szNowShow);
        }
            
        return;
    }
    //显示在列表框控件内
    char szResult[200]={0};
    sprintf(szResult,"%08X    %s        %s",dwStartAddress,szOpCode,szAsm);
    m_Result.AddString(szResult);

    m_Result.SetTopIndex(m_Result.GetCount()-1);
   
    //如果在自动F8模式
    if (m_IsAutoF8)
    {
        OPCODE_RECORD op={0};
        //如果该指令在映射表中已存在 就不再写文件 (判断地址)
        if (m_Opcode.Lookup(dwStartAddress,op))
        {
            return;
        }
        //如果没有就加入映射表并写文件
        op.dwAddress=dwStartAddress;
        m_Opcode.SetAt(dwStartAddress,op);
        WriteOpcodeToFile(dwStartAddress,szAsm);

    }

    
    
  
}

void CEasyDbgDlg::UIinit()
{
    //汇编代码的窗口初始化
    m_AsmList.InsertColumn(0,"地址",LVCFMT_LEFT,90);
    m_AsmList.InsertColumn(1,"HEX数据",LVCFMT_LEFT,140);
    m_AsmList.InsertColumn(2,"反汇编",LVCFMT_LEFT,400);
    
    m_AsmList.SetExtendedStyle(m_AsmList.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

    COLORREF crBkColor=::GetSysColor(COLOR_3DHILIGHT);
    m_AsmList.SetBkColor(crBkColor);

    //堆栈窗口初始化
    m_Stack.InsertColumn(0,"地址",LVCFMT_LEFT,100);
    m_Stack.InsertColumn(1,"值",LVCFMT_LEFT,100);
    m_Stack.SetExtendedStyle(m_Stack.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

    //寄存器列表控件的初始化
     
    m_reglist.InsertColumn(0,"寄存器",LVCFMT_LEFT,50);
    m_reglist.InsertColumn(1,"寄存器值",LVCFMT_LEFT,200);
    m_reglist.InsertItem(0,"EAX");
    m_reglist.InsertItem(1,"EBX");
    m_reglist.InsertItem(2,"ECX");
    m_reglist.InsertItem(3,"EDX");
    m_reglist.InsertItem(4,"EBP");
    m_reglist.InsertItem(5,"ESP");
    m_reglist.InsertItem(6,"ESI");
    m_reglist.InsertItem(7,"EDI");
    m_reglist.InsertItem(8,"EIP");
    m_reglist.InsertItem(9,"CS");
    m_reglist.InsertItem(10,"SS");
    m_reglist.InsertItem(11,"DS");
    m_reglist.InsertItem(12,"ES");
    m_reglist.InsertItem(13,"GS");
    m_reglist.InsertItem(14,"FS");
    m_reglist.InsertItem(15,"CF");
    m_reglist.InsertItem(16,"PF");
    m_reglist.InsertItem(17,"AF");
    m_reglist.InsertItem(18,"ZF");
    m_reglist.InsertItem(19,"SF");
    m_reglist.InsertItem(20,"OF");
    m_reglist.SetExtendedStyle(m_reglist.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    //数据窗口列表的初始化
    m_DataList.InsertColumn(0,"地址",LVCFMT_LEFT,70);
    m_DataList.InsertColumn(1,"HEX数据",LVCFMT_LEFT,170);
    m_DataList.InsertColumn(2,"ASCII",LVCFMT_LEFT,100);
    m_DataList.SetExtendedStyle(m_DataList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);


}

//输出错误信息 
void CEasyDbgDlg::GetErrorMessage(DWORD dwErrorCode)
{
    LPVOID lpMsgBuf;
    FormatMessage( 
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | 
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
        (LPTSTR) &lpMsgBuf,
        0,
        NULL 
        );
    
    ::MessageBox( NULL, (LPCTSTR)lpMsgBuf, TEXT("Error"), MB_OK | MB_ICONINFORMATION );
    //释放堆空间
    LocalFree( lpMsgBuf );
}

//得到加载DLL时的路径
void CEasyDbgDlg::GetFileNameFromHandle(HANDLE hFile,LPVOID pBase)
{
    //有DLL加载 提示模块信息表要更新
    m_GetModule=TRUE;

    //传入参数的有效性判断
    if (hFile==NULL)
    {
        AfxMessageBox("句柄无效");
        return;
    }
    
    TCHAR pszFilename[MAX_PATH+1];
    HANDLE hFileMap;
    
    // Get the file size.
    DWORD dwFileSizeHi = 0;
    DWORD dwFileSizeLo = GetFileSize(hFile, &dwFileSizeHi); 
    
    if( dwFileSizeLo == 0 && dwFileSizeHi == 0 )
    {
        _tprintf(TEXT("Cannot map a file with a length of zero.\n"));
        return ;
    }
    
    // Create a file mapping object.

    hFileMap = CreateFileMapping(hFile, 
        NULL, 
        PAGE_READONLY,
        0, 
        0,
        NULL);
    
    if (hFileMap) 
    {
        // Create a file mapping to get the file name.
        void* pMem = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
        
        if (pMem) 
        {
            //获得导出函数信息
            GetExportFunAddress(hFile,(char*)pMem,pBase);
            if (GetMappedFileName (GetCurrentProcess(), 
                pMem, 
                pszFilename,
                MAX_PATH)) 
            {
                
                // Translate path with device name to drive letters.
                TCHAR szTemp[BUFSIZE];
                szTemp[0] = '\0';
                
                if (GetLogicalDriveStrings(BUFSIZE-1, szTemp)) 
                {
                    TCHAR szName[MAX_PATH];
                    TCHAR szDrive[3] = TEXT(" :");
                    BOOL bFound = FALSE;
                    TCHAR* p = szTemp;
                    
                    do 
                    {
                        // Copy the drive letter to the template string
                        *szDrive = *p;
                        
                        // Look up each device name
                        if (QueryDosDevice(szDrive, szName, MAX_PATH))
                        {
                            size_t uNameLen = _tcslen(szName);
                            
                            if (uNameLen < MAX_PATH) 
                            {
                                bFound = _tcsnicmp(pszFilename, szName, uNameLen) == 0;
                                
                                if (bFound && *(pszFilename + uNameLen) == _T('\\')) 
                                {
                                    // Reconstruct pszFilename using szTempFile
                                    // Replace device path with DOS path
                                     TCHAR szTempFile[MAX_PATH];
                                     _stprintf(szTempFile,
                                        
                                         TEXT("%s%s"),
                                         szDrive,
                                         pszFilename+uNameLen);
                                     _tcsncpy(pszFilename, szTempFile, MAX_PATH);
                                }
                            }
                        }
                        
                        // Go to the next NULL character.
                        while (*p++);
                    } while (!bFound && *p); // end of string
                }
            }
            
            UnmapViewOfFile(pMem);
            pMem=NULL;
        } 
        
        CloseHandle(hFileMap);
        hFileMap=NULL;
    }
    

    m_Result.AddString(pszFilename);

    m_Result.SetTopIndex(m_Result.GetCount()-1); 



}

//处理 CREATE_PROCESS_DEBUG_EVENT 事件的函数 
DWORD CEasyDbgDlg::ON_CREATE_PROCESS_DEBUG_EVENT(DWORD dwProcessId,DWORD dwThreadId,LPTHREAD_START_ROUTINE lpOepAddress)
{
   //由于事件信息中的句柄可能为空,所以这里采用直接OpenProcess和OpenThread来获得被调试进程和主线程的句柄

    //初始化  m_tpInfo结构体
    HMODULE hDll=GetModuleHandle("Kernel32.dll");
    if (hDll==NULL)
    {
        
        hDll=LoadLibrary("Kernel32.dll");
        if (hDll==NULL)
        {
            OutputDebugString("EasyDbgDlg.cpp 897行出错");
            DWORD dwErrorCode=0;
            dwErrorCode=GetLastError();
            //向用户输出出错信息
            GetErrorMessage(dwErrorCode);
            return DBG_EXCEPTION_NOT_HANDLED ;
        }
    }
    pfnOpenThread=(OpenThread_)GetProcAddress(hDll,"OpenThread");
    if (pfnOpenThread==NULL)
    {
        OutputDebugString("EasyDbgDlg.cpp 908行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        GetErrorMessage(dwErrorCode);
        return DBG_EXCEPTION_NOT_HANDLED ;
    }
    
    m_tpInfo.hThread=pfnOpenThread(THREAD_ALL_ACCESS,FALSE,dwThreadId);
    if (m_tpInfo.hThread==NULL)
    {
        OutputDebugString("EasyDbgDlg.cpp 918行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        GetErrorMessage(dwErrorCode);
        return DBG_EXCEPTION_NOT_HANDLED ;

    }

    m_tpInfo.hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessId);
    if (m_tpInfo.hProcess==NULL)
    {
        OutputDebugString("EasyDbgDlg.cpp 929行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        GetErrorMessage(dwErrorCode);
        return DBG_EXCEPTION_NOT_HANDLED ;
    }
    //全局句柄赋值
    g_hProcess=m_tpInfo.hProcess;
    g_hThread=m_tpInfo.hThread;

    m_tpInfo.dwProcessId=dwProcessId;
    m_tpInfo.dwThreadId=dwThreadId;
    m_tpInfo.OepAddress=lpOepAddress;
    //在OEP处下断点
    if (!ReadProcessMemory(m_tpInfo.hProcess,m_tpInfo.OepAddress,&m_tpInfo.OriginalCode,1,NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 946行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        GetErrorMessage(dwErrorCode);
        return DBG_EXCEPTION_NOT_HANDLED ;
    }
    if (!WriteProcessMemory(m_tpInfo.hProcess,m_tpInfo.OepAddress,&m_tpInfo.bCC,1,NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 954行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        GetErrorMessage(dwErrorCode);
        return DBG_EXCEPTION_NOT_HANDLED ;

    }
    return DBG_CONTINUE;

    
}

//文件拖拽
void CEasyDbgDlg::OnDropFiles( HDROP hDropInfo )
{
    
    
    if (m_isDebuging==TRUE)
    {
        AfxMessageBox("调试器正在调试中!不能在调试另一个程序");
        return;
    }

    ::DragQueryFile 
        (hDropInfo,0,m_SzFilePath,sizeof(m_SzFilePath)); //0表示取第一个被拖拽的文件名  
    ::DragFinish (hDropInfo); //释放内存   
    

    if (!MapPEFile())
    {
        return;
     }
    m_isDebuging=TRUE;
    CreateThread(NULL,0,DebugThreadProc,this,NULL,NULL);

    
    
    
    
    
}

//显示被调试进程内存数据,默认从OEP开始,显示800字节,每行8字节
void CEasyDbgDlg::ShowProcessMemory(DWORD dwStartAddress)
{

   


    //传入参数检查
    if (dwStartAddress==NULL)
    {
        AfxMessageBox("无效地址");
        return;
    }
    MEMORY_BASIC_INFORMATION mbi={0};
    //判断是不是有效分页的地址
    if (!IsAddressValid(dwStartAddress,mbi))
    {
        AfxMessageBox("地址无效");
        return;
    }
    MEMORY_BASIC_INFORMATION mbiend={0};
    //判断终止地址是否是有效地址
    if (!IsAddressValid(dwStartAddress+800,mbiend))
    {
        //如果不是就显示到mbi所在的有效分页的地址的内容

        //每次显示先清空列表中的内容
        m_DataList.DeleteAllItems();
        
        BYTE bBuffer[800]={0};
        DWORD dwOldProtect=0;
        DWORD dwRet=0;
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwStartAddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);
        if (!ReadProcessMemory(m_tpInfo.hProcess,(LPVOID)dwStartAddress,bBuffer,((DWORD)mbi.BaseAddress+(DWORD)mbi.RegionSize-dwStartAddress),NULL))
        {
            OutputDebugString("EasyDbgDlg.cpp 1031行出错");
            DWORD dwErrorCode=0;
            dwErrorCode=GetLastError();
            //向用户输出错误信息
            GetErrorMessage(dwErrorCode);
            return;
        }
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwStartAddress,4,dwOldProtect,&dwRet);
        
        //要判断是断点的情况 如果是断点则不能显示出CC而要在缓冲区中还原它
        for (DWORD j=0;j<((DWORD)mbi.BaseAddress+(DWORD)mbi.RegionSize-dwStartAddress);j++)
        {
            POSITION pos=NULL;
            pos = m_Int3BpList.GetHeadPosition();
            while(pos!=NULL)
            {
                INT3_BP bp=m_Int3BpList.GetNext(pos);
                //判断断点地址是否命名在这段缓冲区中
                if (bp.dwAddress==dwStartAddress+j)
                {
                    //如果命中 则说明此为用户断点则把原字节还原
                    bBuffer[j]=bp.bOriginalCode;
                }
            }
            
        }
        
        CString szText;
        
        for (DWORD i=0;i<(((DWORD)mbi.BaseAddress+(DWORD)mbi.RegionSize-dwStartAddress)/8);i++)
        {
            szText.Format("%08X",dwStartAddress+i*8);
            m_DataList.InsertItem(i,szText);
            szText.Format("%02X %02X %02X %02X %02X %02X %02X %02X" ,bBuffer[0+i*8],bBuffer[1+i*8],bBuffer[2+i*8],
                bBuffer[3+i*8],bBuffer[4+i*8],bBuffer[5+i*8],bBuffer[6+i*8],bBuffer[7+i*8]);
            
            m_DataList.SetItemText(i,1,szText);
            char szBffer[9]={0};
            for (int j=0;j<8;j++)
            {
                szBffer[j]=bBuffer[j+i*8];
                //处理特殊字符 如回车 TAB等
                if (szBffer[j]==NULL||szBffer[j]==0xFF||szBffer[j]==0x0A ||szBffer[j]==0x0D||szBffer[j]==0x09||szBffer[j]==0x07)
                    
                {
                    //特殊字符显示点
                    szBffer[j]=0x2e;
                }
            }
            m_DataList.SetItemText(i,2,szBffer);
        }



        return;
    }
    //每次显示先清空列表中的内容
    m_DataList.DeleteAllItems();
 
    BYTE bBuffer[800]={0};
    DWORD dwOldProtect=0;
    DWORD dwRet=0;
    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwStartAddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);
    if (!ReadProcessMemory(m_tpInfo.hProcess,(LPVOID)dwStartAddress,bBuffer,sizeof(bBuffer),NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 1098行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //向用户输出错误信息
        GetErrorMessage(dwErrorCode);
        return;
    }
    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwStartAddress,4,dwOldProtect,&dwRet);

    //要判断是断点的情况 如果是断点则不能显示出CC而要在缓冲区中还原它
    for (int j=0;j<800;j++)
    {
        POSITION pos=NULL;
        pos = m_Int3BpList.GetHeadPosition();
        while(pos!=NULL)
        {
            INT3_BP bp=m_Int3BpList.GetNext(pos);
            //判断断点地址是否命名在这段缓冲区中
            if (bp.dwAddress==dwStartAddress+j)
            {
                //如果命中 则说明此为用户断点则把原字节还原
                bBuffer[j]=bp.bOriginalCode;
            }
        }
        
   }

    CString szText;

    
    for (int i=0;i<100;i++)
    {
        szText.Format("%08X",dwStartAddress+i*8);
        m_DataList.InsertItem(i,szText);
        szText.Format("%02X %02X %02X %02X %02X %02X %02X %02X" ,bBuffer[0+i*8],bBuffer[1+i*8],bBuffer[2+i*8],
            bBuffer[3+i*8],bBuffer[4+i*8],bBuffer[5+i*8],bBuffer[6+i*8],bBuffer[7+i*8]);

        m_DataList.SetItemText(i,1,szText);
        char szBffer[9]={0};
        for (int j=0;j<8;j++)
        {
            szBffer[j]=bBuffer[j+i*8];
            //处理特殊字符 如回车 TAB等
            if (szBffer[j]==NULL||szBffer[j]==0xFF||szBffer[j]==0x0A ||szBffer[j]==0x0D||szBffer[j]==0x09||szBffer[j]==0x07)
                
            {
                //特殊字符显示点
                szBffer[j]=0x2e;
            }
        }
        m_DataList.SetItemText(i,2,szBffer);
    }

}


//F7键的处理函数 单步步入
void CEasyDbgDlg::ON_VK_F7()
{
    //置单步
    SetDlgItemText(IDC_STATE,"");
    CONTEXT ct;
    ct.ContextFlags=CONTEXT_FULL;
    GetThreadContext(m_tpInfo.hThread,&ct);
    ct.EFlags|=0x100;
    SetThreadContext(m_tpInfo.hThread,&ct);

    SetEvent(hEvent);
}

//F8键的处理函数 单步步过
void CEasyDbgDlg::ON_VK_F8()
{
    
    SetDlgItemText(IDC_STATE,"");
    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL;
    if (!GetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 1178行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        GetErrorMessage(dwErrorCode);
        return;
    }
    BYTE szCodeBuffer[40]={0};

    DWORD dwOldProtect=0;
    DWORD dwRet=0;

    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)ct.Eip,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);
    //获得当前EIP处的指令
    if (!ReadProcessMemory(m_tpInfo.hProcess,(LPCVOID)ct.Eip,szCodeBuffer,sizeof(szCodeBuffer),NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 1193行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        GetErrorMessage(dwErrorCode);
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)ct.Eip,4,dwOldProtect,&dwRet);
        return;
    }
     VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)ct.Eip,4,dwOldProtect,&dwRet);

   char szAsm[120]={0};
   char szOpCode[120]={0};
   UINT CodeSize=0;
   //反汇编并判断当前指令是不是call指令
   Decode2AsmOpcode(szCodeBuffer,szAsm,szOpCode,&CodeSize,ct.Eip);

   if (szAsm[0]=='c' && szAsm[1]=='a' && szAsm[2]=='l' && szAsm[3]=='l')
   {
        //如果当前指令是call指令,那么就在下一条指令上下临时断点

       //判断如果下一条指令已经有断点了,则不需要在下
       POSITION pos=NULL;
       pos=m_Int3BpList.GetHeadPosition();
       INT3_BP bp={0};
       
       while(pos!=NULL)
       {
           bp=m_Int3BpList.GetNext(pos);
           //如果找到断点则 不需要在下断点
           if (bp.dwAddress==ct.Eip+CodeSize)
           {
               //设置标志位
               m_IsF8=TRUE;

               SetEvent(hEvent);
               return;

           }

       }
       //非永久断点
       bp.dwAddress=ct.Eip+CodeSize;
       bp.isForever=FALSE;

       VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)bp.dwAddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);
       if (!ReadProcessMemory(m_tpInfo.hProcess,(LPCVOID)bp.dwAddress,&bp.bOriginalCode,sizeof(BYTE),NULL))
       {
           OutputDebugString("EasyDbgDlg.cpp 1239行出错");
           DWORD dwErrorCode=0;
           dwErrorCode=GetLastError();
           GetErrorMessage(dwErrorCode);
           VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)bp.dwAddress,4,dwOldProtect,&dwRet);
           return;
       }
       if (!WriteProcessMemory(m_tpInfo.hProcess,(LPVOID)bp.dwAddress,&m_tpInfo.bCC,sizeof(BYTE),NULL))
       {
           OutputDebugString("EasyDbgDlg.cpp 1248行出错");
           DWORD dwErrorCode=0;
           dwErrorCode=GetLastError();
           GetErrorMessage(dwErrorCode);
           VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)bp.dwAddress,4,dwOldProtect,&dwRet);
           return;
       }
       VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)bp.dwAddress,4,dwOldProtect,&dwRet);
       FlushInstructionCache(m_tpInfo.hProcess,(LPCVOID)bp.dwAddress,sizeof(BYTE));
       //把断点加入链表
       m_Int3BpList.AddTail(bp);
       //设置标志位
       m_IsF8=TRUE;
       

       SetEvent(hEvent);
       
   }
   else
   {
       //如果当前指令不是CALL指令,那么就置单步
       ON_VK_F7();
   }
    
    
}


//响应菜单消息
void CEasyDbgDlg::OnStepinto() 
{
	// TODO: Add your command handler code here
    ON_VK_F7();
    
	
}
//响应菜单消息
void CEasyDbgDlg::OnStepover() 
{
	// TODO: Add your command handler code here
    ON_VK_F8();
    
    	
}

//去掉命令的左边和右边的空格字符
BOOL CEasyDbgDlg::Kill_SPACE(char* szCommand)
{
    //计算字符数 不包括最后的终止符
    int nSize=strlen(szCommand);
    //没输入命令就按回车键
    if (*szCommand==0)
    {
        AfxMessageBox("没有输入命令");
        return FALSE;
    }
    //去掉前面的空格
    for (int i=0;i<nSize;i++)
    {
        if (szCommand[i]!=0x20)
        {
            //去掉前面的空格之后的字符串大小
            int  nNowSize=nSize-i;
            for (int j=0;j<nNowSize;j++)
            {
                //向前移动
                szCommand[j]=szCommand[i];
                i++;
            }
            szCommand[nNowSize]=0;
            
        }
    }
    //之后再去掉后面的空格
    for (int i=strlen(szCommand)-1;i>0;i--)
    {
        //从后向前遍历,遇到第一个不是空格的字符即可
        if (szCommand[i]!=0x20)
        {
            //后面置为终止符
            szCommand[i+1]=0;
            break;
        }
   }

    return TRUE;

}

//用户命令的处理函数
void CEasyDbgDlg::Handle_User_Command(char* szCommand)
{
    //去掉前后的空格
    if (!Kill_SPACE(szCommand))
    {
        AfxMessageBox("命令输入错误");
        return;
    }
    //根据命令处理
    switch (szCommand[0])
    {
    case 't':
    case 'T':
        ON_VK_F7();
        break;
    case 'p':
    case 'P':
        ON_VK_F8();
        break;
    case 'u':
    case 'U':
        {
            //uf  函数 对函数反汇编
            if (szCommand[1]=='F'||szCommand[1]=='f')
            {
                char szName[100]={0};
                sscanf(szCommand, "%s%s", stderr, &szName);
                DisassemblerExcFun(szName);
                
                
            }
            else
            {
                unsigned int dwAddress = 0 ;
                //提取U后面的地址
                sscanf(szCommand, "%s%x", stderr, &dwAddress);
                ON_U_COMMAND(dwAddress);
            }
        break;
        }
    case 'b':
    case 'B':
        Handle_B_Command(szCommand);
        
        break;
    case 'g':
    case 'G':
        {
        unsigned int dwAddress = 0 ;
            //提取U后面的地址
        sscanf(szCommand, "%s%x", stderr, &dwAddress);
        ON_G_COMMAND(dwAddress);
        break;

        }
    case 's':
    case 'S':
        
            //自动步过
            OnAutostepout();
            break;
    case 'o':
    case 'O':
        //跳出函数
        StepOutFromFun();
        break;
    default:
        AfxMessageBox(TEXT("命令错误"));
    }
    

    
}

//显示内存数据
void CEasyDbgDlg::OnShowdata() 
{
	// TODO: Add your control notification handler code here

    //把输入的数据当做16进制处理,也就是只接受16进制数据 
    char szBuffer[40];
    GetDlgItemText(EDIT_DATA,szBuffer,sizeof(szBuffer));
    DWORD dwAddress = 0 ;
    //提取16进制数据
    sscanf(szBuffer, "%x", &dwAddress) ; 
    ShowProcessMemory(dwAddress);
    
	
}


//查看被调试进程地址空间
void CEasyDbgDlg::OnMemory() 
{
	// TODO: Add your command handler code here
    CVirtualMemory dlg;
    dlg.DoModal();


	
}
//查看被调试进程的模块
void CEasyDbgDlg::OnModule() 
{
	// TODO: Add your command handler code here
    CProcessModule dlg;
    dlg.DoModal();
	
}

//断点异常处理函数

//参数dwExpAddress:异常地址

DWORD CEasyDbgDlg::ON_EXCEPTION_BREAKPOINT(DWORD dwExpAddress)
{

    //判断是否是OEP断点
    if (m_IsOepBP)
    {
        //恢复断点
        RecoverBP(m_tpInfo.hProcess,(DWORD)m_tpInfo.OepAddress,m_tpInfo.OriginalCode);
        //EIP--
        ReduceEIP();

        ShowReg(m_tpInfo.hThread);

        ShowAsm(dwExpAddress); 
        //设置U命令的默认地址
        m_Uaddress=dwExpAddress;
        
        //设置为FALSE
        m_IsOepBP=FALSE;
        ShowProcessMemory(dwExpAddress);
        WaitForSingleObject(hEvent,INFINITE);
        return DBG_CONTINUE;
      
    }

    //如果为其他断点就直接执行过去就像OD WINDBG一样
    //判断是用户设置的断点还是被调试程序本来就存在的断点指令
    if (isUserBP(dwExpAddress))
    {
        SetDlgItemText(IDC_STATE,TEXT("INT3断点到达"));
       
        RecoverBP(m_tpInfo.hProcess,dwExpAddress,m_Recover_BP.bOrginalCode);
        //EIP--
        ReduceEIP();
        //如果是自动单步模式的INT3
        if (m_IsAutoF8)
        {

            
            ShowReg(m_tpInfo.hThread);
           
           ShowAsm(dwExpAddress);
           m_Uaddress=dwExpAddress;
           //删除这两类断点 非永久性断点
            DeleteUserBP(m_tpInfo.hProcess,dwExpAddress);

            ON_VK_F8();

            return DBG_CONTINUE;
            
        }


        if (m_IsF8 || m_IsGo)
        {
            if(m_IsGo)
            {
                //清空列表框
                m_Result.ResetContent();

            }
            //删除这两类断点 非永久性断点
            DeleteUserBP(m_tpInfo.hProcess,dwExpAddress);


           ShowReg(m_tpInfo.hThread);

           ShowAsm(dwExpAddress);
           m_Uaddress=dwExpAddress;

           m_IsF8=FALSE;
           m_IsGo=FALSE;

           
           WaitForSingleObject(hEvent,INFINITE);
        }
        

        return DBG_CONTINUE;
    }

    //不是用户断点 就不处理
  return DBG_EXCEPTION_NOT_HANDLED;
  
}




//单步异常处理函数  参数异常地址
DWORD CEasyDbgDlg::ON_EXCEPTION_SINGLE_STEP(DWORD dwExpAddress)
{


    //是否是自动步过模式
    if (m_IsAutoF8)
    {
        //置单步

        ShowAsm(dwExpAddress);
        ShowReg(m_tpInfo.hThread);
        ON_VK_F8();


        return DBG_CONTINUE;

    }
    //是否为自动步入模式
    if (m_IsAutoF7)
    {
        ShowAsm(dwExpAddress);
        ShowReg(m_tpInfo.hThread);
        ON_VK_F7();
        
        return DBG_CONTINUE;

    }
    //先判断有没有要重新恢复的INT3断点
    if (m_Recover_BP.isNeedRecover)
    {

        DebugSetBp(m_tpInfo.hProcess,m_Recover_BP.dwAddress,m_tpInfo.bCC);

        //重新置为FALSE
        m_Recover_BP.isNeedRecover=FALSE;
    }
    //如果有硬件断点要恢复
    if (m_Recover_HBP.dwIndex!=-1)
    {
        //恢复硬件断点
        RecoverHBP(m_Recover_HBP.dwIndex);


        m_Recover_HBP.dwIndex=-1;
    }
    //如果有内存断点要恢复
    if (m_Recover_Mpage.isNeedRecover)
    {
        DWORD dwOldProtect=0;
        if (!VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)m_Recover_Mpage.dwBaseAddress,4,
                                  m_Recover_Mpage.dwProtect,&dwOldProtect)
            )
        {
            
            OutputDebugString("EasyDbgDlg.cpp 1595行出错");
            DWORD dwErrorCode=0;
            
            dwErrorCode=GetLastError();
            //输出错误信息
            GetErrorMessage(dwErrorCode);
        }
        //重新置为FALSE
        m_Recover_Mpage.isNeedRecover=FALSE;
    }


    //判断单步异常是不是因为硬件断点 即DR6的低四位有没有置位
    DWORD dwBpAddress=0;
    if (IfStepHard(dwBpAddress))
    {
        SetDlgItemText(IDC_STATE,TEXT("硬件断点到达"));
        //让硬件断点无效
        InvalidHardBP(dwBpAddress);

    }
    //如果是因为一个G模式的内存断点就不要等待同步事件
    if (m_isMoreMem)
    {
        
        if (m_IsF8)
        {
            m_IsGo=FALSE;
            m_isMoreMem=FALSE;
            //ShowAsm(dwExpAddress);
           // ShowReg(m_tpInfo.hThread);
            //设置U命令的起始地址
            m_Uaddress=dwExpAddress;
            
            //WaitForSingleObject(hEvent,INFINITE);
           //m_IsF8=FALSE;
           return DBG_CONTINUE;
 

        }
        //多次G到达多个N内存断点直接运行到断点
        if (m_IsGo)
        {
            m_isMoreMem=FALSE;

            return DBG_CONTINUE;


        }
       
        m_isMoreMem=FALSE;
        ShowAsm(dwExpAddress);
        ShowReg(m_tpInfo.hThread);

        m_Uaddress=dwExpAddress;
        
        WaitForSingleObject(hEvent,INFINITE);
        
        return DBG_CONTINUE;

        
    }


    ShowAsm(dwExpAddress);
    ShowReg(m_tpInfo.hThread);
    //设置U命令的起始地址
    m_Uaddress=dwExpAddress;

    WaitForSingleObject(hEvent,INFINITE);

    return DBG_CONTINUE;
}

//用于断点异常处理 EIP--
void CEasyDbgDlg::ReduceEIP()
{

    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL;
    GetThreadContext(m_tpInfo.hThread,&ct);
    ct.Eip--;
    SetThreadContext(m_tpInfo.hThread,&ct);
    
}


//恢复断点为原数据 用于断点异常
void CEasyDbgDlg::RecoverBP(HANDLE hProcess ,DWORD dwBpAddress,BYTE bOrignalCode)
{
    DWORD dwOldProtect=0;
    DWORD dwRet=0;
    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBpAddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);

    if (!WriteProcessMemory(hProcess,(LPVOID)dwBpAddress,&bOrignalCode,sizeof(bOrignalCode),NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 1694行出错");
        DWORD dwErrcode=0;
        dwErrcode=GetLastError();
        //向用户输出错误信息
        GetErrorMessage(dwErrcode);
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBpAddress,4,dwOldProtect,&dwRet);
        return ;
    }




    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBpAddress,4,dwOldProtect,&dwRet);
    //刷新
    FlushInstructionCache(hProcess,(LPCVOID)dwBpAddress,sizeof(BYTE));
    

    return;

}

//设置断点  断点地址 0xCC 用于永久断点重新恢复为断点
void CEasyDbgDlg::DebugSetBp(HANDLE hProcess,DWORD dwBpAddress,BYTE bCCode)
{
    DWORD dwOldProtect=0;
    DWORD dwRet=0;
    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBpAddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);
    if (!WriteProcessMemory(hProcess,(LPVOID)dwBpAddress,&bCCode,sizeof(bCCode),NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 1724行出错");
        DWORD dwErrcode=0;
        dwErrcode=GetLastError();
        //向用户输出错误信息
        GetErrorMessage(dwErrcode);
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBpAddress,4,dwOldProtect,&dwRet);
        return ;
    }

    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBpAddress,4,dwOldProtect,&dwRet);
    //刷新
    FlushInstructionCache(hProcess,(LPCVOID)dwBpAddress,sizeof(BYTE));

}

//判断是否是用户设置的INT3断点 通过查询INT3链表 
BOOL CEasyDbgDlg::isUserBP(DWORD dwBpAddress)
{
    POSITION pos=NULL;
    //标志是否找到
    BOOL isYes=FALSE;
    pos=m_Int3BpList.GetHeadPosition();
    while(pos!=NULL)
    {
       INT3_BP bp=m_Int3BpList.GetNext(pos);
       //判断该断点地址是否在地址列表中
       if (bp.dwAddress==dwBpAddress)
       {
           //如果找到,判断是否是永久断点 是则需要在但不异常中在设置为断点
           //在单步异常中重设断点后在重设m_Recover_BP.isNeedRecover为FALSE
           m_Recover_BP.isNeedRecover=bp.isForever;
           m_Recover_BP.dwAddress=bp.dwAddress;
           m_Recover_BP.bOrginalCode=bp.bOriginalCode;
           

           isYes=TRUE;

           break;
       }
    }

    return isYes;

}


//运行被调试程序
void CEasyDbgDlg::OnRun() 
{
	// TODO: Add your command handler code here
    ON_VK_F9();
	
}

//退出程序
void CEasyDbgDlg::OnQuit() 
{
	// TODO: Add your command handler code here
    ExitProcess(0);
	
}


//处理U命令 如果没有地址就从以前的地址接着U 在单步或者断点异常中再把这个地址设为当前EIP的值
void CEasyDbgDlg::ON_U_COMMAND(DWORD dwAddress)
{
    //默认显示8条指令
    //如果指明了地址则赋值m_Uaddress
    if (dwAddress)
    {
        m_Uaddress=dwAddress;      
    }

    

   BYTE pCode[120]={0};
   DWORD dwOldProtect=0;
   DWORD dwRet=0;
   VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)m_Uaddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);

   if (!ReadProcessMemory(m_tpInfo.hProcess,(LPCVOID)m_Uaddress,pCode,sizeof(pCode),NULL))
   {
       OutputDebugString("EasyDbgDlg.cpp 1804行出错");
       DWORD dwErrorCode=0;
       dwErrorCode=GetLastError();
       GetErrorMessage(dwErrorCode);

       VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)m_Uaddress,4,dwOldProtect,&dwRet);
       return;

   }
  
   VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)m_Uaddress,4,dwOldProtect,&dwRet);


   //要判断是断点的情况 如果是断点则不能显示出CC而要在缓冲区中还原它
   for (int j=0;j<120;j++)
   {
       POSITION pos=NULL;
       pos = m_Int3BpList.GetHeadPosition();
       while(pos!=NULL)
       {
           INT3_BP bp=m_Int3BpList.GetNext(pos);
           //判断断点地址是否命名在这段缓冲区中
           if (bp.dwAddress==m_Uaddress+j)
           {
               //如果命中 则说明此为用户断点则把原字节还原
               pCode[j]=bp.bOriginalCode;
           }
       }
       
   }


   char szAsm[120]={0};
   char szOpCode[120]={0};
   UINT CodeSize=0;
   int nIndex=0;
   //开始反汇编
   for (int i=0;i<8;i++)
   {
       
       Decode2AsmOpcode(&pCode[nIndex],szAsm,szOpCode,&CodeSize,m_Uaddress);
       //显示在列表框控件内
       char szResult[200]={0};
       EXPORT_FUN_INFO expFun={0};
       //如果是导出函数则解析出来
       if (IsExportFun(szAsm,expFun))
       {
           sprintf(szResult,"%08X    %s       %s <%s.%s>",m_Uaddress,szOpCode,szAsm,expFun.szDLLName,expFun.szFunName);
           m_Result.AddString(szResult);
          
           m_Result.SetTopIndex(m_Result.GetCount()-1);
           m_Uaddress+=CodeSize;
           nIndex+=CodeSize;
           continue;
           

       }
       sprintf(szResult,"%08X    %s        %s",m_Uaddress,szOpCode,szAsm);
       m_Result.AddString(szResult);
      
       m_Result.SetTopIndex(m_Result.GetCount()-1);
       m_Uaddress+=CodeSize;
       nIndex+=CodeSize;

   }



}

//设置断点
void CEasyDbgDlg::OnSetbp() 
{
	// TODO: Add your command handler code here
    CSetBP dlg;
    if (dlg.DoModal()==IDOK)
    {
        switch (dlg.m_Select)
        {
        case 1:
            //设置INT3断点
            UserSetBP(m_tpInfo.hProcess,dlg.m_dwBpAddress,m_tpInfo.bCC);
            break;
        case 2:
            //设置硬件断点
            SetHardBP(dlg.m_dwBpAddress,dlg.m_dwAttribute,dlg.m_dwLength);
            break;
        case 3:
            SetMemBP(dlg.m_dwBpAddress,dlg.m_dwAttribute,dlg.m_dwLength);
            break;
        }

    }

	
}

//用户设置断点
void CEasyDbgDlg::UserSetBP(HANDLE hProcess,DWORD dwBpAddress,BYTE bCCode)
{
    //判断该地址是否已经是断点
    POSITION pos=NULL;
    INT3_BP bp={0};
    while (pos!=NULL)
    {
        bp=m_Int3BpList.GetNext(pos);
        if (bp.dwAddress==dwBpAddress)
        {
            AfxMessageBox("此地址已经设置断点,设置无效");
            return;
        }
    }
    memset(&bp,0,sizeof(INT3_BP));
    bp.dwAddress=dwBpAddress;
    bp.isForever=TRUE;

    DWORD dwOldProtect=0;
    DWORD dwRet=0;
    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBpAddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);

    //读取首字节
    if (!ReadProcessMemory(hProcess,(LPVOID)dwBpAddress,&bp.bOriginalCode,sizeof(BYTE),NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 1927行出错");
        DWORD dwErrcode=0;
        dwErrcode=GetLastError();
        //向用户输出错误信息
        GetErrorMessage(dwErrcode);

        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBpAddress,4,dwOldProtect,&dwRet);
        return ;

    }

    
     //写入0xCC
    if (!WriteProcessMemory(hProcess,(LPVOID)dwBpAddress,&bCCode,sizeof(bCCode),NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 1942行出错");
        DWORD dwErrcode=0;
        dwErrcode=GetLastError();
        //向用户输出错误信息
        GetErrorMessage(dwErrcode);
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBpAddress,4,dwOldProtect,&dwRet);
        return ;
    }
    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBpAddress,4,dwOldProtect,&dwRet);
    //刷新
    SetDlgItemText(IDC_STATE,"设置断点成功");
    FlushInstructionCache(hProcess,(LPCVOID)dwBpAddress,sizeof(BYTE));
    m_Int3BpList.AddTail(bp);
    

}

//F9键的处理函数 运行
void CEasyDbgDlg::ON_VK_F9()
{
    SetDlgItemText(IDC_STATE,"");
    m_IsGo=TRUE;
    SetEvent(hEvent);
    SetDlgItemText(IDC_STATE,"被调试程序运行成功");
}


//删除用户断点
void CEasyDbgDlg::DeleteUserBP(HANDLE hProcess,DWORD dwBpAddress)
{
    //判断要删除断点地址在不在断点链表中
    POSITION pos=NULL;
    INT3_BP bp={0};
    BOOL isFind=FALSE;
    pos=m_Int3BpList.GetHeadPosition();
    while (pos!=NULL)
    {
        bp=m_Int3BpList.GetNext(pos);
        if (bp.dwAddress==dwBpAddress)
        {
            //考虑到有同一地址下两个断点 即临时断点和永久断点如G命令全部用continue
            if (bp.isForever)
            {
               isFind=TRUE;
               //恢复为原来的字节
               RecoverBP(hProcess,dwBpAddress,bp.bOriginalCode);
               
               
               if (m_isDelete)
               {
                   
                   if (m_Int3BpList.GetCount()==1)
                   {
                       m_Int3BpList.RemoveHead();
                       m_isDelete=FALSE;
                       SetDlgItemText(IDC_STATE,"断点删除成功");
                       return;
                   }
                   
                   if (pos==NULL)
                   {
                       m_Int3BpList.RemoveTail();
                       m_isDelete=FALSE;
                       SetDlgItemText(IDC_STATE,"断点删除成功");
                       return;
                       
                       
                   }
                   
                   m_Int3BpList.GetPrev(pos);
                   
                   m_Int3BpList.RemoveAt(pos);
                   SetDlgItemText(IDC_STATE,"断点删除成功");
               }
              m_isDelete=FALSE;
                
               continue;;
            }
            else
            {
                //在这里删除非永久断点

                if (m_Int3BpList.GetCount()==1)
                {
                    m_Int3BpList.RemoveHead();
                    m_isDelete=FALSE;
                    SetDlgItemText(IDC_STATE,"断点删除成功");
                    return;
                }
                
                if (pos==NULL)
                {
                    m_Int3BpList.RemoveTail();
                    m_isDelete=FALSE;
                    SetDlgItemText(IDC_STATE,"断点删除成功");
                    return;
                    
                    
                }
                
                m_Int3BpList.GetPrev(pos);
                
                m_Int3BpList.RemoveAt(pos);
                SetDlgItemText(IDC_STATE,"断点删除成功");

                continue;
            }
            //找到

        }
    }
    //如果没有在断点链表中找到此地址
    if (!isFind)
    {
         AfxMessageBox("要删除的断点是无效断点");
         return;
       
    }



    

}

//查看当前断点
void CEasyDbgDlg::OnBreakpoint() 
{
	// TODO: Add your command handler code here
    //先把全局的清空 否则多次查看会有重复

    g_Int3BpList.RemoveAll();
    g_MemBpList.RemoveAll();
    POSITION pos=NULL;
    //把m_Int3BpList的断点拷贝到g_Int3BpList中去
    pos=m_Int3BpList.GetHeadPosition();
    while(pos!=NULL)
    {
        INT3_BP bp=m_Int3BpList.GetNext(pos);
        g_Int3BpList.AddTail(bp);

    }
    //拷贝内存断点
    pos=NULL;
    pos=m_MemBpList.GetHeadPosition();
     MEM_BP mBP={0};
    while (pos!=NULL)
    {
        mBP=m_MemBpList.GetNext(pos);
        g_MemBpList.AddTail(mBP);
       
    }

    CViewBP dlg;  
    
    dlg.DoModal();
 
	
}

//处理B命令
void CEasyDbgDlg::Handle_B_Command(char* szCommand)
{
    switch (szCommand[1])
    {
    case 'p':
    case 'P':
        {
        unsigned int dwAddress = 0 ;
        
        //提取P后面的地址
        sscanf(szCommand, "%s%x", stderr, &dwAddress);
        if (dwAddress==0)
        {
            AfxMessageBox("输入错误,请输入断点地址");
            break;
        }
        //设置断点
       UserSetBP(m_tpInfo.hProcess,dwAddress,m_tpInfo.bCC);
          
        break;
        }
    case 'l':
    case 'L':
        //列出当前断点
        ListBP();
      
        break;
    case 'c':
    case 'C':
        {
            unsigned int dwAddress = 0 ;
            //提取C后面的地址
            sscanf(szCommand, "%s%x", stderr, &dwAddress);
            if (dwAddress==0)
            {
                AfxMessageBox("输入错误,请输入断点地址");
                return;
            }
            //删除永久性断点 标志设为TRUE
            m_isDelete=TRUE;
            DeleteUserBP(m_tpInfo.hProcess,dwAddress);
        
        break;
        }
    case 'h':
        //设置硬件断点或删除
    case 'H':
        {
            //对用户输入的指令只做简单的判断
            if (szCommand[2]=='C'||szCommand[2]=='c')
            {
                unsigned int dwAddress = 0 ;
                //提取C后面的地址
                sscanf(szCommand, "%s%x", stderr, &dwAddress);
                if (dwAddress==0)
                {
                    AfxMessageBox("输入错误,请输入断点地址");
                    return;
                }
                DeleteHardBP(dwAddress);
               
            }
            else
            {

            DWORD dwAddress = 0 ;
            DWORD dwAttribute=0;
            DWORD dwLength=0;
            //提取各个值
            sscanf(szCommand, "%s%x%x%x", stderr, &dwAddress, &dwAttribute, &dwLength);
            //设置硬件断点
            SetHardBP(dwAddress,dwAttribute,dwLength);
            }

          break;
        }
    case 'm':
    case 'M':
        {
            //清除内存断点
            if (szCommand[2]=='C'||szCommand[2]=='c')
            {
                unsigned int dwAddress = 0 ;
                //提取C后面的地址
                sscanf(szCommand, "%s%x", stderr, &dwAddress);
                if (dwAddress==0)
                {
                    AfxMessageBox("输入错误,请输入断点地址");
                    return;
                }
                DeleteMemBP(dwAddress);
                
            }
            else
            {

            DWORD dwAddress = 0 ;
            DWORD dwAttribute=0;
            DWORD dwLength=0;
            //提取各个值
            sscanf(szCommand, "%s%x%x%x", stderr, &dwAddress, &dwAttribute, &dwLength);
            SetMemBP(dwAddress,dwAttribute,dwLength);
            }

            break;
        }
    }


}

//枚举断点
void CEasyDbgDlg::ListBP()
{

    POSITION pos=NULL;
    pos=m_Int3BpList.GetHeadPosition();
    INT3_BP bp={0};
    CString szText;
    if (m_Int3BpList.GetCount()!=0)
    {
        //列举INT3断点
        while(pos!=NULL)
        {
            bp=m_Int3BpList.GetNext(pos);
            szText.Format("INT3断点 断点地址:%08X   断点处原数据:%2X 是否是永久断点: %d",bp.dwAddress,bp.bOriginalCode,bp.isForever);
            m_Result.AddString(szText);
            m_Result.SetTopIndex(m_Result.GetCount()-1);
            
        }
    }
    else
    {
        szText.Format("当前无INT3断点");
        m_Result.AddString(szText);
        m_Result.SetTopIndex(m_Result.GetCount()-1);
        

    }


    //列举硬件断点
    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    
    if (!GetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 2251行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        return;
    }
    
    DR7 tagDr7={0};
    tagDr7.dwDr7=ct.Dr7;
    
    
    if (m_Dr_Use.Dr0)
    {
        szText.Format("硬件断点 断点地址:%08X 断点类型:%d  断点长度:%d",ct.Dr0,tagDr7.DRFlag.rw0,tagDr7.DRFlag.len0+1);
        m_Result.AddString(szText);
        m_Result.SetTopIndex(m_Result.GetCount()-1);
        
    }
    if (m_Dr_Use.Dr1)
    {
        szText.Format("硬件断点 断点地址:%08X 断点类型:%d  断点长度:%d",ct.Dr1,tagDr7.DRFlag.rw1,tagDr7.DRFlag.len1+1);
        m_Result.AddString(szText);
        m_Result.SetTopIndex(m_Result.GetCount()-1);
        
    }
    if (m_Dr_Use.Dr2)
    {
        szText.Format("硬件断点 断点地址:%08X 断点类型:%d  断点长度:%d",ct.Dr2,tagDr7.DRFlag.rw2,tagDr7.DRFlag.len2+1);
        m_Result.AddString(szText);
        m_Result.SetTopIndex(m_Result.GetCount()-1);
        
    }
    if (m_Dr_Use.Dr3)
    {
        szText.Format("硬件断点 断点地址:%08X 断点类型:%d  断点长度:%d",ct.Dr3,tagDr7.DRFlag.rw3,tagDr7.DRFlag.len3+1);
        m_Result.AddString(szText);
        m_Result.SetTopIndex(m_Result.GetCount()-1);
        
    }

    //列举内存断点
    pos=NULL;
    pos=m_MemBpList.GetHeadPosition();
    MEM_BP mBP={0};
    if (m_MemBpList.GetCount()!=0)
    {
        while (pos!=NULL)
        {
            mBP=m_MemBpList.GetNext(pos);
            switch (mBP.dwNumPage)
            {
            case 1:
                {
                    szText.Format("内存断点 断点地址:%08X 断点类型 %d 断点长度:%d  断点所跨分页:%08X",
                        mBP.dwBpAddress,mBP.dwAttribute,mBP.dwLength,mBP.dwMemPage[0]);
                    m_Result.AddString(szText);
                    m_Result.SetTopIndex(m_Result.GetCount()-1);
                    break;
                }
            case 2:
                {
                    szText.Format("内存断点 断点地址:%08X 断点类型 %d 断点长度:%d  断点所跨分页:%08X %08X",
                        mBP.dwBpAddress,mBP.dwAttribute,mBP.dwLength,mBP.dwMemPage[0],mBP.dwMemPage[1]);
                    m_Result.AddString(szText);
                    m_Result.SetTopIndex(m_Result.GetCount()-1);
                    break;
                }
            case 3:
                {
                    szText.Format("内存断点 断点地址:%08X 断点类型 %d 断点长度:%d  断点所跨分页:%08X %08X %08X",
                        mBP.dwBpAddress,mBP.dwAttribute,mBP.dwLength,mBP.dwMemPage[0],mBP.dwMemPage[1],
                        mBP.dwMemPage[2]);
                    m_Result.AddString(szText);
                    m_Result.SetTopIndex(m_Result.GetCount()-1);
                    
                    
                    break;
                }
            case 4:
                {
                    szText.Format("内存断点 断点地址:%08X 断点类型 %d 断点长度:%d  断点所跨分页:%08X %08X %08X %08X",
                        mBP.dwBpAddress,mBP.dwAttribute,mBP.dwLength,mBP.dwMemPage[0],mBP.dwMemPage[1],
                        mBP.dwMemPage[2],mBP.dwMemPage[3]);
                    m_Result.AddString(szText);
                    m_Result.SetTopIndex(m_Result.GetCount()-1);
                    break;
                }
            case 5:
                {
                    szText.Format("内存断点 断点地址:%08X 断点类型 %d 断点长度:%d  断点所跨分页:%08X %08X %08X %08X %08X",
                        mBP.dwBpAddress,mBP.dwAttribute,mBP.dwLength,mBP.dwMemPage[0],mBP.dwMemPage[1],
                        mBP.dwMemPage[2],mBP.dwMemPage[3],mBP.dwMemPage[4]);
                    m_Result.AddString(szText);
                    m_Result.SetTopIndex(m_Result.GetCount()-1);
                    
                    break;
                }
            }
            


        }

    }
    else
    {
        szText.Format("当前无内存断点");
        m_Result.AddString(szText);
        m_Result.SetTopIndex(m_Result.GetCount()-1);

    }






}

//G命令处理
void CEasyDbgDlg::ON_G_COMMAND(DWORD dwAddress)
{
    //如果不指定地址默认和F9处理一样
    if(dwAddress==0)
    {
        m_Result.ResetContent();
       ON_VK_F9();
       return;
    }
    INT3_BP bp={0};
    bp.dwAddress=dwAddress;

    DWORD dwOldProtect=0;
    DWORD dwRet=0;

    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);


    if (!ReadProcessMemory(m_tpInfo.hProcess,(LPVOID)dwAddress,&bp.bOriginalCode,sizeof(BYTE),NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 2392行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //向用户输出错误信息
        GetErrorMessage(dwErrorCode);
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,dwOldProtect,&dwRet);
        return;
    }
    //非永久断点
    bp.isForever=FALSE;
    //写入0XCC
    if (!WriteProcessMemory(m_tpInfo.hProcess,(LPVOID)dwAddress,&m_tpInfo.bCC,sizeof(BYTE),NULL))
    {

        OutputDebugString("EasyDbgDlg.cpp 2405行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //向用户输出错误信息
        GetErrorMessage(dwErrorCode);
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,dwOldProtect,&dwRet);
        return;

    }

    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,dwOldProtect,&dwRet);
    //加入断点链表
    m_Int3BpList.AddTail(bp);

    m_IsGo=TRUE;
    //运行
    ON_VK_F9();



}


//设置硬件断点 参数 地址 属性 长度
//dwAttribute 0表示执行断点 3表示访问断点 1 表示写入断点
//dwLength 取值 1 2 4
void CEasyDbgDlg::SetHardBP(DWORD dwBpAddress,DWORD dwAttribute,DWORD dwLength)
{
    

    if (dwLength!=1 && dwLength!=2 &&dwLength!=4)
    {
        AfxMessageBox("断点长度设置错误");
        return;
    }
    //强制把执行断点的长度改为1
    if (dwAttribute==0)
    {
        dwLength=1;
    }
    
    int nIndex=0;
    //获得当前空闲调寄存器编号
    nIndex=FindFreeDebugRegister();

    if (nIndex==-1)
    {
        AfxMessageBox("当前硬件断点已满,请删除在设置");
        return;
    }
    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 2460行出错");

        DWORD dwErrorCode=0;

        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        return;

    }
    //赋值我们定义的DR7结构体,这样省去了位移操作的繁琐
    DR7 tagDr7={0};
    tagDr7.dwDr7=ct.Dr7;

    switch (nIndex)
    {
    case 0:
        //中断地址
        ct.Dr0=dwBpAddress;
        //断点长度
        tagDr7.DRFlag.len0=dwLength-1;
        //属性
        tagDr7.DRFlag.rw0=dwAttribute;
        //局部断点
        tagDr7.DRFlag.L0=1;
        //设置标志位记录调试寄存器的使用情况
        m_Dr_Use.Dr0=TRUE;

        break;
    case 1:
        ct.Dr1=dwBpAddress;

        tagDr7.DRFlag.len1=dwLength-1;

        tagDr7.DRFlag.rw1=dwAttribute;

        tagDr7.DRFlag.L1=1;

        m_Dr_Use.Dr1=TRUE;
        

        break;
    case 2:
        ct.Dr2=dwBpAddress;

        tagDr7.DRFlag.len2=dwLength-1;
        
        tagDr7.DRFlag.rw2=dwAttribute;

        tagDr7.DRFlag.L2=1;

        m_Dr_Use.Dr2=TRUE;

        break;
    case 3:
        ct.Dr3=dwBpAddress;

        tagDr7.DRFlag.len3=dwLength-1;

        tagDr7.DRFlag.rw3=dwAttribute;

        tagDr7.DRFlag.L3=1;

        m_Dr_Use.Dr3=TRUE;
        break;
    }

    //赋值回去
    ct.Dr7=tagDr7.dwDr7;
    if (!SetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 2531行出错");
        DWORD dwErrorCode=0;

        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        return;
    }
    SetDlgItemText(IDC_STATE,"设置硬件断点成功");

   
}


//返回当前可用的调试寄存器
int CEasyDbgDlg::FindFreeDebugRegister()
{
     if (!m_Dr_Use.Dr0)
     {
         return 0;

     }
     if (!m_Dr_Use.Dr1)
     {
         return 1;
     }
     if (!m_Dr_Use.Dr2)
     {
         return 2;

     }
     if (!m_Dr_Use.Dr3)
     {
         return 3;
     }
     //如果Dr0-Dr3都被使用则返回-1
     return -1;
}

//删除硬件断点
void CEasyDbgDlg::DeleteHardBP(DWORD dwAddress)
{

    if (dwAddress==0)
    {
        AfxMessageBox("没输入断点地址");
        return;
    }
    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 2583行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        return;
    }
    DR7 tagDr7={0};
    tagDr7.dwDr7=ct.Dr7;
    //找到对应断点的调试寄存器
    int nIndex=GetDeletedDrIndex(dwAddress,ct);
    if (nIndex==-1)
    {
        AfxMessageBox("断点无效");
        return;
    }
   //清0并设置对应标志位为FALSE
    switch (nIndex)
    {
    case 0:
        //地址
        ct.Dr0=0;
        //属性
        tagDr7.DRFlag.rw0=0;
        //局部断点
        tagDr7.DRFlag.L0=0;
        //长度
        tagDr7.DRFlag.len0=0;

        m_Dr_Use.Dr0=FALSE;
        break;
    case 1:
        
        ct.Dr1=0;
        
        tagDr7.DRFlag.rw1=0;
        
        tagDr7.DRFlag.L1=0;
        
        tagDr7.DRFlag.len1=0;

        m_Dr_Use.Dr1=FALSE;
        break;
    case 2:
        
        ct.Dr2=0;
        
        tagDr7.DRFlag.rw2=0;
        
        tagDr7.DRFlag.L2=0;
        
        tagDr7.DRFlag.len2=0;

        m_Dr_Use.Dr2=FALSE;
        break;
    case 3:
        
        ct.Dr3=0;
        
        tagDr7.DRFlag.rw3=0;
        
        tagDr7.DRFlag.L3=0;
        
        tagDr7.DRFlag.len3=0;

        m_Dr_Use.Dr3=FALSE;
        break;
    }
    //赋值
    ct.Dr7=tagDr7.dwDr7;

    if (!SetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 2656行出错");

        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);

        return;

    }
    SetDlgItemText(IDC_STATE,TEXT("删除硬件断点成功"));


}


//获得要被删除的硬件断点的调试寄存器编号 返回-1表示没找到
int CEasyDbgDlg::GetDeletedDrIndex(DWORD dwAddress,CONTEXT ct)
{
    if (dwAddress == ct.Dr0)
    {
        return 0;
    }
    if (dwAddress == ct.Dr1)
    {
        return 1;
    }
    if (dwAddress == ct.Dr2)
    {
        return 2;
    }
    if (dwAddress == ct.Dr3)
    {
        return 3;
    }

    return -1;

}


//判断单步异常是否是硬件断点引起的 传出参数 断点地址
BOOL CEasyDbgDlg::IfStepHard(DWORD& dwBPAddress)
{

    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 2705行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        return FALSE;
    }
    //判断Dr6的低4位是否为0
    int nIndex=ct.Dr6 &0xf;
    if (nIndex==0)
    {
        return FALSE;
    }

    switch (nIndex)
    {
    case 0x1:
        //保存找到的断点地址
        dwBPAddress=ct.Dr0;
        break;
    case 0x2:
        dwBPAddress=ct.Dr1;
        break;
    case 0x4:
        dwBPAddress=ct.Dr2;
        break;
    case 0x8:
       dwBPAddress=ct.Dr3;
        break;
    }
    return TRUE;


}

//使硬件断点暂时无效
void CEasyDbgDlg::InvalidHardBP(DWORD dwBpAddress)
{
    //传入参数的判断
    if (dwBpAddress==0)
    {
        AfxMessageBox("断点为0无效值");
        return;
    }

    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 2754行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        return;
    }
    //判断中断地址在那个调试寄存器中
    DR7 tagDr7={0};
    tagDr7.dwDr7=ct.Dr7;
//     DWORD dwDr0=ct.Dr0;
//     DWORD dwDr1=ct.Dr1;
//     DWORD dwDr2=ct.Dr2;
//     DWORD dwDr3=ct.Dr3;
//     switch (dwBpAddress)
//     {
// 
//     case dwDr0:
//         //清L位让断点无效 不清地址了,免得还得保存地址
//         tagDr7.DRFlag.L0=0;
//         //设置要恢复的调试寄存器编号
//         m_Recover_HBP.dwIndex=0;
// 
//         break;
//     case dwDr1:
//         tagDr7.DRFlag.L1=0;
//         m_Recover_HBP.dwIndex=1;
// 
//         break;
//     case dwDr2:
//         tagDr7.DRFlag.L2=0;
//         m_Recover_HBP.dwIndex=2;
// 
//         break;
//     case dwDr3:
//         tagDr7.DRFlag.L3=0;
//         m_Recover_HBP.dwIndex=3;
//         break;
// 
//     }
    if (ct.Dr0==dwBpAddress)
    {
        //清L位让断点无效 不清地址了,免得还得保存地址
        tagDr7.DRFlag.L0=0;
        //设置要恢复的调试寄存器编号
        m_Recover_HBP.dwIndex=0;

    }

    if (ct.Dr1==dwBpAddress)
    {
        tagDr7.DRFlag.L1=0;
        m_Recover_HBP.dwIndex=1;

    }

    if (ct.Dr2==dwBpAddress)
    {
        tagDr7.DRFlag.L2=0;
        m_Recover_HBP.dwIndex=2;

    }

    if (ct.Dr3==dwBpAddress)
    {
        tagDr7.DRFlag.L3=0;
        m_Recover_HBP.dwIndex=3;     
    }

    //赋值回去
    ct.Dr7=tagDr7.dwDr7;
    //设置线程上下文
    if (!SetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 2828行出错");

        DWORD dwErrorCode=0;

        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        return;
    }


}



//恢复硬件断点 参数为 调试寄存器的编号
void CEasyDbgDlg::RecoverHBP(DWORD dwIndex)
{
    //传入参数的判断
    if (dwIndex==-1)
    {
        AfxMessageBox("恢复硬件断点出错");
        return;
    }

    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    if (!GetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 2857行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        return;
    }
    
    DR7 tagDr7={0};
    tagDr7.dwDr7=ct.Dr7;

    switch (dwIndex)
    {

    case 0:
        //设置L位
        tagDr7.DRFlag.L0=1;
        m_Recover_HBP.dwIndex=-1;
        break;
    case 1:
        tagDr7.DRFlag.L1=1;
        m_Recover_HBP.dwIndex=-1;
        break;
    case 2:
        tagDr7.DRFlag.L2=1;
        m_Recover_HBP.dwIndex=-1;
        break;
    case 3:
        tagDr7.DRFlag.L3=1;
        m_Recover_HBP.dwIndex=-1;
        break;

    }
    //写 回CONTEXT
    ct.Dr7=tagDr7.dwDr7;

    if (!SetThreadContext(m_tpInfo.hThread,&ct))
    {
        OutputDebugString("EasyDbgDlg.cpp 2895行出错");
        
        DWORD dwErrorCode=0;
        
        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        return;
    }



    

}




//设置内存断点  dwAttribute 1表示写入断点 3表示访问断点
void CEasyDbgDlg::SetMemBP(DWORD dwBpAddress,DWORD dwAttribute,DWORD dwLength)
{
    if (dwAttribute!=1 && dwAttribute!=3)
    {
        AfxMessageBox("内存断点类型弄错");
        return;
    }
    
    MEMORY_BASIC_INFORMATION mbi={0};
    MEM_BP mbp={0};

    if (!IsAddressValid(dwBpAddress,mbi))
    {
        AfxMessageBox("断点地址无效");
        return;
    }
    //判断地址和长度占了几个分页并加入内存分页表 也把断点加入断点表
    if (!AddMemBpPage(dwBpAddress,dwLength,mbi,dwAttribute,mbp))
    {
        AfxMessageBox("断点添加失败");
        return;
    }
    //该保护属性
    for (DWORD i=0;i<mbp.dwNumPage;i++)
    {
        DWORD dwOldProtect=0;
        
        if (!VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)mbp.dwMemPage[i],4,m_Attribute[dwAttribute],&dwOldProtect))
        {
            OutputDebugString("EasyDbgDlg.cpp 2944行出错");
            DWORD dwErrorCode=0;
            dwErrorCode=GetLastError();
            //输出错误信息
            GetErrorMessage(dwErrorCode);
            AfxMessageBox("断点加入失败");
            return;

        }
        
        
    }

   SetDlgItemText(IDC_STATE,"内存断点设置成功");  
    
}


//判断地址是否有效
BOOL CEasyDbgDlg::IsAddressValid(DWORD dwAddress,MEMORY_BASIC_INFORMATION& mbi)
{
    

    DWORD dwRet=0;

    dwRet=VirtualQueryEx(m_tpInfo.hProcess,(LPVOID)dwAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
    //返回值与其缓冲区长度不相同则表示地址无效
    if (dwRet!=sizeof(MEMORY_BASIC_INFORMATION))
    {
        return FALSE;
    }
    //MEM_FREE 的不能访问 MEM_RESERVE的保护属性未知
    if (mbi.State==MEM_COMMIT)
    {
        return TRUE;
    }

    return FALSE;
}


//判断地址和长度占了几个分页并加入内存分页表 并把断点也加入断点链表
BOOL CEasyDbgDlg::AddMemBpPage(DWORD dwBpAddress,DWORD dwLength,MEMORY_BASIC_INFORMATION mbi,DWORD dwAttribute,MEM_BP& mbp)
{
    //如果在一个分页中(不跨分页)
    MEM_BP_PAGE mBPage={0};
    
    
    if (dwBpAddress>=(DWORD)mbi.BaseAddress && (DWORD)mbi.BaseAddress+mbi.RegionSize>=dwBpAddress+dwLength)
    {
        mBPage.dwBaseAddress=(DWORD)mbi.BaseAddress;
        mBPage.dwProtect=mbi.Protect;
        //在内存链表中没找到就添加
        if (!FindMemPage((DWORD)mbi.BaseAddress))
        {
              m_MemPageList.AddTail(mBPage);
        }
        //添加内存链表
        mbp.dwAttribute=dwAttribute;
        mbp.dwBpAddress=dwBpAddress;
        mbp.dwLength=dwLength;
        mbp.dwMemPage[0]=mBPage.dwBaseAddress;
        mbp.dwNumPage=1;
        //查看该地址处是否已经有内存断点如果有不能在下断点
        if (FindMemBP(dwBpAddress))
        {
             AfxMessageBox("该地址已经有内存断点,不能在下断点");
             return FALSE;
        }
        else
        {   
            //添加到断点链表
            m_MemBpList.AddTail(mbp);
        }
        return TRUE;


    }
    //跨多个分页的情况 因为跨太多页属于脑残行为,因为就不建立所有的内存页链表了
    //直接比较 其实跨3个页的就属于脑残行为....
    int i=0;

    mbp.dwAttribute=dwAttribute;
    mbp.dwBpAddress=dwBpAddress;
    mbp.dwLength=dwLength;

    while ((DWORD)mbi.BaseAddress+mbi.RegionSize<dwBpAddress+dwLength)
    {
        if (i>4)
        {
            AfxMessageBox("我对你无语下这么多分页的内存断点");
            return FALSE;
        }
        mBPage.dwBaseAddress=(DWORD)mbi.BaseAddress;
        mBPage.dwProtect=mbi.Protect;
        //在内存链表中没找到就添加
        if (!FindMemPage((DWORD)mbi.BaseAddress))
        {
            m_MemPageList.AddTail(mBPage);
        }
        mbp.dwMemPage[i]=mBPage.dwBaseAddress;

        DWORD dwRet=0;

        //找下一个分页
        dwRet=VirtualQueryEx(m_tpInfo.hProcess,(LPVOID)((DWORD)mbi.BaseAddress+mbi.RegionSize),&mbi,sizeof(MEMORY_BASIC_INFORMATION));
        //返回值与其缓冲区长度不相同则表示地址无效
        if (dwRet!=sizeof(MEMORY_BASIC_INFORMATION))
        {
            return FALSE;
        }

        i++;

    
    }

    if (i>4)
    {
        AfxMessageBox("我对你无语下这么多分页的内存断点");
        return FALSE;
    }

    mBPage.dwBaseAddress=(DWORD)mbi.BaseAddress;
    mBPage.dwProtect=mbi.Protect;
    //在内存链表中没找到就添加
    if (!FindMemPage((DWORD)mbi.BaseAddress))
    {
        m_MemPageList.AddTail(mBPage);
    }
    mbp.dwMemPage[i]=mBPage.dwBaseAddress;

    if (FindMemBP(dwBpAddress))
    {
        AfxMessageBox("该地址已经有内存断点,不能在下断点");
        return FALSE;
    }
    else
    {   
        //添加到断点链表
        m_MemBpList.AddTail(mbp);
    }
    return TRUE;

}

//判断某一页首地址是否存在于页链表中
BOOL CEasyDbgDlg::FindMemPage(DWORD dwBaseAddress)
{

    POSITION pos;
    pos=m_MemPageList.GetHeadPosition();
    while (pos!=NULL)
    {
        MEM_BP_PAGE mBPage={0};
        mBPage=m_MemPageList.GetNext(pos);
        //如果找到返回TRUE
        if (mBPage.dwBaseAddress==dwBaseAddress)
        {
            return TRUE;
          
        }
    }
    return FALSE;
}


//判断地址是否重复下内存断点
BOOL CEasyDbgDlg::FindMemBP(DWORD dwBpAddress)
{
     POSITION pos=NULL;
     pos=m_MemBpList.GetHeadPosition();
     while (pos!=NULL)
     {
         MEM_BP memBp={0};
         memBp=m_MemBpList.GetNext(pos);
         //如果找到返回TRUE
         if (memBp.dwBpAddress==dwBpAddress)
         {
             return TRUE;
         }

     }
     return FALSE;
}

//处理EXCEPTION_ACCESS_VIOLATION异常 参数 异常地址
DWORD CEasyDbgDlg::ON_EXCEPTION_ACCESS_VIOLATION(DWORD dwExpAddress,DWORD dwAccessAddress)
{
    
    if (dwAccessAddress==0)
    {
        return DBG_EXCEPTION_NOT_HANDLED;
    }
    //得出当前地址所在的分页
    DWORD dwRet=0;
    MEMORY_BASIC_INFORMATION mbi={0};
    dwRet=VirtualQueryEx(m_tpInfo.hProcess,(LPVOID)dwAccessAddress,&mbi,sizeof(MEMORY_BASIC_INFORMATION));
    //返回值与其缓冲区长度不相同则表示地址无效
    if (dwRet!=sizeof(MEMORY_BASIC_INFORMATION))
    {
        OutputDebugString("EasyDbgDlg.cpp 3143行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();

        //输出错误信息
        GetErrorMessage(dwErrorCode);

        return DBG_EXCEPTION_NOT_HANDLED;
    }
    //如果不是断点 且在内存分页表中找到这个首地址
    if (!FindMemBP(dwAccessAddress))
    {
        if (FindMemOriginalProtect(mbi))
        {
            //恢复断点
            RecoverMemBP((DWORD)mbi.BaseAddress,mbi.Protect);

            
            return DBG_CONTINUE;
        }
        //如果没找到 那就是应用程序自己的访问异常
        else
        {

            return DBG_EXCEPTION_NOT_HANDLED;

        }
    
    }
    //如果是断点

    if (FindMemOriginalProtect(mbi))
    {
        SetDlgItemText(IDC_STATE,TEXT("内存断点到达"));
        //恢复断点
        RecoverMemBP((DWORD)mbi.BaseAddress,mbi.Protect);
        //如果是GO模式,就暂停下来显示反汇编寄存器等 其他模式直接走过去
        if (m_IsGo)
        {
            ShowAsm(dwExpAddress);
            ShowReg(m_tpInfo.hThread);
            m_IsGo=FALSE;
            CONTEXT ct={0};
            ct.ContextFlags=CONTEXT_FULL;
            GetThreadContext(m_tpInfo.hThread,&ct);
            ct.EFlags|=0x100;
            SetThreadContext(m_tpInfo.hThread,&ct);
            //做个标志 让在单步中不显示
            m_isMoreMem=TRUE;
            WaitForSingleObject(hEvent,INFINITE);
            
        }
        
        
        return DBG_CONTINUE;
    }
    //如果没找到 那就是应用程序自己的访问异常
    else
    {
        
        return DBG_EXCEPTION_NOT_HANDLED;
        
    }



   

    return DBG_CONTINUE;
    
}


//暂时恢复内存断点(使内存断点无效) 参数 内存页地址 原保护属性
void CEasyDbgDlg::RecoverMemBP(DWORD dwBaseAddress,DWORD dwProtect)
{
    if (dwBaseAddress==0)
    {
        AfxMessageBox("内存断点恢复失败");
        return;
    }
    m_Recover_Mpage.dwBaseAddress=dwBaseAddress;
    m_Recover_Mpage.isNeedRecover=TRUE;

    DWORD dwOldProtect=0;
    if (!VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBaseAddress,4,dwProtect,&dwOldProtect))
    {
        
        OutputDebugString("EasyDbgDlg.cpp 3232行出错");
        DWORD dwErrorCode=0;

        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        return;
    }
    //保存断点的保护属性
    m_Recover_Mpage.dwProtect=dwOldProtect;
    

}


//找到原始内存页链表的对应属性并传出 引用类型
BOOL CEasyDbgDlg::FindMemOriginalProtect(MEMORY_BASIC_INFORMATION& mbi)
{

    POSITION pos;
    pos=m_MemPageList.GetHeadPosition();
    while (pos!=NULL)
    {
        MEM_BP_PAGE mBPage={0};
        mBPage=m_MemPageList.GetNext(pos);
        //如果找到返回TRUE
        if (mBPage.dwBaseAddress==(DWORD)mbi.BaseAddress)
        {
            //赋值原始属性
            mbi.Protect=mBPage.dwProtect;
            return TRUE;
          
        }
    }
    return FALSE;


}

//删除内存断点
void CEasyDbgDlg::DeleteMemBP(DWORD dwBpAddress)
{
    MEM_BP mBP={0};
    //找到内存断点并从链表中移除
    if (!FindMemBPInformation(mBP,dwBpAddress))
    {
        AfxMessageBox("此地址不是断点");
        return;
    }

    for (DWORD i=0;i<mBP.dwNumPage;i++)
    {
        //先判断有没有另一个内存断点在这个分页上,如果存在就修改为另一个断点所要求的属性
        if (!ModifyPageProtect(mBP.dwMemPage[i]))
        {
            //如果没有其他内存断点在此内存页上就直接遍历内存页表修改为原来的属性
            MEMORY_BASIC_INFORMATION mbi={0};
            mbi.BaseAddress=(PVOID)mBP.dwMemPage[i];
            //如果成功返回原属性
            if (FindMemOriginalProtect(mbi))
            {
                DWORD dwOldProtect=0;
                //修改
                if (!VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)mBP.dwMemPage[i],4,mbi.Protect,&dwOldProtect))
                {
                    OutputDebugString("EasyDbgDlg.cpp 3299行出错");
                    DWORD dwErrorCode=0;
                    dwErrorCode=GetLastError();
                    //输出错误信息
                    GetErrorMessage(dwErrorCode);
                }
                

            }

        }


    }


}

//找到符合的内存断点信息并返回 参数类型为引用 并从链表中删除此元素
BOOL CEasyDbgDlg::FindMemBPInformation(MEM_BP& mBP,DWORD dwBpAddress)
{

    POSITION pos=NULL;
    pos=m_MemBpList.GetHeadPosition();
    while (pos!=NULL)
    {
        
        mBP=m_MemBpList.GetNext(pos);
        //如果找到返回TRUE
        if (mBP.dwBpAddress==dwBpAddress)
        {
            
            if (m_MemBpList.GetCount()==1)
            {
                m_MemBpList.RemoveHead();
                
                SetDlgItemText(IDC_STATE,"内存断点删除成功");
                return TRUE;
                
            }
            
            if (pos==NULL)
            {
                m_MemBpList.RemoveTail();
                
                SetDlgItemText(IDC_STATE,"内存断点删除成功");
                return TRUE;
                           
            }
            
            m_MemBpList.GetPrev(pos);
            
            m_MemBpList.RemoveAt(pos);
            SetDlgItemText(IDC_STATE,"内存断点删除成功");

            return TRUE;
        }
        
    }
     return FALSE;

}



//先判断有没有另一个内存断点在这个分页上,如果存在就修改为另一个断点所要求的属性
//参数 内存页的首地址
BOOL CEasyDbgDlg::ModifyPageProtect(DWORD dwBaseAddress)
{
    POSITION pos=NULL;
    pos=m_MemBpList.GetHeadPosition();
    
    MEM_BP mBP={0};
    BOOL isFind=FALSE;
     //遍历
    while(pos!=NULL)
    {
        mBP=m_MemBpList.GetNext(pos);
        for (DWORD i=0;i<mBP.dwNumPage;i++)
        {
            //如果找到内存断点还在此内存页面上
            if (mBP.dwMemPage[i]==dwBaseAddress)
            {
                isFind=TRUE;
                DWORD dwOldProtect=0;
                //就把内存保护属性恢复为他的所需要的
                if (!VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwBaseAddress,4,
                    m_Attribute[mBP.dwAttribute],&dwOldProtect))
                {
                    OutputDebugString("EasyDbgDlg.cpp 3387行出错");
                    DWORD dwErrorCode=0;
                    dwErrorCode=GetLastError();
                    
                    GetErrorMessage(dwErrorCode);
                }
            }
        }


    }
    
    return isFind;
}




//PE查看工具
void CEasyDbgDlg::OnViewpe() 
{
	// TODO: Add your command handler code here

    //默认查看的是被调试程序的PE信息
    CPeScan dlg;
    dlg.DoModal();
	
}

//映射文件 并检查PE有效性以及是不是EXE文件
BOOL CEasyDbgDlg::MapPEFile()
{
    HANDLE hFile=NULL;
    //打开文件获得文件句柄
    hFile=CreateFile(m_SzFilePath,GENERIC_ALL,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if (hFile==INVALID_HANDLE_VALUE)
    {
        OutputDebugString("EasyDbgDlg.cpp 3424行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        
        return FALSE;
    }
    HANDLE hFileMap=NULL;
    //创建文件映射
    hFileMap=CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,0,NULL);
    if (hFileMap==NULL)
    {
        OutputDebugString("EasyDbgDlg.cpp 3437行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        GetErrorMessage(dwErrorCode);
        CloseHandle(hFile);
        return FALSE;
    }
    //映射文件
    pFile=(char*)MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS,0,0,0);
    if (pFile==NULL)
    {
        OutputDebugString("EasyDbgDlg.cpp 3448行出错");
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        GetErrorMessage(dwErrorCode);
        
        CloseHandle(hFile);
        CloseHandle(hFileMap);
        return FALSE;
    }

     //判断PE有效性
    PIMAGE_DOS_HEADER pDos=NULL;
    pDos=(PIMAGE_DOS_HEADER)pFile;
    PIMAGE_NT_HEADERS pNt=(PIMAGE_NT_HEADERS)(pFile+pDos->e_lfanew);
    
    //检查MZ PE 两个标志
    if (pDos->e_magic!=IMAGE_DOS_SIGNATURE || pNt->Signature!=IMAGE_NT_SIGNATURE)
    {
        AfxMessageBox("不是有效的PE文件");
        CloseHandle(hFile);
        CloseHandle(hFileMap);
        return FALSE;
    }
    if (pNt->FileHeader.Characteristics&IMAGE_FILE_DLL)
    {
        AfxMessageBox("该文件是DLL,EXE文件");
        CloseHandle(hFile);
        CloseHandle(hFileMap);
        return FALSE;
    }
    
   
    CloseHandle(hFile);
    CloseHandle(hFileMap);

    return TRUE;
    
}




// 获得导入表函数地址
BOOL CEasyDbgDlg::GetExportFunAddress(HANDLE hFile,char* pDll,LPVOID pBase)
{
   

    PIMAGE_DOS_HEADER pDos=NULL;
    PIMAGE_FILE_HEADER pFileHeader=NULL;
    PIMAGE_OPTIONAL_HEADER pOption=NULL;
    PIMAGE_SECTION_HEADER pSec=NULL;

    //获取各结构的指针
    pDos=(PIMAGE_DOS_HEADER)pDll;
 
    pFileHeader=(PIMAGE_FILE_HEADER)(pDll+pDos->e_lfanew+4);
    pOption=(PIMAGE_OPTIONAL_HEADER)((char*)pFileHeader+sizeof(IMAGE_FILE_HEADER));
    pSec=(PIMAGE_SECTION_HEADER)((char*)pOption+pFileHeader->SizeOfOptionalHeader);
    //节表数目
    DWORD dwSecNum=0;
    dwSecNum=pFileHeader->NumberOfSections;
    //导出表偏移
    DWORD dwExportRva=0;
       
    dwExportRva=pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;


    DWORD dwExportOffset=0;
    //获得导入表的文件偏移
    dwExportOffset=RvaToFileOffset(dwExportRva,dwSecNum,pSec);
    PIMAGE_EXPORT_DIRECTORY pExp=NULL;
    pExp=(PIMAGE_EXPORT_DIRECTORY)(pDll+dwExportOffset);

    EXPORT_FUN_INFO ExFun={0};
   
    
    DWORD dwNameOffset=0;
    dwNameOffset=RvaToFileOffset(pExp->Name,dwSecNum,pSec);
    char*pName=NULL;
    //DLL名
    pName=(char*)(pDll+dwNameOffset);
    strcpy(ExFun.szDLLName,pName);
    
    DWORD dwBase=0;
    dwBase=pExp->Base;
    DWORD dwFunNum=0;
    dwFunNum=pExp->NumberOfFunctions;
    for (DWORD j=0;j<dwFunNum;j++)
    {
        //先遍历函数地址数组
        PDWORD pAddr=(PDWORD)(pDll+RvaToFileOffset(pExp->AddressOfFunctions,dwSecNum,pSec));
        //地址有效
        if (pAddr[j]!=0)
        {
            //通过序号得到相应函数名数组下标
            //序号数组
            PWORD pNum=(PWORD)(pDll+RvaToFileOffset(pExp->AddressOfNameOrdinals,dwSecNum,pSec));
            for (WORD k=0;k<pExp->NumberOfNames;k++ )
            {
                //在序号数组里找序号相同的 找到下标然后读函数名
                if (j==pNum[k])
                {
                    //导出函数名(或变量名数组) 得到的是RVA
                    PDWORD pName=(PDWORD)(pDll+RvaToFileOffset(pExp->AddressOfNames,dwSecNum,pSec));
                    
                    char *pszName=(char*)(pDll+RvaToFileOffset(pName[k],dwSecNum,pSec));
                    
                    memcpy(&ExFun.szFunName,pszName,strlen(pszName)+1);
                    
                    
                    if (pBase)
                    {
                        ExFun.dwAddress=(DWORD)pBase+pAddr[j];
                        //加入CMAP中
                        m_ExFunList.SetAt(ExFun.dwAddress,ExFun);
                        //加入函数名与地址对应表
                        m_Fun_Address.SetAt(pszName,ExFun.dwAddress);
                    }
                    
                    
                    break;
                }
            }
            
            
        }
       
        
    }


    return TRUE;
  
}


//参数一 导入表的RVA 参数2区块表的数目 参数3区块表的首地址
DWORD CEasyDbgDlg::RvaToFileOffset(DWORD dwRva,DWORD dwSecNum,PIMAGE_SECTION_HEADER pSec)
{
    if (dwSecNum==0)
    {
        return 0;
    }
    
    for (DWORD i=0;i<dwSecNum;i++)
    {
        
        if (dwRva>=pSec[i].VirtualAddress&&dwRva<pSec[i].VirtualAddress+pSec[i].SizeOfRawData)
        {
            
            return dwRva-pSec[i].VirtualAddress+pSec[i].PointerToRawData;
            
        }
    }
    return 0;
    
}


//处理加载DLL事件
void CEasyDbgDlg::ON_LOAD_DLL_DEBUG_EVENT(HANDLE hFile,LPVOID pBase)
{
    if (hFile==NULL || pBase==NULL)
    {
        return;
    }
    GetFileNameFromHandle(hFile,pBase);

    
}

//判断解析指令中的函数调用
BOOL CEasyDbgDlg::IsExportFun(char* szBuffer, EXPORT_FUN_INFO& expFun)
{
    if (szBuffer==NULL)
    {
        return FALSE;
    }
    //指令长度
    int nLength=0;
    nLength=strlen(szBuffer);
    char szCall[5]={0};
    char szJmp[4]={0};
    //看是不是CALL JMP之类的 //对CALL 寄存器要处理
    //对 call [00400000] call dword ptr[00400000]  jmp [00400000]进行解析 注意一定要有[]否则是在调用自身函数
    memcpy(szCall,szBuffer,4);
    memcpy(szJmp,szBuffer,3);
     

    //暂时不处理CALL reg的情况
    if (szBuffer[5]=='e')
    {
        return FALSE;
    }



    if (strcmp(szCall,"call")==0||strcmp(szJmp,"jmp")==0)
    {
       //如果直接是[]则直接解析
       if (nLength!=0 && szBuffer[nLength-1]==']')
       {
           //找到[]内的地址值 并解析函数名
           char Address[10]={0};
           for (int i=0;i<8;i++)
           {
               Address[i]=szBuffer[nLength-9+i];

           }
           DWORD dwAddress = 0 ;
         
          sscanf(Address, "%x", &dwAddress);
          //读取地址值处的内容
          DWORD dwActualAddress=0;
          //修改保护属性 
          DWORD dwOldProtect=0;
          DWORD dwRet=0;
          VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,PAGE_READONLY,&dwOldProtect);
          if (!ReadProcessMemory(m_tpInfo.hProcess,(LPVOID)dwAddress,&dwActualAddress,sizeof(DWORD),NULL))
          {
              OutputDebugString("EasyDbgDlg.cpp 3669行出错");
              DWORD dwErrorCode=0;
              dwErrorCode=GetLastError();
            
              
              VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,dwOldProtect,&dwRet);
              return FALSE;
          }
         
        
          VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,dwOldProtect,&dwRet);
          //查询有没有符合的函数地址
          if (m_ExFunList.Lookup(dwActualAddress,expFun))
          {
              return TRUE;
          }



       }


       //如果不是[]就看其下一条是不是[]
       if (nLength!=0 && szBuffer[nLength-1]!=']')
       {
           //不解析多级跳了 直解析到两级

           //找到地址值 反汇编下一条指令
           char Address[10]={0};
           for (int j=0;j<8;j++)
           {
               Address[j]=szBuffer[nLength-8+j];
               
           }
           DWORD dwAddress = 0 ;
           
           sscanf(Address, "%x", &dwAddress);
           //读取地址值处的内容
           DWORD dwActualAddress=0;
           //修改保护属性 
           DWORD dwOldProtect=0;
           DWORD dwRet=0;
           //这里就不判断了判断Read是一样的效果
           VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);
           BYTE pCode[40]={0};
           if (!ReadProcessMemory(m_tpInfo.hProcess,(LPVOID)dwAddress,&pCode,sizeof(pCode),NULL))
           {
               OutputDebugString("EasyDbgDlg.cpp 3717行出错");
               DWORD dwErrorCode=0;
               dwErrorCode=GetLastError();

               //GetErrorMessage(dwErrorCode);
               return FALSE;
           }

           VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,dwOldProtect,&dwRet);

           
           for (int i=0;i<16;i++)
           {
               
               POSITION pos=NULL;
               pos = m_Int3BpList.GetHeadPosition();
               while(pos!=NULL)
               {
                   INT3_BP bp=m_Int3BpList.GetNext(pos);
                   //判断断点地址是否命名在这段缓冲区中
                   if (bp.dwAddress==dwAddress+i)
                   {
                       //如果命中 则说明此为用户断点则把原字节还原
                       pCode[i]=bp.bOriginalCode;
                   }
               }
           }

           char szAsm[120]={0};
           char szOpCode[120]={0};
           UINT CodeSize=0;
           Decode2AsmOpcode(pCode,szAsm,szOpCode,&CodeSize,dwAddress);
           //判断本条指令

           //指令长度
           int nLength=0;
           nLength=strlen(szAsm);
           char szCall[5]={0};
           char szJmp[4]={0};
           //看是不是CALL JMP之类的
           //对 call [00400000] call dword ptr[00400000]  jmp [00400000]进行解析 注意一定要有[]否则是在调用自身函数
           memcpy(szCall,szBuffer,4);
           memcpy(szJmp,szBuffer,3);
           if (strcmp(szCall,"call")==0||strcmp(szJmp,"jmp")==0)
           {
               //如果直接是[]则直接解析
               if (nLength!=0 && szAsm[nLength-1]==']')
               {
                   //找到[]内的地址值 并解析函数名
                   char Address[10]={0};
                   for (int i=0;i<8;i++)
                   {
                       Address[i]=szAsm[nLength-9+i];
                       
                   }
                   DWORD dwAddress = 0 ;
                   
                   sscanf(Address, "%x", &dwAddress);
                   //读取地址值处的内容
                   DWORD dwActualAddress=0;
                   //修改保护属性 
                   DWORD dwOldProtect=0;
                   DWORD dwRet=0;
                   VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,PAGE_READONLY,&dwOldProtect);
                   if (!ReadProcessMemory(m_tpInfo.hProcess,(LPVOID)dwAddress,&dwActualAddress,sizeof(DWORD),NULL))
                   {
                       OutputDebugString("EasyDbgDlg.cpp 3783行出错");
                       DWORD dwErrorCode=0;
                       dwErrorCode=GetLastError();
                      // GetErrorMessage(dwErrorCode);
                       
                       VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,dwOldProtect,&dwRet);
                       return FALSE;
                   }
                   
                  
                   VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwAddress,4,dwOldProtect,&dwRet);
                   //查询有没有符合的函数地址
                   if (m_ExFunList.Lookup(dwActualAddress,expFun))
                   {
                       return TRUE;
                   }
                   
                   
                   
               }
           }
           
       }
    }
return FALSE;

}




//删除所有断点 用于记录指令
void CEasyDbgDlg::DeleteAllBreakPoint()
{


    POSITION pos=NULL;

    //删除所有的内存断点
    pos=m_MemBpList.GetHeadPosition();
    MEM_BP memBP={0};
    while (pos!=NULL)
    {
        memBP=m_MemBpList.GetNext(pos);
        DeleteMemBP(memBP.dwBpAddress);
        
        
    }
    //删除所有的INT3断点
    INT3_BP bp={0};
    pos=NULL;
    pos=m_Int3BpList.GetHeadPosition();
    while (pos!=NULL)
    {
        bp=m_Int3BpList.GetNext(pos);
        //恢复为原来的字节
        RecoverBP(m_tpInfo.hProcess,bp.dwAddress,bp.bOriginalCode);
            
        
    }

    m_Int3BpList.RemoveAll();

    //删除所有的硬件断点

    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    GetThreadContext(m_tpInfo.hThread,&ct);
    if (ct.Dr0!=0)
    {
        DeleteHardBP(ct.Dr0);
        
    }
    if (ct.Dr1!=0)
    {
        DeleteHardBP(ct.Dr1);
        
    }
    if (ct.Dr2!=0)
    {
        DeleteHardBP(ct.Dr2);
        
    }
    if (ct.Dr3!=0)
    {
        DeleteHardBP(ct.Dr3);
        
    }

}


//自动步过 
void CEasyDbgDlg::OnAutostepout() 
{
	// TODO: Add your command handler code here
    m_hFile=INVALID_HANDLE_VALUE;
   
    char            szFileName[MAX_PATH] = "Record";	
    OPENFILENAME    ofn={0};
    char            CodeBuf[24] = {0};
    ofn.lStructSize  = sizeof(OPENFILENAME);
    ofn.lpstrFile	 = szFileName;
    ofn.lpstrDefExt="txt";
    ofn.nMaxFile     = MAX_PATH;
    ofn.lpstrFilter  = "填入要保存的指令记录文件名(*.txt)\0*.txt\0";
    ofn.nFilterIndex = 1;
    if( GetSaveFileName(&ofn) == FALSE)
    {
        return ;
    }
     //创建文件
    m_hFile=CreateFile(szFileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    //自动单步 删除所有断点
    DeleteAllBreakPoint();
    m_IsAutoF8=TRUE;
    ON_VK_F8();

	
}

//自动步入
void CEasyDbgDlg::OnAutostepinto() 
{
    DeleteAllBreakPoint();
    m_IsAutoF7=TRUE;
    ON_VK_F7();
	// TODO: Add your command handler code here
	
}

//把记录写入文件  参数 指令地址 指令缓冲 不显示机器码 了,
//显示了在文本文件中不好对齐 
void CEasyDbgDlg::WriteOpcodeToFile(DWORD dwAddress,char* szAsm)
{
    if (m_hFile==INVALID_HANDLE_VALUE||szAsm==NULL || dwAddress ==0)
    {
        return;
    }

    DWORD dwLength=0;
    dwLength=strlen(szAsm);
    //回车换行
    szAsm[dwLength]='\r';
    szAsm[dwLength+1]='\n';
    char szBuffer[16]={0};

    sprintf(szBuffer,"%08X",dwAddress);

    WriteFile(m_hFile,(LPVOID)szBuffer,sizeof(szBuffer),&dwLength,NULL);
  
    WriteFile(m_hFile,(LPVOID)szAsm,strlen(szAsm),&dwLength,NULL);


    

    

}


//跳出函数  仅适用于MOV EBP ,ESP指令之后 POP EBP之前 利用堆栈原理读取返回地址
void CEasyDbgDlg::StepOutFromFun()
{
    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL;
    if (GetThreadContext(m_tpInfo.hThread,&ct)==0)
    {
        return;
    }
    
    DWORD dwBpAddress=0;
    DWORD dwOldProtect=0;
    DWORD dwRet=0;
    //读取函数的返回地址
    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)(ct.Ebp+4),4,PAGE_EXECUTE_READWRITE,&dwOldProtect);
    if (!ReadProcessMemory(m_tpInfo.hProcess,(LPVOID)(ct.Ebp+4),&dwBpAddress,4,NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 3961行出错");
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)(ct.Ebp+4),4,dwOldProtect,&dwRet);
        return;

    }
    VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)(ct.Ebp+4),4,dwOldProtect,&dwRet);
    ON_G_COMMAND(dwBpAddress);
    



}


//跳出函数体
void CEasyDbgDlg::OnOutfun() 
{
	// TODO: Add your command handler code here
    StepOutFromFun();
	
}

//显示堆栈
void CEasyDbgDlg::ShowStack()
{
    m_Stack.DeleteAllItems();
    CONTEXT ct={0};
    ct.ContextFlags=CONTEXT_FULL;
    if (GetThreadContext(m_tpInfo.hThread,&ct)==0)
    {
        return;
    }
    CString szText;
    for (int i=0;i<20;i++)
    {
        szText.Format("%08X",(ct.Esp+i*4));
        m_Stack.InsertItem(i,szText);
        DWORD dwRet=0;
        DWORD dwOldProtect=0;
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)(ct.Esp+i*4),4,PAGE_EXECUTE_READWRITE,&dwOldProtect);
        if (!ReadProcessMemory(m_tpInfo.hProcess,(LPVOID)(ct.Esp+i*4),&dwRet,4,NULL))
        {
            VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)(ct.Esp+i*4),4,dwOldProtect,&dwRet);
            if (i<20)
            {
                continue;
            }
            return;
        }
        
        szText.Format("%08X",dwRet);
        m_Stack.SetItemText(i,1,szText);
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)(ct.Esp+i*4),4,dwOldProtect,&dwRet);
        
    }

    

}

//DUMP
void CEasyDbgDlg::OnDump() 
{
	// TODO: Add your command handler code here
	DumpTargetProcess();
}


//DUMP被调试进程
void CEasyDbgDlg::DumpTargetProcess()
{


    char            szFileName[MAX_PATH] = "Dump";	
    OPENFILENAME    ofn={0};
    char            CodeBuf[24] = {0};
    ofn.lStructSize  = sizeof(OPENFILENAME);
    ofn.lpstrFile	 = szFileName;
    
    ofn.lpstrDefExt="exe";
    ofn.nMaxFile     = MAX_PATH;
    ofn.lpstrFilter  = "填入要保存的转存文件名(*.exe)\0*.exe\0";
    ofn.nFilterIndex = 1;
    if( GetSaveFileName(&ofn) == FALSE)
    {
        return ;
    }

    //需要把所有的内存断点和INT3断点硬件断点全部清除
    DeleteAllBreakPoint();


    HANDLE hSnap=NULL;
    HANDLE hFile=NULL;
    //创建被调试进程的模块快照
    hSnap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,m_tpInfo.dwProcessId);
    if (hSnap==INVALID_HANDLE_VALUE)
    {
        OutputDebugString("EasyDbgDlg.cpp 4059行出错");
        DWORD dwErorCode=0;
        dwErorCode=GetLastError();
        GetErrorMessage(dwErorCode);

        return;
    }

    MODULEENTRY32 me32={0};
    //枚举到的第一个模块是应用程序本身的映象 保存着映象基址及其大小
    me32.dwSize=sizeof(MODULEENTRY32);
    if (!Module32First(hSnap,&me32))
    {
        OutputDebugString("EasyDbgDlg.cpp 4073行出错");
        DWORD dwErorCode=0;
        dwErorCode=GetLastError();
        GetErrorMessage(dwErorCode);
        
        CloseHandle(hSnap);
        return;
    }

    HGLOBAL hBase=NULL;
    //在本进程申请同样大小的空间 来容纳目标进程的数据
    hBase=GlobalAlloc(0,me32.modBaseSize);
    if (hBase==NULL)
    {
        OutputDebugString("EasyDbgDlg.cpp 4086行出错");
        DWORD dwErorCode=0;
        dwErorCode=GetLastError();
        GetErrorMessage(dwErorCode);
        CloseHandle(hSnap);
        return;
    }
  
    //读取目标进程
    if ( !ReadProcessMemory(m_tpInfo.hProcess,me32.modBaseAddr,hBase,me32.modBaseSize,NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 4098行出错");
        DWORD dwErorCode=0;
        dwErorCode=GetLastError();
        GetErrorMessage(dwErorCode);

        CloseHandle(hSnap);
        return;
    }
    //创建文件
    hFile=CreateFile(szFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if (hFile==INVALID_HANDLE_VALUE)
    {
        OutputDebugString("EasyDbgDlg.cpp 4108行出错");
        DWORD dwErorCode=0;
        dwErorCode=GetLastError();
        GetErrorMessage(dwErorCode);
        
        CloseHandle(hSnap);
        return;

    }
    DWORD dwRet=0;
    //写文件
    if (!WriteFile(hFile,hBase,me32.modBaseSize,&dwRet,NULL))
    {
        OutputDebugString("EasyDbgDlg.cpp 4123行出错");
        DWORD dwErorCode=0;
        dwErorCode=GetLastError();
        GetErrorMessage(dwErorCode);
        
        CloseHandle(hSnap);
        return;

    }
    CloseHandle(hFile);
    //释放堆空间
    GlobalFree(hBase);
   
    hFile=NULL;
    hFile=CreateFile(szFileName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if (hFile==INVALID_HANDLE_VALUE)
    {
        OutputDebugString("EasyDbgDlg.cpp 4139行出错");
        DWORD dwErorCode=0;
        dwErorCode=GetLastError();
        GetErrorMessage(dwErorCode);
        CloseHandle(hSnap);
       
        return;
        
    }

    //修复区块表的文件偏移让RVA的值赋值给文件偏移 
     //并根据用户选择修改OEP使OEP等于当前EIP

    PIMAGE_DOS_HEADER pDosHeader=new IMAGE_DOS_HEADER;
    PIMAGE_FILE_HEADER pFileHeader=new IMAGE_FILE_HEADER;

    if (!ReadFile(hFile,pDosHeader,sizeof(IMAGE_DOS_HEADER),&dwRet,NULL))
    {
        dwRet=GetLastError();
        AfxMessageBox("读取失败");
        CloseHandle(hSnap);
        CloseHandle(hFile);
        return;
    }
    //移动文件指针到IMAGE_FILE_HEADER处
    SetFilePointer(hFile,pDosHeader->e_lfanew+4,NULL,FILE_BEGIN);
    //读取IMAGE_FILE_HEADER
    if (!ReadFile(hFile,pFileHeader,sizeof(IMAGE_FILE_HEADER),&dwRet,NULL))
    {
        AfxMessageBox("读取失败");
        CloseHandle(hSnap);
     
        CloseHandle(hFile);
        return;
    }
    if (::MessageBox(NULL,"是否将当前EIP作为OEP","提示",MB_OKCANCEL)==IDOK)
    {
        SetFilePointer(hFile,pDosHeader->e_lfanew+4+sizeof(IMAGE_FILE_HEADER)+16,NULL,FILE_BEGIN);
 
        DWORD dwNewOEP=0;
        ReadFile(hFile,&dwNewOEP,4,&dwRet,NULL);
        CONTEXT ct={0};
        ct.ContextFlags=CONTEXT_FULL;
        GetThreadContext(m_tpInfo.hThread,&ct);
        //新的OEP的RVA
        dwNewOEP=ct.Eip-(DWORD)me32.modBaseAddr;
       
        SetFilePointer(hFile,pDosHeader->e_lfanew+4+sizeof(IMAGE_FILE_HEADER)+16,NULL,FILE_BEGIN);
        
        WriteFile(hFile,&dwNewOEP,sizeof(DWORD),&dwRet,NULL);
   
    }

    DWORD dwRva=0;
    DWORD dwVirtualOffset=0;
    //循环该变节区表的 PointerToRawData,使其与RVA相等
    for (int i=0;i<pFileHeader->NumberOfSections;i++)
    {
        dwRva=0;
        dwVirtualOffset=0;
        //移动指针到节区表中的VirtualAddress 
        dwVirtualOffset=pDosHeader->e_lfanew+4+sizeof(IMAGE_FILE_HEADER)+pFileHeader->SizeOfOptionalHeader+12+i*sizeof(IMAGE_SECTION_HEADER);
        SetFilePointer(hFile,dwVirtualOffset,NULL,FILE_BEGIN);
        //读取VirtualAddress
        if (!ReadFile(hFile,&dwRva,sizeof(DWORD),&dwRet,NULL))
        {
            AfxMessageBox("读取失败");
            CloseHandle(hSnap);
         
            CloseHandle(hFile);
            return;
            
        }
        //移动指针到PointerToRawData
        dwVirtualOffset+=8;
        SetFilePointer(hFile,dwVirtualOffset,NULL,FILE_BEGIN);

        //改变PointerToRawData
        if (!WriteFile(hFile,&dwRva,sizeof(DWORD),&dwRet,NULL))
        {
            AfxMessageBox("写入失败");
            CloseHandle(hSnap);
           
            CloseHandle(hFile);
            return;
       }
 
        dwVirtualOffset-=12;
        SetFilePointer(hFile,dwVirtualOffset,NULL,FILE_BEGIN);
        //移动指针并读取VirtualSize
        if (!ReadFile(hFile,&dwRva,sizeof(DWORD),&dwRet,NULL))
        {
            AfxMessageBox("读取失败");
            CloseHandle(hSnap);
            
            CloseHandle(hFile);
            return;
            
        }

        dwVirtualOffset+=8;
        SetFilePointer(hFile,dwVirtualOffset,NULL,FILE_BEGIN);
        
        //改变SizeOfRawData 使其等于VirtualSize
        if (!WriteFile(hFile,&dwRva,sizeof(DWORD),&dwRet,NULL))
        {
            AfxMessageBox("写入失败");
            CloseHandle(hSnap);
            
            CloseHandle(hFile);
            return;
       }





    }
    delete pFileHeader;
    delete pDosHeader;

    CloseHandle(hSnap);
    CloseHandle(hFile);


}

//在反汇编窗口显示汇编代码  参数 要高亮的指令地址
void CEasyDbgDlg::ShowAsmInWindow(DWORD dwStartAddress)
{
    //设置标题
    SetDebuggerTitle(dwStartAddress);

    //判断该地址是否在当前显示的指令地址数组中
    DWORD dwRet=0;
    dwRet=IsFindAsmAddress(dwStartAddress);
    if (dwRet!=-1)
    {
        
        m_AsmList.SetItemState(dwRet, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
        return;

    }
    //如果不在数组就重新读
    m_AsmList.DeleteAllItems();
    CString szText;
    for (int k=0;k<22;k++)
    {
        BYTE pCode[40]={0};
        
        DWORD dwOldProtect=0;
        DWORD dwRet=0;
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwStartAddress,4,PAGE_EXECUTE_READWRITE,&dwOldProtect);
        if (!ReadProcessMemory(m_tpInfo.hProcess,(LPCVOID)dwStartAddress,pCode,sizeof(pCode),NULL))
        {
            OutputDebugString("EasyDbgDlg.cpp 4296行出错");
            DWORD dwErrorCode=0;
            dwErrorCode=GetLastError();
            //向用户输出错误信息
            GetErrorMessage(dwErrorCode);
           VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwStartAddress,4,dwOldProtect,&dwRet);
            return;
        }
        VirtualProtectEx(m_tpInfo.hProcess,(LPVOID)dwStartAddress,4,dwOldProtect,&dwRet);
        
        for (int i=0;i<16;i++)
        {
            
            POSITION pos=NULL;
            pos = m_Int3BpList.GetHeadPosition();
            while(pos!=NULL)
            {
                INT3_BP bp=m_Int3BpList.GetNext(pos);
                //判断断点地址是否命名在这段缓冲区中
                if (bp.dwAddress==dwStartAddress+i)
                {
                    //如果命中 则说明此为用户断点则把原字节还原
                    pCode[i]=bp.bOriginalCode;
                }
            }
            
            
        }
        
        char szAsm[120]={0};
        char szOpCode[120]={0};
        UINT CodeSize=0;
        Decode2AsmOpcode(pCode,szAsm,szOpCode,&CodeSize,dwStartAddress);
        EXPORT_FUN_INFO expFun={0};
        //如果找到改变显示方式
        if (IsExportFun(szAsm,expFun))
        {
            //显示在列表框控件内
            szText.Format("%08X",dwStartAddress);
            m_AsmList.InsertItem(k,szText);
            m_AsmList.SetItemText(k,1,szOpCode);
            szText.Format("%s <%s.%s>",szAsm,expFun.szDLLName,expFun.szFunName); 
            m_AsmList.SetItemText(k,2,szText);
             
            m_AsmAddress[k]=dwStartAddress;
            dwStartAddress=CodeSize+dwStartAddress;
            continue;
        }
        //显示在列表框控件内
        szText.Format("%08X",dwStartAddress);
        m_AsmList.InsertItem(k,szText);
        m_AsmList.SetItemText(k,1,szOpCode);
        m_AsmList.SetItemText(k,2,szAsm);
        
        m_AsmAddress[k]=dwStartAddress;
        dwStartAddress=CodeSize+dwStartAddress;

    }

     
     m_AsmList.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);







}


//根据要显示的指令地址 判断当前指令地址数组中是否有 若有就返回其下标 否则返回-1
DWORD CEasyDbgDlg::IsFindAsmAddress(DWORD dwStartAddress)
{
    for (DWORD i=0;i<25;i++)
    {
        if (dwStartAddress==m_AsmAddress[i])
        {
            return i;
        }
    }
    return -1;
}


//鼠标双击处理 下断点
void CEasyDbgDlg::OnDblclkList3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

    POSITION pos=NULL;
    pos=m_AsmList.GetFirstSelectedItemPosition();
   
    if (pos==NULL)
    {
        return;
    }
  
    int nIndex=m_AsmList.GetNextSelectedItem(pos);
 
    UserSetBP(m_tpInfo.hProcess,m_AsmAddress[nIndex],m_tpInfo.bCC);



	
	*pResult = 0;
}





//右键弹出菜单
void CEasyDbgDlg::OnRclickList3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

    POINT pt;
    
    GetCursorPos(&pt);
    
    CMenu menu;
    
    menu.LoadMenu(CG_IDR_POPUP_EASY_DBG_DLG);
    
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	
	*pResult = 0;
}

//下INT3断点
void CEasyDbgDlg::OnInt3() 
{
	// TODO: Add your command handler code here

    POSITION pos=NULL;
    pos=m_AsmList.GetFirstSelectedItemPosition();
   
    if (pos==NULL)
    {
        return;
    }
    
    int nIndex=m_AsmList.GetNextSelectedItem(pos);
    
    UserSetBP(m_tpInfo.hProcess,m_AsmAddress[nIndex],m_tpInfo.bCC);
	
}



//执行到当前地址
void CEasyDbgDlg::OnExecute() 
{
	// TODO: Add your command handler code here
    POSITION pos=NULL;
    pos=m_AsmList.GetFirstSelectedItemPosition();
    
    if (pos==NULL)
    {
        return;
    }
    
    int nIndex=m_AsmList.GetNextSelectedItem(pos);
    
    ON_G_COMMAND(m_AsmAddress[nIndex]);

	
}

//下硬件执行断点
void CEasyDbgDlg::OnHard() 
{
	// TODO: Add your command handler code here

    POSITION pos=NULL;
    pos=m_AsmList.GetFirstSelectedItemPosition();
    
    if (pos==NULL)
    {
        return;
    }
    
    int nIndex=m_AsmList.GetNextSelectedItem(pos);
    
    SetHardBP(m_AsmAddress[nIndex],0,1);


	
}



//在函数入口处下API断点
void CEasyDbgDlg::OnApi() 
{
	// TODO: Add your control notification handler code here
    char szBuffer[40];
    GetDlgItemText(IDC_API,szBuffer,sizeof(szBuffer));
    DWORD dwAddress=0;
    if (!m_Fun_Address.Lookup(szBuffer,dwAddress))
    {
        AfxMessageBox("无此函数");
        return ;
    }
    //在API入口地址下断点  而如果在CALL上下断点 需要首先遍历全部的指令,这里就不做了
    UserSetBP(m_tpInfo.hProcess,dwAddress,m_tpInfo.bCC);
   

	
}






void CEasyDbgDlg::OnHelp() 
{
	// TODO: Add your command handler code here
    CHelp dlg;
    dlg.DoModal();
	
}

//获得当前加载模块信息
BOOL CEasyDbgDlg::GetCurrentModuleList(HANDLE hProcess)
{
    if (hProcess==NULL)
    {
        return FALSE;
    }
    //删除所有元素
    m_Module.RemoveAll();

    
    HMODULE  hModule[500];
    //接收返回的字节数
    DWORD nRetrun=0;
    //枚举
    BOOL isSuccess=EnumProcessModules(hProcess,hModule,sizeof(hModule),&nRetrun);
    if (isSuccess==0)
    {
        OutputDebugString(TEXT("EasyDbgDlg.cpp 4538行出错"));
        DWORD dwErrorCode=0;
        dwErrorCode=GetLastError();
        //输出错误信息
        GetErrorMessage(dwErrorCode);
        
        return FALSE;
        
    }
    
    TCHAR ModuleName[500];
    //模块信息结构体
    MODULEINFO minfo;
    //开始添加
    for (DWORD i=0;i<(nRetrun/sizeof(HMODULE));i++)
    {
        MODULE_INFO mem={0};
        //获取模块名
        DWORD nLength=GetModuleBaseName(hProcess,hModule[i],ModuleName,sizeof(ModuleName));
        if (nLength==0)
        {
            OutputDebugString(TEXT("EasyDbgDlg.cpp 4559行出错"));
            DWORD dwErrorCode=0;
            dwErrorCode=GetLastError();
            //输出错误信息
            GetErrorMessage(dwErrorCode);
            
            return FALSE;
        }
        
        strncpy(mem.szModuleName,ModuleName,strlen(ModuleName)+1);
        //格式化模块基址
        mem.dwBaseAddress=(DWORD)hModule[i];
        //获取模块信息
        nLength=GetModuleInformation(g_hProcess,hModule[i],&minfo,sizeof(minfo));
        if (nLength==0)
        {
            OutputDebugString(TEXT("EasyDbgDlg.cpp 4575行出错"));
            DWORD dwErrorCode=0;
            dwErrorCode=GetLastError();
            //输出错误信息
            GetErrorMessage(dwErrorCode);
            
            return FALSE;

        }
       
        mem.dwSize=minfo.SizeOfImage;
       //添加到链表
        m_Module.AddTail(mem);
        
        
    }
    //把标志设为FALSE
    m_GetModule=FALSE;
    return TRUE;


}


//设置调试器标题(在调试什么程序,以及当前指令在哪个模块)
//参数为当前指令地址
void CEasyDbgDlg::SetDebuggerTitle(DWORD dwAddress)
{

    //如果模块需要更新  只要有DLL加载就需要更新
    if (m_GetModule)
    {

        if (!GetCurrentModuleList(m_tpInfo.hProcess))
        {
            return;

        }

    }

    //判断当前地址在哪个模块

    POSITION pos=NULL;
    pos=m_Module.GetHeadPosition();
    CString szText;
    while (pos!=NULL)
    {
         MODULE_INFO mem={0};
         mem=m_Module.GetNext(pos);
         if (dwAddress>=mem.dwBaseAddress && dwAddress<=(mem.dwSize+mem.dwBaseAddress))
         {
             
             MODULE_INFO mFirst={0};
             mFirst=m_Module.GetHead();
             //设置标题
             szText.Format("EasyDbg -%s- [CPU - 主线程,模块 - %s]",mFirst.szModuleName,mem.szModuleName);
             SetWindowText(szText);
             break;

         }


    }



}


//对DLL导出函数进行反汇编
void CEasyDbgDlg::DisassemblerExcFun(char* szFunName)
{

    DWORD dwAddress=0;
    if (!m_Fun_Address.Lookup(szFunName,dwAddress))
    {
        AfxMessageBox("无此函数");
        return ;
    }
    
    m_Result.ResetContent();

    //如果模块需要更新  只要有DLL加载就需要更新
    if (m_GetModule)
    {
        
        if (!GetCurrentModuleList(m_tpInfo.hProcess))
        {
            return;
            
        }
        
    }
    
    //获得当前反汇编函数在哪个模块
    
    POSITION pos=NULL;
    pos=m_Module.GetHeadPosition();
    CString szText;
    while (pos!=NULL)
    {
        MODULE_INFO mem={0};
        mem=m_Module.GetNext(pos);
        if (dwAddress>=mem.dwBaseAddress && dwAddress<=(mem.dwSize+mem.dwBaseAddress))
        {
            
            //显示要反汇编的函数名以及其所在模块
            szText.Format("%s!%s:",mem.szModuleName,szFunName);
            m_Result.AddString(szText);
            m_Result.SetTopIndex(m_Result.GetCount()-1);
            break;
            
        }
        
        
    }

    //反汇编

    ON_U_COMMAND(dwAddress);

}




