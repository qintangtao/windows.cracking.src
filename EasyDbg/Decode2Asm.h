// Decode2Asm.h: interface for the CDecode2Asm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECODE2ASM_H__7AE15245_B351_41F6_B8B2_09D157829988__INCLUDED_)
#define AFX_DECODE2ASM_H__7AE15245_B351_41F6_B8B2_09D157829988__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 无机器码解析
extern "C"
void
__stdcall
Decode2Asm(IN PBYTE pCodeEntry,   // 需要解析指令地址
           OUT char* strAsmCode,  // 得到反汇编指令信息
           OUT UINT* pnCodeSize,  // 解析指令长度
           UINT nAddress);  

// 带机器码解析 
extern "C"
void
__stdcall
Decode2AsmOpcode(IN PBYTE pCodeEntry,   // 需要解析指令地址
           OUT char* strAsmCode,  // 得到反汇编指令信息
           OUT char* strOpcode,   // 解析机器码信息
           OUT UINT* pnCodeSize,  // 解析指令长度
           UINT nAddress);        
#endif // !defined(AFX_DECODE2ASM_H__7AE15245_B351_41F6_B8B2_09D157829988__INCLUDED_)
