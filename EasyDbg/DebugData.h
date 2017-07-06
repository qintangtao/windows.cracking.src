// DebugData.h: interface for the CDebugData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUGDATA_H__80BE350B_6F26_41DA_B22D_CA024071BE5A__INCLUDED_)
#define AFX_DEBUGDATA_H__80BE350B_6F26_41DA_B22D_CA024071BE5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//用与存储与被调试进程相关的信息
typedef struct  TargetProcess_info
{
    //保存被调试进程的句柄
    HANDLE hProcess;
    //保存被调试线程的句柄
    HANDLE hThread;
    //保存被调试进程的ID
    DWORD dwProcessId;
    //保存被调试线程的ID
    DWORD dwThreadId;
    //入口点地址
    LPTHREAD_START_ROUTINE OepAddress;
    //保存入口点首地址数据
    BYTE OriginalCode;
    //用于设置INT3断点的CC
    BYTE bCC;

}TARGET_PROCESS_INFO;

//INT3断点结构体
typedef struct INT3BREAKPOINT
{
   //断点地址
   DWORD dwAddress;
   //断点首字节数据
   BYTE  bOriginalCode;
   //是否是永久断点 永久断点需要恢复 一次性断点如：go address 此时为一次性断点 
   //OEP处的断点也是一次性断点,不需要在恢复为断点
   BOOL  isForever;

}INT3_BP;

//保存需要被恢复为INT3断点的地址
typedef struct RECOVER_BREAKPOINT
{
    //需要重新恢复为断点的地址(永久断点)
    DWORD dwAddress;
    // 是否需要被恢复为断点
    BOOL  isNeedRecover;
    //原字节 //用于恢复断点
    BYTE  bOrginalCode;
}RECOVER_BP;

//dr7调试控制寄存器
typedef union _Tag_DR7
{
    struct __DRFlag
    {
        unsigned int L0:  1 ;
        unsigned int G0:  1 ;
        unsigned int L1:  1 ;
        unsigned int G1:  1 ;
        unsigned int L2:  1 ;
        unsigned int G2:  1 ;
        unsigned int L3:  1 ;
        unsigned int G3:  1 ;
        unsigned int Le:  1 ;
        unsigned int Ge:  1 ;
        unsigned int b:   3 ;
        unsigned int gd:  1 ;
        unsigned int a:   2 ;
        unsigned int rw0: 2 ;
        unsigned int len0:2 ;
        unsigned int rw1: 2 ;
        unsigned int len1:2 ;
        unsigned int rw2: 2 ;
        unsigned int len2:2 ;
        unsigned int rw3: 2 ;
        unsigned int len3:2 ;
    } DRFlag;
    DWORD dwDr7 ;
}DR7 ;

//DR0-DR3的使用情况
typedef struct _DR_USE
{
    BOOL Dr0;
    BOOL Dr1;
    BOOL Dr2;
    BOOL Dr3;
 
} DR_USE;

//要恢复的硬件断点结构体
typedef struct RECOVER_HARD_BREAKPOINT
{
    //要恢复的调试寄存器编号 0-3 //如为-1表示没有要恢复的 
    //想来想去就一个成员,晕
    DWORD dwIndex;

}RECOVER_HARDBP;

//内存断点结构体

typedef struct MEMORYBREAKPOINT
{
    //地址
    DWORD dwBpAddress;
    //长度
    DWORD dwLength;
    //类型 是访问断点还是写入断点 
    DWORD dwAttribute;
    //内存页保存页的首地址数组 一个断点跨几个内存页,最大5个分页!在多就脑残了
    DWORD dwMemPage[5];
    //记录占的分页数
    DWORD dwNumPage;
   
    

}MEM_BP;

//内存分页结构体(仅限有断点的分页)

typedef struct MEMORYPAGE
{
    //内存页的首地址
    DWORD dwBaseAddress;
    //原访问属性
    DWORD dwProtect;


}MEM_BP_PAGE;

//要恢复的内存页属性

typedef struct _RECOVER_MEMPAGE
{
    //内存首地址
    DWORD dwBaseAddress;
    //内存页断点的保护属性(不是原保护属性)
    DWORD dwProtect;
    //是否需要恢复
    BOOL  isNeedRecover;
}RECOVER_MEMPAGE;


//导出函数地址表
typedef struct _EXPORT_FUN_INFO
{
    //函数地址
    DWORD dwAddress;
    //DLL名称
    char  szDLLName[40];
    
    char  szFunName[280];


}EXPORT_FUN_INFO;


//指令记录结构体
typedef struct _OPCODE_RECORD
{
    //指令地址
    DWORD dwAddress;
    
}OPCODE_RECORD;

//模块信息 用于显示标题即当前位于那个模块
typedef struct _MODULE_INFO
{
    //模块名
    char szModuleName[200];
    //模块基址
    DWORD dwBaseAddress;
    //模块大小
    DWORD dwSize;

}MODULE_INFO;













#endif // !defined(AFX_DEBUGDATA_H__80BE350B_6F26_41DA_B22D_CA024071BE5A__INCLUDED_)
