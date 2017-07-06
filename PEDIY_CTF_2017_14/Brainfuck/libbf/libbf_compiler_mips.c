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

typedef enum
{
  ZERO_MIPS,
  
  AT_MIPS,   /* reserved for assembler */

  V0_MIPS,   /* expression evaluation and */
  V1_MIPS,   /* results of a function */

  A0_MIPS,   /* argument reg */
  A1_MIPS,
  A2_MIPS,
  A3_MIPS,
  
  T0_MIPS,   /* temporary reg */
  T1_MIPS,
  T2_MIPS,
  T3_MIPS,
  T4_MIPS,
  T5_MIPS,
  T6_MIPS,
  T7_MIPS,
  
  S0_MIPS,   /* saved reg */
  S1_MIPS,
  S2_MIPS,
  S3_MIPS,
  S4_MIPS,
  S5_MIPS,
  S6_MIPS,
  S7_MIPS,

  T8_MIPS,   /* temporary reg */
  T9_MIPS,

  K0_MIPS,   /* reserved for kernel */
  K1_MIPS,

  GP_MIPS,   /* general pointer to global area */
  SP_MIPS,
  FP_MIPS,
#define S8_MIPS  FP_MIPS
  RA_MIPS,   /* return address */

} RegMips_t;

/* S0...S7, GP, SP, FP, RA are callee-saved */

#define INDEX_REG  S1_MIPS

#define INT_TO_TGT(asmGeneration, x) ((asmGeneration->cpu == TARGET_MIPS) ? int_to_be(x) : int_to_le(x))

static void libbf_gen_asm_mips_ops(AsmGeneration_t* asmGeneration, int code32)
{
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration, code32));
}

static void libbf_gen_asm_mips_nop(AsmGeneration_t* asmGeneration)
{
  libbf_gen_asm_mips_ops(asmGeneration, 0x00000000);
}

static void libbf_gen_asm_mips_mov_mem32_to_reg(AsmGeneration_t* asmGeneration, RegMips_t regPtr, int offset, RegMips_t regDst)
{
  if (offset < -32768 || offset > 32767)
  {
    NOT_IMPLEMENTED();
  }
  libbf_gen_asm_mips_ops(asmGeneration, (0x8c << 24) + (regPtr << 21) + (regDst << 16) + (offset & 0xFFFF));
}

static void libbf_gen_asm_mips_mov_mem_to_reg(AsmGeneration_t* asmGeneration, RegMips_t regPtr, int offset, RegMips_t regDst)
{
  int seed;
  
  offset = offset * asmGeneration->sizeofData;
  if (offset < -32768 || offset > 32767)
  {
    NOT_IMPLEMENTED();
  }
  if (asmGeneration->sizeofData == 4)
    seed = 0x8c;
  else if (asmGeneration->sizeofData == 2)
    seed = 0x84;
  else
    seed = 0x90;
  libbf_gen_asm_mips_ops(asmGeneration, (seed << 24) + (regPtr << 21) + (regDst << 16) + (offset & 0xFFFF));
}


static void libbf_gen_asm_mips_mov_reg_to_mem32(AsmGeneration_t* asmGeneration, RegMips_t regSrc, RegMips_t regPtr, int offset)
{
  if (offset < -32768 || offset > 32767)
  {
    NOT_IMPLEMENTED();
  }
  libbf_gen_asm_mips_ops(asmGeneration, (0xac << 24) + (regPtr << 21) + (regSrc << 16) + (offset & 0xFFFF));
}

static void libbf_gen_asm_mips_mov_reg_to_mem(AsmGeneration_t* asmGeneration, RegMips_t regSrc, RegMips_t regPtr, int offset)
{
  int seed;
  
  offset = offset * asmGeneration->sizeofData;
  if (offset < -32768 || offset > 32767)
  {
    NOT_IMPLEMENTED();
  }
  if (asmGeneration->sizeofData == 4)
    seed = 0xac;
  else if (asmGeneration->sizeofData == 2)
    seed = 0xa4;
  else
    seed = 0xa0;
  libbf_gen_asm_mips_ops(asmGeneration, (seed << 24) + (regPtr << 21) + (regSrc << 16) + (offset & 0xFFFF));
}

static void libbf_gen_asm_mips_mov_reg_to_reg(AsmGeneration_t* asmGeneration, RegMips_t regSrc, RegMips_t regDst)
{
  libbf_gen_asm_mips_ops(asmGeneration, (regSrc << 21) + (regDst << 11) + 0x21);
}

static void libbf_gen_asm_mips_add_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegMips_t regSrc2, RegMips_t regDst)
{
  if (constant < -32768 || constant > 32767)
  {
    NOT_IMPLEMENTED();
  }
  libbf_gen_asm_mips_ops(asmGeneration, (0x24 << 24) + (regSrc2 << 21) + (regDst << 16) + (constant & 0xFFFF));
}

#if 0
/* regDst = (regSrc2 < constant) ? 1 : 0 */
static void libbf_gen_asm_mips_slt_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegMips_t regSrc2, RegMips_t regDst)
{
  if (constant < -32768 || constant > 32767)
  {
    NOT_IMPLEMENTED();
  }
  libbf_gen_asm_mips_ops(asmGeneration, (0x2c << 24) + (regSrc2 << 21) + (regDst << 16) + (constant & 0xFFFF));
}
#endif

static void libbf_gen_asm_mips_mov_cst_to_high_reg(AsmGeneration_t* asmGeneration, unsigned int constant, RegMips_t regDst)
{
  if (constant & 0xffff0000)
  {
    SHOULDNT_HAPPEN();
  }
  libbf_gen_asm_mips_ops(asmGeneration, (0x3c << 24) + (regDst << 16) + constant);
}

static void libbf_gen_asm_mips_ori_cst_to_reg(AsmGeneration_t* asmGeneration, unsigned int constant, RegMips_t regSrc2, RegMips_t regDst)
{
  if (constant & 0xffff0000)
  {
    SHOULDNT_HAPPEN();
  }
  libbf_gen_asm_mips_ops(asmGeneration, (0x34 << 24) + (regSrc2 << 21) + (regDst << 16) + constant);
}

static void libbf_gen_asm_mips_mov_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegMips_t regDst)
{
  if (constant == 0)
    libbf_gen_asm_mips_mov_reg_to_reg(asmGeneration, ZERO_MIPS, regDst);
  else if (constant < -32768 || constant > 32767)
  {
    unsigned int uconstant = (unsigned int)constant;
    libbf_gen_asm_mips_mov_cst_to_high_reg(asmGeneration, uconstant >> 16, regDst);
    libbf_gen_asm_mips_ori_cst_to_reg(asmGeneration, uconstant & 0xffff, regDst, regDst);
  }
  else
  {
    libbf_gen_asm_mips_add_cst_to_reg(asmGeneration, constant, ZERO_MIPS, regDst);
  }
}


/* regDst = regSrc1 + regSrc2 */
static void libbf_gen_asm_mips_add_reg(AsmGeneration_t* asmGeneration, RegMips_t regSrc1, RegMips_t regSrc2, RegMips_t regDst)
{
  libbf_gen_asm_mips_ops(asmGeneration, (regSrc1 << 21) + (regSrc2 << 16) + (regDst << 11) + 0x21);
}

/* regDst = regSrc1 - regSrc2 */
static void libbf_gen_asm_mips_sub_reg(AsmGeneration_t* asmGeneration, RegMips_t regSrc1, RegMips_t regSrc2, RegMips_t regDst)
{
  libbf_gen_asm_mips_ops(asmGeneration, (regSrc1 << 21) + (regSrc2 << 16) + (regDst << 11) + 0x23);
}

/* regDst = constant ^ regSrc2 */
static void libbf_gen_asm_mips_xor_constant_to_reg(AsmGeneration_t* asmGeneration, int constant, RegMips_t regSrc2, RegMips_t regDst)
{
  if (constant < -32768 || constant > 32767)
  {
    NOT_IMPLEMENTED();
  }
  libbf_gen_asm_mips_ops(asmGeneration, (0x38 << 24) + (regSrc2 << 21) + (regDst << 16) + (constant & 0xFFFF));
}

/* regDst = regSrc1 | regSrc2 */
static void libbf_gen_asm_mips_or_reg(AsmGeneration_t* asmGeneration, RegMips_t regSrc1, RegMips_t regSrc2, RegMips_t regDst)
{
  libbf_gen_asm_mips_ops(asmGeneration, (regSrc1 << 21) + (regSrc2 << 16) + (regDst << 11) + 0x25);
}

/* regDst = (regSrc1 < regSrc2) ? 1 : 0 */
static void libbf_gen_asm_mips_slt_reg(AsmGeneration_t* asmGeneration, RegMips_t regSrc1, RegMips_t regSrc2, RegMips_t regDst)
{
  libbf_gen_asm_mips_ops(asmGeneration, (regSrc1 << 21) + (regSrc2 << 16) + (regDst << 11) + 0x2b);
}

/* regDst = regSrc1 * regSrc2 */
static void libbf_gen_asm_mips_mul_reg(AsmGeneration_t* asmGeneration, RegMips_t regSrc1, RegMips_t regSrc2, RegMips_t regDst)
{
  /* multu */
  libbf_gen_asm_mips_ops(asmGeneration, (regSrc1 << 21) + (regSrc2 << 16) + 0x19);
  
  /* mflo */
  libbf_gen_asm_mips_ops(asmGeneration, (regDst << 11) + 0x12);
}

/* (regDstHigh, regDstLow) = regSrc1 * regSrc2 */
static void libbf_gen_asm_mips_mul64_reg(AsmGeneration_t* asmGeneration, RegMips_t regSrc1, RegMips_t regSrc2, RegMips_t regDstLow, RegMips_t regDstHigh)
{
  libbf_gen_asm_mips_mul_reg(asmGeneration, regSrc1, regSrc2, regDstLow);
  
  /* mfhi */
  libbf_gen_asm_mips_ops(asmGeneration, (regDstHigh << 11) + 0x10);
}

static void libbf_gen_asm_mips_sll_cst_to_reg(AsmGeneration_t* asmGeneration, int shift, RegMips_t regSrc, RegMips_t regDst)
{
  if (shift < 0 || shift > 31)
  {
    SHOULDNT_HAPPEN();
  }
  
  libbf_gen_asm_mips_ops(asmGeneration, (regSrc << 16) + (regDst << 11) + (shift << 6));
}


/* 03e0f809        jalr    ra */
static void libbf_gen_asm_mips_call_at_reg(AsmGeneration_t* asmGeneration, RegMips_t reg)
{
  ASSERT(reg == T9_MIPS); /* gcc convention */
  libbf_gen_asm_mips_ops(asmGeneration, 0x0000f809 + (reg << 21));
}

static void libbf_gen_asm_mips_preambule(AsmGeneration_t* asmGeneration)
{
  libbf_gen_asm_mips_ops(asmGeneration, (0x10 << 24) | (5+1)); /* jump over */
  libbf_gen_asm_mips_nop(asmGeneration);

  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration, 0)); /* CODE SIZE : TO BE POST-FIXED */
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration, asmGeneration->cpu));
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration, asmGeneration->sizeofData));
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration, asmGeneration->min_delta_offset));
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration, asmGeneration->max_delta_offset));

  libbf_gen_asm_mips_add_cst_to_reg(asmGeneration, -48, SP_MIPS, SP_MIPS);

  libbf_gen_asm_mips_mov_reg_to_mem32(asmGeneration, RA_MIPS, SP_MIPS, 40);
  libbf_gen_asm_mips_mov_reg_to_mem32(asmGeneration, S0_MIPS, SP_MIPS, 36);
  libbf_gen_asm_mips_mov_reg_to_mem32(asmGeneration, S1_MIPS, SP_MIPS, 32);

  libbf_gen_asm_mips_mov_reg_to_reg(asmGeneration, A0_MIPS, S0_MIPS); /* asmGeneration_t* */
  libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, A0_MIPS, OFFSET_BASE_DATA_PTR, INDEX_REG);
  libbf_gen_asm_mips_nop(asmGeneration);
}

static void libbf_gen_asm_mips_epilogue(AsmGeneration_t* asmGeneration)
{
  libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, SP_MIPS, 32, S1_MIPS);
  libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, SP_MIPS, 36, S0_MIPS);
  libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, SP_MIPS, 40, RA_MIPS);

  libbf_gen_asm_mips_add_cst_to_reg(asmGeneration, 48, SP_MIPS, SP_MIPS);

  /* 03e00008        jr      ra */
  libbf_gen_asm_mips_ops(asmGeneration, 0x03e00008);
  libbf_gen_asm_mips_nop(asmGeneration);

  *(int*)&asmGeneration->compiled_code[8] = INT_TO_TGT(asmGeneration, asmGeneration->filled_size);
}

static void libbf_gen_asm_mips_debug_code(AsmGeneration_t* asmGeneration, OptimizationUnit* current_code_ptr)
{
#if defined(__mips__)
  libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, S0_MIPS, OFFSET_DEBUG_FUNC, T9_MIPS);

  libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, S0_MIPS, OFFSET_DEBUG_DATA, A0_MIPS);
    
  libbf_gen_asm_mips_mov_cst_to_reg(asmGeneration, (int)current_code_ptr, A1_MIPS);
  
  libbf_gen_asm_mips_mov_reg_to_reg(asmGeneration, INDEX_REG, A2_MIPS);
  
  libbf_gen_asm_mips_call_at_reg(asmGeneration, T9_MIPS);
  libbf_gen_asm_mips_nop(asmGeneration);
#endif
}

static void libbf_gen_asm_mips_longlong_mov_mem_to_reg(AsmGeneration_t* asmGeneration, RegMips_t regPtr, int offset, int suboffset, RegMips_t regDst)
{
  offset = libbf_compiler_compute_longlong_offset(asmGeneration, offset, suboffset);
  libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, regPtr, offset, regDst);
}

static void libbf_gen_asm_mips_longlong_mov_reg_to_mem(AsmGeneration_t* asmGeneration, RegMips_t regSrc, RegMips_t regPtr, int offset, int suboffset)
{
  offset = libbf_compiler_compute_longlong_offset(asmGeneration, offset, suboffset);
  libbf_gen_asm_mips_mov_reg_to_mem32(asmGeneration, regSrc, regPtr, offset);
}

static void libbf_gen_asm_mips_add_reg_with_carry(AsmGeneration_t* asmGeneration, 
    RegMips_t reg1, RegMips_t reg2,
    RegMips_t regAdd, RegMips_t regCarry)
{
  libbf_gen_asm_mips_add_reg(asmGeneration, reg1, reg2, regAdd);
  libbf_gen_asm_mips_add_reg(asmGeneration, regAdd, regCarry, regAdd);
}

static void libbf_gen_asm_mips_add_reg_update_carry(AsmGeneration_t* asmGeneration, 
    RegMips_t reg1, RegMips_t reg2,
    RegMips_t regAdd, RegMips_t regCarry)
{
  ASSERT(regAdd != reg1);
  libbf_gen_asm_mips_add_reg(asmGeneration, reg1, reg2, regAdd);
  libbf_gen_asm_mips_slt_reg(asmGeneration, regAdd, reg1, regCarry);
}

static void libbf_gen_asm_mips_add_reg_with_carry_update_carry(AsmGeneration_t* asmGeneration, 
    RegMips_t reg1, RegMips_t reg2,
    RegMips_t regAdd, RegMips_t regCarry)
{
  ASSERT(regAdd != reg1);
  libbf_gen_asm_mips_add_reg(asmGeneration, reg1, reg2, regAdd);
  libbf_gen_asm_mips_add_reg(asmGeneration, regAdd, regCarry, regAdd);

  /* beqz */
  libbf_gen_asm_mips_ops(asmGeneration, (0x10 << 24) + (regCarry << 21) + 3);
  libbf_gen_asm_mips_slt_reg(asmGeneration, regAdd, reg1, regCarry); /* regCarry = regAdd < reg1 */ /* delay slot */
  
  libbf_gen_asm_mips_slt_reg(asmGeneration, reg1, regAdd, regCarry);
  libbf_gen_asm_mips_xor_constant_to_reg(asmGeneration, 0x1, regCarry, regCarry);  /* regCarry = regAdd <= reg1 */
}


static void libbf_gen_asm_mips_sub_reg_with_carry(AsmGeneration_t* asmGeneration, 
    RegMips_t reg1, RegMips_t reg2,
    RegMips_t regAdd, RegMips_t regCarry)
{
  libbf_gen_asm_mips_sub_reg(asmGeneration, reg1, reg2, regAdd);
  libbf_gen_asm_mips_sub_reg(asmGeneration, regAdd, regCarry, regAdd);
}

static void libbf_gen_asm_mips_sub_reg_update_carry(AsmGeneration_t* asmGeneration, 
    RegMips_t reg1, RegMips_t reg2,
    RegMips_t regAdd, RegMips_t regCarry)
{
  libbf_gen_asm_mips_sub_reg(asmGeneration, reg1, reg2, regAdd);
  libbf_gen_asm_mips_slt_reg(asmGeneration, reg1, reg2, regCarry);
}

static void libbf_gen_asm_mips_sub_reg_with_carry_update_carry(AsmGeneration_t* asmGeneration, 
    RegMips_t reg1, RegMips_t reg2,
    RegMips_t regAdd, RegMips_t regCarry)
{
  ASSERT(regAdd != reg1);
  libbf_gen_asm_mips_sub_reg(asmGeneration, reg1, reg2, regAdd);
  libbf_gen_asm_mips_sub_reg(asmGeneration, regAdd, regCarry, regAdd);

  /* beqz */
  libbf_gen_asm_mips_ops(asmGeneration, (0x10 << 24) + (regCarry << 21) + 3);
  libbf_gen_asm_mips_slt_reg(asmGeneration, reg1, reg2, regCarry); /* regCarry = reg1 < reg2 */ /* delay slot */
  
  libbf_gen_asm_mips_slt_reg(asmGeneration, reg2, reg1, regCarry);
  libbf_gen_asm_mips_xor_constant_to_reg(asmGeneration, 0x1, regCarry, regCarry);  /* regCarry = reg1 <= reg2 */
}

static void libbf_gen_asm_mips_test_while(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  int i;
  int nWords = NWORDS(asmGeneration);
  if (nWords <= 1)
  {
    libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, T0_MIPS);
    libbf_gen_asm_mips_nop(asmGeneration);
  }
  else
  {
    libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, T0_MIPS);
    libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 1, T1_MIPS);
    if (nWords > 2)
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 2, T2_MIPS);
    else
      libbf_gen_asm_mips_nop(asmGeneration);
      
    for(i=4;i<nWords;i++)
    {
      if ((i % 2) == 0)
      { 
        libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i-1, T1_MIPS);
        libbf_gen_asm_mips_or_reg(asmGeneration, T2_MIPS, T0_MIPS, T0_MIPS);
      }
      else
      {
        libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i-1, T2_MIPS);
        libbf_gen_asm_mips_or_reg(asmGeneration, T1_MIPS, T0_MIPS, T0_MIPS);
      }
    }
    if ((nWords % 2) == 0)
      libbf_gen_asm_mips_or_reg(asmGeneration, T1_MIPS, T0_MIPS, T0_MIPS);
    else
      libbf_gen_asm_mips_or_reg(asmGeneration, T2_MIPS, T0_MIPS, T0_MIPS);
  }
}

static void libbf_gen_asm_mips_while(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  if (asmGeneration->generate_debug)
    libbf_gen_asm_debug_code(asmGeneration, code_ptr);
      
  libbf_gen_asm_mips_test_while(asmGeneration, code_ptr);
      
  code_ptr->ou_offset_to_patch = asmGeneration->filled_size;
      
  libbf_gen_asm_mips_nop(asmGeneration); /* beqz to be patched */
  libbf_gen_asm_mips_nop(asmGeneration); /* delay slot of beqz */

  *(asmGeneration->p_stack_while) = code_ptr;
  asmGeneration->p_stack_while++;
}

static void libbf_gen_asm_mips_end_while(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  OptimizationUnit* while_instr;
  asmGeneration->p_stack_while--;
  while_instr = *(asmGeneration->p_stack_while);
      
  if (code_ptr->ou_loop_type != LIBBF_IF)
  {
    if (asmGeneration->generate_debug)
      libbf_gen_asm_debug_code(asmGeneration, while_instr);
        
    libbf_gen_asm_mips_test_while(asmGeneration, code_ptr);
        
    /* 1460fff8        bnez */
    libbf_gen_asm_mips_ops(asmGeneration, (0x14 << 24) + (T0_MIPS << 21) + (((while_instr->ou_offset_to_patch - asmGeneration->filled_size) / 4) & 0x0000FFFF));
        
    libbf_gen_asm_mips_nop(asmGeneration); /* delay slof of bnez */
  }
      
  /* patch beqz */
  *(int*)(asmGeneration->compiled_code + while_instr->ou_offset_to_patch) =
      INT_TO_TGT(asmGeneration, (0x10 << 24) + (T0_MIPS << 21) + (((asmGeneration->filled_size - while_instr->ou_offset_to_patch) / 4) - 1));

}

static void libbf_gen_asm_mips(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  switch(code_ptr->ou_instr)
  {
    case BF_INC_DATA_PTR:
    {
      libbf_gen_asm_mips_add_cst_to_reg(asmGeneration, code_ptr->ou_offset * asmGeneration->sizeofData, INDEX_REG, INDEX_REG);
      break;
    }
    
    case BF_PRINT_DATA:
    {
      libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, S0_MIPS, OFFSET_OUTPUT_FUNC, T9_MIPS);
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, A0_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
      
      libbf_gen_asm_mips_call_at_reg(asmGeneration, T9_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
        
      break;
    }
    
    case BF_READ_DATA:
    {
      libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, S0_MIPS, OFFSET_INPUT_FUNC, T9_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
      
      libbf_gen_asm_mips_call_at_reg(asmGeneration, T9_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
      
      libbf_gen_asm_mips_mov_reg_to_mem(asmGeneration, V0_MIPS, INDEX_REG, code_ptr->ou_offset);
        
      break;
    }
    
    case BF_SET_CST:
    {
      if (code_ptr->ou_constant == 0)
      {
        libbf_gen_asm_mips_mov_reg_to_mem(asmGeneration, ZERO_MIPS, INDEX_REG, code_ptr->ou_offset);
      }
      else
      {
        libbf_gen_asm_mips_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, T0_MIPS);
        libbf_gen_asm_mips_mov_reg_to_mem(asmGeneration, T0_MIPS, INDEX_REG, code_ptr->ou_offset);
      }
      break;
    }
    
    case BF_INC_CST:
    {
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, T0_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
      libbf_gen_asm_mips_add_cst_to_reg(asmGeneration, code_ptr->ou_constant, T0_MIPS, T0_MIPS);
      libbf_gen_asm_mips_mov_reg_to_mem(asmGeneration, T0_MIPS, INDEX_REG, code_ptr->ou_offset);
      break;
    }
    
    case BF_SET_DATA:
    {
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, T0_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
      libbf_gen_asm_mips_mov_reg_to_mem(asmGeneration, T0_MIPS, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
        
    case BF_INC_DATA:
    {
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, T0_MIPS);
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, T1_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
      libbf_gen_asm_mips_add_reg(asmGeneration, T0_MIPS, T1_MIPS, T1_MIPS);
      libbf_gen_asm_mips_mov_reg_to_mem(asmGeneration, T1_MIPS, INDEX_REG, code_ptr->ou_offset);

      break;
    }
    
    case BF_DEC_DATA:
    {
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, T0_MIPS);
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, T1_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
      libbf_gen_asm_mips_sub_reg(asmGeneration, T0_MIPS, T1_MIPS, T1_MIPS);
      libbf_gen_asm_mips_mov_reg_to_mem(asmGeneration, T1_MIPS, INDEX_REG, code_ptr->ou_offset);

      break;
    }
    
    case BF_WHILE_DATA:  /* while (data_ptr[offset]) { */
    {
      libbf_gen_asm_mips_while(asmGeneration, code_ptr);
      break;
    }
      
    case BF_END_WHILE:
    {
      libbf_gen_asm_mips_end_while(asmGeneration, code_ptr);
      break;
    }
    
    case BF_INC_CST_MUL_DATA:
    {
      int shift1, shift2;
      int constant_pos = (code_ptr->ou_constant > 0) ? code_ptr->ou_constant : -code_ptr->ou_constant;
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, T0_MIPS);
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, T1_MIPS);

      if (libbf_is_composed_poweroftwo(constant_pos, &shift1, &shift2) && shift2 == 0)
      {
        libbf_gen_asm_mips_sll_cst_to_reg(asmGeneration, shift1, T0_MIPS, T0_MIPS);
      }
      else
      {
        libbf_gen_asm_mips_mov_cst_to_reg(asmGeneration, constant_pos, T2_MIPS);
        libbf_gen_asm_mips_mul_reg(asmGeneration, T2_MIPS, T0_MIPS, T0_MIPS);
      }

      if (code_ptr->ou_constant > 0)
        libbf_gen_asm_mips_add_reg(asmGeneration, T1_MIPS, T0_MIPS, T0_MIPS);
      else
        libbf_gen_asm_mips_sub_reg(asmGeneration, T1_MIPS, T0_MIPS, T0_MIPS);
      libbf_gen_asm_mips_mov_reg_to_mem(asmGeneration, T0_MIPS, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    case BF_INC_DATA_MUL_DATA:
    {
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, T0_MIPS);
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset3, T1_MIPS);
      libbf_gen_asm_mips_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, T2_MIPS);
      libbf_gen_asm_mips_mul_reg(asmGeneration, T1_MIPS, T0_MIPS, T0_MIPS);
      libbf_gen_asm_mips_add_reg(asmGeneration, T0_MIPS, T2_MIPS, T0_MIPS);
      libbf_gen_asm_mips_mov_reg_to_mem(asmGeneration, T0_MIPS, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    default:
    {
      libbf_gen_asm_default(asmGeneration, code_ptr);
      break;
    }
  }
}

static void libbf_gen_asm_mips_longlong(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  int nWords = NWORDS(asmGeneration);
  
  switch(code_ptr->ou_instr)
  {
    case BF_INC_DATA_PTR:
    {
      libbf_gen_asm_mips_add_cst_to_reg(asmGeneration, code_ptr->ou_offset * asmGeneration->sizeofData, INDEX_REG, INDEX_REG);
      break;
    }
    
    case BF_PRINT_DATA:
    {
      libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, S0_MIPS, OFFSET_OUTPUT_FUNC, T9_MIPS);
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, A0_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
      
      libbf_gen_asm_mips_call_at_reg(asmGeneration, T9_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
        
      break;
    }
    
    case BF_READ_DATA:
    {
      libbf_gen_asm_mips_mov_mem32_to_reg(asmGeneration, S0_MIPS, OFFSET_INPUT_FUNC, T9_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
      
      libbf_gen_asm_mips_call_at_reg(asmGeneration, T9_MIPS);
      libbf_gen_asm_mips_nop(asmGeneration);
      
      libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, V0_MIPS, INDEX_REG, code_ptr->ou_offset, 0);
        
      break;
    }
    
    case BF_SET_CST:
    {
      int i;
      if (code_ptr->ou_constant == 0)
      {
        libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, ZERO_MIPS, INDEX_REG, code_ptr->ou_offset, 0);
      }
      else
      {
        libbf_gen_asm_mips_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, T0_MIPS);
        libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, T0_MIPS, INDEX_REG, code_ptr->ou_offset, 0);
      }
      if (code_ptr->ou_constant < 0)
      {
        libbf_gen_asm_mips_mov_cst_to_reg(asmGeneration, -1, T0_MIPS);
      }
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration,
                                          (code_ptr->ou_constant < 0 ) ? T0_MIPS : ZERO_MIPS,
                                          INDEX_REG, code_ptr->ou_offset, i);
      }
      break;
    }
    
#define SWAP_REG(a, b) do { RegMips_t c = a; a = b; b = c; } while(0)

    case BF_INC_CST:
    {
      int i;
      RegMips_t reg1 = T0_MIPS, reg2 = T1_MIPS;
      RegMips_t regAdd = T2_MIPS;
      RegMips_t regCarry = T3_MIPS;
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, reg1);
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 1, reg2);
      libbf_gen_asm_mips_add_cst_to_reg(asmGeneration, code_ptr->ou_constant, reg1, regAdd);
      libbf_gen_asm_mips_slt_reg(asmGeneration, regAdd, reg1, regCarry); /* tmpReg = carry */
      
      libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regAdd, INDEX_REG, code_ptr->ou_offset, 0);
      
      for(i=1;i<nWords;i++)
      {
        SWAP_REG(reg1, reg2);
        
        if (code_ptr->ou_constant < 0)
        {
          libbf_gen_asm_mips_add_cst_to_reg(asmGeneration, -1, regCarry, regAdd);
          libbf_gen_asm_mips_add_reg(asmGeneration, regAdd, reg1, regAdd);
        }
        else
        {
          libbf_gen_asm_mips_add_reg(asmGeneration, regCarry, reg1, regAdd);
        }
        if (i != nWords - 1)
        {
          /* beqz */
          libbf_gen_asm_mips_ops(asmGeneration, (0x10 << 24) + (regCarry << 21) + 3);
  
          libbf_gen_asm_mips_slt_reg(asmGeneration, regAdd, reg1, regCarry); /* regCarry = regAdd < reg1 */ /* delay slot */
  
          libbf_gen_asm_mips_slt_reg(asmGeneration, reg1, regAdd, regCarry);
          libbf_gen_asm_mips_xor_constant_to_reg(asmGeneration, 0x1, regCarry, regCarry);  /* regCarry = regAdd <= reg1 */
          
          libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+1, reg2);
        }
        libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regAdd, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }
    
    case BF_SET_DATA:
    {
      int i;
      for(i=0;i<nWords;i+=2)
      {
        libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, T0_MIPS);
        if (i+1 < nWords)
          libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i+1, T1_MIPS);
        libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, T0_MIPS, INDEX_REG, code_ptr->ou_offset, i);
        if (i+1 < nWords)
          libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, T1_MIPS, INDEX_REG, code_ptr->ou_offset, i+1);
      }
      break;
    }
    
    case BF_INC_DATA:
    {
      int i;
      RegMips_t reg1 = T0_MIPS, reg2 = T1_MIPS;
      RegMips_t regAdd = T2_MIPS;
      RegMips_t regCarry = T3_MIPS;
      RegMips_t regNext1 = T4_MIPS, regNext2 = T5_MIPS;
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, reg1);
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 0, reg2);
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 1, regNext1);
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 1, regNext2);
      libbf_gen_asm_mips_add_reg_update_carry(asmGeneration, reg1, reg2, regAdd, regCarry);
      SWAP_REG(reg1, regNext1);
      SWAP_REG(reg2, regNext2);
      
      for(i=1;i<nWords;i++)
      {
        if (i != 1)
        {
          libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, reg1);
          libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, reg2);
        }
        libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regAdd, INDEX_REG, code_ptr->ou_offset, i-1);
        
        if (i != nWords - 1)
        {
          libbf_gen_asm_mips_add_reg_with_carry_update_carry(asmGeneration, reg1, reg2, regAdd, regCarry);
        }
        else
        {
          libbf_gen_asm_mips_add_reg_with_carry(asmGeneration, reg1, reg2, regAdd, regCarry);
        }
      }
      libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regAdd, INDEX_REG, code_ptr->ou_offset, i-1);

      break;
    }
    
    case BF_DEC_DATA:
    {
      int i;
      RegMips_t reg1 = T0_MIPS, reg2 = T1_MIPS;
      RegMips_t regAdd = T2_MIPS;
      RegMips_t regCarry = T3_MIPS;
      RegMips_t regNext1 = T4_MIPS, regNext2 = T5_MIPS;
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, reg1);
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 0, reg2);
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 1, regNext1);
      libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 1, regNext2);
      libbf_gen_asm_mips_sub_reg_update_carry(asmGeneration, reg1, reg2, regAdd, regCarry);
      SWAP_REG(reg1, regNext1);
      SWAP_REG(reg2, regNext2);
      
      for(i=1;i<nWords;i++)
      {
        if (i != 1)
        {
          libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, reg1);
          libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, reg2);
        }
        libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regAdd, INDEX_REG, code_ptr->ou_offset, i-1);
        
        if (i != nWords - 1)
        {
          libbf_gen_asm_mips_sub_reg_with_carry_update_carry(asmGeneration, reg1, reg2, regAdd, regCarry);
        }
        else
        {
          libbf_gen_asm_mips_sub_reg_with_carry(asmGeneration, reg1, reg2, regAdd, regCarry);
        }
        
      }
      libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regAdd, INDEX_REG, code_ptr->ou_offset, i-1);

      break;
    }

    case BF_WHILE_DATA:  /* while (data_ptr[offset]) { */
    {
      libbf_gen_asm_mips_while(asmGeneration, code_ptr);
      break;
    }
      
    case BF_END_WHILE:
    {
      libbf_gen_asm_mips_end_while(asmGeneration, code_ptr);
      break;
    }
    
    case BF_INC_CST_MUL_DATA:
    {
      int i, j;
      RegMips_t regSrc1 = T0_MIPS;
      RegMips_t regSrc2 = T1_MIPS;
      RegMips_t regMulLow = T2_MIPS;
      RegMips_t regMulHigh = T3_MIPS;
      RegMips_t regDstLow = T4_MIPS;
      RegMips_t regDstHigh = T5_MIPS;
      RegMips_t regDst = T6_MIPS;
      RegMips_t regAdd = T7_MIPS;
      RegMips_t regCarry = T8_MIPS;
            
      for(j=0;j<nWords;j++)
      {
        if (j == 0)
          libbf_gen_asm_mips_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, regSrc1);
        else if (code_ptr->ou_constant < 0)
        {
          if (j == 1)
            libbf_gen_asm_mips_mov_cst_to_reg(asmGeneration, -1, regSrc1);
        }
        else
        {
          break;
        }
                
        for(i=0;i<nWords-j;i++)
        {
          libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, regSrc2);
          libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j, regDst);
              
          if (i != 0)
          {
            if (i+j < nWords-1)
            {
              libbf_gen_asm_mips_mul64_reg(asmGeneration, regSrc1, regSrc2, regMulLow, regMulHigh);
              libbf_gen_asm_mips_add_reg_update_carry(asmGeneration, regDstHigh, regMulLow, regDstLow, regCarry);
              libbf_gen_asm_mips_add_reg(asmGeneration, regCarry, regMulHigh, regDstHigh);
            }
            else
            {
              libbf_gen_asm_mips_mul_reg(asmGeneration, regSrc1, regSrc2, regMulLow);
              libbf_gen_asm_mips_add_reg(asmGeneration, regDstHigh, regMulLow, regDstLow);
            }
          }
          else
          {
            libbf_gen_asm_mips_mul64_reg(asmGeneration, regSrc1, regSrc2, regDstLow, regDstHigh);
          }
                
          if (i+j < nWords-1)
          {
            libbf_gen_asm_mips_add_reg_update_carry(asmGeneration, regDstLow, regDst, regAdd, regCarry);
            libbf_gen_asm_mips_add_reg(asmGeneration, regCarry, regDstHigh, regDstHigh);
          }
          else
          {
            libbf_gen_asm_mips_add_reg(asmGeneration, regDstLow, regDst, regAdd);
          }
          libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regAdd, INDEX_REG, code_ptr->ou_offset, i+j);
        }
      }
      break;
    }
    
    case BF_INC_DATA_MUL_DATA:
    {
      int i, j;
      RegMips_t regSrc1 = T0_MIPS;
      RegMips_t regSrc2 = T1_MIPS;
      RegMips_t regMulLow = T2_MIPS;
      RegMips_t regMulHigh = T3_MIPS;
      RegMips_t regDstLow = T4_MIPS;
      RegMips_t regDstHigh = T5_MIPS;
      RegMips_t regDst1 = T6_MIPS;
      RegMips_t regDst2 = T7_MIPS;
      RegMips_t regCarry = T8_MIPS;
      
      for(j=0;j<nWords;j++)
      {
        libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset3, j, regSrc1);
          
        for(i=0;i<nWords - j;i++)
        {
          int k;
              
          libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, regSrc2);
          
          if (i+j != nWords - 1)
          {
            libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j, regDst1);
            libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j+1, regDst2);
            libbf_gen_asm_mips_mul64_reg(asmGeneration, regSrc1, regSrc2, regMulLow, regMulHigh);
              
            libbf_gen_asm_mips_add_reg_update_carry(asmGeneration, regDst1, regMulLow, regDstLow, regCarry);
            libbf_gen_asm_mips_add_reg_with_carry_update_carry(asmGeneration, regDst2, regMulHigh, regDstHigh, regCarry);
            
            k=i+j+2;
            if (k<nWords)
              libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, k, regDst1);
            libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regDstLow, INDEX_REG, code_ptr->ou_offset, i+j);
            libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regDstHigh, INDEX_REG, code_ptr->ou_offset, i+j+1);
            
            for(k=i+j+2;k<nWords;k++)
            {
              libbf_gen_asm_mips_add_reg_with_carry_update_carry(asmGeneration, ZERO_MIPS, regDst1, regDstLow, regCarry);
              if (k+1 <nWords)
                libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, k+1, regDst1);
              libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regDstLow, INDEX_REG, code_ptr->ou_offset, k);
            }
          }
          else
          {
            libbf_gen_asm_mips_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j, regDst1);
            libbf_gen_asm_mips_mul_reg(asmGeneration, regSrc1, regSrc2, regMulLow);
            
            libbf_gen_asm_mips_add_reg(asmGeneration, regDst1, regMulLow, regDstLow);
            libbf_gen_asm_mips_longlong_mov_reg_to_mem(asmGeneration, regDstLow, INDEX_REG, code_ptr->ou_offset, i+j);
          }
        }
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



#if defined(__mips__)
int libbf_is_compatible_compiled_code  (char* compiled_code,
                                        TargetCpu_t* cpu,
                                        int* sizeofData,
                                        int* min_delta_offset,
                                        int* max_delta_offset)
{
  if (!((*(int*)compiled_code) == (0x10000000 | (5+1))))
  {
    warning("Non mips libbf-compiled code\n");
    return FALSE;
  }

  *cpu = (*(int*)(compiled_code + 8 + 4));
  *sizeofData = (*(int*)(compiled_code + 8 + 8));
  *min_delta_offset = (*(int*)(compiled_code + 8 + 12));
  *max_delta_offset = (*(int*)(compiled_code + 8 + 16));
    
  return TRUE;
}
#endif

static void libbf_gen_asm_mips_init_funcs()
{
  asmGenerationFuncs[TARGET_MIPS].gen_preambule = libbf_gen_asm_mips_preambule;
  asmGenerationFuncs[TARGET_MIPS].gen_epilogue = libbf_gen_asm_mips_epilogue;
  asmGenerationFuncs[TARGET_MIPS].gen_code = libbf_gen_asm_mips;
  asmGenerationFuncs[TARGET_MIPS].gen_code_longlong = libbf_gen_asm_mips_longlong;
  asmGenerationFuncs[TARGET_MIPS].gen_debug_code = libbf_gen_asm_mips_debug_code;
  memcpy(&asmGenerationFuncs[TARGET_MIPSEL], &asmGenerationFuncs[TARGET_MIPS], sizeof(asmGenerationFuncs[TARGET_MIPS]));
}

#undef INDEX_REG
#undef INT_TO_TGT
