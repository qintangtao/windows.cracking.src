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

#ifndef LIBBF_INTERNAL_H
#define LIBBF_INTERNAL_H 
 
#include <stdio.h>

#include "libbf_config.h"

#define _STR(x) #x
#define STR(x) _STR(x)

#define CONCAT_I(a, b) a ## b
#define CONCAT(a, b) CONCAT_I(a, b)

#define alloc0(type, nb) (type*)calloc(nb, sizeof(type))
#define fatal(x...) do { fprintf(stderr, "fatal: %s, at line %d : ", __FILE__, __LINE__); fprintf(stderr, x); fflush(stderr); abort(); } while(0)
#define warning(x...) do { fprintf(stderr, "warning: %s, at line %d : ", __FILE__, __LINE__); fprintf(stderr, x); } while(0)
#define ASSERT(cond) do { if (!(cond)) fatal("cond \"" #cond "\" failed\n"); } while(0)
#define SHOULDNT_HAPPEN() fatal("Shouldn't happen!\n")
#define NOT_IMPLEMENTED() fatal("Not implemented!\n")

#define CHECK_ENV_VAR(x) (getenv(x) ? (strcmp(getenv(x), "yes") == 0) : FALSE)

#define TRUE  1
#define FALSE 0

#define SIGNATURE_1   0x14071980
#define SIGNATURE_2   0x08031980

typedef int OFFSET_t;
typedef int CONSTANT_t;

 /* -------------------------------------------------------- */
/* Expression management */
/* -------------------------------------------------------- */


typedef enum
{
  EXPR_INVALID = -1,
  
  EXPR_CONSTANT,
  EXPR_DATA,
  EXPR_ADD,
  EXPR_MUL,
} ExprType;

typedef struct _Expression Expression;

struct _Expression
{
  ExprType   e_type;
  union
  {
    CONSTANT_t e_constant;
    OFFSET_t   e_offset;
    struct
    {
      Expression* e_expr1;
      Expression* e_expr2;
    } s;
  } u;
};

#define e_constant u.e_constant
#define e_offset   u.e_offset
#define e_expr1    u.s.e_expr1
#define e_expr2    u.s.e_expr2

/* -------------------------------------------------------- */

typedef struct _OptimizationUnit OptimizationUnit;

typedef enum
{
  BF_AFFECT_EXPR,     /* data_ptr[offset] = f(...) */
  BF_INC_DATA_PTR,    /* data_ptr += offset */
  BF_PRINT_DATA,      /* putchar(data_ptr[offset]) */
  BF_READ_DATA,       /* data_ptr[offset] = getchar() */
  BF_WHILE_DATA,      /* while (data_ptr[offset]) { */
  BF_END_WHILE,       /* } */

  /* The following opcodes are only used by the interpreter and compiler */
  BF_SET_CST,         /* data_ptr[offset] = cst */
  BF_INC_CST,         /* data_ptr[offset] += cst */
  BF_SET_DATA,        /* data_ptr[offset] = data_ptr[offset2] */
  BF_INC_DATA,        /* data_ptr[offset] += data_ptr[offset2] */
  BF_DEC_DATA,        /* data_ptr[offset] -= data_ptr[offset2] */
  BF_INC_CST_MUL_DATA,/* data_ptr[offset] += cst * data_ptr[offset2] */
  BF_INC_CST_ADD_DATA,/* data_ptr[offset] += cst + data_ptr[offset2] */
  BF_INC_DATA_MUL_DATA,/*data_ptr[offset] += data_ptr[offset2] * data_ptr[offset3] */
  BF_WHILE_INC_DATA_PTR, /* while(data_ptr[offset]) data_ptr += offset2; */
  
  /* The following opcodes are only used by the interpreter */
  BF_END,
  BF_SET_CST_0,       /* data_ptr[offset] = 0 */
  BF_INC_CST_1,       /* data_ptr[offset] += 1 */
  BF_INC_CST_M_1,     /* data_ptr[offset] -= 1 */
#ifdef USE_END_WHILE_INC_DATA_PTR  
  BF_END_WHILE_INC_DATA_PTR, /* data_ptr += offset2; } while(data_ptr[offset]); */
#endif
  
  BF_NOP,                     /* used only for profiling */
  BF_DYNALLOC_REALLOC_NEEDED, /* very special opcode that is generated when a realloc is necessary */
  
  BF_COUNT_INSTR,
} InterpreterOptimizerInstr;

typedef enum
{
  LIBBF_WHILE,
  LIBBF_IF,
  LIBBF_DO_WHILE,
} LoopType;

struct _OptimizationUnit
{
  InterpreterOptimizerInstr ou_instr;
  OFFSET_t                  ou_offset;
  int                       ou_nb_visits;        /* only used by the interpeter (profiling) */
  union
  {
    void*                     ou_interpreter_addr; /* only used by the interpeter */
    int                       ou_offset_to_patch; /* only used by the compiler */
  } u1;
  union
  {
    Expression*         ou_expr;
    struct
    {
      union
      {
        OptimizationUnit* ou_jump_to; /* only used by the interpeter */
        CONSTANT_t        ou_constant;
      } u3;
      OFFSET_t          ou_offset2;   /* only used by the interpeter */
      union
      {
        OFFSET_t          ou_offset3; /* only used by the interpeter */
        LoopType          ou_loop_type;
        /* while(data[offset] != 0) */
        /* { */
        /*   blah... */
        /*   data[offset] = 0; */
        /* } <--> */
        /* if(data[offset] != 0) */
        /* { */
        /*   blah... */
        /*   data[offset] = 0; */
        /* } <--> */        
      } u4;
    } s1;
  } u2;
};

#define ou_interpreter_addr   u1.ou_interpreter_addr
#define ou_offset_to_patch   u1.ou_offset_to_patch
#define ou_expr   u2.ou_expr
#define ou_jump_to   u2.s1.u3.ou_jump_to
#define ou_constant   u2.s1.u3.ou_constant
#define ou_offset2   u2.s1.ou_offset2
#define ou_offset3   u2.s1.u4.ou_offset3
#define ou_loop_type   u2.s1.u4.ou_loop_type

void libbf_interpreter_optimizer_cdump_internal_instr(FILE* f, OptimizationUnit* code_ptr, int* pinc, int* pnumline);

/* -------------------------------------------------------- */

#if defined(__i386__)
  #define TARGET_HOST_CPU TARGET_I386
#elif defined(__x86_64__)
  #define TARGET_HOST_CPU TARGET_X86_64
#elif defined(__powerpc__)
  #define TARGET_HOST_CPU TARGET_PPC
#elif defined(__arm__)
  #define TARGET_HOST_CPU TARGET_ARM
#elif defined(__sparc__)
  #define TARGET_HOST_CPU TARGET_SPARC
#elif defined(__mips__) && defined(__MIPSEB__)
  #define TARGET_HOST_CPU TARGET_MIPS
#elif defined(__mips__) && defined(__MIPSEL__)
  #define TARGET_HOST_CPU TARGET_MIPSEL
#endif

#if defined(TARGET_HOST_CPU)
  #define SUPPORT_EXECUTE_COMPILED_CODE
#endif

#endif
