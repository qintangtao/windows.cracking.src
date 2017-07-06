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

#ifndef LIBBF_H
#define LIBBF_H 

#ifndef NO_INTERPRETER
void libbf_naive_interpreter_char(const char* code);
void libbf_naive_interpreter_short(const char* code);
void libbf_naive_interpreter_int(const char* code);
void libbf_naive_interpreter_long(const char* code);
void libbf_naive_interpreter_long_long(const char* code);

void libbf_interpreter_char(const char* code);
void libbf_interpreter_short(const char* code);
void libbf_interpreter_int(const char* code);
void libbf_interpreter_long(const char* code);
void libbf_interpreter_long_long(const char* code);

void libbf_interpreter_optimizer_char(const char* code);
void libbf_interpreter_optimizer_short(const char* code);
void libbf_interpreter_optimizer_int(const char* code);
void libbf_interpreter_optimizer_long(const char* code);
void libbf_interpreter_optimizer_long_long(const char* code);

void libbf_interpreter_optimizer_profile_char(const char* code);
void libbf_interpreter_optimizer_profile_short(const char* code);
void libbf_interpreter_optimizer_profile_int(const char* code);
void libbf_interpreter_optimizer_profile_long(const char* code);
void libbf_interpreter_optimizer_profile_long_long(const char* code);

void libbf_interpreter_optimizer_safe_jump_char(const char* code);
void libbf_interpreter_optimizer_safe_jump_short(const char* code);
void libbf_interpreter_optimizer_safe_jump_int(const char* code);
void libbf_interpreter_optimizer_safe_jump_long(const char* code);
void libbf_interpreter_optimizer_safe_jump_long_long(const char* code);
#endif

void libbf_interpreter_optimizer_cdump(const char* code, const char* data_type);
void libbf_interpreter_optimizer_javadump(const char* code, const char* data_type);

void libbf_c_to_bf(const char* code);

typedef struct
{
  int    stop_after_ms;
  int    average_load;
  char*  suspend_file;
} AdvancedOptions_t;

#ifndef NO_COMPILER
void libbf_compile_execute_char(const char* code, AdvancedOptions_t* options);
void libbf_compile_execute_short(const char* code, AdvancedOptions_t* options);
void libbf_compile_execute_int(const char* code, AdvancedOptions_t* options);
void libbf_compile_execute_long_long(const char* code, AdvancedOptions_t* options);
void libbf_compile_execute_int_n(const char* code, int sizeofData, AdvancedOptions_t* options);

typedef enum
{
  TARGET_I386,
  TARGET_X86_64,
  TARGET_PPC,
  TARGET_ARM,
  TARGET_SPARC,
  TARGET_MIPS,
  TARGET_MIPSEL,
  TARGET_COUNT, /* always in last pos */
} TargetCpu_t;

void libbf_gen_compiled_code(TargetCpu_t cpu, int sizeofData, const char* source_code, char** compiled_code, int* size_compiled_code);
void libbf_execute_compiled_code(char* compiled_code, int size_compiled_code, AdvancedOptions_t* options);

void libbf_resume(AdvancedOptions_t* options);
#endif

#endif
