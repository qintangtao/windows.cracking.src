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

#ifndef LIBBF_INTERPRETER_DYNALLOC_H
#define LIBBF_INTERPRETER_DYNALLOC_H

#include "libbf_internal.h"

#if defined(WIN32)
#include <windows.h> 
#include <excpt.h>

EXCEPTION_DISPOSITION libbf_interpreter_dynalloc_handler_win32(struct _EXCEPTION_RECORD *exception_record,
                                                   void * EstablisherFrame,
                                                   struct _CONTEXT *ContextRecord,
                                                   void * DispatcherContext);
#endif

void libbf_interpreter_dynalloc_init (OFFSET_t min_delta_offset,
                                 OFFSET_t max_delta_offset,
                                 int sizeof_my_type,
                                 OptimizationUnit* source_code,
                                 const void* addr_dynalloc_needed,
                                 void** p_void_ptr_old_action,
                                 void** p_void_data_ptr);

long libbf_interpreter_dynalloc_realloc(void* data_ptr);

void libbf_interpreter_dynalloc_end(void* void_ptr_old_action, void* data_ptr);

#endif
