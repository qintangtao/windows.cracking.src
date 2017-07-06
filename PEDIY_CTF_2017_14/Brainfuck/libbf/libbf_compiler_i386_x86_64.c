/***************************************************************************
        libbf - Free BrainFuck interpreter, optimizer, compiler library
                            -------------------
    copyright            : (C) 2005-2006 by Even Rouault
    email                : even 'dot' rouault 'at' mines-paris 'dot' org
***************************************************************************/

/*
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */

/* For i386, %ebx, %esi, %edi and %ebp are callee save,
 * while %eax, %ecx and %edx are caller save 
 */

/* Standard amd64 calling conventions specify the following registers
 * to be used for passing the first 6 arguments:
 *
 *   %rdi, %rsi, %rdx, %rcx, %r8, %r9
 */
 
typedef enum
{
  EAX,
  ECX,
  EDX,
  EBX,
  ESP,
  EBP,
  ESI,
  EDI,
  
  R8,
  R9,
  R10,
  R11,
  R12,
  R13,
  R14,
  R15
} RegI386_t;

#define WORK_REG  EAX
#define WORK_REG2  EDX

static void libbf_gen_asm_i386_x86_64_offset_raw(AsmGeneration_t* asmGeneration, int offset, RegI386_t regPtr, RegI386_t reg, char addr_init)
{
  addr_init += regPtr + 8 * reg;
  if (offset == 0 && regPtr != EBP)
  {
    libbf_write_asm_byte(asmGeneration, addr_init + 0x00);
    if (regPtr == ESP)
      libbf_write_asm_byte(asmGeneration, 0x24);
  }
  else if (offset >= -128 && offset < 128)
  {
    libbf_write_asm_byte(asmGeneration, addr_init + 0x40);
    if (regPtr == ESP)
      libbf_write_asm_byte(asmGeneration, 0x24);
    libbf_write_asm_byte(asmGeneration, offset);
  }
  else
  {
    libbf_write_asm_byte(asmGeneration, addr_init + 0x80);
    if (regPtr == ESP)
      libbf_write_asm_byte(asmGeneration, 0x24);
    libbf_write_asm_int(asmGeneration, int_to_le(offset));
  }
}

static void libbf_gen_asm_i386_x86_64_offset(AsmGeneration_t* asmGeneration, int offset, RegI386_t regPtr, RegI386_t reg, char addr_init)
{
  offset *= asmGeneration->sizeofData;
  libbf_gen_asm_i386_x86_64_offset_raw(asmGeneration, offset, regPtr, reg, addr_init);
}

static void libbf_gen_asm_i386_x86_64_offset_longlong(AsmGeneration_t* asmGeneration, int offset, int suboffset, RegI386_t regPtr, RegI386_t reg, char addr_init)
{
  offset = libbf_compiler_compute_longlong_offset(asmGeneration, offset, suboffset);
  libbf_gen_asm_i386_x86_64_offset_raw(asmGeneration, offset, regPtr, reg, addr_init);
}

static void libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(AsmGeneration_t* asmGeneration)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  int sizeofData = asmGeneration->sizeofData;
  if (sizeofData >= 8 && cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  else if (sizeofData == 2)
  {
    libbf_write_asm_byte(asmGeneration, 0x66);
  }
}

static void libbf_gen_asm_i386_x86_64_operand_prefix_for_reg(AsmGeneration_t* asmGeneration, RegI386_t reg)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48 + (reg >= R8));
  }
}

static void libbf_gen_asm_i386_x86_64_operand_prefix_for_reg_src_and_dest(AsmGeneration_t* asmGeneration,
                                                                          RegI386_t regSrc, RegI386_t regDst)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48 + 4 * (regSrc >= R8) + (regDst >= R8));
  }
}


#if defined(__x86_64__)
static void libbf_gen_asm_i386_x86_64_mov_64cst_to_reg(AsmGeneration_t* asmGeneration, long constant, RegI386_t reg)
{
  libbf_gen_asm_i386_x86_64_operand_prefix_for_reg(asmGeneration, reg);
  libbf_write_asm_byte(asmGeneration, 0xb8 + (reg % 8));
  libbf_write_asm_long(asmGeneration, constant);
}
#endif

/* lea constant(regSrc1, regSrc2, mulCst), regDst */
/* if mulCst == 0, regSrc2 is ignored */
/* if regSrc1 == -1, regSrc1 is ignored */
static void libbf_gen_asm_i386_x86_64_lea(AsmGeneration_t* asmGeneration,
                                          int constant,
                                          RegI386_t regSrc1,
                                          RegI386_t regSrc2, 
                                          int mulCst,
                                          RegI386_t regDst)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  int addSeed;
  int shiftMulCst;
  
  ASSERT(regSrc1 < R8 && regSrc2 < R8 && regDst < R8);
  
  switch (mulCst)
  {
    case 0: shiftMulCst = 0; break;
    case 1: shiftMulCst = 0; break;
    case 2: shiftMulCst = 1; break;
    case 4: shiftMulCst = 2; break;
    case 8: shiftMulCst = 3; break;
    default: SHOULDNT_HAPPEN();
  }
  
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0x8d);
  if (regSrc1 == -1)
  {
    libbf_write_asm_byte(asmGeneration, 0x04 + regDst * 8);
    libbf_write_asm_byte(asmGeneration, 0x05 + regSrc2 * 8 + 0x40 * shiftMulCst);
    libbf_write_asm_int(asmGeneration, int_to_le(constant));
  }
  else
  {
    if (constant == 0) addSeed = 0;
    else if (constant >= -128 && constant < 128) addSeed = 0x40;
    else addSeed = 0x80;
    if (mulCst)
    {
      libbf_write_asm_byte(asmGeneration, 0x04 + addSeed + regDst * 8);
      libbf_write_asm_byte(asmGeneration, regSrc2 * 8 + regSrc1 + 0x40 * shiftMulCst);
    }
    else
    {
      libbf_write_asm_byte(asmGeneration, addSeed + regDst * 8 + regSrc1 + 0x40 * shiftMulCst);
    }
    if (constant == 0) {}
    else if (constant >= -128 && constant < 128) libbf_write_asm_byte(asmGeneration, constant);
    else libbf_write_asm_int(asmGeneration, int_to_le(constant));
  }
}

static void libbf_gen_asm_i386_x86_64_mov_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegI386_t reg)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  
  ASSERT(reg < R8);
  
  if (constant == 0)
  {
    /* xor %eax, %eax */
    if (cpu == TARGET_X86_64)
    {
      libbf_write_asm_byte(asmGeneration, 0x48);
    }
    libbf_write_asm_byte(asmGeneration, 0x31);
    libbf_write_asm_byte(asmGeneration, 0xc0 + reg + 8 * reg);
  }
  else
  {
    if (cpu == TARGET_X86_64)
    {
      libbf_write_asm_byte(asmGeneration, 0x48);
      libbf_write_asm_byte(asmGeneration, 0xc7);
      libbf_write_asm_byte(asmGeneration, 0xc0 + reg);
    }
    else
    {
      libbf_write_asm_byte(asmGeneration, 0xb8 + reg);
    }
    libbf_write_asm_int(asmGeneration, int_to_le(constant));
  }
}

static void libbf_gen_asm_i386_x86_64_add_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegI386_t reg)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  
  ASSERT(reg < R8);
  
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  if (constant >= 0)
  {
    if (constant < 128)
    {
      libbf_write_asm_byte(asmGeneration, 0x83);
      libbf_write_asm_byte(asmGeneration, 0xc0 + reg);
      libbf_write_asm_byte(asmGeneration, constant);
    }
    else
    {
      if (reg == EAX)
        libbf_write_asm_byte(asmGeneration, 0x05);
      else
      {
        libbf_write_asm_byte(asmGeneration, 0x81);
        libbf_write_asm_byte(asmGeneration, 0xc0 + reg);
      }
      libbf_write_asm_int(asmGeneration, int_to_le(constant));
    }
  }
  else
  {
    constant = -constant;
    if (constant < 128)
    {
      libbf_write_asm_byte(asmGeneration, 0x83);
      libbf_write_asm_byte(asmGeneration, 0xe8 + reg);
      libbf_write_asm_byte(asmGeneration, constant);
    }
    else
    {
      if (reg == EAX)
        libbf_write_asm_byte(asmGeneration, 0x2d);
      else
      {
        libbf_write_asm_byte(asmGeneration, 0x81);
        libbf_write_asm_byte(asmGeneration, 0xe8 + reg);
      }
      libbf_write_asm_int(asmGeneration, int_to_le(constant));
    }
  }  
}

static void libbf_gen_asm_i386_x86_64_add_reg_to_reg(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regDst)
{
  libbf_gen_asm_i386_x86_64_operand_prefix_for_reg_src_and_dest(asmGeneration, regSrc, regDst);
  libbf_write_asm_byte(asmGeneration, 0x01);
  libbf_write_asm_byte(asmGeneration, 0xc0 + 8 * (regSrc % 8) + (regDst % 8));
}


static void libbf_gen_asm_i386_x86_64_adc_reg_to_reg(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regDst)
{
  libbf_gen_asm_i386_x86_64_operand_prefix_for_reg_src_and_dest(asmGeneration, regSrc, regDst);
  libbf_write_asm_byte(asmGeneration, 0x11);
  libbf_write_asm_byte(asmGeneration, 0xc0 + 8 * (regSrc % 8) + (regDst % 8));
}

static void libbf_gen_asm_i386_x86_64_mov_reg_to_reg(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regDst)
{
  if (regSrc == regDst) return;
  libbf_gen_asm_i386_x86_64_operand_prefix_for_reg_src_and_dest(asmGeneration, regSrc, regDst);
  libbf_write_asm_byte(asmGeneration, 0x89);
  libbf_write_asm_byte(asmGeneration, 0xc0 + 8 * (regSrc % 8) + (regDst % 8));
}

/*
static void libbf_gen_asm_i386_x86_64_or_reg_to_reg(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regDst)
{
  libbf_gen_asm_i386_x86_64_operand_prefix_for_reg_src_and_dest(asmGeneration, regSrc, regDst);
  libbf_write_asm_byte(asmGeneration, 0x09);
  libbf_write_asm_byte(asmGeneration, 0xc0 + 8 * (regSrc % 8) + (regDst % 8));
}
*/

static void libbf_gen_asm_i386_x86_64_longlong_mov_cst_to_mem(AsmGeneration_t* asmGeneration, int constant, RegI386_t regPtr, int offset)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  int i;
  ASSERT(regPtr < R8);
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0xc7);
  libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, 0, regPtr, (RegI386_t)0, 0x00);
  if ((int)constant != constant)
  {
    NOT_IMPLEMENTED();
  }
  libbf_write_asm_int(asmGeneration, int_to_le(constant));
  
  for(i=1;i<NWORDS(asmGeneration);i++)
  {
    if (cpu == TARGET_X86_64)
    {
      libbf_write_asm_byte(asmGeneration, 0x48);
    }
    libbf_write_asm_byte(asmGeneration, 0xc7);
    libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, i, regPtr, (RegI386_t)0, 0x00);
    libbf_write_asm_int(asmGeneration, (constant >= 0) ? 0 : -1);
  }
}

static void libbf_gen_asm_i386_x86_64_mov_cst_to_mem(AsmGeneration_t* asmGeneration, int constant, RegI386_t regPtr, int offset)
{
  int sizeofData = asmGeneration->sizeofData;
  ASSERT(regPtr < R8);
  
  libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
      
  if (sizeofData >= 2)
  {
    libbf_write_asm_byte(asmGeneration, 0xc7);
        
    libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, (RegI386_t)0, 0x00);
  
    if ((int)constant != constant)
    {
      NOT_IMPLEMENTED();
    }
    if (sizeofData >= 4)
    {
      libbf_write_asm_int(asmGeneration, int_to_le(constant));
    }
    else
    {
      libbf_write_asm_short(asmGeneration, short_to_le(constant));
    }
  }
  else if (sizeofData == 1)
  {
    libbf_write_asm_byte(asmGeneration, 0xc6);
        
    libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, (RegI386_t)0, 0x00);
        
    libbf_write_asm_byte(asmGeneration, constant);
  }  
}

static void libbf_gen_asm_i386_x86_64_longlong_add_cst_to_mem(AsmGeneration_t* asmGeneration, int constant, RegI386_t regPtr, int offset)
{
  /* addl */
  int i;
  TargetCpu_t cpu = asmGeneration->cpu;
  ASSERT(regPtr < R8);
  
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, (constant >= -128 && constant < 128) ? 0x83 : 0x81);
  libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, 0, regPtr, (RegI386_t)0, 0x00);
  if (constant >= -128 && constant < 128)
  {
    libbf_write_asm_byte(asmGeneration, constant);
  }
  else
  {
    libbf_write_asm_int(asmGeneration, int_to_le(constant));
  }
  
  for(i=1;i<NWORDS(asmGeneration);i++)
  {
    /* adcl */
    if (cpu == TARGET_X86_64)
    {
      libbf_write_asm_byte(asmGeneration, 0x48);
    }
    libbf_write_asm_byte(asmGeneration, 0x83);
    libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, i, regPtr, (RegI386_t)0, 0x10);
    libbf_write_asm_byte(asmGeneration, (constant >= 0) ? 0 : -1);
  }
}


static void libbf_gen_asm_i386_x86_64_add_cst_to_mem(AsmGeneration_t* asmGeneration, int constant, RegI386_t regPtr, int offset)
{
  int sizeofData = asmGeneration->sizeofData;
  ASSERT(regPtr < R8);
    
  if (constant == 1 || constant == -1)
  {
    /* incb, incl, incq */
    libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
      
    libbf_write_asm_byte(asmGeneration, (sizeofData >= 2) ? 0xff : 0xfe);
    libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, (RegI386_t)0, (constant == 1) ? 0x00 : 0x08);
  }
  else
  {
    char add_or_sub = ((constant >= 0) ? 0x00 : 0x28);
    
    libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
        
    constant = (constant >= 0) ? constant : -constant;
    if (sizeofData >= 2)
    {
      libbf_write_asm_byte(asmGeneration, (constant < 128) ? 0x83 : 0x81);
      libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, (RegI386_t)0, add_or_sub);
      if (constant < 128)
      {
        libbf_write_asm_byte(asmGeneration, constant);
      }
      else if (sizeofData >= 4)
      {
        libbf_write_asm_int(asmGeneration, int_to_le(constant));
      }
      else
      {
        libbf_write_asm_short(asmGeneration, short_to_le(constant));
      }
    }
    else if (sizeofData == 1)
    {
      libbf_write_asm_byte(asmGeneration, 0x80);
      libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, (RegI386_t)0, add_or_sub);
      libbf_write_asm_byte(asmGeneration, constant);
    }
  }

}

static void libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset, int sub_offset, RegI386_t regDst)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  ASSERT(regPtr < R8 && regDst < R8);
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0x8b);
  libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, sub_offset, regPtr, regDst, 0x00);
}

static void libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset, RegI386_t regDst)
{
  ASSERT(regPtr < R8 && regDst < R8);
  if (asmGeneration->cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0x8b);
  libbf_gen_asm_i386_x86_64_offset_raw(asmGeneration, offset, regPtr, regDst, 0x00);
}


static void libbf_gen_asm_i386_x86_64_mov_mem_to_reg(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset, RegI386_t regDst)
{
  int sizeofData = asmGeneration->sizeofData;
  ASSERT(regPtr < R8 && regDst < R8);
  libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
  libbf_write_asm_byte(asmGeneration, (sizeofData >= 2) ? 0x8b : 0x8a);
  libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, regDst, 0x00);
}

static void libbf_gen_asm_i386_x86_64_mov_mem_to_reg_with_sign_extension(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset, RegI386_t regDst)
{
  int sizeofData = asmGeneration->sizeofData;
  ASSERT(regPtr < R8 && regDst < R8);
  
  /* 0f be 18                movsbl (%eax),%ebx */
  if (sizeofData == 1)
  {
    libbf_write_asm_byte(asmGeneration, 0x0f);
    libbf_write_asm_byte(asmGeneration, 0xbe);
  }
  /* 0f bf 98 80 00 00 00    movswl 0x80(%eax),%ebx */
  else if (sizeofData == 2)
  {
    libbf_write_asm_byte(asmGeneration, 0x0f);
    libbf_write_asm_byte(asmGeneration, 0xbf);
  }
  else
  {
    /* mov data_ptr[offset2], %rbx */
    libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
    libbf_write_asm_byte(asmGeneration, 0x8b);
  }
  libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, regDst, 0x00);
}

static void libbf_gen_asm_i386_x86_64_mov_mem_to_reg_with_zero_extension(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset, RegI386_t regDst)
{
  int sizeofData = asmGeneration->sizeofData;
  ASSERT(regPtr < R8 && regDst < R8);
  
  /* 0f be 18                movzbl (%eax),%ebx */
  if (sizeofData == 1)
  {
    libbf_write_asm_byte(asmGeneration, 0x0f);
    libbf_write_asm_byte(asmGeneration, 0xb6);
  }
  /* 0f bf 98 80 00 00 00    movzwl 0x80(%eax),%ebx */
  else if (sizeofData == 2)
  {
    libbf_write_asm_byte(asmGeneration, 0x0f);
    libbf_write_asm_byte(asmGeneration, 0xb7);
  }
  else
  {
    /* mov data_ptr[offset2], %rbx */
    libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
    libbf_write_asm_byte(asmGeneration, 0x8b);
  }
  libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, regDst, 0x00);
}

static void libbf_gen_asm_i386_x86_64_longlong_mov_reg_to_mem(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regPtr, int offset, int sub_offset)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  ASSERT(regPtr < R8);
  
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0x89);
  libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, sub_offset, regPtr, regSrc, 0x00);
}

static void libbf_gen_asm_i386_x86_64_longlong_mov_zero_reg_to_mem(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regPtr, int offset)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  int i;
  ASSERT(regSrc < R8 && regPtr < R8);
  
  libbf_gen_asm_i386_x86_64_longlong_mov_reg_to_mem(asmGeneration, regSrc, regPtr, offset, 0);
  
  for(i=1;i<NWORDS(asmGeneration);i++)
  {
    if (cpu == TARGET_X86_64)
    {
      libbf_write_asm_byte(asmGeneration, 0x48);
    }
    libbf_write_asm_byte(asmGeneration, 0xc7);
    libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, i, regPtr, (RegI386_t)0, 0x00);
    libbf_write_asm_int(asmGeneration, 0);
  }
}

static void libbf_gen_asm_i386_x86_64_mov_reg_to_mem(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regPtr, int offset)
{
  int sizeofData = asmGeneration->sizeofData;
  ASSERT(regSrc < R8 && regPtr < R8);
  libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
  libbf_write_asm_byte(asmGeneration, (sizeofData >= 2) ? 0x89 : 0x88);
  libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, regSrc, 0x00);
}

static void libbf_gen_asm_i386_x86_64_longlong_add_reg_to_mem(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regPtr, int offset, int sub_offset)
{
  /* add */
  TargetCpu_t cpu = asmGeneration->cpu;
  ASSERT(regSrc < R8 && regPtr < R8);
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0x01);
  libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, sub_offset, regPtr, regSrc, 0x00);
}

static void libbf_gen_asm_i386_x86_64_adc_0_to_reg(AsmGeneration_t* asmGeneration, RegI386_t reg)
{
  /* adc */
  TargetCpu_t cpu = asmGeneration->cpu;
  ASSERT(reg < R8);
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0x83);
  libbf_write_asm_byte(asmGeneration, 0xd0 + reg);
  libbf_write_asm_byte(asmGeneration, 0x00);
}

static void libbf_gen_asm_i386_x86_64_longlong_adc_reg_to_mem(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regPtr, int offset, int sub_offset)
{
  /* adc */
  TargetCpu_t cpu = asmGeneration->cpu;
  ASSERT(regSrc < R8 && regPtr < R8);
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0x11);
  libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, sub_offset, regPtr, regSrc, 0x00);
}


static void libbf_gen_asm_i386_x86_64_add_reg_to_mem(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regPtr, int offset)
{
  int sizeofData = asmGeneration->sizeofData;
  ASSERT(regSrc < R8 && regPtr < R8);
  libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
  libbf_write_asm_byte(asmGeneration, (sizeofData >= 2) ? 0x01 : 0x00);
  libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, regSrc, 0x00);
}


static void libbf_gen_asm_i386_x86_64_longlong_sub_reg_to_mem(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regPtr, int offset, int sub_offset)
{
  /* sub */
  TargetCpu_t cpu = asmGeneration->cpu;
  ASSERT(regSrc < R8 && regPtr < R8);
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0x29);
  libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, sub_offset, regPtr, regSrc, 0x00);
}

static void libbf_gen_asm_i386_x86_64_longlong_sbb_reg_to_mem(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regPtr, int offset, int sub_offset)
{
  /* sbb */
  TargetCpu_t cpu = asmGeneration->cpu;
  ASSERT(regSrc < R8 && regPtr < R8);
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0x19);
  libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, sub_offset, regPtr, regSrc, 0x00);
}

static void libbf_gen_asm_i386_x86_64_sub_reg_to_mem(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regPtr, int offset)
{
  int sizeofData = asmGeneration->sizeofData;
  libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
  libbf_write_asm_byte(asmGeneration, (sizeofData >= 2) ? 0x29 : 0x28);
  libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, regSrc, 0x00);
}

static void libbf_gen_asm_i386_x86_64_shl_cst_reg(AsmGeneration_t* asmGeneration, int shift, RegI386_t reg)
{
  if (shift == 0) return;
  
  libbf_gen_asm_i386_x86_64_operand_prefix_for_reg(asmGeneration, reg);
  if (shift == 1)
  {
    libbf_write_asm_byte(asmGeneration, 0xd1);
    libbf_write_asm_byte(asmGeneration, 0xe0 + (reg % 8));
  }
  else
  {
    libbf_write_asm_byte(asmGeneration, 0xc1);
    libbf_write_asm_byte(asmGeneration, 0xe0 + (reg % 8));
    libbf_write_asm_byte(asmGeneration, shift);
  }
}


static void libbf_gen_asm_i386_x86_64_shld_cst_reg_reg(AsmGeneration_t* asmGeneration, int shift, RegI386_t reg1, RegI386_t reg2)
{
  if (shift == 0) return;
  
  libbf_gen_asm_i386_x86_64_operand_prefix_for_reg_src_and_dest(asmGeneration, reg1, reg2);
  libbf_write_asm_byte(asmGeneration, 0x0f);
  libbf_write_asm_byte(asmGeneration, 0xa4);
  libbf_write_asm_byte(asmGeneration, 0xc0 + (reg1 % 8) * 8 + (reg2 % 8));
  libbf_write_asm_byte(asmGeneration, shift);
}


static void libbf_gen_asm_i386_x86_64_mul_reg(AsmGeneration_t* asmGeneration, RegI386_t reg)
{
  libbf_gen_asm_i386_x86_64_operand_prefix_for_reg(asmGeneration, reg); 
  libbf_write_asm_byte(asmGeneration, 0xf7);
  libbf_write_asm_byte(asmGeneration, 0xe0 + (reg % 8));
}

/* peut utiliser EDX de facon temporaire */
static void libbf_gen_asm_i386_x86_64_mul_cst_by_reg_to_reg(AsmGeneration_t* asmGeneration, int constant, RegI386_t regSrc, RegI386_t regDst)
{
  int shift1, shift2;
  if (libbf_is_composed_poweroftwo(constant, &shift1, &shift2))
  {
    if (shift1 == 0 && shift2 <= 3)
    {
      libbf_gen_asm_i386_x86_64_lea(asmGeneration, 0, regSrc, regSrc, 1 << shift2, regDst);
    }
    else if (shift2)
    {
      libbf_gen_asm_i386_x86_64_mov_reg_to_reg(asmGeneration, regSrc, EDX);
      libbf_gen_asm_i386_x86_64_shl_cst_reg(asmGeneration, shift1, regSrc);
      libbf_gen_asm_i386_x86_64_shl_cst_reg(asmGeneration, shift2, EDX);
      /*
      libbf_gen_asm_i386_x86_64_add_reg_to_reg(asmGeneration, regSrc, EDX);
      libbf_gen_asm_i386_x86_64_mov_reg_to_reg(asmGeneration, EDX, regDst);*/
      libbf_gen_asm_i386_x86_64_lea(asmGeneration, 0, regSrc, EDX, 1, regDst);
    }
    else
    {
      libbf_gen_asm_i386_x86_64_shl_cst_reg(asmGeneration, shift1, regSrc);
      libbf_gen_asm_i386_x86_64_mov_reg_to_reg(asmGeneration, regSrc, regDst);
    }
  }
  else
  {
    libbf_gen_asm_i386_x86_64_operand_prefix_for_reg_src_and_dest(asmGeneration, regDst, regSrc); /* exception */
    if (constant >= -128 && constant < 128)
      libbf_write_asm_byte(asmGeneration, 0x6b);
    else
      libbf_write_asm_byte(asmGeneration, 0x69);
    libbf_write_asm_byte(asmGeneration, 0xc0 + (regSrc % 8) + (regDst % 8) * 8);  /* exception */
    if (constant >= -128 && constant < 128)
      libbf_write_asm_byte(asmGeneration, constant);
    else
      libbf_write_asm_int(asmGeneration, int_to_le(constant));
  }
}

static void libbf_gen_asm_i386_x86_64_mul_reg_to_reg(AsmGeneration_t* asmGeneration, RegI386_t regSrc, RegI386_t regDst)
{
  libbf_gen_asm_i386_x86_64_operand_prefix_for_reg_src_and_dest(asmGeneration, regDst, regSrc);  /* exception */
  libbf_write_asm_byte(asmGeneration, 0x0f);
  libbf_write_asm_byte(asmGeneration, 0xaf);
  libbf_write_asm_byte(asmGeneration, 0xc0 + (regSrc % 8) + (regDst % 8) * 8);  /* exception */
}

static void libbf_gen_asm_i386_x86_64_mul_mem_to_reg(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset, RegI386_t regDst)
{
  ASSERT(regPtr < R8 && regDst < R8);
  libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
  libbf_write_asm_byte(asmGeneration, 0x0f);
  libbf_write_asm_byte(asmGeneration, 0xaf);
  libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, regDst, 0x00);
}

static void libbf_gen_asm_i386_x86_64_test_reg(AsmGeneration_t* asmGeneration, RegI386_t reg)
{
  libbf_gen_asm_i386_x86_64_operand_prefix_for_reg_src_and_dest(asmGeneration, reg, reg);
  libbf_write_asm_byte(asmGeneration, 0x85);
  libbf_write_asm_byte(asmGeneration, 0xc0 + 9 * (reg % 8));
}

static void libbf_gen_asm_i386_x86_64_longlong_cmp_mem_to_0(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset, RegI386_t regTmp)
{
  /* mov regPtr[offset], regTmp */
  TargetCpu_t cpu = asmGeneration->cpu;
  int i;
  
  ASSERT(regPtr < R8 && regTmp < R8);
  
  if (cpu == TARGET_X86_64)
  {
    libbf_write_asm_byte(asmGeneration, 0x48);
  }
  libbf_write_asm_byte(asmGeneration, 0x8b);
  libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, 0, regPtr, regTmp, 0x00);
  
  for(i=1;i<NWORDS(asmGeneration);i++)
  {
    /* or regPtr[offset+1], regTmp */
    if (cpu == TARGET_X86_64)
    {
      libbf_write_asm_byte(asmGeneration, 0x48);
    }
    libbf_write_asm_byte(asmGeneration, 0x0b);
    libbf_gen_asm_i386_x86_64_offset_longlong(asmGeneration, offset, i, regPtr, regTmp, 0x00);
  }
}

static void libbf_gen_asm_i386_x86_64_cmp_mem_to_0(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  int sizeofData = asmGeneration->sizeofData;
  ASSERT(regPtr < R8);
  
  if (cpu == TARGET_I386 && sizeofData == 4)
  {
    libbf_gen_asm_i386_x86_64_mov_mem_to_reg(asmGeneration, regPtr, offset, EDX);
    libbf_gen_asm_i386_x86_64_test_reg(asmGeneration, EDX);
  }
  else
  {
    libbf_gen_asm_i386_x86_64_operand_prefix_for_mem_access(asmGeneration);
    libbf_write_asm_byte(asmGeneration, (sizeofData >= 2) ? 0x83 : 0x80);
    libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, (RegI386_t)0, 0x38);
    libbf_write_asm_byte(asmGeneration, 0x00);
  }  
}

#if 0
static void libbf_gen_asm_i386_x86_64_push_mem(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset)
{
  ASSERT(regPtr < R8);
  libbf_write_asm_byte(asmGeneration, 0xff);
  libbf_gen_asm_i386_x86_64_offset(asmGeneration, offset, regPtr, 0, 0x30);
}
#endif

static void libbf_gen_asm_i386_x86_64_push_mem32(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset)
{
  ASSERT(regPtr < R8);
  libbf_write_asm_byte(asmGeneration, 0xff);
  libbf_gen_asm_i386_x86_64_offset_raw(asmGeneration, offset, regPtr, (RegI386_t)0, 0x30);
}

static void libbf_gen_asm_i386_x86_64_push_reg(AsmGeneration_t* asmGeneration, RegI386_t reg)
{
  if (reg >= R8) libbf_write_asm_byte(asmGeneration, 0x41);
  libbf_write_asm_byte(asmGeneration, 0x50 + (reg % 8));
}


static void libbf_gen_asm_i386_x86_64_pop_reg(AsmGeneration_t* asmGeneration, RegI386_t reg)
{
  if (reg >= R8) libbf_write_asm_byte(asmGeneration, 0x41);
  libbf_write_asm_byte(asmGeneration, 0x58 + (reg % 8));
}

#if (defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(WIN32)) && (defined(__i386__) || defined(__x86_64__))
static void libbf_gen_asm_i386_x86_64_popf(AsmGeneration_t* asmGeneration)
{
  libbf_write_asm_byte(asmGeneration, 0x9d);
}
#endif

static void libbf_gen_asm_i386_x86_64_push_cst(AsmGeneration_t* asmGeneration, int constant)
{
  libbf_write_asm_byte(asmGeneration, 0x68);
  libbf_write_asm_int(asmGeneration, int_to_le(constant));
}

/* call *offset(reg) */
static void libbf_gen_asm_i386_x86_64_call_at_mem(AsmGeneration_t* asmGeneration, RegI386_t regPtr, int offset)
{
  ASSERT(regPtr < R8);
  libbf_write_asm_byte(asmGeneration, 0xff);
  libbf_gen_asm_i386_x86_64_offset_raw(asmGeneration, offset, regPtr, (RegI386_t)0, 0x10);
}

static void libbf_gen_asm_i386_x86_64_retq(AsmGeneration_t* asmGeneration)
{
  libbf_write_asm_byte(asmGeneration, 0xc3);
}


static void libbf_gen_asm_i386_x86_64_debug_code(AsmGeneration_t* asmGeneration, OptimizationUnit* current_code_ptr)
{
#if defined(__i386__)
  if (asmGeneration->cpu == TARGET_I386)
  {
    libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
        
    libbf_gen_asm_i386_x86_64_push_reg(asmGeneration, (RegI386_t)asmGeneration->index_register);
      
    libbf_gen_asm_i386_x86_64_push_cst(asmGeneration, (int)(long)current_code_ptr);
      
    libbf_gen_asm_i386_x86_64_push_mem32(asmGeneration, EAX, OFFSET_DEBUG_DATA);
    
    libbf_gen_asm_i386_x86_64_call_at_mem(asmGeneration, EAX, OFFSET_DEBUG_FUNC);
    
    libbf_gen_asm_i386_x86_64_add_cst_to_reg(asmGeneration, asmGeneration->sizeofLong * 3, ESP);
  }
  else
#elif defined(__x86_64__)
  if (asmGeneration->cpu == TARGET_X86_64)
  {
    libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
    
    libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, EAX, OFFSET_DEBUG_DATA, EDI);
    
    libbf_gen_asm_i386_x86_64_mov_64cst_to_reg(asmGeneration, (long)current_code_ptr, ESI);
    
    libbf_gen_asm_i386_x86_64_mov_reg_to_reg(asmGeneration, (RegI386_t)asmGeneration->index_register, EDX);
    
    libbf_gen_asm_i386_x86_64_call_at_mem(asmGeneration, EAX, OFFSET_DEBUG_FUNC);
  }
  else
#endif
  {
    SHOULDNT_HAPPEN();
  }
}

static void libbf_gen_asm_i386_x86_64_while(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
/* je  = 74 "11" ou 0f 84 "44 33 22 11"
   jne = 75 "11" ou 0f 85 "44 33 22 11"
   jle = 7e "11" ou 0f 8e "44 33 22 11"
  jmp = eb "11" ou e9 "44 33 22 11" */
  int indexReg = asmGeneration->index_register;
  
  if (code_ptr->ou_loop_type != LIBBF_DO_WHILE)
  {
    if (asmGeneration->generate_debug)
      libbf_gen_asm_debug_code(asmGeneration, code_ptr);
        
    if (NWORDS(asmGeneration) <= 1)
      libbf_gen_asm_i386_x86_64_cmp_mem_to_0(asmGeneration, (RegI386_t)indexReg, code_ptr->ou_offset);
    else
      libbf_gen_asm_i386_x86_64_longlong_cmp_mem_to_0(asmGeneration, (RegI386_t)indexReg, code_ptr->ou_offset, WORK_REG);
        
    /* je */
    libbf_write_asm_byte(asmGeneration, 0x0f);
    libbf_write_asm_byte(asmGeneration, 0x84);
        
    code_ptr->ou_offset_to_patch = asmGeneration->filled_size;
    libbf_write_asm_int(asmGeneration, 0x00000000);
  }
  else
  {
    code_ptr->ou_offset_to_patch = asmGeneration->filled_size;
  }
      
  *(asmGeneration->p_stack_while) = code_ptr;
  asmGeneration->p_stack_while++;
}

static void libbf_gen_asm_i386_x86_64_end_while(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  OptimizationUnit* while_instr;
  int positive_jump_offset;
  int indexReg = asmGeneration->index_register;
  
  asmGeneration->p_stack_while--;
  while_instr = *(asmGeneration->p_stack_while);
  if (code_ptr->ou_loop_type != LIBBF_IF)
  {
    int negative_jump_offset;
    int offset_after_while;
          
    if (asmGeneration->generate_debug)
      libbf_gen_asm_debug_code(asmGeneration, code_ptr);
        
    if (NWORDS(asmGeneration) <= 1)
      libbf_gen_asm_i386_x86_64_cmp_mem_to_0(asmGeneration, (RegI386_t)indexReg, code_ptr->ou_offset);
    else
      libbf_gen_asm_i386_x86_64_longlong_cmp_mem_to_0(asmGeneration, (RegI386_t)indexReg, code_ptr->ou_offset, WORK_REG);
        
    /* jne */
    offset_after_while =
        (code_ptr->ou_loop_type != LIBBF_DO_WHILE) ? while_instr->ou_offset_to_patch + 4 : while_instr->ou_offset_to_patch;
    negative_jump_offset = offset_after_while - (asmGeneration->filled_size + 2 + 4);
    if (negative_jump_offset < -128)
    {
      libbf_write_asm_byte(asmGeneration, 0x0f);
      libbf_write_asm_byte(asmGeneration, 0x85);
      libbf_write_asm_int(asmGeneration, int_to_le(offset_after_while - (asmGeneration->filled_size + 4)));
    }
    else
    {
      libbf_write_asm_byte(asmGeneration, 0x75);
      libbf_write_asm_byte(asmGeneration, (char)(offset_after_while - (asmGeneration->filled_size + 1)));
    }
  }
  if (code_ptr->ou_loop_type != LIBBF_DO_WHILE)
  {
    positive_jump_offset = asmGeneration->filled_size - (while_instr->ou_offset_to_patch + 4);
    /* patch the je instr in the corresponding while */
    if (positive_jump_offset < 128 - 4)
    {
      asmGeneration->compiled_code[while_instr->ou_offset_to_patch - 2] = 0x74;
      asmGeneration->compiled_code[while_instr->ou_offset_to_patch - 1] = (char)(positive_jump_offset);
      memmove(asmGeneration->compiled_code + while_instr->ou_offset_to_patch,
              asmGeneration->compiled_code + while_instr->ou_offset_to_patch + 4,
              positive_jump_offset);
          
      asmGeneration->filled_size -= 4;
    }
    else
    {
      *(int*)(asmGeneration->compiled_code + while_instr->ou_offset_to_patch) = int_to_le(positive_jump_offset);
    }
  }
}
  
static void libbf_gen_asm_i386_x86_64(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  int sizeofData = asmGeneration->sizeofData;
  RegI386_t indexReg = (RegI386_t)asmGeneration->index_register;
  
  switch(code_ptr->ou_instr)
  {
    case BF_INC_DATA_PTR:
    {
      libbf_gen_asm_i386_x86_64_add_cst_to_reg(asmGeneration, code_ptr->ou_offset * sizeofData, indexReg);
      break;
    }
      
    case BF_SET_CST:
    {
      libbf_gen_asm_i386_x86_64_mov_cst_to_mem(asmGeneration, code_ptr->ou_constant, indexReg, code_ptr->ou_offset);
      break;
    }
      
    case BF_INC_CST:
    {
      libbf_gen_asm_i386_x86_64_add_cst_to_mem(asmGeneration, code_ptr->ou_constant, indexReg, code_ptr->ou_offset);
      break;
    }

    case BF_SET_DATA:
    {
      libbf_gen_asm_i386_x86_64_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, WORK_REG);
      libbf_gen_asm_i386_x86_64_mov_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset);
      break;
    }
      
    case BF_INC_DATA:
    {
      libbf_gen_asm_i386_x86_64_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, WORK_REG);
      libbf_gen_asm_i386_x86_64_add_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset);
      break;
    }
      
    case BF_DEC_DATA:
    {
      libbf_gen_asm_i386_x86_64_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, WORK_REG);
      libbf_gen_asm_i386_x86_64_sub_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset);
      break;
    }
    
    case BF_INC_CST_ADD_DATA :/* data_ptr[offset] += cst + data_ptr[offset2] */
    {
      libbf_gen_asm_i386_x86_64_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset, WORK_REG);
      libbf_gen_asm_i386_x86_64_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, WORK_REG2);
      libbf_gen_asm_i386_x86_64_lea(asmGeneration, code_ptr->ou_constant, WORK_REG, WORK_REG2, 1, WORK_REG);
      libbf_gen_asm_i386_x86_64_mov_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset);
      break;
    }

    case BF_INC_CST_MUL_DATA:/* data_ptr[offset] += cst * data_ptr[offset2] */
    {
      int constant_pos = (code_ptr->ou_constant > 0) ? code_ptr->ou_constant : -code_ptr->ou_constant;
      
      libbf_gen_asm_i386_x86_64_mov_mem_to_reg_with_sign_extension(asmGeneration, indexReg, code_ptr->ou_offset2, WORK_REG);
      libbf_gen_asm_i386_x86_64_mul_cst_by_reg_to_reg(asmGeneration, constant_pos, WORK_REG, WORK_REG);
      
      if (code_ptr->ou_constant >= 0)
      {
        libbf_gen_asm_i386_x86_64_add_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset);
      }
      else
      {
        libbf_gen_asm_i386_x86_64_sub_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset);
      }

      break;
    }
      
    case BF_INC_DATA_MUL_DATA:/*data_ptr[offset] += data_ptr[offset2] * data_ptr[offset3] */
    {
      if (sizeofData == 1 || sizeofData == 2)
      {
        libbf_gen_asm_i386_x86_64_mov_mem_to_reg_with_zero_extension(asmGeneration, indexReg, code_ptr->ou_offset2, WORK_REG);
        libbf_gen_asm_i386_x86_64_mov_mem_to_reg_with_zero_extension(asmGeneration, indexReg, code_ptr->ou_offset3, EDX);
        libbf_gen_asm_i386_x86_64_mul_reg_to_reg(asmGeneration, EDX, WORK_REG);
      }
      else
      {
        libbf_gen_asm_i386_x86_64_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, WORK_REG);
        libbf_gen_asm_i386_x86_64_mul_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset3, WORK_REG);
      }
      
      libbf_gen_asm_i386_x86_64_add_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset);

      break;
    }
      
    case BF_WHILE_DATA:  /* while (data_ptr[offset]) { */
    {
      libbf_gen_asm_i386_x86_64_while(asmGeneration, code_ptr);
      
      break;
    }
      
    case BF_END_WHILE:
    {
      libbf_gen_asm_i386_x86_64_end_while(asmGeneration, code_ptr);

      break;
    }
    
    case BF_READ_DATA:       /* data_ptr[offset] = getchar() */
    {
      if (cpu == TARGET_I386)
      {
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
        
        libbf_gen_asm_i386_x86_64_push_mem32(asmGeneration, EAX, OFFSET_IN_OUT_DATA);
        
        libbf_gen_asm_i386_x86_64_call_at_mem(asmGeneration, EAX, OFFSET_INPUT_FUNC);
        
        libbf_gen_asm_i386_x86_64_add_cst_to_reg(asmGeneration, asmGeneration->sizeofLong * 1, ESP);
      }
      else
      {
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
        
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, EAX, OFFSET_IN_OUT_DATA, EDI);
        
        libbf_gen_asm_i386_x86_64_call_at_mem(asmGeneration, EAX, OFFSET_INPUT_FUNC);
      }
      
      libbf_gen_asm_i386_x86_64_mov_reg_to_mem(asmGeneration, EAX, indexReg, code_ptr->ou_offset);
      
      break;
    }
 
    case BF_PRINT_DATA:
    {
      if (cpu == TARGET_I386)
      {
        libbf_gen_asm_i386_x86_64_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset, WORK_REG2);
		
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
        
        libbf_gen_asm_i386_x86_64_push_mem32(asmGeneration, EAX, OFFSET_IN_OUT_DATA);
        libbf_gen_asm_i386_x86_64_push_reg(asmGeneration, WORK_REG2);
        
        libbf_gen_asm_i386_x86_64_call_at_mem(asmGeneration, EAX, OFFSET_OUTPUT_FUNC);
        
        libbf_gen_asm_i386_x86_64_add_cst_to_reg(asmGeneration, asmGeneration->sizeofLong * 2, ESP);
      }
      else
      {
        libbf_gen_asm_i386_x86_64_mov_mem_to_reg_with_zero_extension(asmGeneration, indexReg, code_ptr->ou_offset, EDI);
        
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
        
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, EAX, OFFSET_IN_OUT_DATA, ESI);
        
        libbf_gen_asm_i386_x86_64_call_at_mem(asmGeneration, EAX, OFFSET_OUTPUT_FUNC);
      }

      break;
    }
    
    default:
    {
      libbf_gen_asm_default(asmGeneration, code_ptr);
      break;
    }

  }
}

static void libbf_gen_asm_i386_x86_64_longlong(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  int sizeofData = asmGeneration->sizeofData;
  int sizeofLong = asmGeneration->sizeofLong;
  int nWords = NWORDS(asmGeneration);
  RegI386_t indexReg = (RegI386_t)asmGeneration->index_register;
  RegI386_t ebx_or_esi = (asmGeneration->cpu == TARGET_I386) ? EBX : ESI;
  
  switch(code_ptr->ou_instr)
  {
    case BF_INC_DATA_PTR:
    {
      libbf_gen_asm_i386_x86_64_add_cst_to_reg(asmGeneration, code_ptr->ou_offset * sizeofData, indexReg);
      break;
    }
      
    case BF_SET_CST:
    {
      libbf_gen_asm_i386_x86_64_longlong_mov_cst_to_mem(asmGeneration, code_ptr->ou_constant, indexReg, code_ptr->ou_offset);
      break;
    }
      
    case BF_INC_CST:
    {
      libbf_gen_asm_i386_x86_64_longlong_add_cst_to_mem(asmGeneration, code_ptr->ou_constant, indexReg, code_ptr->ou_offset);
      break;
    }

    case BF_SET_DATA:
    {
      int i;
      for(i=0;i<sizeofData/(sizeofLong * 2);i++)
      {
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i * 2, WORK_REG);
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i * 2 + 1, WORK_REG2);
        libbf_gen_asm_i386_x86_64_longlong_mov_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset, i * 2);
        libbf_gen_asm_i386_x86_64_longlong_mov_reg_to_mem(asmGeneration, WORK_REG2, indexReg, code_ptr->ou_offset, i * 2 + 1);
      }
      if (sizeofData % (sizeofLong * 2))
      {
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, nWords - 1, WORK_REG);
        libbf_gen_asm_i386_x86_64_longlong_mov_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset, nWords - 1);
      }
      
      break;
    }
      
    case BF_INC_DATA:
    {
      int i;
      int first_time = 0;
      for(i=0;i<sizeofData/(sizeofLong * 2);i++)
      {
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i * 2, WORK_REG);
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i * 2 + 1, WORK_REG2);
        if (!first_time)
        {
          libbf_gen_asm_i386_x86_64_longlong_add_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset, i * 2);
          first_time = 1;
        }
        else
        {
          libbf_gen_asm_i386_x86_64_longlong_adc_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset, i * 2);
        }
        libbf_gen_asm_i386_x86_64_longlong_adc_reg_to_mem(asmGeneration, WORK_REG2, indexReg, code_ptr->ou_offset, i * 2 + 1);
      }
      if (sizeofData % (sizeofLong * 2))
      {
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, nWords - 1, WORK_REG);
        if (!first_time)
        {
          libbf_gen_asm_i386_x86_64_longlong_add_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset, nWords - 1);
        }
        else
        {
          libbf_gen_asm_i386_x86_64_longlong_adc_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset, nWords - 1);
        }
      }

      break;
    }
      
    case BF_DEC_DATA:
    {
      int i;
      int first_time = 0;
      for(i=0;i<sizeofData/(sizeofLong * 2);i++)
      {
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i * 2, WORK_REG);
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i * 2 + 1, WORK_REG2);
        if (!first_time)
        {
          libbf_gen_asm_i386_x86_64_longlong_sub_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset, i * 2);
          first_time = 1;
        }
        else
        {
          libbf_gen_asm_i386_x86_64_longlong_sbb_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset, i * 2);
        }
        libbf_gen_asm_i386_x86_64_longlong_sbb_reg_to_mem(asmGeneration, WORK_REG2, indexReg, code_ptr->ou_offset, i * 2 + 1);
      }
      if (sizeofData % (sizeofLong * 2))
      {
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, NWORDS(asmGeneration) - 1, WORK_REG);
        if (!first_time)
        {
          libbf_gen_asm_i386_x86_64_longlong_sub_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset, NWORDS(asmGeneration) - 1);
        }
        else
        {
          libbf_gen_asm_i386_x86_64_longlong_sbb_reg_to_mem(asmGeneration, WORK_REG, indexReg, code_ptr->ou_offset, NWORDS(asmGeneration) - 1);
        }
      }
      
      break;
    }
     
    case BF_INC_CST_MUL_DATA:/* data_ptr[offset] += cst * data_ptr[offset2] */
    {
      int constant_pos = (code_ptr->ou_constant >= 0) ? code_ptr->ou_constant : -code_ptr->ou_constant;
      
      if (nWords > 2)
      {
        int shift1, shift2;
        if (code_ptr->ou_constant >= 0 && libbf_is_composed_poweroftwo(constant_pos, &shift1, &shift2) && shift2 == 0)
        {
          int i;
          for(i=0;i<nWords;i++)
          {
            if (i == 0)
            {
              libbf_gen_asm_i386_x86_64_mov_cst_to_reg(asmGeneration, 0, EDX);
              libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i, EAX);
              libbf_gen_asm_i386_x86_64_shl_cst_reg(asmGeneration, shift1, EAX);
            }
            else
            {
              libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i-1, ECX);
              libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i, EAX);
              libbf_gen_asm_i386_x86_64_mov_reg_to_reg(asmGeneration, EDX, ebx_or_esi);
              libbf_gen_asm_i386_x86_64_shld_cst_reg_reg(asmGeneration, shift1, ECX, EAX);
              if (i<nWords-1)
              {
                libbf_gen_asm_i386_x86_64_mov_cst_to_reg(asmGeneration, 0, EDX);
              }
              libbf_gen_asm_i386_x86_64_add_reg_to_reg(asmGeneration, ebx_or_esi, EAX);
              if (i<nWords-1)
              {
                libbf_gen_asm_i386_x86_64_adc_0_to_reg(asmGeneration, EDX);
              }
            }
            libbf_gen_asm_i386_x86_64_longlong_add_reg_to_mem(asmGeneration, EAX, indexReg, code_ptr->ou_offset, i);
            if (i<nWords-1)
            {
              libbf_gen_asm_i386_x86_64_adc_0_to_reg(asmGeneration, EDX);
            }
          }
        }
        else
        { 
          
          int i, j;
          for(j=0;j<nWords;j++)
          {
            if (j == 0)
              libbf_gen_asm_i386_x86_64_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, ECX);
            else if (code_ptr->ou_constant < 0)
            {
              if (j == 1)
                libbf_gen_asm_i386_x86_64_mov_cst_to_reg(asmGeneration, -1, ECX);
            }
            else
            {
              break;
            }
            
            for(i=0;i<nWords-j;i++)
            {
              libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i, EAX);

              if (i != 0)
              {
                libbf_gen_asm_i386_x86_64_mov_reg_to_reg(asmGeneration, EDX, ebx_or_esi);
              }
                
              libbf_gen_asm_i386_x86_64_mul_reg(asmGeneration, ECX);
          
              if (i != 0)
              {
                libbf_gen_asm_i386_x86_64_add_reg_to_reg(asmGeneration, ebx_or_esi, EAX);
                if (i+j < nWords-1)
                {
                  libbf_gen_asm_i386_x86_64_adc_0_to_reg(asmGeneration, EDX);
                }
              }
              libbf_gen_asm_i386_x86_64_longlong_add_reg_to_mem(asmGeneration, EAX, indexReg, code_ptr->ou_offset, i+j);
              if (i+j < nWords-1)
              {
                libbf_gen_asm_i386_x86_64_adc_0_to_reg(asmGeneration, EDX);
              }
            }
          }
        }
      }
      else
      {
        int shift1, shift2;
        if (libbf_is_composed_poweroftwo(constant_pos, &shift1, &shift2))
        {
          libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, 0, EAX);
          libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, 1, EDX);
          
          if (shift2 == 0)
          {
            libbf_gen_asm_i386_x86_64_shld_cst_reg_reg(asmGeneration, shift1, EAX, EDX);
            libbf_gen_asm_i386_x86_64_shl_cst_reg(asmGeneration, shift1, EAX);
          }
          else
          {
            libbf_gen_asm_i386_x86_64_mov_reg_to_reg(asmGeneration, EAX, ebx_or_esi);
            libbf_gen_asm_i386_x86_64_mov_reg_to_reg(asmGeneration, EDX, ECX);
          
            libbf_gen_asm_i386_x86_64_shld_cst_reg_reg(asmGeneration, shift1, EAX, EDX);
            libbf_gen_asm_i386_x86_64_shld_cst_reg_reg(asmGeneration, shift2, ebx_or_esi, ECX);
            libbf_gen_asm_i386_x86_64_shl_cst_reg(asmGeneration, shift1, EAX);
            libbf_gen_asm_i386_x86_64_shl_cst_reg(asmGeneration, shift2, ebx_or_esi);
  
            libbf_gen_asm_i386_x86_64_add_reg_to_reg(asmGeneration, ebx_or_esi, EAX);
            libbf_gen_asm_i386_x86_64_adc_reg_to_reg(asmGeneration, ECX, EDX);          
          }
        }
        else
        {
          /*
          b8 4d 00 00 00          mov    $0x4d,%eax
          8b 8e f0 00 00 00       mov    0xf0(%esi),%ecx
          8b 9e f4 00 00 00       mov    0xf4(%esi),%ebx
          f7 e1                   mul    %ecx
          89 d9                   mov    %ebx,%ecx
          bb 4d 00 00 00          mov    $0x4d,%ebx
          0f af cb                imul   %ebx,%ecx
          8d 14 0a                lea    (%edx,%ecx,1),%edx
              */
          libbf_gen_asm_i386_x86_64_mov_cst_to_reg(asmGeneration, constant_pos, EAX);
          
          libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, 0, ECX);
          libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, 1, ebx_or_esi);
          
          libbf_gen_asm_i386_x86_64_mul_reg(asmGeneration, ECX);
          
          libbf_gen_asm_i386_x86_64_mov_reg_to_reg(asmGeneration, ebx_or_esi, ECX);
          
          libbf_gen_asm_i386_x86_64_mov_cst_to_reg(asmGeneration, constant_pos, ebx_or_esi);
          
          libbf_gen_asm_i386_x86_64_mul_reg_to_reg(asmGeneration, ebx_or_esi, ECX);
          
          /* lea    (%edx,%ecx,1),%edx */
          if (asmGeneration->cpu == TARGET_X86_64)
          {
            libbf_write_asm_byte(asmGeneration, 0x48);
          }
          libbf_write_asm_byte(asmGeneration, 0x8d);
          libbf_write_asm_byte(asmGeneration, 0x14);
          libbf_write_asm_byte(asmGeneration, 0x0a);
        }
        
        if (code_ptr->ou_constant >= 0)
        {
          libbf_gen_asm_i386_x86_64_longlong_add_reg_to_mem(asmGeneration, EAX, indexReg, code_ptr->ou_offset, 0);
          
          libbf_gen_asm_i386_x86_64_longlong_adc_reg_to_mem(asmGeneration, EDX, indexReg, code_ptr->ou_offset, 1);
        }
        else
        {
          libbf_gen_asm_i386_x86_64_longlong_sub_reg_to_mem(asmGeneration, EAX, indexReg, code_ptr->ou_offset, 0);
          
          libbf_gen_asm_i386_x86_64_longlong_sbb_reg_to_mem(asmGeneration, EDX, indexReg, code_ptr->ou_offset, 1);
        }
      }
      
      break;
    }
    
    case BF_INC_DATA_MUL_DATA:
    {
      if (nWords > 2)
      {
        int i, j;
        libbf_gen_asm_i386_x86_64_mov_cst_to_reg(asmGeneration, 0, ebx_or_esi);
        
        for(j=0;j<nWords;j++)
        {
          for(i=0;i<nWords - j;i++)
          {
            int k;
            libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, i, EAX);
            
            libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset3, j, EDX);
            
            libbf_gen_asm_i386_x86_64_mul_reg(asmGeneration, EDX);
      
            libbf_gen_asm_i386_x86_64_longlong_add_reg_to_mem(asmGeneration, EAX, indexReg, code_ptr->ou_offset, i+j);

            if (i+j != nWords - 1)
            {
              libbf_gen_asm_i386_x86_64_longlong_adc_reg_to_mem(asmGeneration, EDX, indexReg, code_ptr->ou_offset, i+j+1);
            }
            
            for(k=i+j+2;k<nWords;k++)
            {
              libbf_gen_asm_i386_x86_64_longlong_adc_reg_to_mem(asmGeneration, ebx_or_esi, indexReg, code_ptr->ou_offset, k);
            }
          }
        }
      }
      else
      {
      /*      
        8b 46 00                mov    0x0(%esi),%eax
        8b 4e 08                mov    0x8(%esi),%ecx
        89 c3                   mov    %eax,%ebx
        f7 e1                   mul    %ecx
        01 46 10                add    %eax,0x10(%esi)
        11 56 14                adc    %edx,0x14(%esi)
        8b 46 04                mov    0x4(%esi),%eax
        f7 e1                   mul    %ecx
        01 46 14                add    %eax,0x14(%esi)
        8b 46 0c                mov    0xc(%esi),%eax
        f7 e3                   mul    %ebx
        01 46 14                add    %eax,0x14(%esi)
      */
                
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, 0, EAX);
        
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset3, 0, ECX);
        
        libbf_gen_asm_i386_x86_64_mov_reg_to_reg(asmGeneration, EAX, ebx_or_esi);
        
        libbf_gen_asm_i386_x86_64_mul_reg(asmGeneration, ECX);
        
        libbf_gen_asm_i386_x86_64_longlong_add_reg_to_mem(asmGeneration, EAX, indexReg, code_ptr->ou_offset, 0);
        
        libbf_gen_asm_i386_x86_64_longlong_adc_reg_to_mem(asmGeneration, EDX, indexReg, code_ptr->ou_offset, 1);
        
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset2, 1, EAX);
        
        libbf_gen_asm_i386_x86_64_mul_reg(asmGeneration, ECX);
        
        libbf_gen_asm_i386_x86_64_longlong_add_reg_to_mem(asmGeneration, EAX, indexReg, code_ptr->ou_offset, 0);
        
        libbf_gen_asm_i386_x86_64_longlong_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset3, 1, EAX);
        
        libbf_gen_asm_i386_x86_64_mul_reg(asmGeneration, ebx_or_esi);
  
        libbf_gen_asm_i386_x86_64_longlong_add_reg_to_mem(asmGeneration, EAX, indexReg, code_ptr->ou_offset, 0);
      }
      break;
    }
    
      
    case BF_WHILE_DATA:  /* while (data_ptr[offset]) { */
    {
      libbf_gen_asm_i386_x86_64_while(asmGeneration, code_ptr);

      break;
    }
      
    case BF_END_WHILE:
    {
      libbf_gen_asm_i386_x86_64_end_while(asmGeneration, code_ptr);

      break;
    }
    
    case BF_READ_DATA:       /* data_ptr[offset] = getchar() */
    {
      if (asmGeneration->cpu == TARGET_I386)
      {
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
        
        libbf_gen_asm_i386_x86_64_push_mem32(asmGeneration, EAX, OFFSET_IN_OUT_DATA);
        
        libbf_gen_asm_i386_x86_64_call_at_mem(asmGeneration, EAX, OFFSET_INPUT_FUNC);
        
        libbf_gen_asm_i386_x86_64_add_cst_to_reg(asmGeneration, asmGeneration->sizeofLong * 1, ESP);
      }
      else
      {
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
        
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, EAX, OFFSET_IN_OUT_DATA, EDI);
        
        libbf_gen_asm_i386_x86_64_call_at_mem(asmGeneration, EAX, OFFSET_INPUT_FUNC);
      }
      
      libbf_gen_asm_i386_x86_64_longlong_mov_zero_reg_to_mem(asmGeneration, EAX, indexReg, code_ptr->ou_offset);
      
      break;
    }
 
    case BF_PRINT_DATA:
    {
      if (asmGeneration->cpu == TARGET_I386)
      {
        libbf_gen_asm_i386_x86_64_mov_mem_to_reg(asmGeneration, indexReg, code_ptr->ou_offset, WORK_REG2);
		
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
        
        libbf_gen_asm_i386_x86_64_push_mem32(asmGeneration, EAX, OFFSET_IN_OUT_DATA);
        libbf_gen_asm_i386_x86_64_push_reg(asmGeneration, WORK_REG2);
        
        libbf_gen_asm_i386_x86_64_call_at_mem(asmGeneration, EAX, OFFSET_OUTPUT_FUNC);
        
        libbf_gen_asm_i386_x86_64_add_cst_to_reg(asmGeneration, asmGeneration->sizeofLong * 2, ESP);
      }
      else
      {
        libbf_gen_asm_i386_x86_64_mov_mem_to_reg_with_zero_extension(asmGeneration, indexReg, code_ptr->ou_offset, EDI);
        
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
        
        libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, EAX, OFFSET_IN_OUT_DATA, ESI);

        libbf_gen_asm_i386_x86_64_call_at_mem(asmGeneration, EAX, OFFSET_OUTPUT_FUNC);
      }

      break;
    }
    
    default:
    {
      libbf_gen_asm_default(asmGeneration, code_ptr);
      break;
    }

  }
}

static void libbf_gen_asm_i386_x86_64_preambule(AsmGeneration_t* asmGeneration)
{
  TargetCpu_t cpu = asmGeneration->cpu;
  
  /* jmp */
  libbf_write_asm_byte(asmGeneration, 0xeb);
  libbf_write_asm_byte(asmGeneration, 4 * 5);
  
  libbf_write_asm_int(asmGeneration, int_to_le(0)); /* CODE SIZE : TO BE POST-FIXED */
  libbf_write_asm_int(asmGeneration, int_to_le(asmGeneration->cpu));
  libbf_write_asm_int(asmGeneration, int_to_le(asmGeneration->sizeofData));
  libbf_write_asm_int(asmGeneration, int_to_le(asmGeneration->min_delta_offset));
  libbf_write_asm_int(asmGeneration, int_to_le(asmGeneration->max_delta_offset));
      
  libbf_gen_asm_i386_x86_64_push_reg(asmGeneration, EBX);
  libbf_gen_asm_i386_x86_64_push_reg(asmGeneration, ESI);
  
  if (cpu == TARGET_X86_64)
  {
    asmGeneration->index_register = EBX;
        
    libbf_gen_asm_i386_x86_64_push_cst(asmGeneration, SIGNATURE_2);
    libbf_gen_asm_i386_x86_64_push_cst(asmGeneration, SIGNATURE_1);
    
    libbf_gen_asm_i386_x86_64_push_reg(asmGeneration, EDI);
    
    libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
    
    libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, EAX, OFFSET_BASE_DATA_PTR,
                                                 (RegI386_t)asmGeneration->index_register);
  }
  else
  {
    asmGeneration->index_register = ESI;
    
    libbf_gen_asm_i386_x86_64_push_cst(asmGeneration, SIGNATURE_2);
    libbf_gen_asm_i386_x86_64_push_cst(asmGeneration, SIGNATURE_1);
    
    libbf_gen_asm_i386_x86_64_push_mem32(asmGeneration, ESP, asmGeneration->sizeofLong * 5);
    
    libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 0, EAX);
    
    libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, EAX, OFFSET_BASE_DATA_PTR,
                                                 (RegI386_t)asmGeneration->index_register);
  }
}

static void libbf_gen_asm_i386_x86_64_epilogue(AsmGeneration_t* asmGeneration)
{
  libbf_gen_asm_i386_x86_64_add_cst_to_reg(asmGeneration, asmGeneration->sizeofLong * 3, ESP);

  libbf_gen_asm_i386_x86_64_pop_reg(asmGeneration, ESI);
  libbf_gen_asm_i386_x86_64_pop_reg(asmGeneration, EBX);
  
  libbf_gen_asm_i386_x86_64_retq(asmGeneration);
  
  *(int*)&asmGeneration->compiled_code[2] = int_to_le(asmGeneration->filled_size);
}

#if defined(WIN32)
static void libbf_compiler_i386_add_win32_glue(AsmGeneration_t* asmGeneration, void* executable_code)
{
  /*__try1(libbf_compiler_dynalloc_handler_win32) */
  libbf_gen_asm_i386_x86_64_push_cst(asmGeneration, (int)libbf_compiler_dynalloc_handler_win32);
  
  /* 64 ff 35 00 00 00 00    pushl  %fs:0x0 */
  libbf_write_asm_byte(asmGeneration, 0x64);
  libbf_write_asm_byte(asmGeneration, 0xff);
  libbf_write_asm_byte(asmGeneration, 0x35);
  libbf_write_asm_int(asmGeneration, 0);
  
  /* 64 89 25 00 00 00 00    mov    %esp,%fs:0x0 */
  libbf_write_asm_byte(asmGeneration, 0x64);
  libbf_write_asm_byte(asmGeneration, 0x89);
  libbf_write_asm_byte(asmGeneration, 0x25);
  libbf_write_asm_int(asmGeneration, 0);
  
  
  
  libbf_gen_asm_i386_x86_64_push_mem32(asmGeneration, ESP, asmGeneration->sizeofLong * 3);
  
  libbf_gen_asm_i386_x86_64_mov_cst_to_reg(asmGeneration, (int)executable_code, EAX);
  
  /* ff d0                   call   *%eax */
  libbf_write_asm_byte(asmGeneration, 0xff);
  libbf_write_asm_byte(asmGeneration, 0xd0);
  
  libbf_gen_asm_i386_x86_64_add_cst_to_reg(asmGeneration, asmGeneration->sizeofLong * 1, ESP);
  
    
  /* __except1 */
  
  libbf_gen_asm_i386_x86_64_mov_mem_reg_to_reg(asmGeneration, ESP, 4, EAX);
  
  /* 64 a3 00 00 00 00       mov    %eax,%fs:0x0 */
  libbf_write_asm_byte(asmGeneration, 0x64);
  libbf_write_asm_byte(asmGeneration, 0xa3);
  libbf_write_asm_int(asmGeneration, 0);
  
  libbf_gen_asm_i386_x86_64_add_cst_to_reg(asmGeneration, asmGeneration->sizeofLong * 2, ESP);
  
  libbf_gen_asm_i386_x86_64_retq(asmGeneration);
}

#endif


#if defined(__i386__) || defined(__x86_64__)
int libbf_is_compatible_compiled_code  (char* compiled_code,
                                        TargetCpu_t* cpu,
                                        int* sizeofData,
                                        int* min_delta_offset,
                                        int* max_delta_offset)
{
  if (!(compiled_code[0] == (char)0xeb && compiled_code[1] == 5 * 4))
  {
    warning("Non i386 or x86_64 libbf-compiled code\n");
    return FALSE;
  }
  
  *cpu = (*(TargetCpu_t*)(compiled_code + 2 + 4));
  *sizeofData = (*(int*)(compiled_code + 2 + 8));
  *min_delta_offset = (*(int*)(compiled_code + 2 + 12));
  *max_delta_offset = (*(int*)(compiled_code + 2 + 16));
  
  return TRUE;
}
#endif


static void libbf_gen_asm_i386_x86_64_init_funcs()
{
  asmGenerationFuncs[TARGET_I386].gen_preambule = libbf_gen_asm_i386_x86_64_preambule;
  asmGenerationFuncs[TARGET_I386].gen_epilogue = libbf_gen_asm_i386_x86_64_epilogue;
  asmGenerationFuncs[TARGET_I386].gen_code = libbf_gen_asm_i386_x86_64;
  asmGenerationFuncs[TARGET_I386].gen_code_longlong = libbf_gen_asm_i386_x86_64_longlong;
  asmGenerationFuncs[TARGET_I386].gen_debug_code = libbf_gen_asm_i386_x86_64_debug_code;
  memcpy(&asmGenerationFuncs[TARGET_X86_64], &asmGenerationFuncs[TARGET_I386], sizeof(asmGenerationFuncs[TARGET_I386]));
}
