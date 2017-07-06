/***************************************************************************
        libbf - Free BrainFuck interpreter, optimizer, compiler library
                            -------------------
    copyright            : (C) 2005-2006 by Even Rouault
    email                : even 'dot' rouault 'at' mines-paris 'dot' org
***************************************************************************/

#ifndef NO_COMPILER
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

#include <stdlib.h>
#include <string.h>

#include "libbf_internal.h"
#include "libbf_compiler.h"
#include "libbf_compiler_dynalloc.h"
#include "libbf_posix_emulation.h"

#if defined(COMPILER_FOR_ALL_TARGET)
  #define COMPILER_FOR_I386_X86_64_TARGET
  #define COMPILER_FOR_PPC_TARGET
  #define COMPILER_FOR_ARM_TARGET
  #define COMPILER_FOR_SPARC_TARGET
  #define COMPILER_FOR_MIPS_TARGET
#else
  #if defined(__i386__)
    #define COMPILER_FOR_I386_X86_64_TARGET
  #elif defined(__x86_64__)
    #define COMPILER_FOR_I386_X86_64_TARGET
  #elif defined(__powerpc__)
    #define COMPILER_FOR_PPC_TARGET
  #elif defined(__arm__)
    #define COMPILER_FOR_ARM_TARGET
  #elif defined(__sparc__)
    #define COMPILER_FOR_SPARC_TARGET
  #elif defined(__mips__)
    #define COMPILER_FOR_MIPS_TARGET
  #endif
#endif

typedef struct
{
  TargetCpu_t         cpu;
  int                 sizeofData;
  OptimizationUnit**  p_stack_while;
  char*               compiled_code;
  int                 allocated_size;
  int                 filled_size;
  int                 generate_debug;
  int                 min_delta_offset;
  int                 max_delta_offset;
  
  int                 index_register; /* for i386 */
  int                 sizeofLong; /* size of target long */
} AsmGeneration_t;

#define NWORDS(asmGeneration) (asmGeneration->sizeofData / asmGeneration->sizeofLong)

static void libbf_gen_asm_debug_code(AsmGeneration_t* asmGeneration, OptimizationUnit* current_code_ptr);

static int is_le()
{
#if defined(__LITTLE_ENDIAN__)
  return TRUE;
#elif defined(__BIG_ENDIAN__)
  return FALSE;
#else
  static int _is_init = FALSE;
  static int _is_le;
  if (!_is_init)
  {
    short s = 0x1234;
    _is_le = ((*(char*)&s) == 0x34);
    _is_init = TRUE;
  }
  return _is_le;
#endif
}

static short short_swap(unsigned short s)
{
  return ((s & 0xFF00) >> 8) | ((s & 0xFF) << 8);
}

static short short_to_le(short s)
{
  if (is_le()) return s; else return short_swap(s);
}

static short short_to_be(short s)
{
  if (!is_le()) return s; else return short_swap(s);
}


static int int_swap(unsigned int i)
{
  return (((i & 0xFF000000) >> 24) | ((i & 0x00FF0000) >> 8) | ((i & 0x0000FF00) << 8) | (((i & 0x000000FF) << 24)));
}

static int int_to_le(int i)
{
  if (is_le()) return i; else return int_swap(i);
}

static int int_to_be(int i)
{
  if (!is_le()) return i; else return int_swap(i);
}

static void libbf_check_asm_size(AsmGeneration_t* asmGeneration, int data_size)
{
  if (asmGeneration->filled_size + data_size >= asmGeneration->allocated_size)
  {
    asmGeneration->allocated_size = asmGeneration->allocated_size * 2 + data_size;
    asmGeneration->compiled_code = (char*)realloc(asmGeneration->compiled_code, asmGeneration->allocated_size);
  }
}

static void libbf_write_asm_byte(AsmGeneration_t* asmGeneration, char data)
{
  libbf_check_asm_size(asmGeneration, sizeof(data));
  *(char*)(asmGeneration->compiled_code + asmGeneration->filled_size) = data;
  asmGeneration->filled_size += sizeof(data);
}

static void libbf_write_asm_4byte(AsmGeneration_t* asmGeneration, char data0, char data1, char data2, char data3)
{
  libbf_check_asm_size(asmGeneration, 4);
  *(char*)(asmGeneration->compiled_code + asmGeneration->filled_size+0) = data0;
  *(char*)(asmGeneration->compiled_code + asmGeneration->filled_size+1) = data1;
  *(char*)(asmGeneration->compiled_code + asmGeneration->filled_size+2) = data2;
  *(char*)(asmGeneration->compiled_code + asmGeneration->filled_size+3) = data3;
  asmGeneration->filled_size += 4;
}


static void libbf_write_asm_short(AsmGeneration_t* asmGeneration, short data)
{
  libbf_check_asm_size(asmGeneration, sizeof(data));
  *(short*)(asmGeneration->compiled_code + asmGeneration->filled_size) = data;
  asmGeneration->filled_size += sizeof(data);
}

static void libbf_write_asm_int(AsmGeneration_t* asmGeneration, int data)
{
  libbf_check_asm_size(asmGeneration, sizeof(data));
  *(int*)(asmGeneration->compiled_code + asmGeneration->filled_size) = data;
  asmGeneration->filled_size += sizeof(data);
}

#if defined(__x86_64__)
static void libbf_write_asm_long(AsmGeneration_t* asmGeneration, long data)
{
  libbf_check_asm_size(asmGeneration, sizeof(data));
  *(long*)(asmGeneration->compiled_code + asmGeneration->filled_size) = data;
  asmGeneration->filled_size += sizeof(data);
}
#endif

static int libbf_is_composed_poweroftwo(unsigned int number, int* shift1, int* shift2)
{
  int i = 0;
  int count = 0;
  if (number == 0) return 0;
  if (shift1) *shift1 = 0;
  if (shift2) *shift2 = 0;
  while(number)
  {
    if (number & 1)
    {
      count ++;
      if (count == 1)
      {
        if (shift1) *shift1 = i;
      }
      else if (count == 2)
      {
        if (shift2) *shift2 = i;
      }
      else
      {
        return 0;
      }
    }
    i++;
    number >>= 1;
  }
  return 1;
}

static void libbf_gen_asm(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr);
    
static void libbf_gen_asm_default(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  switch(code_ptr->ou_instr)
  {
    case BF_AFFECT_EXPR:
    {
      SHOULDNT_HAPPEN();

      break;
    }
        
    case BF_INC_CST_ADD_DATA :/* data_ptr[offset] += cst + data_ptr[offset2] */
    {
      OptimizationUnit instr1, instr2;
      instr1 = *code_ptr;
      instr1.ou_instr = BF_INC_CST;
      instr2 = *code_ptr;
      instr2.ou_instr = BF_INC_DATA;
      libbf_gen_asm(asmGeneration, &instr1);
      libbf_gen_asm(asmGeneration, &instr2);

      break;
    }
        
    case BF_WHILE_INC_DATA_PTR: /* while(data_ptr[offset]) data_ptr += offset2; */
    {
      OptimizationUnit instr1, instr2, instr3;
      int generate_debug = asmGeneration->generate_debug;
      instr1.ou_instr = BF_WHILE_DATA;
      instr1.ou_offset = code_ptr->ou_offset;
      instr1.ou_loop_type = LIBBF_WHILE;
      instr2.ou_instr = BF_INC_DATA_PTR;
      instr2.ou_offset = code_ptr->ou_offset2;
      instr3.ou_instr = BF_END_WHILE;
      instr3.ou_offset = code_ptr->ou_offset;
      instr3.ou_loop_type = LIBBF_WHILE;
      asmGeneration->generate_debug = FALSE;
      libbf_gen_asm(asmGeneration, &instr1);
      libbf_gen_asm(asmGeneration, &instr2);
      libbf_gen_asm(asmGeneration, &instr3);
      asmGeneration->generate_debug = generate_debug;

      break;
    }
    
    default:
      libbf_interpreter_optimizer_cdump_internal_instr(stderr, code_ptr, NULL, NULL);
      SHOULDNT_HAPPEN();
  }
}


static int libbf_compiler_is_cpu_little_endian(TargetCpu_t cpu)
{
  return (cpu == TARGET_I386 || cpu == TARGET_X86_64 || cpu == TARGET_ARM || cpu == TARGET_MIPSEL);
}

static int libbf_compiler_compute_longlong_offset(AsmGeneration_t* asmGeneration, int offset, int suboffset)
{
  if (libbf_compiler_is_cpu_little_endian(asmGeneration->cpu))
    return offset * asmGeneration->sizeofData + suboffset * asmGeneration->sizeofLong;
  else
    return offset * asmGeneration->sizeofData + (NWORDS(asmGeneration)-1 - suboffset) * asmGeneration->sizeofLong;
}

void libbf_debug_func(int sizeofData, OptimizationUnit* current_code_ptr, void* base, void* current);

typedef struct
{
  void (*gen_preambule)(AsmGeneration_t* asmGeneration);
  void (*gen_epilogue)(AsmGeneration_t* asmGeneration);
  void (*gen_code)(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr);
  void (*gen_code_longlong)(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr);
  void (*gen_debug_code)(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr);
} AsmGenerationFuncs_t;

static AsmGenerationFuncs_t asmGenerationFuncs[TARGET_COUNT];

#if defined(COMPILER_FOR_I386_X86_64_TARGET)
#include "libbf_compiler_i386_x86_64.c"
#endif

#if defined(COMPILER_FOR_PPC_TARGET)
#include "libbf_compiler_ppc.c"
#endif

#if defined(COMPILER_FOR_ARM_TARGET)
#include "libbf_compiler_arm.c"
#endif

#if defined(COMPILER_FOR_SPARC_TARGET)
#include "libbf_compiler_sparc.c"
#endif

#if defined(COMPILER_FOR_MIPS_TARGET)
#include "libbf_compiler_mips.c"
#endif

static void libbf_gen_asm_init_funcs()
{
  static int hasInit = FALSE;
  if (!hasInit)
  {
    memset(&asmGenerationFuncs, 0, sizeof(asmGenerationFuncs));
    
    #if defined(COMPILER_FOR_I386_X86_64_TARGET)
      libbf_gen_asm_i386_x86_64_init_funcs();
    #endif
    #if defined(COMPILER_FOR_PPC_TARGET)
      libbf_gen_asm_ppc_init_funcs();
    #endif
    #if defined(COMPILER_FOR_ARM_TARGET)
      libbf_gen_asm_arm_init_funcs();
    #endif
    #if defined(COMPILER_FOR_SPARC_TARGET)
      libbf_gen_asm_sparc_init_funcs();
    #endif
    #if defined(COMPILER_FOR_MIPS_TARGET)
      libbf_gen_asm_mips_init_funcs();
    #endif
    hasInit = TRUE;
  }
}

static void libbf_gen_asm(AsmGeneration_t* asmGeneration, OptimizationUnit* code_ptr)
{
  if (NWORDS(asmGeneration) >= 2)
    asmGenerationFuncs[asmGeneration->cpu].gen_code_longlong(asmGeneration, code_ptr);
  else
    asmGenerationFuncs[asmGeneration->cpu].gen_code(asmGeneration, code_ptr);
}

static void libbf_gen_asm_debug_code(AsmGeneration_t* asmGeneration, OptimizationUnit* current_code_ptr)
{
  asmGenerationFuncs[asmGeneration->cpu].gen_debug_code(asmGeneration, current_code_ptr);
}

void libbf_gen_compile_internal_from_code_array(
    TargetCpu_t cpu,
    int sizeofData, 
    OptimizationUnit* code,
    int min_delta_offset,
    int max_delta_offset,
    char** compiled_code,
    int* size_compiled_code,
    int debug_compiled_code)
{
  
  OptimizationUnit* stack_while[32768];
  OptimizationUnit* code_ptr;
  AsmGeneration_t asmGeneration;
  asmGeneration.cpu = cpu;
  asmGeneration.sizeofData = sizeofData;
  asmGeneration.p_stack_while = stack_while;
  asmGeneration.compiled_code = NULL;
  asmGeneration.allocated_size = 0;
  asmGeneration.filled_size = 0;
  asmGeneration.generate_debug = debug_compiled_code;
  asmGeneration.min_delta_offset = min_delta_offset;
  asmGeneration.max_delta_offset = max_delta_offset;
  asmGeneration.sizeofLong = (cpu == TARGET_X86_64) ? 8 : 4;
  
  code_ptr = code;
  
  libbf_gen_asm_init_funcs();

  asmGenerationFuncs[cpu].gen_preambule(&asmGeneration);
  
  if (debug_compiled_code)
    libbf_gen_asm_debug_code(&asmGeneration, NULL);
  
  while(code_ptr->ou_instr != BF_END)
  {
    libbf_gen_asm(&asmGeneration, code_ptr);
    
    if (debug_compiled_code)
    {    
      if (!(code_ptr->ou_instr == BF_WHILE_DATA || code_ptr->ou_instr == BF_END_WHILE))
        libbf_gen_asm_debug_code(&asmGeneration, code_ptr);
    }

    code_ptr++;
  }
  asmGenerationFuncs[cpu].gen_epilogue(&asmGeneration);
   
  *compiled_code = asmGeneration.compiled_code;
  *size_compiled_code = asmGeneration.filled_size;
}

static void libbf_debug_func_char(OptimizationUnit* current_code_ptr, char* base, char* current)
{
  /* fprintf(stderr, "base=%lX current=%lX\n", (long)base, (long)current); */
  int i;
  fprintf(stderr, "current-base=%d\n", (int)(current - base));
  
  for(i=0;i<100;i++)
  {
    if (base + i == current) fprintf(stderr, "(*)");
    fprintf(stderr, "%d ", base[i]);
  }
  fprintf(stderr, "\n");
  if (current_code_ptr && current_code_ptr->ou_instr != BF_INC_DATA_PTR)
    fprintf(stderr, "at offset %d = %d\n", current_code_ptr->ou_offset, current[current_code_ptr->ou_offset]);
}

static void libbf_debug_func_short(OptimizationUnit* current_code_ptr, short* base, short* current)
{
  /* fprintf(stderr, "base=%lX current=%lX\n", (long)base, (long)current); */
  int i;
  fprintf(stderr, "current-base=%d\n", (int)(current - base));
  for(i=0;i<100;i++)
  {
    if (base + i == current) fprintf(stderr, "(*)");
    fprintf(stderr, "%d ", base[i]);
  }
  fprintf(stderr, "\n");
  if (current_code_ptr && current_code_ptr->ou_instr != BF_INC_DATA_PTR)
    fprintf(stderr, "at offset %d = %d\n", current_code_ptr->ou_offset, current[current_code_ptr->ou_offset]);
}

static void libbf_debug_func_int(OptimizationUnit* current_code_ptr, int* base, int* current)
{
  /* fprintf(stderr, "base=%lX current=%lX\n", (long)base, (long)current); */
  int i;
  fprintf(stderr, "current-base=%d\n", (int)(current - base));
  for(i=0;i<100;i++)
  {
    if (base + i == current) fprintf(stderr, "(*)");
    fprintf(stderr, "%d ", base[i]);
  }
  fprintf(stderr, "\n");
  if (current_code_ptr && current_code_ptr->ou_instr != BF_INC_DATA_PTR)
    fprintf(stderr, "at offset %d = %d\n", current_code_ptr->ou_offset, current[current_code_ptr->ou_offset]);
}

static void libbf_debug_func_long_long(OptimizationUnit* current_code_ptr, long long* base, long long* current)
{
  /* fprintf(stderr, "base=%lX current=%lX\n", (long)base, (long)current); */
  int i;
  fprintf(stderr, "current-base=%d\n", (int)(current - base));
  for(i=0;i<100;i++)
  {
    if (base + i == current) fprintf(stderr, "(*)");
    fprintf(stderr, "%Ld ", base[i]);
  }
  fprintf(stderr, "\n");
  if (current_code_ptr && current_code_ptr->ou_instr != BF_INC_DATA_PTR)
    fprintf(stderr, "at offset %d = %Ld\n", current_code_ptr->ou_offset, current[current_code_ptr->ou_offset]);

}

static void libbf_debug_verybig_number(void* ptr, int offset, int sizeofData)
{
  long long firstLong, tmp;
  int showOtherLong = 0;
  int i;
  fprintf(stderr, "at offset %d = ", offset);
  firstLong = *(long long*)((long)ptr + sizeofData * offset + 0 * 8);
  fprintf(stderr, "%Ld ", firstLong);
  for(i=1;i<sizeofData/8;i++)
  {
    tmp = *(long long*)((long)ptr + sizeofData * offset + i * 8);
    if (firstLong >= 0)
    {
      if (tmp != 0) showOtherLong = 1;
    }
    else
    {
      if (tmp != -1) showOtherLong = 1;
    }
  }
  if (showOtherLong)
  {
    for(i=1;i<sizeofData/8;i++)
    {
      tmp = *(long long*)((long)ptr + sizeofData * offset + i * 8);
      fprintf(stderr, "%Ld ", tmp);
    }
  }
  fprintf(stderr, "\n");
}

void libbf_debug_func(int sizeofData, OptimizationUnit* current_code_ptr, void* base, void* current)
{
  static int count = 0;
  int can_debug = 1; /* ( (count % 100000000) == 0); */
  if (can_debug) fprintf(stderr, "count = %d\n", count);
  count++;
  
  if (current_code_ptr && can_debug)
  {
    if (current_code_ptr->ou_instr == BF_END_WHILE)
      return;
    
    libbf_interpreter_optimizer_cdump_internal_instr(stderr, current_code_ptr, NULL, NULL);
  }
  
  if (sizeofData == 1)
  {
    libbf_debug_func_char(current_code_ptr, (char*)base, (char*)current);
  }
  else if (sizeofData == 2)
  {
    libbf_debug_func_short(current_code_ptr, (short*)base, (short*)current);
  }
  else if (sizeofData == 4)
  {
    libbf_debug_func_int(current_code_ptr, (int*)base, (int*)current);
  }
  else if (sizeofData == 8)
  {
    libbf_debug_func_long_long(current_code_ptr, (long long*)base, (long long*)current);
  }
  else if (can_debug)
  {
    void* ptr = base;
    fprintf(stderr, "current-base=%d\n", (int)((long)current - (long)base) / sizeofData);
    if (current_code_ptr && current_code_ptr->ou_instr != BF_INC_DATA_PTR)
      ptr = (void*)((long)base + sizeofData * (current_code_ptr->ou_offset - 50));
    if (ptr < base) ptr = base;
    /*for(i=0;i<100;i++)
    {
      if (ptr + i * sizeofData == current) fprintf(stderr, "(*)");
      fprintf(stderr, "%Ld ", *(long long*)(ptr + sizeofData * i));
    }
    fprintf(stderr, "\n");*/
    if (current_code_ptr && current_code_ptr->ou_instr != BF_INC_DATA_PTR)
    {
      libbf_debug_verybig_number(current, current_code_ptr->ou_offset, sizeofData);
      if (current_code_ptr->ou_instr == BF_INC_CST_MUL_DATA)
        libbf_debug_verybig_number(current, current_code_ptr->ou_offset2, sizeofData);
    }
  }

}

static void libbf_just_execute_compiled_code_generic_internal(void* executable_code,
                                                              ExecutableCodeData_t* executableCodeDataPtr)
{
    
#if defined(WIN32)
  AsmGeneration_t asmGeneration;
  asmGeneration.cpu = TARGET_I386;
  asmGeneration.sizeofData = 4;
  asmGeneration.p_stack_while = NULL;
  asmGeneration.compiled_code = NULL;
  asmGeneration.allocated_size = 0;
  asmGeneration.filled_size = 0;
  asmGeneration.generate_debug = FALSE;
  asmGeneration.sizeofLong = 4;
  libbf_compiler_i386_add_win32_glue(&asmGeneration, executable_code);
  
  long pagesize = libbf_getpagesize();
  int aligned_glue_executable_code_size = (asmGeneration.filled_size + pagesize - 1) & (~(pagesize - 1));
  char* glue_executable_code = libbf_alloc_aligned(pagesize, aligned_glue_executable_code_size);
  libbf_mprotect(glue_executable_code, aligned_glue_executable_code_size, PROT_WRITE | PROT_READ | PROT_EXEC);
  memcpy(glue_executable_code, asmGeneration.compiled_code, asmGeneration.filled_size);
  free(asmGeneration.compiled_code);
  
  ((void (*)(void*))glue_executable_code)(executableCodeDataPtr);
  
  libbf_mprotect(glue_executable_code, aligned_glue_executable_code_size, PROT_WRITE | PROT_READ);
  libbf_free_aligned(glue_executable_code);
#else
  ((void (*)(void*))executable_code)(executableCodeDataPtr);
#endif
}

static int libbf_getchar_with_data(void* user_data)
{
  int ret;
#if defined(SUPPORT_COMPILER_DYNALLOC)
  if (user_data != NULL)
  {
    libbf_compiler_dynalloc_toggle_suspend_or_resume(user_data);
  }
#endif
  ret = getchar();
#if defined(SUPPORT_COMPILER_DYNALLOC)
  if (user_data != NULL)
  {
    libbf_compiler_dynalloc_toggle_suspend_or_resume(user_data);
  }
#endif
  return ret;
}

static int libbf_putchar_with_data(int data, void* user_data)
{
  int ret;
  ret = putchar(data);
  return ret;
}

typedef struct
{
  ExecutableCodeData_t* executableCodeData;
  int                   sizeofData;
} DebugFuncData_t;

static void libbf_debug_func_compiler(DebugFuncData_t* debugFuncData,
                                      OptimizationUnit* current_code_ptr,
                                      void* current)
{
  libbf_debug_func(debugFuncData->sizeofData,
                   current_code_ptr,
                   debugFuncData->executableCodeData->base_data_ptr,
                   current);
}

static void libbf_execute_compiled_code_generic_internal(
    TargetCpu_t cpu,
    int min_delta_offset,
    int max_delta_offset,
    int sizeofData,
    char* executable_code,
    int reserved_size,
    int debug_compiled_code,
    AdvancedOptions_t* options)
{
  void* data = NULL;
  void* executableCodeDataPtr;
  ExecutableCodeData_t executableCodeData;
  DebugFuncData_t debugFuncData;
  
  executableCodeData.input_func = (void*)libbf_getchar_with_data;
  executableCodeData.output_func = (void*)libbf_putchar_with_data;
  executableCodeData.input_output_data = NULL;
  executableCodeData.debug_func = (void*)libbf_debug_func_compiler;
  executableCodeData.debug_data = &debugFuncData;
  
#if defined(SUPPORT_COMPILER_DYNALLOC)
  if (!CHECK_ENV_VAR("NO_DYNALLOC"))
  {
    executableCodeDataPtr =
      libbf_compiler_dynalloc_init(&executableCodeData,
                                    min_delta_offset,
                                    max_delta_offset,
                                    sizeofData,
                                    executable_code,
                                    reserved_size,
                                    NULL,
                                    0,
                                    options);
  }
  else
#endif
  {
    data = alloc0(char, 32768 * sizeofData);
    executableCodeDataPtr = &executableCodeData;
    executableCodeData.base_data_ptr = (void*)((long)data + 1024 * sizeofData);
  }
  
  debugFuncData.executableCodeData = (ExecutableCodeData_t*)executableCodeDataPtr;
  debugFuncData.sizeofData = sizeofData;

  
  libbf_just_execute_compiled_code_generic_internal(executable_code, (ExecutableCodeData_t*)executableCodeDataPtr);
  
#if defined(SUPPORT_COMPILER_DYNALLOC)
  if (!CHECK_ENV_VAR("NO_DYNALLOC"))
  {
    libbf_compiler_dynalloc_end (executableCodeDataPtr);
  }
  else
#endif
  {
    free(data);
  }
}

void libbf_resume(AdvancedOptions_t* options)
{
#if (defined(__linux__) || defined(__FreeBSD__) || defined(WIN32) || defined(__OpenBSD__)) && (defined(__i386__) || defined(__x86_64__))
  FILE* f;
  long pagesize = libbf_getpagesize();
  char begin_executable_code[6];
  char* executable_code;
  int size_of_executable_code;
  int aligned_size_of_executable_code;
  
  TargetCpu_t cpu;
  int sizeofData;
  int min_delta_offset;
  int max_delta_offset;
  int relative_ip, relative_data_ptr, count_active_pages;
#if defined(__i386__)
  int eax, ebx, ecx, edx, flags;
#else
  unsigned long rax, rdi, rsi, rcx, rdx, flags;
#endif
  void* data;
  ExecutableCodeData_t executableCodeData;
  void* executableCodeDataPtr;
  AsmGeneration_t asmGeneration;
  void* ip;
  void* data_ptr;
  int aligned_restore_executable_code_size;
  char* restore_executable_code;
  
  if (!(options->suspend_file && (f = fopen(options->suspend_file, "rb")) != NULL))
  {
    warning("Can't read from suspend file\n");
    return;
  }
  
  fread(begin_executable_code, 2, 1, f);
  if (!(begin_executable_code[0] == (char)0xeb && begin_executable_code[1] == 5 * 4))
  {
    warning("Non i386 or x86_64 libbf-compiled code\n");
    return;
  }
  fread(&begin_executable_code[2], sizeof(int), 1, f);
  size_of_executable_code = *(int*)&begin_executable_code[2];
  
  aligned_size_of_executable_code = (size_of_executable_code + pagesize - 1) & (~(pagesize - 1));
  executable_code = (char*)libbf_alloc_aligned(pagesize, aligned_size_of_executable_code);
  libbf_mprotect(executable_code, aligned_size_of_executable_code, PROT_WRITE | PROT_READ | PROT_EXEC);
  memcpy(executable_code, begin_executable_code, 6);
  fread(executable_code + 6, size_of_executable_code - 6, 1, f);

  
  if (!libbf_is_compatible_compiled_code(executable_code,
       &cpu,
       &sizeofData,
       &min_delta_offset,
       &max_delta_offset))
  {
    return;
  }  
  
  if (cpu != TARGET_HOST_CPU)
  {
    warning("Trying to execute libbf-compiled code on a non-compatible platform\n");
    libbf_free_aligned(executable_code);
    return;
  }
  
  fread(&relative_ip, sizeof(int), 1, f);

#if defined(__i386__)
  fread(&eax, sizeof(int), 1, f);
  fread(&ebx, sizeof(int), 1, f);
  fread(&ecx, sizeof(int), 1, f);
  fread(&edx, sizeof(int), 1, f);
  fread(&flags, sizeof(int), 1, f);
#else
  fread(&rax, sizeof(long), 1, f);
  fread(&rdi, sizeof(long), 1, f);
  fread(&rsi, sizeof(long), 1, f);
  fread(&rcx, sizeof(long), 1, f);
  fread(&rdx, sizeof(long), 1, f);
  fread(&flags, sizeof(long), 1, f);
#endif
  fread(&relative_data_ptr, sizeof(int), 1, f);
  fread(&count_active_pages, sizeof(int), 1, f);
  
  data = malloc(count_active_pages * pagesize);
  fread(data, count_active_pages * pagesize, 1, f);
  
  fclose(f);
  
  executableCodeData.input_func = (void*)libbf_getchar_with_data;
  executableCodeData.output_func = (void*)libbf_putchar_with_data;
  executableCodeData.input_output_data = NULL;
  executableCodeData.debug_func = NULL;
  executableCodeData.debug_data = NULL;
  
  executableCodeDataPtr =
    libbf_compiler_dynalloc_init (&executableCodeData,
                                  min_delta_offset,
                                  max_delta_offset,
                                  sizeofData,
                                  executable_code,
                                  size_of_executable_code,
                                  data,
                                  count_active_pages,
                                  options);
  
  free(data);
  
  asmGeneration.cpu = cpu;
  asmGeneration.sizeofData = sizeofData;
  asmGeneration.p_stack_while = NULL;
  asmGeneration.compiled_code = NULL;
  asmGeneration.allocated_size = 0;
  asmGeneration.filled_size = 0;
  asmGeneration.generate_debug = FALSE;
  asmGeneration.sizeofLong = (cpu == TARGET_X86_64) ? 8 : 4;
  
  libbf_gen_asm_i386_x86_64_preambule(&asmGeneration);
  
  ip = executable_code + relative_ip;
  data_ptr = (void*)((long)((ExecutableCodeData_t*)executableCodeDataPtr)->base_data_ptr + relative_data_ptr);
#if defined(__i386__)
  libbf_gen_asm_i386_x86_64_mov_cst_to_reg(&asmGeneration, eax, EAX);
  libbf_gen_asm_i386_x86_64_mov_cst_to_reg(&asmGeneration, ebx, EBX);
  libbf_gen_asm_i386_x86_64_mov_cst_to_reg(&asmGeneration, ecx, ECX);
  libbf_gen_asm_i386_x86_64_mov_cst_to_reg(&asmGeneration, edx, EDX);
  libbf_gen_asm_i386_x86_64_mov_cst_to_reg(&asmGeneration, (int)data_ptr, ESI);
  libbf_gen_asm_i386_x86_64_push_cst(&asmGeneration, flags);
  libbf_gen_asm_i386_x86_64_popf(&asmGeneration);
  libbf_gen_asm_i386_x86_64_push_cst(&asmGeneration, (int)ip);
  libbf_gen_asm_i386_x86_64_retq(&asmGeneration);
#else
  libbf_gen_asm_i386_x86_64_mov_64cst_to_reg(&asmGeneration, rax, EAX);
  libbf_gen_asm_i386_x86_64_mov_64cst_to_reg(&asmGeneration, rdi, EDI);
  libbf_gen_asm_i386_x86_64_mov_64cst_to_reg(&asmGeneration, rsi, ESI);
  libbf_gen_asm_i386_x86_64_mov_64cst_to_reg(&asmGeneration, rcx, ECX);
  libbf_gen_asm_i386_x86_64_mov_64cst_to_reg(&asmGeneration, rdx, EDX);
  libbf_gen_asm_i386_x86_64_mov_64cst_to_reg(&asmGeneration, (long)data_ptr, EBX);
  libbf_gen_asm_i386_x86_64_mov_64cst_to_reg(&asmGeneration, (long)ip, R8);
  libbf_gen_asm_i386_x86_64_push_reg(&asmGeneration, R8);
  libbf_gen_asm_i386_x86_64_mov_64cst_to_reg(&asmGeneration, flags, R8);
  libbf_gen_asm_i386_x86_64_push_reg(&asmGeneration, R8);
  libbf_gen_asm_i386_x86_64_popf(&asmGeneration);
  libbf_gen_asm_i386_x86_64_retq(&asmGeneration);
#endif
  
  aligned_restore_executable_code_size = (asmGeneration.filled_size + pagesize - 1) & (~(pagesize - 1));
  restore_executable_code = (char*)libbf_alloc_aligned(pagesize, aligned_restore_executable_code_size);
  libbf_mprotect(restore_executable_code, aligned_restore_executable_code_size, PROT_WRITE | PROT_READ | PROT_EXEC);
  memcpy(restore_executable_code, asmGeneration.compiled_code, asmGeneration.filled_size);
  free(asmGeneration.compiled_code);
  
  libbf_just_execute_compiled_code_generic_internal(restore_executable_code, (ExecutableCodeData_t*)executableCodeDataPtr);
  
  libbf_mprotect(restore_executable_code, aligned_restore_executable_code_size, PROT_WRITE | PROT_READ);
  libbf_free_aligned(restore_executable_code);
  
  libbf_mprotect(executable_code, aligned_size_of_executable_code, PROT_WRITE | PROT_READ);
  libbf_free_aligned(executable_code);
  
  libbf_compiler_dynalloc_end (executableCodeDataPtr);
#else
  warning("Unsupported function on this platform\n");
#endif
}

int libbf_is_sizeofData_compatible(TargetCpu_t cpu, int sizeofData)
{
  if (cpu == TARGET_X86_64)
  {
    if (!(sizeofData == 1 || sizeofData == 2 || sizeofData == 4 ||
          (sizeofData > 0 && (sizeofData % 8) == 0)))
    {
      warning("sizeofData != 1 && sizeofData != 2 && sizeofData != 4 && sizeofData != 8 && sizeofData != 16...");
      return FALSE;
    }
  }
  else if (cpu == TARGET_I386 || cpu == TARGET_ARM || cpu == TARGET_PPC || cpu == TARGET_SPARC ||
           cpu == TARGET_MIPS || cpu == TARGET_MIPSEL)
  {
    if (!(sizeofData == 1 || sizeofData == 2 ||
          (sizeofData > 0 && (sizeofData % 4) == 0)))
    {
      warning("sizeofData != 1 && sizeofData != 2 && sizeofData != 4 && sizeofData != 8 && sizeofData != 12...");
      return FALSE;
    }
  }
  else
  {
    SHOULDNT_HAPPEN();
  }
  return TRUE;
}

void libbf_execute_compiled_code_internal(char* compiled_code,
                                          int size_compiled_code,
                                          int debug_compiled_code,
                                          AdvancedOptions_t* options)
{
#if defined(SUPPORT_EXECUTE_COMPILED_CODE)

  TargetCpu_t cpu;
  int sizeofData;
  int min_delta_offset;
  int max_delta_offset;
  long pagesize = libbf_getpagesize();
  int aligned_size_compiled_code;
  char* executable_code;
    
  if (!libbf_is_compatible_compiled_code(compiled_code,
                                         &cpu,
                                         &sizeofData,
                                         &min_delta_offset,
                                         &max_delta_offset))
  {
    return;
  }

  if (cpu != TARGET_HOST_CPU)  
  {
    warning("Trying to execute libbf-compiled code on a non-compatible platform\n");
    return;
  }
  
  if (!libbf_is_sizeofData_compatible(cpu, sizeofData)) return;

  aligned_size_compiled_code = (size_compiled_code + pagesize - 1) & (~(pagesize - 1));
  executable_code = (char*)libbf_alloc_aligned(pagesize, aligned_size_compiled_code);
  libbf_mprotect(executable_code, aligned_size_compiled_code, PROT_WRITE | PROT_READ | PROT_EXEC);
  memcpy(executable_code, compiled_code, size_compiled_code);

  libbf_execute_compiled_code_generic_internal(cpu,
                                               min_delta_offset,
                                               max_delta_offset,
                                               sizeofData,
                                               executable_code,
                                               size_compiled_code,
                                               debug_compiled_code,
                                               options);

  libbf_mprotect(executable_code, aligned_size_compiled_code, PROT_WRITE | PROT_READ);
  libbf_free_aligned(executable_code);
#else
  warning("Only i386, x86_64, ppc, arm, sparc, mips, mipsel platforms supported\n");
#endif
}

#endif
