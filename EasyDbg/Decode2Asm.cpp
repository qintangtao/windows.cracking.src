// Decode2Asm.cpp: implementation of the CDecode2Asm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Disasm.h"
#include "Decode2Asm.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void
__stdcall
Decode2Asm(IN PBYTE pCodeEntry,
           OUT char* strAsmCode,
           OUT UINT* pnCodeSize,
           UINT nAddress)
{
  DISASSEMBLY Disasm; // Creates an Disasm Struct
  // Pointer to linear address
  char *Linear = (char *)pCodeEntry;// Points to the address of array to decode.
  // Index of opcoded to decode
  DWORD       Index = 0; 
  Disasm.Address = nAddress; // Common Entry Point (usually default..)
  FlushDecoded(&Disasm);     // 清空反汇编结构体
  
  // Decode instruction
  Decode(&Disasm,
    Linear,
    &Index);
  
  strcpy(strAsmCode, Disasm.Assembly);

  if(strstr((char *)Disasm.Opcode, ":"))
  {
	  Disasm.OpcodeSize++;
 	  char ch =' ';
 	  strncat(strAsmCode,&ch,sizeof(char));
  }

  strcat(strAsmCode,Disasm.Remarks);
  *pnCodeSize = Disasm.OpcodeSize;
  
  // Clear all information
  FlushDecoded(&Disasm);
  
  return;
}

void
__stdcall
Decode2AsmOpcode(IN PBYTE pCodeEntry,   // 需要解析指令地址
           OUT char* strAsmCode,        // 得到反汇编指令信息
           OUT char* strOpcode,         // 解析机器码信息
           OUT UINT* pnCodeSize,        // 解析指令长度
           UINT nAddress)
{
   DISASSEMBLY Disasm;                  // 定义反汇编信息结构体
   char *Linear = (char *)pCodeEntry;   // 保存解析地址
   DWORD       Index = 0;               // 初始化机器码长度
   Disasm.Address = nAddress;           // 设置解析指令地址
   FlushDecoded(&Disasm);               // 清空反汇编结构体

   // 解析指令
   Decode(&Disasm,Linear,&Index);

   // 取出解析后的反汇编信息
   strcpy(strAsmCode, Disasm.Assembly);

   if(strstr((char *)Disasm.Opcode, ":"))
   {
     Disasm.OpcodeSize++;
     char ch =' ';
     strncat(strAsmCode,&ch,sizeof(char));
  }
   // 取出解析后的指令信息
   strcpy(strOpcode, Disasm.Opcode);

   // 取出指令说明信息
   strcat(strAsmCode,Disasm.Remarks);

   // 设置指令长度
   *pnCodeSize = Disasm.OpcodeSize;
   FlushDecoded(&Disasm);               // 清空反汇编结构体
}