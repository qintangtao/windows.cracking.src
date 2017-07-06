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

#define PPC_STACK_SIZE 32

typedef enum
{
  R0_PPC,
  R1_PPC,
  R2_PPC,
  R3_PPC,
  R4_PPC,
  R5_PPC,
  R6_PPC,
  R7_PPC,
  R8_PPC,
  R9_PPC,
  R10_PPC,
  R11_PPC,
  R12_PPC,
  R13_PPC,
  R14_PPC,
#define IN_PARAM_REG  R14_PPC
  R15_PPC,
#define INDEX_REG     R15_PPC
} RegPpc_t;

static void libbf_gen_asm_ppc_op_code_and_2_reg(AsmGeneration_t* asmGeneration, RegPpc_t reg1, RegPpc_t reg2, int base_op_code)
{
  libbf_write_asm_short(asmGeneration, short_to_be((base_op_code << 8) + (reg1 << 5) + reg2));
}

static void libbf_gen_asm_ppc_mov_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegPpc_t reg)
{
  /* 38 60 XX YY     li      r3,0xXXYY */
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, reg, (RegPpc_t)0, 0x38);
  libbf_write_asm_short(asmGeneration, short_to_be(constant & 0xFFFF));
  
  if (constant < -32768 || constant >= 32768)
  {
    /* 3c 63 XX YY     addis   r3,r3,0xXXYY */
    libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, reg, reg, 0x3c);
    libbf_write_asm_short(asmGeneration, short_to_be(((constant >> 16) & 0xFFFF) + ((constant >> 15) & 1)));
  }
}

static void libbf_gen_asm_ppc_offset(AsmGeneration_t* asmGeneration, int offset)
{
  offset = asmGeneration->sizeofData * offset;
  if (offset < -32768 || offset >= 32768)
  {
    NOT_IMPLEMENTED();
  }
  libbf_write_asm_short(asmGeneration, short_to_be(offset));
}


static void libbf_gen_asm_ppc_mov_reg_to_mem32(AsmGeneration_t* asmGeneration, RegPpc_t regSrc, RegPpc_t regPtr, int offset)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, regSrc, regPtr, 0x90);
  if (offset < -32768 || offset >= 32768)
  {
    NOT_IMPLEMENTED();
  }
  libbf_write_asm_short(asmGeneration, short_to_be(offset));
}

static int libbf_gen_asm_ppc_not_expected()
{
  SHOULDNT_HAPPEN();
  return 0;
}

static void libbf_gen_asm_ppc_mov_reg_to_mem(AsmGeneration_t* asmGeneration, RegPpc_t regSrc, RegPpc_t regPtr, int offset)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, regSrc, regPtr,
                                      (asmGeneration->sizeofData == 4) ? 0x90 :
                                      (asmGeneration->sizeofData == 2) ? 0xb0 :
                                      (asmGeneration->sizeofData == 1) ? 0x98 : libbf_gen_asm_ppc_not_expected());
  libbf_gen_asm_ppc_offset(asmGeneration, offset);

}

static void libbf_gen_asm_ppc_mov_mem32_to_reg(AsmGeneration_t* asmGeneration, RegPpc_t regPtr, int offset, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, regDst, regPtr, 0x80);
  if (offset < -32768 || offset >= 32768)
  {
    NOT_IMPLEMENTED();
  }
  libbf_write_asm_short(asmGeneration, short_to_be(offset));
}

static void libbf_gen_asm_ppc_mov_mem_to_reg(AsmGeneration_t* asmGeneration, RegPpc_t regPtr, int offset, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, regDst, regPtr,
                                      (asmGeneration->sizeofData == 4) ? 0x80 :
                                      (asmGeneration->sizeofData == 2) ? 0xa8 :
                                      (asmGeneration->sizeofData == 1) ? 0x88 : libbf_gen_asm_ppc_not_expected());
  libbf_gen_asm_ppc_offset(asmGeneration, offset);
}


static void libbf_gen_asm_ppc_mov_reg_to_reg(AsmGeneration_t* asmGeneration, RegPpc_t regSrc, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, regSrc, regDst, 0x7c);
  libbf_write_asm_byte(asmGeneration, regSrc * 8 + 0x03);
  libbf_write_asm_byte(asmGeneration, 0x78);
}

static void libbf_gen_asm_ppc_add_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegPpc_t reg)
{
  if (constant < -32768 || constant >= 32768)
  {
    NOT_IMPLEMENTED();
  }
  
  /* 38 21 00 20     addi    r1,r1,32 */
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, reg, reg, 0x38);
  libbf_write_asm_short(asmGeneration, short_to_be(constant));
}


static void libbf_gen_asm_ppc_3reg(AsmGeneration_t* asmGeneration, RegPpc_t regSrc1, RegPpc_t regSrc2, RegPpc_t regDst, int seed)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, regDst, regSrc1, 0x7c);
  libbf_write_asm_byte(asmGeneration, regSrc2 * 8 + seed);
}

static void libbf_gen_asm_ppc_add(AsmGeneration_t* asmGeneration, RegPpc_t regSrc1, RegPpc_t regSrc2, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_3reg(asmGeneration, regSrc1, regSrc2, regDst, 0x02);
  libbf_write_asm_byte(asmGeneration, 0x14);
}

static void libbf_gen_asm_ppc_add_reg_to_reg(AsmGeneration_t* asmGeneration,  RegPpc_t regSrc, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_add(asmGeneration, regSrc, regDst, regDst);
}

static void libbf_gen_asm_ppc_subf_reg_to_reg(AsmGeneration_t* asmGeneration,  RegPpc_t regSrc, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_3reg(asmGeneration, regSrc, regDst, regDst, 0x00);
  libbf_write_asm_byte(asmGeneration, 0x50);
}

static void libbf_gen_asm_ppc_mul_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegPpc_t regDst)
{
  if (constant < -32768 || constant >= 32768)
  {
    NOT_IMPLEMENTED();
  }
  
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, regDst, regDst, 0x1c);
  libbf_write_asm_short(asmGeneration, short_to_be(constant));
}

static void libbf_gen_asm_ppc_mullw(AsmGeneration_t* asmGeneration, RegPpc_t regSrc1, RegPpc_t regSrc2, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_3reg(asmGeneration, regSrc1, regSrc2, regDst, 0x01);
  libbf_write_asm_byte(asmGeneration, 0xd6);
}
    
static void libbf_gen_asm_ppc_mullhwu(AsmGeneration_t* asmGeneration, RegPpc_t regSrc1, RegPpc_t regSrc2, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_3reg(asmGeneration, regSrc1, regSrc2, regDst, 0x00);
  libbf_write_asm_byte(asmGeneration, 0x16);
}


static void libbf_gen_asm_ppc_mtlr_reg(AsmGeneration_t* asmGeneration, RegPpc_t reg)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, reg, (RegPpc_t)0x08, 0x7c);
  libbf_write_asm_byte(asmGeneration,  0x03);
  libbf_write_asm_byte(asmGeneration,  0xa6);
}

static void libbf_gen_asm_ppc_mflr_reg(AsmGeneration_t* asmGeneration, RegPpc_t reg)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, reg, (RegPpc_t)0x08, 0x7c);
  libbf_write_asm_byte(asmGeneration,  0x02);
  libbf_write_asm_byte(asmGeneration,  0xa6);
}

static void libbf_gen_asm_ppc_blrl(AsmGeneration_t* asmGeneration)
{
  /* 4e 80 00 21     blrl */
  libbf_write_asm_4byte(asmGeneration, 0x4e, 0x80, 0x00, 0x21);
}

static void libbf_gen_asm_ppc_blr(AsmGeneration_t* asmGeneration)
{
  /* 4e 80 00 20     blr */
  libbf_write_asm_4byte(asmGeneration, 0x4e, 0x80, 0x00, 0x20);
}


static void libbf_gen_asm_ppc_cmp_reg_to_0(AsmGeneration_t* asmGeneration, RegPpc_t reg)
{
  libbf_write_asm_4byte(asmGeneration, 0x2c, reg, 0x00, 0x00);
}

static void libbf_gen_asm_ppc_preambule(AsmGeneration_t* asmGeneration)
{
  /* 48 zz yy xx     b   */
  libbf_write_asm_byte(asmGeneration, 0x48);
  libbf_write_asm_byte(asmGeneration, 0x00);
  libbf_write_asm_short(asmGeneration, short_to_be(4+5*4));

  libbf_write_asm_int(asmGeneration, int_to_be(0)); /* CODE SIZE : TO BE POST-FIXED */
  libbf_write_asm_int(asmGeneration, int_to_be(asmGeneration->cpu));
  libbf_write_asm_int(asmGeneration, int_to_be(asmGeneration->sizeofData));
  libbf_write_asm_int(asmGeneration, int_to_be(asmGeneration->min_delta_offset));
  libbf_write_asm_int(asmGeneration, int_to_be(asmGeneration->max_delta_offset));
        
  /* 94 21 ff e0     stwu    r1,-32(r1) */
  libbf_write_asm_byte(asmGeneration, 0x94);
  libbf_write_asm_byte(asmGeneration, 0x21);
  libbf_write_asm_short(asmGeneration, short_to_be(-(32 + PPC_STACK_SIZE)));
  
  libbf_gen_asm_ppc_mflr_reg(asmGeneration, R0_PPC);
  
  libbf_gen_asm_ppc_mov_reg_to_mem32(asmGeneration, R0_PPC, R1_PPC, 36+PPC_STACK_SIZE);
  libbf_gen_asm_ppc_mov_reg_to_mem32(asmGeneration, R13_PPC, R1_PPC, 24+PPC_STACK_SIZE);
  libbf_gen_asm_ppc_mov_reg_to_mem32(asmGeneration, IN_PARAM_REG, R1_PPC, 20+PPC_STACK_SIZE);
  libbf_gen_asm_ppc_mov_reg_to_mem32(asmGeneration, INDEX_REG, R1_PPC, 16+PPC_STACK_SIZE);

  
  libbf_gen_asm_ppc_mov_reg_to_reg(asmGeneration, R3_PPC, IN_PARAM_REG);  /* r14 : ExecutableCodeData_t* */

  libbf_gen_asm_ppc_mov_reg_to_mem32(asmGeneration, IN_PARAM_REG, R1_PPC, 4+PPC_STACK_SIZE);
  
  libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, SIGNATURE_1, R3_PPC);
  libbf_gen_asm_ppc_mov_reg_to_mem32(asmGeneration, R3_PPC, R1_PPC, 8+PPC_STACK_SIZE);
  
  libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, SIGNATURE_2, R3_PPC);
  libbf_gen_asm_ppc_mov_reg_to_mem32(asmGeneration, R3_PPC, R1_PPC, 12+PPC_STACK_SIZE);
  
  libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, IN_PARAM_REG, OFFSET_BASE_DATA_PTR, INDEX_REG); /* r15 : pointer to base data array */
}

static void libbf_gen_asm_ppc_epilogue(AsmGeneration_t* asmGeneration)
{
  libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, R1_PPC, 36+PPC_STACK_SIZE, R0_PPC);
  libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, R1_PPC, 24+PPC_STACK_SIZE, R13_PPC);
  libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, R1_PPC, 20+PPC_STACK_SIZE, IN_PARAM_REG);
  libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, R1_PPC, 16+PPC_STACK_SIZE, INDEX_REG);

  libbf_gen_asm_ppc_mtlr_reg(asmGeneration, R0_PPC);
  
  libbf_gen_asm_ppc_add_cst_to_reg(asmGeneration, 32 + PPC_STACK_SIZE, R1_PPC);
  
  libbf_gen_asm_ppc_blr(asmGeneration);
  
  
  *(int*)&asmGeneration->compiled_code[4] = int_to_be(asmGeneration->filled_size);
}

static void libbf_gen_asm_ppc_debug_code(AsmGeneration_t* asmGeneration, OptimizationUnit* current_code_ptr)
{
#if defined(__powerpc__)
  libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, IN_PARAM_REG, OFFSET_DEBUG_FUNC, R13_PPC);
  
  libbf_gen_asm_ppc_mtlr_reg(asmGeneration, R13_PPC);
    
  libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, IN_PARAM_REG, OFFSET_DEBUG_DATA, R3_PPC);
    
  libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, (int)current_code_ptr, R4_PPC);
    
  libbf_gen_asm_ppc_mov_reg_to_reg(asmGeneration, INDEX_REG, R5_PPC);
    
  libbf_gen_asm_ppc_blrl(asmGeneration);
#endif
}


static void libbf_gen_asm_ppc_longlong_offset(AsmGeneration_t* asmGeneration, int offset, int suboffset)
{
  offset = libbf_compiler_compute_longlong_offset(asmGeneration, offset, suboffset);
  if (offset < -32768 || offset >= 32768)
  {
    NOT_IMPLEMENTED();
  }
  libbf_write_asm_short(asmGeneration, short_to_be(offset));
}

static void libbf_gen_asm_ppc_longlong_mov_mem_to_reg(AsmGeneration_t* asmGeneration, RegPpc_t regPtr, int offset, int suboffset, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, regDst, regPtr, 0x80);
  libbf_gen_asm_ppc_longlong_offset(asmGeneration, offset, NWORDS(asmGeneration)-1 - suboffset);
}

static void libbf_gen_asm_ppc_longlong_mov_reg_to_mem(AsmGeneration_t* asmGeneration, RegPpc_t regSrc, RegPpc_t regPtr, int offset, int suboffset)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, regSrc, regPtr, 0x90);
  libbf_gen_asm_ppc_longlong_offset(asmGeneration, offset, NWORDS(asmGeneration)-1 - suboffset);
}

static void libbf_gen_asm_ppc_addic(AsmGeneration_t* asmGeneration, int constant, RegPpc_t reg)
{
  if (constant < -32768 || constant >= 32768)
  {
    NOT_IMPLEMENTED();
  }
  
  /* 30 21 00 20     addic    r1,r1,32 */
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, reg, reg, 0x30);
  libbf_write_asm_short(asmGeneration, short_to_be(constant));
}

static void libbf_gen_asm_ppc_addme(AsmGeneration_t* asmGeneration, RegPpc_t reg)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, reg, reg, 0x7c);
  libbf_write_asm_byte(asmGeneration,  0x01);
  libbf_write_asm_byte(asmGeneration,  0xd4);
}

static void libbf_gen_asm_ppc_addze(AsmGeneration_t* asmGeneration, RegPpc_t reg)
{
  libbf_gen_asm_ppc_op_code_and_2_reg(asmGeneration, reg, reg, 0x7c);
  libbf_write_asm_byte(asmGeneration,  0x01);
  libbf_write_asm_byte(asmGeneration,  0x94);
}

static void libbf_gen_asm_ppc_or_reg_to_reg(AsmGeneration_t* asmGeneration, RegPpc_t regSrc, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_3reg(asmGeneration, regSrc, regDst, regDst, 0x03);
  libbf_write_asm_byte(asmGeneration,  0x78);
}

static void libbf_gen_asm_ppc_or_dot_reg_to_reg(AsmGeneration_t* asmGeneration, RegPpc_t regSrc, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_3reg(asmGeneration, regSrc, regDst, regDst, 0x03);
  libbf_write_asm_byte(asmGeneration,  0x79);
}

static void libbf_gen_asm_ppc_addc_reg_to_reg(AsmGeneration_t* asmGeneration,  RegPpc_t regSrc, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_3reg(asmGeneration, regSrc, regDst, regDst, 0x00);
  libbf_write_asm_byte(asmGeneration, 0x14);
}

static void libbf_gen_asm_ppc_adde_reg_to_reg(AsmGeneration_t* asmGeneration,  RegPpc_t regSrc, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_3reg(asmGeneration, regSrc, regDst, regDst, 0x01);
  libbf_write_asm_byte(asmGeneration, 0x14);
}

static void libbf_gen_asm_ppc_subfc_reg_to_reg(AsmGeneration_t* asmGeneration,  RegPpc_t regSrc, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_3reg(asmGeneration, regSrc, regDst, regDst, 0x00);
  libbf_write_asm_byte(asmGeneration, 0x10);
}

static void libbf_gen_asm_ppc_subfe_reg_to_reg(AsmGeneration_t* asmGeneration,  RegPpc_t regSrc, RegPpc_t regDst)
{
  libbf_gen_asm_ppc_3reg(asmGeneration, regSrc, regDst, regDst, 0x01);
  libbf_write_asm_byte(asmGeneration, 0x10);
}

static void libbf_gen_asm_ppc_test_while(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  int nWords = NWORDS(asmGeneration);
  
  if (nWords >= 2)
  {
    int i;
    libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R4_PPC);
        
    for(i=1;i<nWords;i++)
    {
      libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, R3_PPC);
      if (i == nWords - 1)
        libbf_gen_asm_ppc_or_dot_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);
      else
        libbf_gen_asm_ppc_or_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);
    }
  }
  else
  {
    libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R3_PPC);
      
    libbf_gen_asm_ppc_cmp_reg_to_0(asmGeneration, R3_PPC);
  }   
}

static void libbf_gen_asm_ppc_while(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  if (asmGeneration->generate_debug)
    libbf_gen_asm_debug_code(asmGeneration, code_ptr);

  libbf_gen_asm_ppc_test_while(asmGeneration, code_ptr);
  
  /* bne */
  libbf_write_asm_byte(asmGeneration, 0x40);
  libbf_write_asm_byte(asmGeneration, 0x82);
  libbf_write_asm_byte(asmGeneration, 0x00);
  libbf_write_asm_byte(asmGeneration, 0x08);
  
  code_ptr->ou_offset_to_patch = asmGeneration->filled_size;
  
  /* b */
  libbf_write_asm_byte(asmGeneration, 0x48);
  libbf_write_asm_byte(asmGeneration, 0x00);
  libbf_write_asm_byte(asmGeneration, 0x00);
  libbf_write_asm_byte(asmGeneration, 0x00);

  *(asmGeneration->p_stack_while) = code_ptr;
  asmGeneration->p_stack_while++;
}

static void libbf_gen_asm_ppc_end_while(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  OptimizationUnit* while_instr;
  int positive_jump_offset;
    
  asmGeneration->p_stack_while--;
  while_instr = *(asmGeneration->p_stack_while);
      
  if (code_ptr->ou_loop_type != LIBBF_IF)
  {
    int offset_to_patch;
    int negative_jump_offset;
    
    if (asmGeneration->generate_debug)
      libbf_gen_asm_debug_code(asmGeneration, while_instr);
        
    libbf_gen_asm_ppc_test_while(asmGeneration, code_ptr);

    offset_to_patch = asmGeneration->filled_size;
    /* bne */
    libbf_write_asm_byte(asmGeneration, 0x40);
    libbf_write_asm_byte(asmGeneration, 0x82);
    libbf_write_asm_byte(asmGeneration, 0x00);
    libbf_write_asm_byte(asmGeneration, 0x00);
    
    negative_jump_offset = while_instr->ou_offset_to_patch + 4 - offset_to_patch;
    if (negative_jump_offset < -32768) 
    {
      /* beq */
      *(char*)(asmGeneration->compiled_code + offset_to_patch + 0) = 0x41;
      *(char*)(asmGeneration->compiled_code + offset_to_patch + 1) = 0x82;
      *(char*)(asmGeneration->compiled_code + offset_to_patch + 2) = 0x00;
      *(char*)(asmGeneration->compiled_code + offset_to_patch + 3) = 0x08;

      offset_to_patch = asmGeneration->filled_size;
      /* b */
      libbf_write_asm_byte(asmGeneration, 0x4b);
      libbf_write_asm_byte(asmGeneration, 0x00);
      libbf_write_asm_byte(asmGeneration, 0x00);
      libbf_write_asm_byte(asmGeneration, 0x00);
      
      negative_jump_offset = while_instr->ou_offset_to_patch + 4 - offset_to_patch;
      *(int*)(asmGeneration->compiled_code + offset_to_patch + 0) = int_to_be(negative_jump_offset);
      *(char*)(asmGeneration->compiled_code + offset_to_patch + 0) = 0x4b;
    }
    else
    {
      *(short*)(asmGeneration->compiled_code + offset_to_patch + 2) = short_to_be(negative_jump_offset);
    }

  }
      
  positive_jump_offset = asmGeneration->filled_size - while_instr->ou_offset_to_patch;
  if (positive_jump_offset < 32768)
  {
    asmGeneration->compiled_code[while_instr->ou_offset_to_patch - 4] = 0x41;
    asmGeneration->compiled_code[while_instr->ou_offset_to_patch - 3] = 0x82;
    *(short*)(&asmGeneration->compiled_code[while_instr->ou_offset_to_patch - 2]) = short_to_be(positive_jump_offset);
          
    memmove(asmGeneration->compiled_code + while_instr->ou_offset_to_patch,
            asmGeneration->compiled_code + while_instr->ou_offset_to_patch + 4,
            positive_jump_offset);
          
    asmGeneration->filled_size -= 4;
  }
  else
  {
    *(int*)(asmGeneration->compiled_code + while_instr->ou_offset_to_patch + 0) = int_to_be(positive_jump_offset);
    *(char*)(asmGeneration->compiled_code + while_instr->ou_offset_to_patch + 0) = 0x48;
  }

}
static void libbf_gen_asm_ppc(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  switch(code_ptr->ou_instr)
  {
    case BF_INC_DATA_PTR:
    {
      libbf_gen_asm_ppc_add_cst_to_reg(asmGeneration, code_ptr->ou_offset * asmGeneration->sizeofData, INDEX_REG);
      break;
    }
      
    case BF_PRINT_DATA:
    {
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R3_PPC);
      libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, IN_PARAM_REG, OFFSET_OUTPUT_FUNC, R13_PPC);
  
      libbf_gen_asm_ppc_mtlr_reg(asmGeneration, R13_PPC);
      
      libbf_gen_asm_ppc_blrl(asmGeneration);
      
      break;
    }
      
    case BF_READ_DATA:
    {
      libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, IN_PARAM_REG, OFFSET_INPUT_FUNC, R13_PPC);
  
      libbf_gen_asm_ppc_mtlr_reg(asmGeneration, R13_PPC);

      libbf_gen_asm_ppc_blrl(asmGeneration);
      
      libbf_gen_asm_ppc_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
      
      
    case BF_WHILE_DATA:  /* while (data_ptr[offset]) { */
    {
      libbf_gen_asm_ppc_while(asmGeneration, code_ptr);
      
      break;
    }
      
    case BF_END_WHILE:
    {
      libbf_gen_asm_ppc_end_while(asmGeneration, code_ptr);

      break;
    }
      
    case BF_SET_CST:
    {
      libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, R3_PPC);

      libbf_gen_asm_ppc_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
      
    case BF_INC_CST:
    {
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R4_PPC);
      
      if (code_ptr->ou_constant >= -32768 && code_ptr->ou_constant < 32768)
      {
        libbf_gen_asm_ppc_add_cst_to_reg(asmGeneration, code_ptr->ou_constant, R4_PPC);
      }
      else
      {
        libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, R3_PPC);
        
        libbf_gen_asm_ppc_add_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);
      }
      
      libbf_gen_asm_ppc_mov_reg_to_mem(asmGeneration, R4_PPC, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    case BF_SET_DATA:
    {
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, R3_PPC);
      
      libbf_gen_asm_ppc_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    case BF_INC_DATA:
    {
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, R3_PPC);
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R4_PPC);
      
      libbf_gen_asm_ppc_add_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);

      libbf_gen_asm_ppc_mov_reg_to_mem(asmGeneration, R4_PPC, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    case BF_DEC_DATA:
    {
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, R3_PPC);
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R4_PPC);
      
      libbf_gen_asm_ppc_subf_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);

      libbf_gen_asm_ppc_mov_reg_to_mem(asmGeneration, R4_PPC, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
       
    case BF_INC_CST_MUL_DATA:
    {
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, R3_PPC);
      
      libbf_gen_asm_ppc_mul_cst_to_reg(asmGeneration, code_ptr->ou_constant, R3_PPC);
            
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R4_PPC);
      
      libbf_gen_asm_ppc_add_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);

      libbf_gen_asm_ppc_mov_reg_to_mem(asmGeneration, R4_PPC, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    case BF_INC_DATA_MUL_DATA:
    {
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, R3_PPC);
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset3, R4_PPC);
      
      libbf_gen_asm_ppc_mullw(asmGeneration, R3_PPC, R4_PPC, R3_PPC); /* r3 = r3 * r4 */
      
      libbf_gen_asm_ppc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R4_PPC);
      
      libbf_gen_asm_ppc_add_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);

      libbf_gen_asm_ppc_mov_reg_to_mem(asmGeneration, R4_PPC, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    default:
    {
      libbf_gen_asm_default(asmGeneration, code_ptr);
      break;
    }
      
  }

}

static void libbf_gen_asm_ppc_longlong(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  int nWords = NWORDS(asmGeneration);
  
  switch(code_ptr->ou_instr)
  {
    case BF_INC_DATA_PTR:
    {
      libbf_gen_asm_ppc_add_cst_to_reg(asmGeneration, code_ptr->ou_offset * asmGeneration->sizeofData, INDEX_REG);
      break;
    }
      
    case BF_PRINT_DATA:
    {
      libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R3_PPC);
      libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, IN_PARAM_REG, OFFSET_OUTPUT_FUNC, R13_PPC);
  
      libbf_gen_asm_ppc_mtlr_reg(asmGeneration, R13_PPC);
      
      libbf_gen_asm_ppc_blrl(asmGeneration);
      
      break;
    }
    
      
    case BF_READ_DATA:
    {
      int i;
      libbf_gen_asm_ppc_mov_mem32_to_reg(asmGeneration, IN_PARAM_REG, OFFSET_INPUT_FUNC, R13_PPC);
  
      libbf_gen_asm_ppc_mtlr_reg(asmGeneration, R13_PPC);

      libbf_gen_asm_ppc_blrl(asmGeneration);
      
      libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset, 0);
      
      libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, 0, R3_PPC);
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }    
      
    case BF_SET_CST:
    {
      int i;
      libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, R3_PPC);
      libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset, 0);
      
      if (code_ptr->ou_constant >= 0)
        libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, 0, R3_PPC);
      else
        libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, -1, R3_PPC);
      
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }
        
      
    case BF_INC_CST:
    {
      int i;
        
      if (!(code_ptr->ou_constant >= -32768 && code_ptr->ou_constant < 32768))
      {
        NOT_IMPLEMENTED();
      }
      
      libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R3_PPC);
      
      libbf_gen_asm_ppc_addic(asmGeneration, code_ptr->ou_constant, R3_PPC);
      
      libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset, 0);
      
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, R3_PPC);
        
        if (code_ptr->ou_constant >= 0)
          libbf_gen_asm_ppc_addze(asmGeneration, R3_PPC);
        else
          libbf_gen_asm_ppc_addme(asmGeneration, R3_PPC);
        
        libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }    
    

    case BF_SET_DATA:
    {
      int i;
      for(i=0;i<nWords;i++)
      {
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, R3_PPC);
        
        libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }
        
      
    case BF_WHILE_DATA:  /* while (data_ptr[offset]) { */
    {
      libbf_gen_asm_ppc_while(asmGeneration, code_ptr);
      
      break;
    }
      
    case BF_END_WHILE:
    {
      libbf_gen_asm_ppc_end_while(asmGeneration, code_ptr);
      
      break;
    }
        
    case BF_INC_DATA:
    {
      int i;
        
      libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 0, R3_PPC);
      libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R4_PPC);
      
      libbf_gen_asm_ppc_addc_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);

      libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R4_PPC, INDEX_REG, code_ptr->ou_offset, 0);
      
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, R3_PPC);
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, R4_PPC);
      
        libbf_gen_asm_ppc_adde_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);

        libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R4_PPC, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }
    
    case BF_DEC_DATA:
    {
      int i;
        
      libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 0, R3_PPC);
      libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R4_PPC);
      
      libbf_gen_asm_ppc_subfc_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);

      libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R4_PPC, INDEX_REG, code_ptr->ou_offset, 0);
      
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, R3_PPC);
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, R4_PPC);
      
        libbf_gen_asm_ppc_subfe_reg_to_reg(asmGeneration, R3_PPC, R4_PPC);

        libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R4_PPC, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }
    
    case BF_INC_CST_MUL_DATA:
    {
      if (!(code_ptr->ou_constant >= -32768 && code_ptr->ou_constant < 32768))
      {
        NOT_IMPLEMENTED();
      }
      
      if (NWORDS(asmGeneration) > 2)
      {
        int i, j;
        
        for(j=0;j<nWords;j++)
        {
          if (j == 0)
            libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, R3_PPC);
          else if (code_ptr->ou_constant < 0)
          {
            if (j == 1)
              libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, -1, R3_PPC);
          }
          else
          {
            break;
          }
            
          for(i=0;i<nWords-j;i++)
          {
            libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, R4_PPC);

            if (i != 0)
            {
              libbf_gen_asm_ppc_mov_reg_to_reg(asmGeneration, R7_PPC, R8_PPC);
            }
                
            libbf_gen_asm_ppc_mullw(asmGeneration, R3_PPC, R4_PPC, R6_PPC); /* r6 = r3 * r4 */
            
            if (i+j < nWords-1)
            {
              libbf_gen_asm_ppc_mullhwu(asmGeneration, R3_PPC, R4_PPC, R7_PPC); /* r7 = r3 * r4 (high) */
            }
          
            if (i != 0)
            {
              if (i+j < nWords-1)
              {
                libbf_gen_asm_ppc_addc_reg_to_reg(asmGeneration, R8_PPC, R6_PPC);
                libbf_gen_asm_ppc_addze(asmGeneration, R7_PPC);
              }
              else
              {
                libbf_gen_asm_ppc_add_reg_to_reg(asmGeneration, R8_PPC, R6_PPC);
              }
            }
            
            libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j, R8_PPC);
            if (i+j < nWords-1)
            {
              libbf_gen_asm_ppc_addc_reg_to_reg(asmGeneration, R6_PPC, R8_PPC);
              libbf_gen_asm_ppc_addze(asmGeneration, R7_PPC);
            }
            else
            {
              libbf_gen_asm_ppc_add_reg_to_reg(asmGeneration, R6_PPC, R8_PPC);
            }
            libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R8_PPC, INDEX_REG, code_ptr->ou_offset, i+j);
          }
        }
      }
      else
      {
        int constant_pos = (code_ptr->ou_constant >= 0) ? code_ptr->ou_constant : -code_ptr->ou_constant;
        
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 0, R3_PPC);
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 1, R4_PPC);
        libbf_gen_asm_ppc_mov_cst_to_reg(asmGeneration, constant_pos, R5_PPC);
        
        libbf_gen_asm_ppc_mullw(asmGeneration, R3_PPC, R5_PPC, R6_PPC); /* r6 = r3 * r5 */
        libbf_gen_asm_ppc_mullhwu(asmGeneration, R3_PPC, R5_PPC, R7_PPC); /* r7 = r3 * r5 (high) */
        
        libbf_gen_asm_ppc_mul_cst_to_reg(asmGeneration, constant_pos, R4_PPC);
        
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R8_PPC);
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 1, R9_PPC);
        
        libbf_gen_asm_ppc_add_reg_to_reg(asmGeneration, R7_PPC, R4_PPC);
        
        if (code_ptr->ou_constant >= 0)
        {
          libbf_gen_asm_ppc_addc_reg_to_reg(asmGeneration, R6_PPC, R8_PPC);
          libbf_gen_asm_ppc_adde_reg_to_reg(asmGeneration, R4_PPC, R9_PPC);
        }
        else
        {
          libbf_gen_asm_ppc_subfc_reg_to_reg(asmGeneration, R6_PPC, R8_PPC);
          libbf_gen_asm_ppc_subfe_reg_to_reg(asmGeneration, R4_PPC, R9_PPC);
        }
        
        libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R8_PPC, INDEX_REG, code_ptr->ou_offset, 0);
        libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R9_PPC, INDEX_REG, code_ptr->ou_offset, 1);
      }
      
      break;
    }
    
    case BF_INC_DATA_MUL_DATA:
    {
      if (NWORDS(asmGeneration) != 2)
      {
        int i, j;
        for(j=0;j<nWords;j++)
        {
          libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset3, j, R4_PPC);
          
          for(i=0;i<nWords - j;i++)
          {
            int k;
              
            libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, R3_PPC);
            
            libbf_gen_asm_ppc_mullw(asmGeneration, R3_PPC, R4_PPC, R6_PPC); /* r6 = r3 * r4 */
            
            if (i+j != nWords - 1)
            {
              libbf_gen_asm_ppc_mullhwu(asmGeneration, R3_PPC, R4_PPC, R7_PPC); /* r7 = r3 * r4 (high) */
              
              libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j, R8_PPC);
              libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j+1, R9_PPC);
              
              libbf_gen_asm_ppc_addc_reg_to_reg(asmGeneration, R6_PPC, R8_PPC);
              libbf_gen_asm_ppc_adde_reg_to_reg(asmGeneration, R7_PPC, R9_PPC);
            
              libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R8_PPC, INDEX_REG, code_ptr->ou_offset, i+j);
              libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R9_PPC, INDEX_REG, code_ptr->ou_offset, i+j+1);
            }
            else
            {
              libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j, R8_PPC);
              libbf_gen_asm_ppc_add_reg_to_reg(asmGeneration, R6_PPC, R8_PPC);
              libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R8_PPC, INDEX_REG, code_ptr->ou_offset, i+j);
            }
            
            for(k=i+j+2;k<nWords;k++)
            {
              libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, k, R8_PPC);
              libbf_gen_asm_ppc_addze(asmGeneration, R8_PPC);
              libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R8_PPC, INDEX_REG, code_ptr->ou_offset, k);
            }
          }
        }
      }
      else
      {
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 0, R3_PPC);
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 1, R4_PPC);
        
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset3, 0, R5_PPC);
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset3, 1, R6_PPC);
        
        libbf_gen_asm_ppc_mullw(asmGeneration, R3_PPC, R5_PPC, R7_PPC); /* r7 = r3 * r5 */
        libbf_gen_asm_ppc_mullhwu(asmGeneration, R3_PPC, R5_PPC, R8_PPC); /* r8 = r3 * r5 (high) */
        libbf_gen_asm_ppc_mullw(asmGeneration, R3_PPC, R6_PPC, R9_PPC); /* r9 = r3 * r6 */
        libbf_gen_asm_ppc_mullw(asmGeneration, R4_PPC, R5_PPC, R10_PPC); /* r10 = r4 * r5 */
        
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R3_PPC);
        libbf_gen_asm_ppc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 1, R4_PPC);
        
        libbf_gen_asm_ppc_addc_reg_to_reg(asmGeneration, R7_PPC, R3_PPC);
        libbf_gen_asm_ppc_adde_reg_to_reg(asmGeneration, R8_PPC, R4_PPC);
        libbf_gen_asm_ppc_add_reg_to_reg(asmGeneration, R9_PPC, R4_PPC);
        libbf_gen_asm_ppc_add_reg_to_reg(asmGeneration, R10_PPC, R4_PPC);
        
        libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R3_PPC, INDEX_REG, code_ptr->ou_offset, 0);
        libbf_gen_asm_ppc_longlong_mov_reg_to_mem(asmGeneration, R4_PPC, INDEX_REG, code_ptr->ou_offset, 1);
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

#if defined(__powerpc__)
int libbf_is_compatible_compiled_code  (char* compiled_code,
                                        TargetCpu_t* cpu,
                                        int* sizeofData,
                                        int* min_delta_offset,
                                        int* max_delta_offset)
{
  if (!(compiled_code[0] == (char)0x48 &&
        compiled_code[1] == (char)0x00 &&
        compiled_code[2] == (char)0x00 &&
        compiled_code[3] == (char)(4 + 5 * 4)))
  {
    warning("Non ppc libbf-compiled code\n");
    return FALSE;
  }

  *cpu = (*(int*)(compiled_code + 4 + 4));
  *sizeofData = (*(int*)(compiled_code + 4 + 8));
  *min_delta_offset = (*(int*)(compiled_code + 4 + 12));
  *max_delta_offset = (*(int*)(compiled_code + 4 + 16));
  
  return TRUE;
}
#endif

static void libbf_gen_asm_ppc_init_funcs()
{
  asmGenerationFuncs[TARGET_PPC].gen_preambule = libbf_gen_asm_ppc_preambule;
  asmGenerationFuncs[TARGET_PPC].gen_epilogue = libbf_gen_asm_ppc_epilogue;
  asmGenerationFuncs[TARGET_PPC].gen_code = libbf_gen_asm_ppc;
  asmGenerationFuncs[TARGET_PPC].gen_code_longlong = libbf_gen_asm_ppc_longlong;
  asmGenerationFuncs[TARGET_PPC].gen_debug_code = libbf_gen_asm_ppc_debug_code;
}

#undef IN_PARAM_REG
#undef INDEX_REG
