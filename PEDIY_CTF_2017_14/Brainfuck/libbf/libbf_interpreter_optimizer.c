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

#include "libbf_config.h"

#ifdef PROFILE
  #define PROFILING() ++count_instr; code_ptr->ou_nb_visits++;
#else
  #define PROFILING()
#endif

#if defined(__GNUC__)
  #define GOTO_ADDR_SUPPORTED
#endif

#if defined(DEBUG_INTERPRETER)
  #define DEAL_INSTR() current_code_ptr = code_ptr; offset = code_ptr->ou_offset; PROFILING();
  #if defined(GOTO_ADDR_SUPPORTED)
    #define SAFE_JUMP_TO(x) do { libbf_debug_func(sizeof(MY_TYPE_T), current_code_ptr, void_data_ptr, data_ptr); goto *(x); } while(0)
  #else
    #define SAFE_JUMP_TO(x) libbf_debug_func(sizeof(MY_TYPE_T), current_code_ptr, void_data_ptr, data_ptr); break
  #endif
  #define JUMP_TO(x)  SAFE_JUMP_TO(x)
#else
  #define DEAL_INSTR() offset = code_ptr->ou_offset; PROFILING();
  #if defined(GOTO_ADDR_SUPPORTED)
    #define SAFE_JUMP_TO(x)   goto *(x)
    #if !defined(ONLY_SAFE_JUMP) && !defined(PROFILE)
      #if defined(__i386__) || defined(__x86_64__)
        #define UNSAFE_JUMP_TO(x) __asm__ __volatile__ ("jmp *%0": : "m"(x));
      #elif defined(__arm__)
        #define UNSAFE_JUMP_TO(x) __asm__ __volatile__ ("ldr pc, %0" : : "m"(x));
      #elif defined(__powerpc__)
        #define UNSAFE_JUMP_TO(x) __asm__ __volatile__ ("lwz 18, %0; mtctr 18; bctr" : : "m"(x) : "18");
      #elif defined(__sparc__)
        #define UNSAFE_JUMP_TO(x) __asm__ __volatile__ ("ld %0, %%g2; jmp %%g2; nop;" : : "m"(x) : "%g2");
      #elif defined(__mips__)
        #define UNSAFE_JUMP_TO(x) __asm__ __volatile__ ("lw $5, %0; nop; jr $5; nop;" : : "m"(x) : "$5");
      #else
        #define UNSAFE_JUMP_TO(x)  SAFE_JUMP_TO(x)
      #endif
    #else
      #define UNSAFE_JUMP_TO(x)  SAFE_JUMP_TO(x)
    #endif
    #define JUMP_TO(x)  UNSAFE_JUMP_TO(x)
  #else
    #define SAFE_JUMP_TO(x) break;
    #define JUMP_TO(x) break;
  #endif
#endif

#if defined(GOTO_ADDR_SUPPORTED)
  #define SAFE_NEXT_INSTR()   code_ptr++;  SAFE_JUMP_TO(code_ptr->ou_interpreter_addr);
  #define NEXT_INSTR()        code_ptr++;  JUMP_TO(code_ptr->ou_interpreter_addr);
  #ifdef PRECISE_DEBUG
    #define LABEL(x) label_##x: libbf_interpreter_optimizer_cdump_internal_instr(stderr, code_ptr, NULL, NULL);
  #else
    #define LABEL(x) label_##x: 
  #endif
#else
  #define SAFE_NEXT_INSTR()   code_ptr++; break;
  #define NEXT_INSTR()        code_ptr++; break;
  #ifdef PRECISE_DEBUG
    #define LABEL(x) case x: libbf_interpreter_optimizer_cdump_internal_instr(stderr, code_ptr, NULL, NULL);
  #else
    #define LABEL(x) case x: 
  #endif
#endif

#if defined(PROFILE)
  #define LIBBF_INTERPRETER_OPTIMIZER_RADIX   libbf_interpreter_optimizer_profile_
#elif defined(ONLY_SAFE_JUMP)
  #define LIBBF_INTERPRETER_OPTIMIZER_RADIX   libbf_interpreter_optimizer_safe_jump_
#else
  #define LIBBF_INTERPRETER_OPTIMIZER_RADIX   libbf_interpreter_optimizer_
#endif

#define DECLARE_LABEL(x) CONCAT(__label__ label_, x)
#ifdef __cplusplus
  #define FILL_TAB_ADDR(x) tab_addr[x] = &&label_##x;
#else
  #define FILL_TAB_ADDR(x) [x] = &&label_##x,
#endif

#if !defined(PROFILE) && !defined(ONLY_SAFE_JUMP)
static unsigned MY_TYPE_T CONCAT(libbf_compute_expr_,MY_TYPE_T_STR) (Expression* expr, unsigned MY_TYPE_T* data_ptr)
{
  switch(expr->e_type)
  {
    case EXPR_CONSTANT:
      return expr->e_constant;
      break;
      
    case EXPR_DATA:
      return data_ptr[expr->e_offset];
      break;
      
    case EXPR_ADD:
      return CONCAT(libbf_compute_expr_,MY_TYPE_T_STR)(expr->e_expr1, data_ptr) +
             CONCAT(libbf_compute_expr_,MY_TYPE_T_STR)(expr->e_expr2, data_ptr);
      break;
      
    case EXPR_MUL:
      return CONCAT(libbf_compute_expr_,MY_TYPE_T_STR)(expr->e_expr1, data_ptr) *
             CONCAT(libbf_compute_expr_,MY_TYPE_T_STR)(expr->e_expr2, data_ptr);
      break;

    default:
      SHOULDNT_HAPPEN();
      return 0;
      break;
  }
}
#endif

static void CONCAT(CONCAT(LIBBF_INTERPRETER_OPTIMIZER_RADIX, internal_), MY_TYPE_T_STR)
             (OptimizationUnit* code, OFFSET_t min_delta_offset, OFFSET_t max_delta_offset)
{
#if defined(GOTO_ADDR_SUPPORTED)
  DECLARE_LABEL(BF_AFFECT_EXPR);
  DECLARE_LABEL(BF_INC_DATA_PTR);
  DECLARE_LABEL(BF_PRINT_DATA);
  DECLARE_LABEL(BF_READ_DATA);
  DECLARE_LABEL(BF_WHILE_DATA);
  DECLARE_LABEL(BF_END_WHILE);
  DECLARE_LABEL(BF_END);
#ifndef PROFILE  
  DECLARE_LABEL(BF_SET_CST);
  DECLARE_LABEL(BF_SET_CST_0);
  DECLARE_LABEL(BF_INC_CST);
  DECLARE_LABEL(BF_INC_CST_1);
  DECLARE_LABEL(BF_INC_CST_M_1);
  DECLARE_LABEL(BF_SET_DATA);
  DECLARE_LABEL(BF_INC_DATA);
  DECLARE_LABEL(BF_DEC_DATA);
  DECLARE_LABEL(BF_INC_CST_MUL_DATA);
  DECLARE_LABEL(BF_INC_CST_ADD_DATA);
  DECLARE_LABEL(BF_INC_DATA_MUL_DATA);
  DECLARE_LABEL(BF_WHILE_INC_DATA_PTR);
#ifdef USE_END_WHILE_INC_DATA_PTR
  DECLARE_LABEL(BF_END_WHILE_INC_DATA_PTR);
#endif
#else
  DECLARE_LABEL(BF_NOP);
#endif  
  DECLARE_LABEL(BF_DYNALLOC_REALLOC_NEEDED);

#ifdef __cplusplus
  static const void* tab_addr[BF_COUNT_INSTR];
#else
  static const void* tab_addr[BF_COUNT_INSTR] =
  {
#endif
    FILL_TAB_ADDR(BF_AFFECT_EXPR)
    FILL_TAB_ADDR(BF_INC_DATA_PTR)
    FILL_TAB_ADDR(BF_PRINT_DATA)
    FILL_TAB_ADDR(BF_READ_DATA)
    FILL_TAB_ADDR(BF_WHILE_DATA)
    FILL_TAB_ADDR(BF_END_WHILE)
    FILL_TAB_ADDR(BF_END)
#ifndef PROFILE      
    FILL_TAB_ADDR(BF_SET_CST)
    FILL_TAB_ADDR(BF_SET_CST_0)
    FILL_TAB_ADDR(BF_INC_CST)
    FILL_TAB_ADDR(BF_INC_CST_1)
    FILL_TAB_ADDR(BF_INC_CST_M_1)
    FILL_TAB_ADDR(BF_SET_DATA)
    FILL_TAB_ADDR(BF_INC_DATA)
    FILL_TAB_ADDR(BF_DEC_DATA)
    FILL_TAB_ADDR(BF_INC_CST_MUL_DATA)
    FILL_TAB_ADDR(BF_INC_CST_ADD_DATA)
    FILL_TAB_ADDR(BF_INC_DATA_MUL_DATA)
    FILL_TAB_ADDR(BF_WHILE_INC_DATA_PTR)
#ifdef USE_END_WHILE_INC_DATA_PTR
    FILL_TAB_ADDR(BF_END_WHILE_INC_DATA_PTR)
#endif
#else
    FILL_TAB_ADDR(BF_NOP)
#endif  
    FILL_TAB_ADDR(BF_DYNALLOC_REALLOC_NEEDED)
#ifndef __cplusplus
  };
#endif
#endif

  void* void_data_ptr;
#if defined(DEBUG_INTERPRETER) || defined(__CYGWIN__) || defined(PROFILE)
  void* ori_data_ptr;
  unsigned MY_TYPE_T* data_ptr;
  void* current_code_ptr;
#else  
  void* old_action;
  register unsigned MY_TYPE_T* data_ptr;
#endif
  register OptimizationUnit* code_ptr;
  
#ifdef PROFILE
  long long count_instr = 0;
#endif
  register OFFSET_t offset;
  
#if defined(GOTO_ADDR_SUPPORTED)
  libbf_optimized_code_resolve_interpeter_addr(code, tab_addr);
#endif

#if defined(DEBUG_INTERPRETER) || defined(__CYGWIN__) || defined(PROFILE)
  void_data_ptr = alloc0(MY_TYPE_T, 32768);
  ori_data_ptr = void_data_ptr;
  data_ptr = ((unsigned MY_TYPE_T*)void_data_ptr) + 1024;
  void_data_ptr = data_ptr;
  current_code_ptr = NULL;
#else  
  libbf_interpreter_dynalloc_init(min_delta_offset,
                             max_delta_offset,
                             sizeof(MY_TYPE_T),
                             code,
#if defined(GOTO_ADDR_SUPPORTED)
                             tab_addr[BF_DYNALLOC_REALLOC_NEEDED],
#else
                             NULL,
#endif
                             &old_action,
                             &void_data_ptr);
  data_ptr = (unsigned MY_TYPE_T*)void_data_ptr;
#endif
  code_ptr = code;

#if defined(GOTO_ADDR_SUPPORTED)
  /* Start up execution ! */
  SAFE_JUMP_TO(code_ptr->ou_interpreter_addr); /* safe jump to begin... */
#else
  while(1)
  {
    switch(code_ptr->ou_instr)
    {
#endif
LABEL(BF_AFFECT_EXPR)
  DEAL_INSTR();
  data_ptr[offset] = CONCAT(libbf_compute_expr_,MY_TYPE_T_STR)(code_ptr->ou_expr, data_ptr);
  SAFE_NEXT_INSTR(); /* safe because of function call */

LABEL(BF_INC_DATA_PTR)
  DEAL_INSTR();
  data_ptr += offset;
  NEXT_INSTR();

LABEL(BF_PRINT_DATA)
  DEAL_INSTR();
  putchar(data_ptr[offset]);
  SAFE_NEXT_INSTR(); /* safe because of function call */

LABEL(BF_READ_DATA)
  DEAL_INSTR();
  data_ptr[offset] = getchar();
  SAFE_NEXT_INSTR(); /* safe because of function call */

LABEL(BF_WHILE_DATA)
  DEAL_INSTR();
  if (data_ptr[offset] == 0)
  {
    code_ptr = code_ptr->ou_jump_to;
    JUMP_TO(code_ptr->ou_interpreter_addr);
  }
  NEXT_INSTR();

LABEL(BF_END_WHILE)
  DEAL_INSTR();
  if (data_ptr[offset] != 0)
  {
    code_ptr = code_ptr->ou_jump_to;
    JUMP_TO(code_ptr->ou_interpreter_addr);
  }
  NEXT_INSTR();

#ifndef PROFILE
/* Very common subcases of BF_AFFECT_EXPR, so much more efficient to create */
/* dedicated opcodes */

LABEL(BF_SET_CST)
  DEAL_INSTR();
  data_ptr[offset] = code_ptr->ou_constant;
  NEXT_INSTR();

LABEL(BF_SET_CST_0)
  DEAL_INSTR();
  data_ptr[offset] = 0;
  NEXT_INSTR();

LABEL(BF_INC_CST)
  DEAL_INSTR();
  data_ptr[offset] += code_ptr->ou_constant;
  NEXT_INSTR();

LABEL(BF_INC_CST_1)
  DEAL_INSTR();
  data_ptr[offset] ++;
  NEXT_INSTR();

LABEL(BF_INC_CST_M_1)
  DEAL_INSTR();
  data_ptr[offset] --;
  NEXT_INSTR();

LABEL(BF_SET_DATA)
  DEAL_INSTR();
  data_ptr[offset] = data_ptr[code_ptr->ou_offset2];
  NEXT_INSTR();

LABEL(BF_INC_DATA)
  DEAL_INSTR();
  data_ptr[offset] += data_ptr[code_ptr->ou_offset2];
  NEXT_INSTR();

LABEL(BF_DEC_DATA)
  DEAL_INSTR();
  data_ptr[offset] -= data_ptr[code_ptr->ou_offset2];
  NEXT_INSTR();

LABEL(BF_INC_CST_ADD_DATA)
  DEAL_INSTR();
  data_ptr[offset] += code_ptr->ou_constant + data_ptr[code_ptr->ou_offset2];
  NEXT_INSTR();

LABEL(BF_INC_CST_MUL_DATA)
  DEAL_INSTR();
  data_ptr[offset] += code_ptr->ou_constant * data_ptr[code_ptr->ou_offset2];
  NEXT_INSTR();

LABEL(BF_INC_DATA_MUL_DATA)
  DEAL_INSTR();
  data_ptr[offset] += data_ptr[code_ptr->ou_offset2] * data_ptr[code_ptr->ou_offset3];
  NEXT_INSTR();

#ifdef USE_END_WHILE_INC_DATA_PTR
LABEL(BF_END_WHILE_INC_DATA_PTR)
  DEAL_INSTR();
  data_ptr += code_ptr->ou_offset2;
  if (data_ptr[offset] != 0)
  {
    code_ptr = code_ptr->ou_jump_to;
    JUMP_TO(code_ptr->ou_interpreter_addr);
  }
  NEXT_INSTR();
#endif

#define INC_PTR_WITH_BYTES(ptr, bytes)   ptr = (unsigned MY_TYPE_T*)(((long)ptr) + bytes)

LABEL(BF_WHILE_INC_DATA_PTR)
{
  register int offset2_bytes;
  DEAL_INSTR();
  offset2_bytes = sizeof(MY_TYPE_T) * code_ptr->ou_offset2;
  data_ptr += offset;
  while (*data_ptr)
  {
    INC_PTR_WITH_BYTES(data_ptr, offset2_bytes);
  }
  data_ptr -= offset;
  SAFE_NEXT_INSTR();
}
#else
LABEL(BF_NOP)
  DEAL_INSTR();
  NEXT_INSTR();

#endif

LABEL(BF_END)
#if !(defined(DEBUG_INTERPRETER)|| defined(__CYGWIN__) || defined(PROFILE))
  libbf_interpreter_dynalloc_end(old_action, data_ptr);
#else
  free(ori_data_ptr);
#endif
  #ifdef PROFILE
    libbf_dump_profiled_code(code);
    fprintf(stderr, "normal exit : %lld instructions executed\n", count_instr);
  #endif
  return;

LABEL(BF_DYNALLOC_REALLOC_NEEDED)
{
  long shift = libbf_interpreter_dynalloc_realloc(data_ptr);
  data_ptr += shift;
  if (shift == 0)
  {
    #if !(defined(DEBUG_INTERPRETER)|| defined(__CYGWIN__) || defined(PROFILE))
      libbf_interpreter_dynalloc_end(old_action, data_ptr);
    #else
      free(ori_data_ptr);
    #endif
    #ifdef PROFILE
      fprintf(stderr, "abnormal exit (not enough memory) : %lld instructions executed\n", count_instr);
    #endif
    return;
  }
  SAFE_JUMP_TO(code_ptr->ou_interpreter_addr); /* safe because of function call */
}

#if !defined(GOTO_ADDR_SUPPORTED)
      default: SHOULDNT_HAPPEN();
    }
  }
#endif

}

void CONCAT(LIBBF_INTERPRETER_OPTIMIZER_RADIX, MY_TYPE_T_STR) (const char* source_code)
{
  libbf_interpreter_optimizer_general_internal(
    source_code,
    (void*)CONCAT(CONCAT(LIBBF_INTERPRETER_OPTIMIZER_RADIX, internal_), MY_TYPE_T_STR),
#ifdef PROFILE
    TRUE
#else
    FALSE
#endif
  );
}


#undef PROFILING
#undef DEAL_INSTR
#undef NEXT_INSTR
#undef UNSAFE_JUMP_TO
#undef LIBBF_INTERPRETER_OPTIMIZER_RADIX

#if !defined(PROFILE) && !defined(ONLY_SAFE_JUMP)
  #define PROFILE
  #include "libbf_interpreter_optimizer.c"
  #undef PROFILE
  #define ONLY_SAFE_JUMP
  #include "libbf_interpreter_optimizer.c"
  #undef ONLY_SAFE_JUMP
#endif
