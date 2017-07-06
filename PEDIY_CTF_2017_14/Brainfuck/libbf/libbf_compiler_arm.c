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
  R0_ARM,
  R1_ARM,
  R2_ARM,
  R3_ARM,
  R4_ARM,
  R5_ARM,
  R6_ARM,
  R7_ARM,
#define IN_PARAM_REG  R7_ARM
  R8_ARM,
#define INDEX_REG     R8_ARM
  R9_ARM,
  R10_ARM,
  R11_ARM,
  R12_ARM,
  R13_ARM,
  LR_ARM,
  PC_ARM,
} RegArm_t;

#ifndef INT_TO_TGT
#define INT_TO_TGT int_to_le
#endif

static void libbf_gen_asm_arm_ops(AsmGeneration_t* asmGeneration, int code32)
{
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(code32));
}

static void libbf_gen_asm_arm_mov_cst_to_reg(AsmGeneration_t* asmGeneration, unsigned int my_int, RegArm_t reg)
{
  int signed_my_int = my_int;
  if (signed_my_int >= 0 && signed_my_int <= 255)
  {
    /* e3a000XX        mov     r0, #0xXX */
    libbf_gen_asm_arm_ops(asmGeneration, 0xe3a00000 | (reg << 12) | (reg << 16) | my_int);
  }
  else if (signed_my_int < 0 && signed_my_int >= - 256)
  {
    /* e3e000fe        mvn     r0, #254  (= -255 = - (-255) - 1) */
    libbf_gen_asm_arm_ops(asmGeneration, 0xe3e00000 | (reg << 12) | (reg << 16) | ((-1-signed_my_int) & 0xFF));
  }
  else
  {
    if ((my_int >> 24) & 0xFF)
    {
      libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, (my_int >> 24) & 0xFF, reg);
    
      /* e1a00c00        mov     r0, r0, lsl #24 */
      libbf_gen_asm_arm_ops(asmGeneration, 0xe1a00c00 | (reg << 12) | reg);
    }
    else
    {
      libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, 0, reg);
    }
    
    if (my_int & 0xFF)
    {
      /* e38000XY        orr     r0, r0, #0xXY */
      libbf_gen_asm_arm_ops(asmGeneration, 0xe3800000 | (reg << 12) | (reg << 16) | (my_int & 0xFF));
    }
    
    if ((my_int >> 8) & 0xFF)
    {
      /* e3800cXY        orr     r0, r0, #0xXY00 */
      libbf_gen_asm_arm_ops(asmGeneration, 0xe3800c00 | (reg << 12) | (reg << 16) | ((my_int >> 8) & 0xFF));
    }
    
    if ((my_int >> 16) & 0xFF)
    {
      /* e38008XY        orr     r0, r0, #0xXY0000 */
      libbf_gen_asm_arm_ops(asmGeneration, 0xe3800800 | (reg << 12) | (reg << 16) | ((my_int >> 16) & 0xFF));
    }
  }
}

static void libbf_gen_asm_arm_mov_reg_to_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe1a00000 | (reg_dst << 12) | reg_src);
}

/* reg_dst = (reg_src << shift) */
static void libbf_gen_asm_arm_mov_reg_lshift_to_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src, int shift, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe1a00000 | (reg_dst << 12) | (reg_src) | ((8 * shift) << 4) );
}

/* reg_dst = reg_src1 - reg_src2 */
/*e0631002        rsb     r1, r3, r2 */  /* r1 = r2 - r3 */
static void libbf_gen_asm_arm_rsb_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src1, RegArm_t reg_src2, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe0600000 | (reg_dst << 12) | (reg_src1) | (reg_src2 << 16));
}

static void libbf_gen_asm_arm_subs_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src1, RegArm_t reg_src2, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe0500000 | (reg_dst << 12) | (reg_src1) | (reg_src2 << 16));
}

static void libbf_gen_asm_arm_sbc_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src1, RegArm_t reg_src2, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe0c00000 | (reg_dst << 12) | (reg_src1) | (reg_src2 << 16));
}

/* reg_dst = reg_src1 + reg_src2 */
/* e0843005        add     r3, r4, r5 */ /* r3 = r4 + r5 */
static void libbf_gen_asm_arm_add_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src1, RegArm_t reg_src2, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe0800000 | (reg_dst << 12) | (reg_src1) | (reg_src2 << 16));
}

/* reg_dst = (reg_src1 << shift) + reg_src2 */
static void libbf_gen_asm_arm_add_reg_lshift(AsmGeneration_t* asmGeneration, RegArm_t reg_src1, int shift, RegArm_t reg_src2, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe0800000 | (reg_dst << 12) | (reg_src1) | (reg_src2 << 16) | ((8 * shift) << 4) );
}

/* update the carry flag */
static void libbf_gen_asm_arm_adds_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src1, RegArm_t reg_src2, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe0800000 | (1 << 20) | (reg_dst << 12) | (reg_src1) | (reg_src2 << 16));
}

/* add with the previous carry flag */
static void libbf_gen_asm_arm_adc_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src1, RegArm_t reg_src2, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe0a00000 | (reg_dst << 12) | (reg_src1) | (reg_src2 << 16));
}

static void libbf_gen_asm_arm_orr_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src1, RegArm_t reg_src2, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe1800000 | (reg_dst << 12) | (reg_src1) | (reg_src2 << 16));
}

static void libbf_gen_asm_arm_orrs_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src1, RegArm_t reg_src2, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe1800000 | (1 << 20) | (reg_dst << 12) | (reg_src1) | (reg_src2 << 16));
}

/* reg_dst = (reg_src1 >> shift) | reg_src2 */
static void libbf_gen_asm_arm_orr_rshift_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_src1, int shift, RegArm_t reg_src2, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_ops(asmGeneration, 0xe1800000 | (reg_dst << 12) | (reg_src1) | (reg_src2 << 16) | ((8 * shift + 2) << 4));
}

static void libbf_gen_asm_arm_load_reg32(AsmGeneration_t* asmGeneration, RegArm_t reg_mem, int offset, RegArm_t reg_dst)
{
  if (offset < -0xFFF || offset > 0xFFF)
  {
    NOT_IMPLEMENTED();
  }
            
  if (offset >= 0)
  {
    /* e5961XYZ        ldr     r1, [r6, #0xXYZ] */
    libbf_gen_asm_arm_ops(asmGeneration, 0xe5900000 | (reg_mem << 16) | (reg_dst << 12) | offset);
  }
  else
  {
    /* e5161XYZ        ldr     r1, [r6, #-0xXYZ] */
    libbf_gen_asm_arm_ops(asmGeneration, 0xe5100000 | (reg_mem << 16) | (reg_dst << 12) | ((-offset) & 0xFFF));
  }
}

static void libbf_gen_asm_arm_store_reg32(AsmGeneration_t* asmGeneration, RegArm_t reg_mem, int offset, RegArm_t reg_dst)
{
  if (offset < -0xFFF || offset > 0xFFF)
  {
    NOT_IMPLEMENTED();
  }
            
  if (offset >= 0)
  {
    /* e5860XYZ        str     r0, [r6, #0xXYZ] */
    libbf_gen_asm_arm_ops(asmGeneration, 0xe5800000 | (reg_mem << 16) | (reg_dst << 12) | offset);
  }
  else
  {
    /* e5060XYZ        str     r0, [r6, #-0xXYZ] */
    libbf_gen_asm_arm_ops(asmGeneration, 0xe5000000 | (reg_mem << 16) | (reg_dst << 12) | ((-offset) & 0xFFF));
  }
}

static void libbf_gen_asm_arm_longlong_mov_mem_to_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_mem, int offset, int suboffset, RegArm_t reg_dst)
{
  libbf_gen_asm_arm_load_reg32(asmGeneration, reg_mem, libbf_compiler_compute_longlong_offset(asmGeneration, offset, suboffset), reg_dst);
}

static void libbf_gen_asm_arm_longlong_mov_reg_to_mem(AsmGeneration_t* asmGeneration, RegArm_t reg_src, RegArm_t reg_mem, int offset, int suboffset)
{
  libbf_gen_asm_arm_store_reg32(asmGeneration, reg_mem, libbf_compiler_compute_longlong_offset(asmGeneration, offset, suboffset), reg_src);
}


static void libbf_gen_asm_arm_offset_short(AsmGeneration_t* asmGeneration, int seed, int offset)
{
  if (offset > 0xFF)
  {
    NOT_IMPLEMENTED();
  }
  
  libbf_gen_asm_arm_ops(asmGeneration, seed | ((offset >> 4) << 8) | (offset & 0xF) );
}

static void libbf_gen_asm_arm_mov_mem_to_reg(AsmGeneration_t* asmGeneration, RegArm_t reg_mem, int offset, RegArm_t reg_dst)
{
  offset *= asmGeneration->sizeofData;
  
  if (offset < -0xFFF || offset > 0xFFF)
  {
    NOT_IMPLEMENTED();
  }
            
  if (offset >= 0)
  {
    /* e5961XYZ        ldr     r1, [r6, #0xXYZ] */
    if (asmGeneration->sizeofData == 4)
      libbf_gen_asm_arm_ops(asmGeneration, 0xe5900000 | (reg_mem << 16) | (reg_dst << 12) | offset);
    else if (asmGeneration->sizeofData == 2)
      libbf_gen_asm_arm_offset_short(asmGeneration, 0xe1d000b0 | (reg_mem << 16) | (reg_dst << 12), offset);
    else if (asmGeneration->sizeofData == 1)
      libbf_gen_asm_arm_ops(asmGeneration, 0xe5d00000 | (reg_mem << 16) | (reg_dst << 12) | offset);
  }
  else
  {
    /* e5161XYZ        ldr     r1, [r6, #-0xXYZ] */
    if (asmGeneration->sizeofData == 4)
      libbf_gen_asm_arm_ops(asmGeneration, 0xe5100000 | (reg_mem << 16) | (reg_dst << 12) | ((-offset) & 0xFFF));
    else if (asmGeneration->sizeofData == 2)
      libbf_gen_asm_arm_offset_short(asmGeneration, 0xe15000b0 | (reg_mem << 16) | (reg_dst << 12), ((-offset) & 0xFFF));
    else if (asmGeneration->sizeofData == 1)
      libbf_gen_asm_arm_ops(asmGeneration, 0xe5500000 | (reg_mem << 16) | (reg_dst << 12) | ((-offset) & 0xFFF));
  }
}

static void libbf_gen_asm_arm_mov_reg_to_mem(AsmGeneration_t* asmGeneration, RegArm_t reg_src, RegArm_t reg_mem, int offset)
{
  offset *= asmGeneration->sizeofData;
  
  if (offset < -0xFFF || offset > 0xFFF)
  {
    NOT_IMPLEMENTED();
  }
            
  if (offset >= 0)
  {
    /* e5860XYZ        str     r0, [r6, #0xXYZ] */
    if (asmGeneration->sizeofData == 4)
      libbf_gen_asm_arm_ops(asmGeneration, 0xe5800000 | (reg_mem << 16) | (reg_src << 12) | offset);
    else if (asmGeneration->sizeofData == 2)
      libbf_gen_asm_arm_offset_short(asmGeneration, 0xe1c000b0 | (reg_mem << 16) | (reg_src << 12), offset);
    else if (asmGeneration->sizeofData == 1)
      libbf_gen_asm_arm_ops(asmGeneration, 0xe5c00000 | (reg_mem << 16) | (reg_src << 12) | offset);
  }
  else
  {
    /* e5060XYZ        str     r0, [r6, #-0xXYZ] */
    if (asmGeneration->sizeofData == 4)
      libbf_gen_asm_arm_ops(asmGeneration, 0xe5000000 | (reg_mem << 16) | (reg_src << 12) | ((-offset) & 0xFFF));
    else if (asmGeneration->sizeofData == 2)
      libbf_gen_asm_arm_offset_short(asmGeneration, 0xe14000b0 | (reg_mem << 16) | (reg_src << 12), ((-offset) & 0xFFF));
    else if (asmGeneration->sizeofData == 1)
      libbf_gen_asm_arm_ops(asmGeneration, 0xe5400000 | (reg_mem << 16) | (reg_src << 12) | ((-offset) & 0xFFF));
  }
}

static void libbf_gen_asm_arm_add_cst(AsmGeneration_t* asmGeneration, RegArm_t reg_dst, int val, RegArm_t reg_tmp)
{ 
  /* e2812001        add     r2, r1, #1      ; 0x1 */
  /* e2433001        sub     r3, r3, #1      ; 0x1 */
  if (val >= 0 && val <= 255)
  {
    libbf_gen_asm_arm_ops(asmGeneration, 0xe2800000  | (reg_dst << 16) | (reg_dst << 12) | val);
  }
  else if (val < 0 && val >= -255)
  {
    libbf_gen_asm_arm_ops(asmGeneration, 0xe2400000 | (reg_dst << 16) | (reg_dst << 12) | (-val));
  }
  else
  {
    libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, val, reg_tmp);
    libbf_gen_asm_arm_add_reg(asmGeneration, reg_dst, reg_tmp, reg_dst); /* reg_dst (3rd) = reg_dst(1st) + reg_tmp(2nd) */
  }
}

static
void libbf_gen_arm_arm_mul_add_reg(AsmGeneration_t* asmGeneration, 
                                                      RegArm_t reg_src_mul1,
                                                      RegArm_t reg_src_mul2,
                                                      RegArm_t reg_src_add,
                                                      RegArm_t reg_dst)
{
      
  libbf_gen_asm_arm_ops(asmGeneration, 0xe0200090 | (reg_dst << 16) | reg_src_mul1 | (reg_src_mul2 << 8) | (reg_src_add << 12));
}

static void libbf_gen_asm_arm_mul64_reg(AsmGeneration_t* asmGeneration, 
                                        RegArm_t reg_src_mul1,
                                        RegArm_t reg_src_mul2,
                                        RegArm_t reg_dst_lo,
                                        RegArm_t reg_dst_high)
{
      
  libbf_gen_asm_arm_ops(asmGeneration, 0xe0800090 | (reg_dst_high << 16) | reg_src_mul1 | (reg_src_mul2 << 8) | (reg_dst_lo << 12));
}

static void libbf_gen_asm_arm_preambule(AsmGeneration_t* asmGeneration)
{
  /* ea000000        b     ... */
  libbf_gen_asm_arm_ops(asmGeneration, 0xea000000 | (5 - 1));

  libbf_write_asm_int(asmGeneration, INT_TO_TGT(0)); /* CODE SIZE : TO BE POST-FIXED */
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration->cpu));
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration->sizeofData));
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration->min_delta_offset));
  libbf_write_asm_int(asmGeneration, INT_TO_TGT(asmGeneration->max_delta_offset));

  /* e92d41f0        stmdb   sp!, {r4, r5, r6, r7, r8, lr} */
  libbf_gen_asm_arm_ops(asmGeneration, 0xe92d41f0);
  
  libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, R0_ARM, IN_PARAM_REG); /* r8 : ExecutableCodeData_t* */
  libbf_gen_asm_arm_load_reg32(asmGeneration, R0_ARM, OFFSET_BASE_DATA_PTR, INDEX_REG); /* current data pointer */
}

static void libbf_gen_asm_arm_epilogue(AsmGeneration_t* asmGeneration)
{
  /* e8bd81f0        ldmia   sp!, {r4, r5, r6, r7, r8, pc} */
  libbf_gen_asm_arm_ops(asmGeneration, 0xe8bd81f0);
  
  
  *(int*)&asmGeneration->compiled_code[4] = INT_TO_TGT(asmGeneration->filled_size);
}


static void libbf_gen_asm_arm_debug_code(AsmGeneration_t* asmGeneration, OptimizationUnit* current_code_ptr)
{
#if defined(__arm__)
  libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, asmGeneration->sizeofData, R0_ARM);
  libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, (unsigned int)(long)current_code_ptr, R1_ARM);
  libbf_gen_asm_arm_load_reg32(asmGeneration, IN_PARAM_REG, OFFSET_BASE_DATA_PTR, R2_ARM); /* base data pointer */
  libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, INDEX_REG, R3_ARM); /* current data pointer */
  libbf_gen_asm_arm_load_reg32(asmGeneration, IN_PARAM_REG, OFFSET_DEBUG_FUNC, R4_ARM); /* debug func */
  libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, PC_ARM, LR_ARM);
  libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, R4_ARM, PC_ARM);
#endif
}

static void libbf_gen_asm_arm(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  switch(code_ptr->ou_instr)
  {
    case BF_INC_DATA_PTR:
    {
      libbf_gen_asm_arm_add_cst(asmGeneration, INDEX_REG, code_ptr->ou_offset * asmGeneration->sizeofData, R0_ARM);
      
      break;
    }
    
    case BF_READ_DATA:
    {
      libbf_gen_asm_arm_load_reg32(asmGeneration, IN_PARAM_REG, OFFSET_INPUT_FUNC, R1_ARM); /* input func */
      
      libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, PC_ARM, LR_ARM);
      libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, R1_ARM, PC_ARM);
      
      libbf_gen_asm_arm_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
        
    case BF_PRINT_DATA:
    {
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R0_ARM);
      libbf_gen_asm_arm_load_reg32(asmGeneration, IN_PARAM_REG, OFFSET_OUTPUT_FUNC, R1_ARM); /* output func */
      
      libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, PC_ARM, LR_ARM);
      libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, R1_ARM, PC_ARM);

      break;
    }

    case BF_SET_CST:
    {
      libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, R0_ARM);
      
      libbf_gen_asm_arm_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    case BF_INC_CST:
    { 
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R0_ARM);
      
      libbf_gen_asm_arm_add_cst(asmGeneration, R0_ARM, code_ptr->ou_constant, R1_ARM);

      libbf_gen_asm_arm_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    case BF_WHILE_DATA:  /* while (data_ptr[offset]) { */
    {
      if (asmGeneration->generate_debug)
        libbf_gen_asm_debug_code(asmGeneration, code_ptr);
      
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R0_ARM);
      
      /* e3500000        cmp     r0, #0  */
      libbf_gen_asm_arm_ops(asmGeneration, 0xe3500000 | (R0_ARM << 16));
      
      /* 0a000000        beq     ... */
      code_ptr->ou_offset_to_patch = asmGeneration->filled_size;
      
      libbf_gen_asm_arm_ops(asmGeneration, 0x0a000000);

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
        
        libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R0_ARM);
        
        /* e3500000        cmp     r0, #0  */
        libbf_gen_asm_arm_ops(asmGeneration, 0xe3500000 | (R0_ARM << 16));
        
        /* 1a000000        bne     ... */
        libbf_gen_asm_arm_ops(asmGeneration, 0x1a000000 | (((while_instr->ou_offset_to_patch - asmGeneration->filled_size) / 4 - 4) & 0x00FFFFFF));
      }
      
      /* patch beq */
      *(int*)(asmGeneration->compiled_code + while_instr->ou_offset_to_patch) = INT_TO_TGT(0x0a000000 | (((asmGeneration->filled_size - while_instr->ou_offset_to_patch) / 4) - 2));
      
      break;
    }
    
    case BF_SET_DATA:
    {
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, R0_ARM);
      libbf_gen_asm_arm_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
        
    case BF_INC_DATA:
    {
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, R0_ARM);
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R1_ARM);
      libbf_gen_asm_arm_add_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM); /* r0 (3rd) = r0(1st) + r1(2nd) */
      libbf_gen_asm_arm_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset);

      break;
    }
    
    case BF_DEC_DATA:
    {
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, R0_ARM);
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R1_ARM); 
      libbf_gen_asm_arm_rsb_reg(asmGeneration, R1_ARM, R0_ARM, R0_ARM); /* r0 (3rd) = r1 (1st) - r0 (2nd) */
      libbf_gen_asm_arm_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset);

      break;
    }
        
    case BF_INC_CST_MUL_DATA:
    {
      int is_neg = (code_ptr->ou_constant < 0);
      int constant = (is_neg) ? -code_ptr->ou_constant : code_ptr->ou_constant;
      int i = 0;

      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, R1_ARM);

      if (is_neg)
      {
        libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, 0, R0_ARM);
      }
      else
      {
        libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R0_ARM);
      }
      
      /* 10 = 8 + 2 = 1010b */
      
      while(constant)
      {
        if (constant & 1)
        {
          libbf_gen_asm_arm_add_reg_lshift(asmGeneration, R1_ARM, i, R0_ARM, R0_ARM);
        }
        constant >>= 1;
        i++;
      }
      
      if (is_neg)
      {
        libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R1_ARM);
        libbf_gen_asm_arm_rsb_reg(asmGeneration, R1_ARM, R0_ARM, R0_ARM); /* r0 (3rd) = r1 (1st) - r0 (2nd) */
      }
      
      libbf_gen_asm_arm_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    case BF_INC_DATA_MUL_DATA:
    {
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, R0_ARM);
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, R1_ARM);
      libbf_gen_asm_arm_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset3, R2_ARM);
      libbf_gen_arm_arm_mul_add_reg(asmGeneration, R1_ARM, R2_ARM, R0_ARM, R0_ARM);
      libbf_gen_asm_arm_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset);
      
      break;
    }
    
    default:
    {
      libbf_gen_asm_default(asmGeneration, code_ptr);
      break;
    }
  }
}

static void libbf_gen_asm_arm_longlong(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  int nWords = NWORDS(asmGeneration);
  
  switch(code_ptr->ou_instr)
  {
    case BF_INC_DATA_PTR:
    {
      libbf_gen_asm_arm_add_cst(asmGeneration, INDEX_REG, code_ptr->ou_offset * asmGeneration->sizeofData, R0_ARM);
      
      break;
    }
    
    case BF_READ_DATA:
    {
      int i;
      libbf_gen_asm_arm_load_reg32(asmGeneration, IN_PARAM_REG, OFFSET_INPUT_FUNC, R1_ARM); /* input func */
      
      libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, PC_ARM, LR_ARM);
      libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, R1_ARM, PC_ARM);
      
      libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset, 0);
      libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, 0, R0_ARM);
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }
        
    case BF_PRINT_DATA:
    {
      libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R0_ARM);
      libbf_gen_asm_arm_load_reg32(asmGeneration, IN_PARAM_REG, OFFSET_OUTPUT_FUNC, R1_ARM); /* output func */
      
      libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, PC_ARM, LR_ARM);
      libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, R1_ARM, PC_ARM);

      break;
    }

    case BF_SET_CST:
    {
      int i;
      libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, R0_ARM);
      
      libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset, 0);
      
      if (code_ptr->ou_constant > 0)
        libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, 0, R0_ARM);
      else if (code_ptr->ou_constant < 0)
        libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, (unsigned int)-1, R0_ARM);
      
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }
    
    case BF_INC_CST:
    { 
      int i;
        
      libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R0_ARM);
      
      libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, R1_ARM);
      
      libbf_gen_asm_arm_adds_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM);

      libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset, 0);
      
      if (code_ptr->ou_constant >= 0)
        libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, 0, R1_ARM);
      else
        libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, (unsigned int)-1, R1_ARM);
      
      for(i=1;i<nWords;i++)
      {
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, R0_ARM);
        
        libbf_gen_asm_arm_adc_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM);
        
        libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }
    
    case BF_WHILE_DATA:  /* while (data_ptr[offset]) { */
    {
      int i;
        
      if (asmGeneration->generate_debug)
        libbf_gen_asm_debug_code(asmGeneration, code_ptr);
      
      libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R0_ARM);
      for(i=1;i<nWords-1;i++)
      {
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, R1_ARM);
        libbf_gen_asm_arm_orr_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM);
      }
        
      libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, nWords-1, R1_ARM);
      libbf_gen_asm_arm_orrs_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM);

      /* 0a000000        beq     ... */
      code_ptr->ou_offset_to_patch = asmGeneration->filled_size;
      
      libbf_gen_asm_arm_ops(asmGeneration, 0x0a000000);

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
        
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R0_ARM);
        for(i=1;i<nWords-1;i++)
        {
          libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, R1_ARM);
          libbf_gen_asm_arm_orr_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM);
        }
        
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, nWords-1, R1_ARM);
        libbf_gen_asm_arm_orrs_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM);
        
        /* 1a000000        bne     ... */
        libbf_gen_asm_arm_ops(asmGeneration, 0x1a000000 | (((while_instr->ou_offset_to_patch - asmGeneration->filled_size) / 4 - 4) & 0x00FFFFFF));
      }
      
      /* patch beq */
      *(int*)(asmGeneration->compiled_code + while_instr->ou_offset_to_patch) = INT_TO_TGT(0x0a000000 | (((asmGeneration->filled_size - while_instr->ou_offset_to_patch) / 4) - 2));
      
      break;
    }
    
    case BF_SET_DATA:
    {
      int i;
      for(i=0;i<nWords;i++)
      {
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, R0_ARM);
        libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset, i);
      }
      
      break;
    }
        
    case BF_INC_DATA:
    {
      int i;
      for(i=0;i<nWords;i++)
      {
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, R0_ARM);
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, R1_ARM);
        if (i == 0)
          libbf_gen_asm_arm_adds_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM);
        else
          libbf_gen_asm_arm_adc_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM);
        libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset, i);
      }
      break;
    }
    
    case BF_DEC_DATA:
    {
      int i;
      for(i=0;i<nWords;i++)
      {
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, R0_ARM);
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i, R1_ARM);
        if (i == 0)
          libbf_gen_asm_arm_subs_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM);
        else
          libbf_gen_asm_arm_sbc_reg(asmGeneration, R0_ARM, R1_ARM, R0_ARM);
        libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset, i);
      }

      break;
    }
    
    case BF_INC_CST_MUL_DATA:
    {
      int shift1, shift2;
      int constant_pos = (code_ptr->ou_constant > 0) ? code_ptr->ou_constant : -code_ptr->ou_constant;
      if (nWords == 2 &&
          libbf_is_composed_poweroftwo(constant_pos, &shift1, &shift2) &&
          shift2 == 0)
      {
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 0, R0_ARM);
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, 1, R1_ARM);
        libbf_gen_asm_arm_mov_reg_lshift_to_reg(asmGeneration, R1_ARM, shift1, R1_ARM); /* r1 = r1 << shift1 */
        libbf_gen_asm_arm_orr_rshift_reg(asmGeneration, R0_ARM, 32 - shift1, R1_ARM, R1_ARM); /* r1 = r1 | (r0 >> (32 - shift1) */
        libbf_gen_asm_arm_mov_reg_lshift_to_reg(asmGeneration, R0_ARM, shift1, R0_ARM); /* r0 = r0 << shift1 */
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 0, R2_ARM);
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, 1, R3_ARM);
        if (code_ptr->ou_constant > 0)
        {
          libbf_gen_asm_arm_adds_reg(asmGeneration, R0_ARM, R2_ARM, R0_ARM);
          libbf_gen_asm_arm_adc_reg(asmGeneration, R1_ARM, R3_ARM, R1_ARM);
        }
        else
        {
          libbf_gen_asm_arm_subs_reg(asmGeneration, R0_ARM, R2_ARM, R0_ARM);
          libbf_gen_asm_arm_sbc_reg(asmGeneration, R1_ARM, R3_ARM, R1_ARM);
        }
        libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R0_ARM, INDEX_REG, code_ptr->ou_offset, 0);
        libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R1_ARM, INDEX_REG, code_ptr->ou_offset, 1);
      }
      else
      {
        int i, j;
          
        libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, 0, R4_ARM);
        for(j=0;j<nWords;j++)
        {
          if (j == 0)
            libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, code_ptr->ou_constant, R0_ARM);
          else if (code_ptr->ou_constant < 0)
          {
            if (j == 1)
              libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, (unsigned int)-1, R0_ARM);
          }
          else
          {
            break;
          }
              
          for(i=0;i<nWords-j;i++)
          {
            libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, R1_ARM);
  
            if (i != 0)
            {
              libbf_gen_asm_arm_mov_reg_to_reg(asmGeneration, R2_ARM, R3_ARM);
            }
            
            libbf_gen_asm_arm_mul64_reg(asmGeneration, R0_ARM, R1_ARM, R1_ARM, R2_ARM); /* (r2,r1) = r0 * r1 */
            
            if (i != 0)
            {
              if (i+j < nWords-1)
              {
                libbf_gen_asm_arm_adds_reg(asmGeneration, R3_ARM, R1_ARM, R1_ARM);
                libbf_gen_asm_arm_adc_reg(asmGeneration, R4_ARM, R2_ARM, R2_ARM);
              }
              else
              {
                libbf_gen_asm_arm_add_reg(asmGeneration, R3_ARM, R1_ARM, R1_ARM);
              }
            }
              
            libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, i+j, R3_ARM);
            if (i+j < nWords-1)
            {
              libbf_gen_asm_arm_adds_reg(asmGeneration, R1_ARM, R3_ARM, R3_ARM);
              libbf_gen_asm_arm_adc_reg(asmGeneration, R4_ARM, R2_ARM, R2_ARM);
            }
            else
            {
              libbf_gen_asm_arm_add_reg(asmGeneration, R1_ARM, R3_ARM, R3_ARM);
            }
            libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R3_ARM, INDEX_REG, code_ptr->ou_offset, i+j);
          }
        }
      }
      break;
    }
    
    case BF_INC_DATA_MUL_DATA:
    {
      int i, j;
      if (nWords > 2)
      {
        libbf_gen_asm_arm_mov_cst_to_reg(asmGeneration, 0, R6_ARM);
      }
      
      for(j=0;j<nWords;j++)
      {
        libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset3, j, R1_ARM);
        
        for(i=0;i<nWords - j;i++)
        {
          int k;
            
          libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset2, i, R0_ARM);
          
          libbf_gen_asm_arm_mul64_reg(asmGeneration, R0_ARM, R1_ARM, R2_ARM, R3_ARM); /* (r3,r2) = r0 * r1 */
          
          if (i+j != nWords - 1)
          {
            libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, (i+j), R4_ARM);
            libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, (i+j+1), R5_ARM);
            
            libbf_gen_asm_arm_adds_reg(asmGeneration, R2_ARM, R4_ARM, R4_ARM);
            libbf_gen_asm_arm_adc_reg(asmGeneration, R3_ARM, R5_ARM, R5_ARM);
          
            libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R4_ARM, INDEX_REG, code_ptr->ou_offset, (i+j));
            libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R5_ARM, INDEX_REG, code_ptr->ou_offset, (i+j+1));
          }
          else
          {
            libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, (i+j), R4_ARM);
            libbf_gen_asm_arm_add_reg(asmGeneration, R2_ARM, R4_ARM, R4_ARM);
            libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R4_ARM, INDEX_REG, code_ptr->ou_offset, (i+j));
          }
          
          for(k=i+j+2;k<nWords;k++)
          {
            libbf_gen_asm_arm_longlong_mov_mem_to_reg(asmGeneration, INDEX_REG, code_ptr->ou_offset, k, R4_ARM);
            libbf_gen_asm_arm_adc_reg(asmGeneration, R6_ARM, R4_ARM, R4_ARM);
            libbf_gen_asm_arm_longlong_mov_reg_to_mem(asmGeneration, R4_ARM, INDEX_REG, code_ptr->ou_offset, k);
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

#if defined(__arm__)
int libbf_is_compatible_compiled_code  (char* compiled_code,
                                        TargetCpu_t* cpu,
                                        int* sizeofData,
                                        int* min_delta_offset,
                                        int* max_delta_offset)
{
  if (!((*(int*)compiled_code) == (0xea000000 | (5 - 1))))
  {
    warning("Non arm libbf-compiled code\n");
    return FALSE;
  }

  *cpu = (*(int*)(compiled_code + 4 + 4));
  *sizeofData = (*(int*)(compiled_code + 4 + 8));
  *min_delta_offset = (*(int*)(compiled_code + 4 + 12));
  *max_delta_offset = (*(int*)(compiled_code + 4 + 16));
  
  return TRUE;
}
#endif

static void libbf_gen_asm_arm_init_funcs()
{
  asmGenerationFuncs[TARGET_ARM].gen_preambule = libbf_gen_asm_arm_preambule;
  asmGenerationFuncs[TARGET_ARM].gen_epilogue = libbf_gen_asm_arm_epilogue;
  asmGenerationFuncs[TARGET_ARM].gen_code = libbf_gen_asm_arm;
  asmGenerationFuncs[TARGET_ARM].gen_code_longlong = libbf_gen_asm_arm_longlong;
  asmGenerationFuncs[TARGET_ARM].gen_debug_code = libbf_gen_asm_arm_debug_code;
}

#undef IN_PARAM_REG
#undef INDEX_REG
#undef INT_TO_TGT
