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

#ifndef LIBBF_COMPILER_DYNALLOC_H
#define LIBBF_COMPILER_DYNALLOC_H

#if ((defined(__linux__) || defined(WIN32) || defined(__FreeBSD__) || defined(__OpenBSD__)) && (defined(__x86_64__) || defined(__i386__))) || (defined(__linux__) && defined(__powerpc__) ) || (defined(sun) && defined(__sparc__))

#define SUPPORT_COMPILER_DYNALLOC

#include "libbf.h"
#include "libbf_internal.h"

#if defined(WIN32)
#include <windows.h> 
#include <excpt.h>
EXCEPTION_DISPOSITION libbf_compiler_dynalloc_handler_win32(struct _EXCEPTION_RECORD *exception_record,
                                                   void * EstablisherFrame,
                                                   struct _CONTEXT *ContextRecord,
                                                   void * DispatcherContext);
#endif

void* libbf_compiler_dynalloc_init (ExecutableCodeData_t* executableCodeData,
                                    OFFSET_t min_delta_offset,
                                    OFFSET_t max_delta_offset,
                                    int sizeof_my_type,
                                    void* executable_code,
                                    int size_of_executable_code,
                                    void* init_data_ptr,
                                    int count_active_pages,
                                    AdvancedOptions_t* options);

void libbf_compiler_dynalloc_toggle_suspend_or_resume(void* ptr);

void libbf_compiler_dynalloc_end(void* executableCodeDataPtr);

#endif

#endif
