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
  G0_SPARC, /* global registers */
  G1_SPARC,
  G2_SPARC,
  G3_SPARC,
  G4_SPARC,
  G5_SPARC,
  G6_SPARC,
  G7_SPARC,
  
  O0_SPARC, /* output registers */
  O1_SPARC,
  O2_SPARC,
  O3_SPARC,
  O4_SPARC,
  O5_SPARC,
  O6_SPARC,
  O7_SPARC,
  
  L0_SPARC, /* local registers */
  L1_SPARC,
  L2_SPARC,
  L3_SPARC,
  L4_SPARC,
  L5_SPARC,
  L6_SPARC,
  L7_SPARC,
  
  I0_SPARC, /* input registers */
  I1_SPARC,
  I2_SPARC,
  I3_SPARC,
  I4_SPARC,
  I5_SPARC,
  I6_SPARC,
  I7_SPARC,
} RegSparc_t;

#define INDEX_REG O5_SPARC

#define INT_TO_TGT int_to_be

/* Enables the use of the mul op (from SPARC V8) */
#define USE_UMUL_SPARC

static void libbf_gen_asm_sparc_ops(AsmGeneration_t* asmGeneration, int code32)
{
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(code32));
}

static void libbf_gen_asm_sparc_mov_3op(AsmGeneration_t* asmGeneration,
                                        unsigned char firstSeed, RegSparc_t regDst,
                                        unsigned char secondSeed, RegSparc_t reg,
                                        unsigned char thirdSeed, int regOrCst)
{
  ASSERT(regDst >= 0 && regDst <= I7_SPARC);
  ASSERT(reg >= 0 && reg <= I7_SPARC);
  ASSERT(secondSeed < 32); /* 5 bits */
  ASSERT(thirdSeed < 4); /* 2 bits */
  if (regOrCst < -4096 || regOrCst > 4095)
  {
    NOT_IMPLEMENTED();
  }
  if (regOrCst < 0)
  {
    thirdSeed += 0x1;
    regOrCst = 4096+regOrCst;
  }
  libbf_gen_asm_sparc_ops(asmGeneration, (firstSeed << 24) +
                                         (regDst << 25) +
                                         (secondSeed << 19) +
                                         (reg << 14) +
                                         (thirdSeed << 12) +
                                         regOrCst);
}

static void libbf_gen_asm_sparc_mov_mem32_to_reg(AsmGeneration_t* asmGeneration, RegSparc_t regPtr, int offset, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0xc0, regDst, 0x0, regPtr, 0x2, offset);
}

/*
static void libbf_gen_asm_sparc_mov_mem64_to_regs(AsmGeneration_t* asmGeneration, RegSparc_t regPtr, int offset, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0xc0, regDst, 0x3, regPtr, 0x2, offset);
}
*/

static void libbf_gen_asm_sparc_mov_mem_to_reg(AsmGeneration_t* asmGeneration, RegSparc_t regPtr, int offset, RegSparc_t regDst)
{
  offset = offset * asmGeneration->sizeofData;
  if (asmGeneration->sizeofData == 4)
    libbf_gen_asm_sparc_mov_mem32_to_reg(asmGeneration, regPtr, offset, regDst);
  else if (asmGeneration->sizeofData == 2)
    libbf_gen_asm_sparc_mov_3op(asmGeneration, 0xc0, regDst, 0x2, regPtr, 0x2, offset);
  else if (asmGeneration->sizeofData == 1)
    libbf_gen_asm_sparc_mov_3op(asmGeneration, 0xc0, regDst, 0x1, regPtr, 0x2, offset);
}

static void libbf_gen_asm_sparc_longlong_mov_mem_to_reg(AsmGeneration_t* asmGeneration, RegSparc_t regPtr, int offset, int suboffset, RegSparc_t regDst)
{
  offset = libbf_compiler_compute_longlong_offset(asmGeneration, offset, suboffset);
  libbf_gen_asm_sparc_mov_mem32_to_reg(asmGeneration, regPtr, offset, regDst);
}

/*
static void libbf_gen_asm_sparc_mov_regs_to_mem64(AsmGeneration_t* asmGeneration, RegSparc_t regSrc, RegSparc_t regPtr, int offset)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0xc0, regSrc, 0x7, regPtr, 0x2, offset);
}
*/

static void libbf_gen_asm_sparc_mov_reg_to_mem(AsmGeneration_t* asmGeneration, RegSparc_t regSrc, RegSparc_t regPtr, int offset)
{
  offset = offset * asmGeneration->sizeofData;
  if (asmGeneration->sizeofData == 4)
    libbf_gen_asm_sparc_mov_3op(asmGeneration, 0xc0, regSrc, 0x4, regPtr, 0x2, offset);
  else if (asmGeneration->sizeofData == 2)
    libbf_gen_asm_sparc_mov_3op(asmGeneration, 0xc0, regSrc, 0x6, regPtr, 0x2, offset);
  else if (asmGeneration->sizeofData == 1)
    libbf_gen_asm_sparc_mov_3op(asmGeneration, 0xc0, regSrc, 0x5, regPtr, 0x2, offset);
}

static void libbf_gen_asm_sparc_longlong_mov_reg_to_mem(AsmGeneration_t* asmGeneration, RegSparc_t regSrc, RegSparc_t regPtr, int offset, int suboffset)
{
  offset = libbf_compiler_compute_longlong_offset(asmGeneration, offset, suboffset);
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0xc0, regSrc, 0x4, regPtr, 0x2, offset);
}

static void libbf_gen_asm_sparc_sethi_to_reg(AsmGeneration_t* asmGeneration, unsigned int constant, RegSparc_t regDst)
{
  if ((constant >> 22) != 0)
  {
    SHOULDNT_HAPPEN();
  }
  libbf_gen_asm_sparc_ops(asmGeneration, ((0x01 + 2 * regDst) << 24) +
                                         (((constant >> 16) & 0xFF) << 16) +
                                         (((constant >> 8) & 0xFF) << 8) +
                                         ((constant) & 0xFF));
}

static void libbf_gen_asm_sparc_or_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegSparc_t regSrc, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x2, regSrc, 0x2, constant);
}

static void libbf_gen_asm_sparc_add_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegSparc_t regSrc, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x0, regSrc, 0x2, constant);
}

/*
static void libbf_gen_asm_sparc_addcc_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegSparc_t regSrc, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x10, regSrc, 0x2, constant);
}

static void libbf_gen_asm_sparc_addx_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegSparc_t regSrc, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x8, regSrc, 0x2, constant);
}

static void libbf_gen_asm_sparc_addxcc_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegSparc_t regSrc, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x18, regSrc, 0x2, constant);
}
*/

static void libbf_gen_asm_sparc_mov_cst_to_reg(AsmGeneration_t* asmGeneration, int constant, RegSparc_t regDst)
{
  if (constant < -0xFFF || constant > 0xFFF)
  {
    unsigned int uconstant = constant;
    libbf_gen_asm_sparc_sethi_to_reg(asmGeneration, uconstant >> 10, regDst);
    libbf_gen_asm_sparc_or_cst_to_reg(asmGeneration, uconstant & 1023, regDst, regDst);
  }
  else
  {
    libbf_gen_asm_sparc_or_cst_to_reg(asmGeneration, constant, G0_SPARC, regDst);
  }
}


static void libbf_gen_asm_sparc_sll_cst_to_reg(AsmGeneration_t* asmGeneration, int shift, RegSparc_t regSrc, RegSparc_t regDst)
{
  if (shift < 0 || shift > 31)
  {
    SHOULDNT_HAPPEN();
  }
  if (shift >= 32)
    libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, 0, regDst);
  else
    libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x81, regDst, 0x5, regSrc, 0x2, shift);
}

static void libbf_gen_asm_sparc_srl_cst_to_reg(AsmGeneration_t* asmGeneration, int shift, RegSparc_t regSrc, RegSparc_t regDst)
{
  if (shift < 0)
  {
    SHOULDNT_HAPPEN();
  }
  if (shift >= 32)
    libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, 0, regDst);
  else
    libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x81, regDst, 0x6, regSrc, 0x2, shift);
}

#if !defined(USE_UMUL_SPARC)
static void libbf_gen_asm_sparc_sll_reg(AsmGeneration_t* asmGeneration, RegSparc_t regShift, RegSparc_t regSrc, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x81, regDst, 0x5, regSrc, 0x0, regShift);
}

static void libbf_gen_asm_sparc_srl_reg(AsmGeneration_t* asmGeneration, RegSparc_t regShift, RegSparc_t regSrc, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x81, regDst, 0x6, regSrc, 0x0, regShift);
}

static void libbf_gen_asm_sparc_btst(AsmGeneration_t* asmGeneration, int constant, RegSparc_t reg)
{
  if (!(constant >= 0 && constant < 0xFFF))
  {
    NOT_IMPLEMENTED();
  }
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, 0x00, 0x11, reg, 0x2, constant);
}
#else
static void libbf_gen_asm_sparc_umul_reg_32(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDstLow)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDstLow, 0xA, regSrc1, 0x0, regSrc2); /* umul regSrc1, regSrc2, regDstLow */
}
static void libbf_gen_asm_sparc_umul_reg_64(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDstLow, RegSparc_t regDstHigh)
{
  libbf_gen_asm_sparc_umul_reg_32(asmGeneration, regSrc1, regSrc2, regDstLow);

  if (regDstHigh >= 0)
    libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDstHigh, 0x8, (RegSparc_t)0, 0x0, 0); /* sr %y, regDstHigh */
}
#endif

static void libbf_gen_asm_sparc_or_reg(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x2, regSrc1, 0x0, regSrc2);
}

static void libbf_gen_asm_sparc_add_reg(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x0, regSrc1, 0x0, regSrc2);
}

static void libbf_gen_asm_sparc_addcc_reg(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x0 + 0x10, regSrc1, 0x0, regSrc2);
}

static void libbf_gen_asm_sparc_addx_reg(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x0 + 0x8, regSrc1, 0x0, regSrc2);
}

static void libbf_gen_asm_sparc_addxcc_reg(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x0 + 0x8 + 0x10, regSrc1, 0x0, regSrc2);
}

static void libbf_gen_asm_sparc_longlong_addX_reg(AsmGeneration_t* asmGeneration, int i, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  if (i == 0)
    libbf_gen_asm_sparc_addcc_reg(asmGeneration, regSrc1, regSrc2, regDst);
  else if (i == NWORDS(asmGeneration) - 1)
    libbf_gen_asm_sparc_addx_reg(asmGeneration, regSrc1, regSrc2, regDst);
  else
    libbf_gen_asm_sparc_addxcc_reg(asmGeneration, regSrc1, regSrc2, regDst);
}



/* regDst = regSrc1 - regSrc2 */
static void libbf_gen_asm_sparc_sub_reg(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x4, regSrc1, 0x0, regSrc2);
}

static void libbf_gen_asm_sparc_subcc_reg(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x4 + 0x10, regSrc1, 0x0, regSrc2);
}

static void libbf_gen_asm_sparc_subx_reg(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x4 + 0x8, regSrc1, 0x0, regSrc2);
}

static void libbf_gen_asm_sparc_subxcc_reg(AsmGeneration_t* asmGeneration, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, regDst, 0x4 + 0x8 + 0x10, regSrc1, 0x0, regSrc2);
}

static void libbf_gen_asm_sparc_longlong_subX_reg(AsmGeneration_t* asmGeneration, int i, RegSparc_t regSrc1, RegSparc_t regSrc2, RegSparc_t regDst)
{
  if (i == 0)
    libbf_gen_asm_sparc_subcc_reg(asmGeneration, regSrc1, regSrc2, regDst);
  else if (i == NWORDS(asmGeneration) - 1)
    libbf_gen_asm_sparc_subx_reg(asmGeneration, regSrc1, regSrc2, regDst);
  else
    libbf_gen_asm_sparc_subxcc_reg(asmGeneration, regSrc1, regSrc2, regDst);
}

static void libbf_gen_asm_sparc_mov_reg_to_reg(AsmGeneration_t* asmGeneration, RegSparc_t regSrc, RegSparc_t regDst)
{
  libbf_gen_asm_sparc_or_reg(asmGeneration, G0_SPARC, regSrc, regDst);
}

static void libbf_gen_asm_sparc_cmp_reg_to_0(AsmGeneration_t* asmGeneration, RegSparc_t reg)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x80, (RegSparc_t)0x00, 0x14, reg, 0x2, 0x00);
}

static void libbf_gen_asm_sparc_nop(AsmGeneration_t* asmGeneration)
{
  libbf_gen_asm_sparc_ops(asmGeneration, 0x01000000);
}

static void libbf_gen_asm_sparc_call_at_reg(AsmGeneration_t* asmGeneration, RegSparc_t reg)
{
  libbf_gen_asm_sparc_mov_3op(asmGeneration, 0x9f, (RegSparc_t)0x00, 0x18, reg, 0x0, 0x00);
}

    
static void libbf_gen_asm_sparc_be(AsmGeneration_t* asmGeneration, int jumpCount)
{
  if (jumpCount > 0x7FFFF || jumpCount < -0x80000)
  {
    NOT_IMPLEMENTED();
  }
  libbf_gen_asm_sparc_ops(asmGeneration, 0x02800000 + (jumpCount & 0xFFFFF)); /* be */
}

#if !defined(USE_UMUL_SPARC)
static void libbf_gen_asm_sparc_bne(AsmGeneration_t* asmGeneration, int jumpCount)
{
  if (jumpCount > 0x7FFFF || jumpCount < -0x80000)
  {
    NOT_IMPLEMENTED();
  }
  libbf_gen_asm_sparc_ops(asmGeneration, 0x12b00000 + (jumpCount & 0xFFFFF)); /* be */
}
#endif

static void libbf_gen_asm_sparc_bnea(AsmGeneration_t* asmGeneration, int jumpCount)
{
  if (jumpCount > 0x7FFFF || jumpCount < -0x80000)
  {
    NOT_IMPLEMENTED();
  }
  libbf_gen_asm_sparc_ops(asmGeneration, 0x32b00000 + (jumpCount & 0xFFFFF)); /* be */
}

static void libbf_gen_asm_sparc_preambule(AsmGeneration_t* asmGeneration)
{
  libbf_gen_asm_sparc_ops(asmGeneration, 0x10800000 | (5+2)); /* b */
  
  libbf_gen_asm_sparc_nop(asmGeneration);

  libbf_write_asm_int(asmGeneration, INT_TO_TGT(0)); /* CODE SIZE : TO BE POST-FIXED */
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration->cpu));
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration->sizeofData));
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration->min_delta_offset));
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration->max_delta_offset));
    
    
  /* 9d e3 bf 98     save  %sp, -104, %sp */
  libbf_gen_asm_sparc_ops(asmGeneration, 0x9de3bf98);
  
  libbf_gen_asm_sparc_mov_mem32_to_reg(asmGeneration, I0_SPARC, OFFSET_BASE_DATA_PTR, INDEX_REG);
  libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, I0_SPARC, O4_SPARC);
  
  libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, -1, L6_SPARC);
  libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, 1, L7_SPARC);
}

static void libbf_gen_asm_sparc_epilogue(AsmGeneration_t* asmGeneration)
{
  /* 81 c7 e0 08     ret */
  libbf_gen_asm_sparc_ops(asmGeneration, 0x81c7e008);
  
  /* 81 e8 00 00     restore */
  libbf_gen_asm_sparc_ops(asmGeneration, 0x81e80000);
  
  *(int*)&asmGeneration->compiled_code[8] = INT_TO_TGT(asmGeneration->filled_size);
}

static void libbf_gen_asm_sparc_debug_code(AsmGeneration_t* asmGeneration, OptimizationUnit* current_code_ptr)
{
#if defined(__sparc__)
  libbf_gen_asm_sparc_mov_mem32_to_reg(asmGeneration, I0_SPARC, OFFSET_DEBUG_FUNC, L1_SPARC);

  libbf_gen_asm_sparc_mov_mem32_to_reg(asmGeneration, I0_SPARC, OFFSET_DEBUG_DATA, O0_SPARC);
    
  libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, (int)current_code_ptr, O1_SPARC);
    
  libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, INDEX_REG, O2_SPARC);
      
  libbf_gen_asm_sparc_call_at_reg(asmGeneration, L1_SPARC);
  libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, O5_SPARC, L5_SPARC); /* done before the call ! */
    
  libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, I0_SPARC, O4_SPARC);
  libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, L5_SPARC, O5_SPARC);
#endif
}

static void libbf_gen_asm_sparc(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  switch(code_ptr->ou_instr)
  {
    case BF_INC_DATA_PTR:
    {
      libbf_gen_asm_sparc_add_cst_to_reg(asmGeneration, code_ptr->ou_offset * asmGeneration->sizeofData, INDEX_REG, INDEX_REG);
      break;
    }
    
    case BF_PRINT_DATA:
    {
      libbf_gen_asm_sparc_mov_mem32_to_reg(asmGeneration, I0_SPARC, OFFSET_OUTPUT_FUNC, L1_SPARC);
      
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, O0_SPARC);

      libbf_gen_asm_sparc_call_at_reg(asmGeneration, L1_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, O5_SPARC, L5_SPARC); /* done before the call ! */
      
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, I0_SPARC, O4_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, L5_SPARC, O5_SPARC);
        
      break;
    }
    
    case BF_READ_DATA:
    {
      libbf_gen_asm_sparc_mov_mem32_to_reg(asmGeneration, I0_SPARC, OFFSET_INPUT_FUNC, L1_SPARC);
  
      libbf_gen_asm_sparc_call_at_reg(asmGeneration, L1_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, O5_SPARC, L5_SPARC);  /* done before the call ! */
      
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, I0_SPARC, O4_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, L5_SPARC, O5_SPARC);

      libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, O0_SPARC, INDEX_REG, code_ptr->ou_offset);
        
      break;
    }
    
    case BF_SET_CST:
    {
      if (code_ptr->ou_constant == 0)
      {
        libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, G0_SPARC, INDEX_REG, code_ptr->ou_offset);
      }
      else if (code_ptr->ou_constant == 1)
      {
        libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, L7_SPARC, INDEX_REG, code_ptr->ou_offset);
      }
      else if (code_ptr->ou_constant == -1)
      {
        libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, L6_SPARC, INDEX_REG, code_ptr->ou_offset);
      }
      else
      {
        libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, L1_SPARC);
        libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset);
      }
      break;
    }
    
    case BF_INC_CST:
    {
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, L1_SPARC);
      libbf_gen_asm_sparc_add_cst_to_reg(asmGeneration, code_ptr->ou_constant, L1_SPARC, L1_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset);
      break;
    }
    
    case BF_SET_DATA:
    {
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, L1_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
        
    case BF_INC_DATA:
    {
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, L1_SPARC);
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, L2_SPARC);
      libbf_gen_asm_sparc_add_reg(asmGeneration, L1_SPARC, L2_SPARC, L1_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset);

      break;
    }
    
    case BF_DEC_DATA:
    {
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, L1_SPARC);
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, L2_SPARC);
      libbf_gen_asm_sparc_sub_reg(asmGeneration, L1_SPARC, L2_SPARC, L1_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset);

      break;
    }
    
    case BF_WHILE_DATA:  /* while (data_ptr[offset]) { */
    {
      if (asmGeneration->generate_debug)
        libbf_gen_asm_debug_code(asmGeneration, code_ptr);
      
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, L1_SPARC);
      libbf_gen_asm_sparc_cmp_reg_to_0(asmGeneration, L1_SPARC);
      
      code_ptr->ou_offset_to_patch = asmGeneration->filled_size;
      
      libbf_gen_asm_sparc_nop(asmGeneration); /* be to be patched */
      libbf_gen_asm_sparc_nop(asmGeneration); /* delay slot */

      *(asmGeneration->p_stack_while) = code_ptr;
      asmGeneration->p_stack_while++;
        
      break;
    }
      
    case BF_END_WHILE:
    {
      OptimizationUnit* while_instr;
      asmGeneration->p_stack_while--;
      while_instr = *(asmGeneration->p_stack_while);
      
      if (code_ptr->ou_loop_type != LIBBF_IF)
      {
        if (asmGeneration->generate_debug)
          libbf_gen_asm_debug_code(asmGeneration, while_instr);
        
        libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, L1_SPARC);
        libbf_gen_asm_sparc_cmp_reg_to_0(asmGeneration, L1_SPARC);
        
        /* 32b00000        bne,a    ... */
        libbf_gen_asm_sparc_bnea(asmGeneration, (while_instr->ou_offset_to_patch - asmGeneration->filled_size) / 4 - 1);
        
        libbf_gen_asm_sparc_nop(asmGeneration); /* delay slot */
      }
      
      /* patch be */
      *(int*)(asmGeneration->compiled_code + while_instr->ou_offset_to_patch) = INT_TO_TGT(0x02800000 | (((asmGeneration->filled_size - while_instr->ou_offset_to_patch) / 4) ));
      
      break;
    }
    
    case BF_WHILE_INC_DATA_PTR: /* while(data_ptr[offset]) data_ptr += offset2; */
    {
      int offset2 = code_ptr->ou_offset2 * asmGeneration->sizeofData;
      if (offset2 < -0xFFF || offset2 > 0xFFF)
      {
        libbf_gen_asm_default(asmGeneration, code_ptr);
      }
      else
      {
        if (asmGeneration->generate_debug)
          libbf_gen_asm_debug_code(asmGeneration, code_ptr);
        
        libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, L1_SPARC);
        libbf_gen_asm_sparc_cmp_reg_to_0(asmGeneration, L1_SPARC);
        
        libbf_gen_asm_sparc_be(asmGeneration, 7);
        libbf_gen_asm_sparc_nop(asmGeneration);
        
        libbf_gen_asm_sparc_add_cst_to_reg(asmGeneration, offset2, INDEX_REG, INDEX_REG);
        
        libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, L1_SPARC);
        libbf_gen_asm_sparc_cmp_reg_to_0(asmGeneration, L1_SPARC);
        
        libbf_gen_asm_sparc_bnea(asmGeneration, -2);
        libbf_gen_asm_sparc_add_cst_to_reg(asmGeneration, offset2, INDEX_REG, INDEX_REG);
      }
      
      break;
    }
    
    case BF_INC_CST_MUL_DATA:
    {
      int constant_pos = (code_ptr->ou_constant > 0) ? code_ptr->ou_constant : -code_ptr->ou_constant;
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, L1_SPARC);
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, L2_SPARC);
#if defined(USE_UMUL_SPARC)
      libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, constant_pos, G1_SPARC);
      libbf_gen_asm_sparc_umul_reg_32(asmGeneration, L1_SPARC, G1_SPARC, L3_SPARC);
#else
{
      int i = 0;
      int firstTime = TRUE;
      while(constant_pos)
      {
        if (constant_pos & 1)
        {
          if (firstTime)
          {
            libbf_gen_asm_sparc_sll_cst_to_reg(asmGeneration, i, L1_SPARC, L3_SPARC);
            firstTime = FALSE;
          }
          else
          {
            libbf_gen_asm_sparc_sll_cst_to_reg(asmGeneration, i, L1_SPARC, L4_SPARC);
            libbf_gen_asm_sparc_add_reg(asmGeneration, L3_SPARC, L4_SPARC, L3_SPARC);
          }
        }
        constant_pos >>= 1;
        i++;
      }
}
#endif
      if (code_ptr->ou_constant > 0)
        libbf_gen_asm_sparc_add_reg(asmGeneration, L2_SPARC, L3_SPARC, L1_SPARC);
      else
        libbf_gen_asm_sparc_sub_reg(asmGeneration, L2_SPARC, L3_SPARC, L1_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    case BF_INC_DATA_MUL_DATA:
    {
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, L1_SPARC);
      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset3, L2_SPARC);
      
#if defined(USE_UMUL_SPARC)
      libbf_gen_asm_sparc_umul_reg_32(asmGeneration, L1_SPARC, L2_SPARC, G2_SPARC);
#else
      libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, 0, G1_SPARC); /* shift */
      libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, 0, G2_SPARC); /* acc */
      
      libbf_gen_asm_sparc_btst(asmGeneration, 1, L1_SPARC); /* if (l1 & 1) */
      libbf_gen_asm_sparc_be(asmGeneration, 4); /* be */
      libbf_gen_asm_sparc_srl_cst_to_reg(asmGeneration, 1, L1_SPARC, L1_SPARC); /* l1 >>= 1 */  /* done before the jump ! */
      
        libbf_gen_asm_sparc_sll_reg(asmGeneration, G1_SPARC, L2_SPARC, G3_SPARC); /* g3 = l2 << shift */
        libbf_gen_asm_sparc_add_reg(asmGeneration, G3_SPARC, G2_SPARC, G2_SPARC);   /* acc += g3 */
        
        libbf_gen_asm_sparc_cmp_reg_to_0(asmGeneration, L1_SPARC); /* if (l1 != 0) */
        libbf_gen_asm_sparc_bne(asmGeneration, -6); /* bne */
        libbf_gen_asm_sparc_add_cst_to_reg(asmGeneration, 1, G1_SPARC, G1_SPARC);   /* shift++ */ /* done before the jump ! */
#endif

      libbf_gen_asm_sparc_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, L1_SPARC);
      libbf_gen_asm_sparc_add_reg(asmGeneration, L1_SPARC, G2_SPARC, L1_SPARC);  
      libbf_gen_asm_sparc_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset);
        
      break;
    }
    
    default:
    {
      libbf_gen_asm_default(asmGeneration, code_ptr);
      break;
    }
  }
}

static void libbf_gen_asm_sparc_longlong(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  int nWords = NWORDS(asmGeneration);
  
  switch(code_ptr->ou_instr)
  {
    case BF_INC_DATA_PTR:
    {
      libbf_gen_asm_sparc_add_cst_to_reg(asmGeneration, code_ptr->ou_offset * asmGeneration->sizeofData, INDEX_REG, INDEX_REG);
      break;
    }
    
    case BF_PRINT_DATA:
    {
      libbf_gen_asm_sparc_mov_mem32_to_reg(asmGeneration, I0_SPARC, OFFSET_OUTPUT_FUNC, L1_SPARC);
      
      libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, O0_SPARC);

      libbf_gen_asm_sparc_call_at_reg(asmGeneration, L1_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, O5_SPARC, L5_SPARC); /* done before the call ! */
      
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, I0_SPARC, O4_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, L5_SPARC, O5_SPARC);
        
      break;
    }
    
    case BF_READ_DATA:
    {
      int i;
      libbf_gen_asm_sparc_mov_mem32_to_reg(asmGeneration, I0_SPARC, OFFSET_INPUT_FUNC, L1_SPARC);
  
      libbf_gen_asm_sparc_call_at_reg(asmGeneration, L1_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, O5_SPARC, L5_SPARC);  /* done before the call ! */
      
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, I0_SPARC, O4_SPARC);
      libbf_gen_asm_sparc_mov_reg_to_reg(asmGeneration, L5_SPARC, O5_SPARC);

      libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, O0_SPARC, INDEX_REG, code_ptr->ou_offset, 0);
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, G0_SPARC, INDEX_REG, code_ptr->ou_offset, i);
      }
        
      break;
    }
    
    case BF_SET_CST:
    {
      int i;
      libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, L1_SPARC);
      libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset, 0);
      if (code_ptr->ou_constant >= 0)
      {
        for(i=1;i<nWords;i++)
        {
          libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, G0_SPARC, INDEX_REG, code_ptr->ou_offset, i);
        }
      }
      else
      {
        libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, -1, L1_SPARC);
        for(i=1;i<nWords;i++)
        {
          libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset, i);
        }
      }
      break;
    }
    
    case BF_INC_CST:
    {
      int i;
        
      libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, L1_SPARC);
      
      libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, L2_SPARC);
      
      libbf_gen_asm_sparc_longlong_addX_reg(asmGeneration, 0, L1_SPARC, L2_SPARC, L1_SPARC);

      libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset, 0);
      
      if (code_ptr->ou_constant >= 0)
        libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, 0, L2_SPARC);
      else
        libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, -1, L2_SPARC);
      
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, L1_SPARC);
        libbf_gen_asm_sparc_longlong_addX_reg(asmGeneration, i, L1_SPARC, L2_SPARC, L1_SPARC);
        libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset, i);
      }      
      break;
    }
    
    case BF_SET_DATA:
    {
      int i;
      for(i=0;i<nWords;i++)
      {
        libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, L1_SPARC);
        libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }
        
    case BF_INC_DATA:
    {
      int i;
      for(i=0;i<nWords;i++)
      {
        libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, L1_SPARC);
        libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, L2_SPARC);
        libbf_gen_asm_sparc_longlong_addX_reg(asmGeneration, i, L1_SPARC, L2_SPARC, L1_SPARC);
        libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset, i);
      }
      break;
    }
    
    case BF_DEC_DATA:
    {
      int i;
      for(i=0;i<nWords;i++)
      {
        libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, L1_SPARC);
        libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, L2_SPARC);
        libbf_gen_asm_sparc_longlong_subX_reg(asmGeneration, i, L1_SPARC, L2_SPARC, L1_SPARC);
        libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset, i);
      }
      break;
    }
    
    case BF_WHILE_DATA:  /* while (data_ptr[offset]) { */
    {
      int i;
      if (asmGeneration->generate_debug)
        libbf_gen_asm_debug_code(asmGeneration, code_ptr);
      
      libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, L1_SPARC);
      for(i=0;i<nWords;i++)
      {
        libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, L2_SPARC);
        libbf_gen_asm_sparc_or_reg(asmGeneration, L2_SPARC, L1_SPARC, L1_SPARC);
      }
      libbf_gen_asm_sparc_cmp_reg_to_0(asmGeneration, L1_SPARC);
      
      code_ptr->ou_offset_to_patch = asmGeneration->filled_size;
      
      libbf_gen_asm_sparc_nop(asmGeneration); /* be to be patched */
      libbf_gen_asm_sparc_nop(asmGeneration); /* delay slot */

      *(asmGeneration->p_stack_while) = code_ptr;
      asmGeneration->p_stack_while++;
        
      break;
    }
      
    case BF_END_WHILE:
    {
      OptimizationUnit* while_instr;
      asmGeneration->p_stack_while--;
      while_instr = *(asmGeneration->p_stack_while);
      
      if (code_ptr->ou_loop_type != LIBBF_IF)
      {
        int i;
        if (asmGeneration->generate_debug)
          libbf_gen_asm_debug_code(asmGeneration, while_instr);
        
        libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, L1_SPARC);
        for(i=0;i<nWords;i++)
        {
          libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, L2_SPARC);
          libbf_gen_asm_sparc_or_reg(asmGeneration, L2_SPARC, L1_SPARC, L1_SPARC);
        }
        libbf_gen_asm_sparc_cmp_reg_to_0(asmGeneration, L1_SPARC);
        
        libbf_gen_asm_sparc_bnea(asmGeneration, (while_instr->ou_offset_to_patch - asmGeneration->filled_size) / 4 - 1);
        libbf_gen_asm_sparc_nop(asmGeneration); /* delay slot */
      }
      
      /* patch be */
      *(int*)(asmGeneration->compiled_code + while_instr->ou_offset_to_patch) = INT_TO_TGT(0x02800000 | (((asmGeneration->filled_size - while_instr->ou_offset_to_patch) / 4) ));
      
      break;
    }
    
    case BF_INC_CST_MUL_DATA:
    {
      int constant_pos = (code_ptr->ou_constant > 0) ? code_ptr->ou_constant : -code_ptr->ou_constant;
      int shift = 0;
      while(constant_pos)
      {
        if (constant_pos & 1)
        {
          int i;
          for(i=0;i<nWords;i++)
          {
            libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, L1_SPARC);
            libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, L2_SPARC);
            libbf_gen_asm_sparc_sll_cst_to_reg(asmGeneration, shift, L1_SPARC, L3_SPARC);
            if (i != 0)
              libbf_gen_asm_sparc_or_reg(asmGeneration, L3_SPARC, L4_SPARC, L3_SPARC);
            libbf_gen_asm_sparc_srl_cst_to_reg(asmGeneration, 32 - shift, L1_SPARC, L4_SPARC);
            if (code_ptr->ou_constant > 0)
              libbf_gen_asm_sparc_longlong_addX_reg(asmGeneration, i, L2_SPARC, L3_SPARC, L1_SPARC);
            else
              libbf_gen_asm_sparc_longlong_subX_reg(asmGeneration, i, L2_SPARC, L3_SPARC, L1_SPARC);
            libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L1_SPARC, INDEX_REG, code_ptr->ou_offset, i);
          }
        }
        constant_pos >>= 1;
        shift++;
      }
      
      break;
    }
        
    case BF_INC_DATA_MUL_DATA:
    {
      int i, j;
      libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, 32, L0_SPARC);
      
      for(j=0;j<nWords;j++)
      {
        libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset3, j, L2_SPARC);
          
        for(i=0;i<nWords - j;i++)
        {
          int k;
              
          libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, L1_SPARC);
            
#if defined(USE_UMUL_SPARC)          
          libbf_gen_asm_sparc_umul_reg_64(asmGeneration, L1_SPARC, L2_SPARC, G2_SPARC, G4_SPARC);
#else
          libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, 0, G1_SPARC); /* shift */
          libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, 0, G2_SPARC); /* acc */
          libbf_gen_asm_sparc_mov_cst_to_reg(asmGeneration, 0, G4_SPARC); /* acc (high) */
          
          libbf_gen_asm_sparc_btst(asmGeneration, 1, L1_SPARC); /* if (l1 & 1) */
          libbf_gen_asm_sparc_be(asmGeneration, 4);
          libbf_gen_asm_sparc_srl_cst_to_reg(asmGeneration, 1, L1_SPARC, L1_SPARC); /* l1 >>= 1 */  /* done before the jump ! */
      
          libbf_gen_asm_sparc_sll_reg(asmGeneration, G1_SPARC, L2_SPARC, G3_SPARC); /* g3 = l2 << shift */
          libbf_gen_asm_sparc_addcc_reg(asmGeneration, G3_SPARC, G2_SPARC, G2_SPARC);   /* acc += g3 */
      
          libbf_gen_asm_sparc_cmp_reg_to_0(asmGeneration, L1_SPARC); /* if (l1 == 0) */
          libbf_gen_asm_sparc_be(asmGeneration, 10);
          libbf_gen_asm_sparc_add_cst_to_reg(asmGeneration, 1, G1_SPARC, G1_SPARC);   /* shift++ */ /* done before the jump ! */
      
          libbf_gen_asm_sparc_btst(asmGeneration, 1, L1_SPARC); /* if (l1 & 1) */
          libbf_gen_asm_sparc_be(asmGeneration, 7);
          libbf_gen_asm_sparc_srl_cst_to_reg(asmGeneration, 1, L1_SPARC, L1_SPARC); /* l1 >>= 1 */  /* done before the jump ! */
      
          libbf_gen_asm_sparc_sll_reg(asmGeneration, G1_SPARC, L2_SPARC, G3_SPARC); /* g3 = l2 << shift */
          libbf_gen_asm_sparc_addcc_reg(asmGeneration, G3_SPARC, G2_SPARC, G2_SPARC);   /* acc += g3 */
      
          libbf_gen_asm_sparc_sub_reg(asmGeneration, L0_SPARC, G1_SPARC, G3_SPARC); /* g3 = 32 - shift */
          libbf_gen_asm_sparc_srl_reg(asmGeneration, G3_SPARC, L2_SPARC, G3_SPARC); /* g3 = l2 >> (32 - shift) */
          libbf_gen_asm_sparc_addx_reg(asmGeneration, G3_SPARC, G4_SPARC, G4_SPARC);   /* acc (high) += g3 */
          
          libbf_gen_asm_sparc_cmp_reg_to_0(asmGeneration, L1_SPARC); /* if (l1 != 0) */
          libbf_gen_asm_sparc_bne(asmGeneration, -9);
          libbf_gen_asm_sparc_add_cst_to_reg(asmGeneration, 1, G1_SPARC, G1_SPARC);   /* shift++ */ /* done before the jump ! */

#endif
          if (i+j != nWords - 1)
          {
            libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j, L3_SPARC);
            libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j+1, L4_SPARC);
              
            libbf_gen_asm_sparc_addcc_reg(asmGeneration, G2_SPARC, L3_SPARC, L3_SPARC);
            libbf_gen_asm_sparc_addxcc_reg(asmGeneration, G4_SPARC, L4_SPARC, L4_SPARC);
            
            libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L3_SPARC, INDEX_REG, code_ptr->ou_offset, i+j);
            libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L4_SPARC, INDEX_REG, code_ptr->ou_offset, i+j+1);
            
            for(k=i+j+2;k<nWords;k++)
            {
              libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, k, L3_SPARC);
              libbf_gen_asm_sparc_addxcc_reg(asmGeneration, G0_SPARC, L3_SPARC, L3_SPARC);
              libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L3_SPARC, INDEX_REG, code_ptr->ou_offset, k);
            }
          }
          else
          {
            libbf_gen_asm_sparc_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j, L3_SPARC);
            libbf_gen_asm_sparc_add_reg(asmGeneration, G2_SPARC, L3_SPARC, L3_SPARC);
            libbf_gen_asm_sparc_longlong_mov_reg_to_mem(asmGeneration, L3_SPARC, INDEX_REG, code_ptr->ou_offset, i+j);
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


#if defined(__sparc__)
int libbf_is_compatible_compiled_code  (char* compiled_code,
                                        TargetCpu_t* cpu,
                                        int* sizeofData,
                                        int* min_delta_offset,
                                        int* max_delta_offset)
{
  if (!((*(int*)compiled_code) == (0x10800000 | (5+2))))
  {
    warning("Non sparc libbf-compiled code\n");
    return FALSE;
  }

  *cpu = (*(int*)(compiled_code + 8 + 4));
  *sizeofData = (*(int*)(compiled_code + 8 + 8));
  *min_delta_offset = (*(int*)(compiled_code + 8 + 12));
  *max_delta_offset = (*(int*)(compiled_code + 8 + 16));
    
  return TRUE;
}
#endif

static void libbf_gen_asm_sparc_init_funcs()
{
  asmGenerationFuncs[TARGET_SPARC].gen_preambule = libbf_gen_asm_sparc_preambule;
  asmGenerationFuncs[TARGET_SPARC].gen_epilogue = libbf_gen_asm_sparc_epilogue;
  asmGenerationFuncs[TARGET_SPARC].gen_code = libbf_gen_asm_sparc;
  asmGenerationFuncs[TARGET_SPARC].gen_code_longlong = libbf_gen_asm_sparc_longlong;
  asmGenerationFuncs[TARGET_SPARC].gen_debug_code = libbf_gen_asm_sparc_debug_code;
}

#undef INDEX_REG
#undef INT_TO_TGT
