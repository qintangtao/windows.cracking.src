// EasyDbgDlg.h : header file
//

#if !defined(AFX_EASYDBGDLG_H__23A333A4_86FB_42E9_9EA7_CE145304AC6E__INCLUDED_)
#define AFX_EASYDBGDLG_H__23A333A4_86FB_42E9_9EA7_CE145304AC6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DebugData.h"
#include <afxtempl.h>
#include "VirtualMemory.h"
#include "ProcessModule.h"
#include "SetBP.h"
#include "ViewBP.h"
#include "PeScan.h"
#include "Help.h"



#pragma   warning(disable:4800)

/////////////////////////////////////////////////////////////////////////////
// CEasyDbgDlg dialog

class CEasyDbgDlg : public CDialog
{
// Construction
public:
	CEasyDbgDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEasyDbgDlg)
	enum { IDD = IDD_EASYDBG_DIALOG };
	CListCtrl	m_Stack;
	CListCtrl	m_AsmList;  
	CListCtrl	m_DataList;
	CListBox	m_Result;
	CEdit	m_command;
	CListCtrl	m_reglist;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasyDbgDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
    public:
        //得到可执行文件的路径
        void GetExeFilePath(char* szFilePath);
        void ShowReg(HANDLE hThread);
        //反汇编
        void ShowAsm(DWORD dwStartAddress);
        char m_SzFilePath[200];
    public:
        //被调试进程信息结构体
        TARGET_PROCESS_INFO m_tpInfo;
        //调试器是否正在调试
        BOOL m_isDebuging;
        //INT3断点链表
        CList<INT3_BP,INT3_BP&> m_Int3BpList;
        //需要被重新恢复为INT3断点的结构体
        RECOVER_BP m_Recover_BP;
        //是否是OEP处断点,如果是 暂停下来,如果不是则执行过去,像OD,WINDBG一样 初值设为TRUE
        BOOL m_IsOepBP;
        //判断是否是单步步过模式 如果是则要在断点异常中输出寄存器和指令
        BOOL m_IsF8;
        //判断是否为f9模式
        BOOL m_IsGo;
        //记录U命令的当前地址
        DWORD m_Uaddress;
        //删除永久断点
        BOOL m_isDelete;
        //调试寄存器Dr0-Dr3的使用情况
        DR_USE m_Dr_Use;
        //要恢复的硬件断点
        RECOVER_HARDBP m_Recover_HBP;
        //内存断点链表
        CList<MEM_BP,MEM_BP&> m_MemBpList;
        //内存页链表
        CList<MEM_BP_PAGE,MEM_BP_PAGE&> m_MemPageList;
        //保存要改变属性的数组 取 1 3个元素值,其他的任意
        DWORD m_Attribute[4];
        //要恢复的内存页
        RECOVER_MEMPAGE m_Recover_Mpage;
        BOOL m_isMoreMem;
        //导出函数映射表  用函数地址做索引 这样查询快
        CMap<DWORD,DWORD&,EXPORT_FUN_INFO,EXPORT_FUN_INFO&> m_ExFunList;
        //用映射表记录已经运行了的指令地址 这样可以过滤重复指令(如循环等)
        CMap<DWORD,DWORD&,OPCODE_RECORD,OPCODE_RECORD&> m_Opcode;
         //函数名与地址对应 用于在函数入口下API断点
        CMap<CString,LPCTSTR,DWORD,DWORD&> m_Fun_Address;
        //是否是自动F8模式 自动单步步过
        BOOL m_IsAutoF8;
        //是否是自动F7模式 自动步入
        BOOL m_IsAutoF7;
        //创建的用于指令记录的文件句柄
        HANDLE m_hFile;
        //反汇编窗口显示的指令地址
        DWORD m_AsmAddress[22];
        //模块信息表 用于显示标题用 即当前指令在哪个模块
        CList<MODULE_INFO,MODULE_INFO&> m_Module;
        //标志 判断是否要重新枚举模块 当有DLL加载时,置为TRUE 此时需要更新模块信息表
        BOOL m_GetModule;


        
        
        

    public:
        //初始化界面
        void UIinit();
        //获取错误信息,并显示给用户
        void GetErrorMessage(DWORD dwErrorCode);
        //得到加载DLL时的路径
        void GetFileNameFromHandle(HANDLE hFile,LPVOID pBase);
        //处理 CREATE_PROCESS_DEBUG_INFO  返回值决定 ContinueDebugEvent的第三个参数
        DWORD ON_CREATE_PROCESS_DEBUG_EVENT(DWORD dwProcessId,DWORD dwThreadId,LPTHREAD_START_ROUTINE lpOepAddress);
        //处理 EXCEPTION_BREAKPOINT 异常  参数为异常地址
        DWORD ON_EXCEPTION_BREAKPOINT(DWORD dwExpAddress);
        //处理EXCEPTION_SINGLE_STEP异常 参数异常地址
        DWORD ON_EXCEPTION_SINGLE_STEP(DWORD dwExpAddress);
        //处理EXCEPTION_ACCESS_VIOLATION异常 参数 异常地址和异常访问地址
        DWORD ON_EXCEPTION_ACCESS_VIOLATION(DWORD dwExpAddress,DWORD dwAccessAddress);
        //处理加载DLL事件
        void ON_LOAD_DLL_DEBUG_EVENT(HANDLE hFile,LPVOID pBase);

        //显示被调试进程内存数据,默认从OEP开始,显示800字节,每行8字节
        void ShowProcessMemory(DWORD dwStartAddress);
        //F7键的处理函数 单步步入
        void ON_VK_F7();
        //F8键的处理函数 单步步过
        void ON_VK_F8();
        //F9键的处理函数 运行
        void ON_VK_F9();
        //去掉命令的左边和右边的空格字符
        BOOL Kill_SPACE(char* szCommand);
        //用户命令的处理函数
        void Handle_User_Command(char* szCommand);
       //是被调试进程的EIP减1,用于断点异常处理
        void ReduceEIP();
        //恢复断点为原数据 参数被调试进程句柄 断点地址 原来的字节
        void RecoverBP(HANDLE hProcess,DWORD dwBpAddress,BYTE bOrignalCode);
        //设置断点 参数 被调试进程句柄  断点地址 0xCC 用于永久断点重新恢复为断点
        void DebugSetBp(HANDLE hProcess,DWORD dwBpAddress,BYTE bCCode);
        //判断是否是用户设置的INT3断点 通过查询INT3链表 参数为 断点地址
        BOOL isUserBP(DWORD dwBpAddress);
        //处理U命令
        void ON_U_COMMAND(DWORD dwAddress=0);
        //用户设置断点
        void UserSetBP(HANDLE hProcess,DWORD dwBpAddress,BYTE bCCode);
        //删除用户断点
        void DeleteUserBP(HANDLE hProcess,DWORD dwBpAddress);
        //处理B命令
        void Handle_B_Command(char* szCommand);
        //枚举断点
        void ListBP();
        //G命令处理
        void ON_G_COMMAND(DWORD dwAddress=0);
        //设置硬件断点
        void SetHardBP(DWORD dwBpAddress,DWORD dwAttribute,DWORD dwLength);
        //返回当前可用的调试寄存器
        int FindFreeDebugRegister();
        //删除硬件断点
        void DeleteHardBP(DWORD dwAddress);
        //获得要被删除的硬件断点的调试寄存器编号 参数断点地址和 CONTEXT
        int GetDeletedDrIndex(DWORD dwAddress,CONTEXT ct);
        //判断单步异常是否是硬件断点引起的  参数引用否则传进去的只是改变的参数的副本
        BOOL IfStepHard(DWORD& dwBPAddress);
        //使硬件断点暂时无效
        void InvalidHardBP(DWORD dwBpAddress);
        //恢复硬件断点 参数为 调试寄存器的编号
        void RecoverHBP(DWORD dwIndex);
        //设置内存断点  dwAttribute 1表示写入断点 3表示访问断点
        void SetMemBP(DWORD dwBpAddress,DWORD dwAttribute,DWORD dwLength);
        //判断地址是否有效 注意最后一个参数是引用
        BOOL IsAddressValid(DWORD dwAddress,MEMORY_BASIC_INFORMATION& mbi);
        //判断地址和长度占了几个分页并加入内存分页表 其中mbi是已经获得好的 也把断点加入断点表
        BOOL AddMemBpPage(DWORD dwBpAddress,DWORD dwLength,MEMORY_BASIC_INFORMATION mbi,DWORD dwAttribute,MEM_BP& mbp);
        //判断某一页首地址是否存在于页链表中
        BOOL FindMemPage(DWORD dwBaseAddress);
        //判断地址是否重复下内存断点
        BOOL FindMemBP(DWORD dwBpAddress);
        //暂时恢复内存断点 参数 内存页地址 原保护属性
        void RecoverMemBP(DWORD dwBaseAddress,DWORD dwProtect);
        //找到原始内存页链表的对应属性并传出 引用类型
        BOOL FindMemOriginalProtect(MEMORY_BASIC_INFORMATION& mbi);
        //删除内存断点
        void DeleteMemBP(DWORD dwBpAddress);
        //找到符合的内存断点信息并返回 参数类型为引用
        BOOL FindMemBPInformation(MEM_BP& mBP,DWORD dwBpAddress);
        //先判断有没有另一个内存断点在这个分页上,如果存在就修改为另一个断点所要求的属性
        //参数 内存页的首地址
        BOOL ModifyPageProtect(DWORD dwBaseAddress);
         //映射文件
        BOOL MapPEFile();

        // 获得导出表函数地址 导入表不全面 因为涉及到动态加载的DLL
        BOOL GetExportFunAddress(HANDLE hFile,char* pDll,LPVOID pBase);
        //RVA转文件偏移
        DWORD RvaToFileOffset(DWORD dwRva,DWORD dwSecNum,PIMAGE_SECTION_HEADER pSec);
        //判断解析指令中的函数调用 参数指令缓冲 导出函数信息结构用于传出
        BOOL IsExportFun(char* szBuffer, EXPORT_FUN_INFO& expFun);

        //删除所有断点 用于记录指令

        void DeleteAllBreakPoint();

        //把指令写入文件  参数指令地址 指令缓冲 
        void WriteOpcodeToFile(DWORD dwAddress , char* szAsm);
        //跳出函数
        void StepOutFromFun();
        //显示堆栈
        void ShowStack();
        //DUMP被调试进程
        void DumpTargetProcess();
        //在反汇编窗口显示汇编代码
        void ShowAsmInWindow(DWORD dwStartAddress);

        //根据要显示的指令地址 判断当前指令地址数组中是否有 若有就返回其下标
        DWORD IsFindAsmAddress(DWORD dwStartAddress);
        //获得当前加载模块信息
        BOOL GetCurrentModuleList(HANDLE hProcess);
        //设置调试器标题(在调试什么程序,以及当前指令在哪个模块)
        //参数为当前指令地址
        void SetDebuggerTitle(DWORD dwAddress);
        //对DLL导出函数进行反汇编
        void DisassemblerExcFun(char* szFunName);


        



// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEasyDbgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    virtual void OnOK();
	afx_msg void OnOpen();
    afx_msg void OnDropFiles( HDROP hDropInfo );
	afx_msg void OnStepinto();   
	afx_msg void OnStepover();  
	afx_msg void OnShowdata();
	afx_msg void OnMemory();
	afx_msg void OnModule();
	afx_msg void OnRun();
	afx_msg void OnQuit();
	afx_msg void OnSetbp();
	afx_msg void OnBreakpoint();
	afx_msg void OnViewpe();
	afx_msg void OnAutostepout();
	afx_msg void OnAutostepinto();
	afx_msg void OnOutfun();
	afx_msg void OnDump();
	afx_msg void OnDblclkList3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickList3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnInt3();
	afx_msg void OnExecute();
	afx_msg void OnHard();
	afx_msg void OnApi();
	afx_msg void OnHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYDBGDLG_H__23A333A4_86FB_42E9_9EA7_CE145304AC6E__INCLUDED_)
