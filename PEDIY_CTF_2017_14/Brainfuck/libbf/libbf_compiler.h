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

#ifndef LIBBF_COMPILER_H
#define LIBBF_COMPILER_H

#include "libbf.h"

typedef struct
{
  void*    input_func;
  void*    output_func;
  void*    input_output_data;
  void*    base_data_ptr;
  void*    debug_func;
  void*    debug_data;
} ExecutableCodeData_t;

#define OFFSET_INPUT_FUNC    (0 * asmGeneration->sizeofLong)
#define OFFSET_OUTPUT_FUNC   (1 * asmGeneration->sizeofLong)
#define OFFSET_IN_OUT_DATA   (2 * asmGeneration->sizeofLong)
#define OFFSET_BASE_DATA_PTR (3 * asmGeneration->sizeofLong)
#define OFFSET_DEBUG_FUNC    (4 * asmGeneration->sizeofLong)
#define OFFSET_DEBUG_DATA    (5 * asmGeneration->sizeofLong)

void libbf_gen_compile_internal_from_code_array(
    TargetCpu_t cpu,
    int sizeofData, 
    OptimizationUnit* code,
    int min_delta_offset,
    int max_delta_offset,
    char** compiled_code,
    int* size_compiled_code,
    int debug_compiled_code);
    
void libbf_execute_compiled_code_internal(char* compiled_code,
                                          int size_compiled_code,
                                          int debug_compiled_code,
                                          AdvancedOptions_t* options);
    
int libbf_is_sizeofData_compatible(TargetCpu_t cpu, int sizeofData);

#endif
