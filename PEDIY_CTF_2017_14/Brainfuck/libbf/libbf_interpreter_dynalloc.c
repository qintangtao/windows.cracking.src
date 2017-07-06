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

#ifndef NO_INTERPRETER

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "libbf_internal.h"
#include "libbf_interpreter_dynalloc.h"
#include "libbf_posix_emulation.h"

#define COUNT_LOW_ACT_HIGH_PAGES(x) \
 (x->count_low_pages + x->count_active_pages + x->count_high_pages)
#define COUNT_LOW_ACT_PAGES(x) \
 (x->count_low_pages + x->count_active_pages)
 
#define NB_MIN_ACTIVE_PAGES   1
#define RESERVED_PAGE 1

/* ------------------------------------------------------------------------ */

typedef struct
{
  int              signature1;
  int              signature2;
  
  int              sizeof_elt;
  int              increase_from_low;
  int              increase_from_high;

  void*            current_mem;
  int              count_low_pages;
  int              count_active_pages;
  int              count_high_pages;
  
  int              max_total_pages;
  
  OptimizationUnit* source_code_src;
  OptimizationUnit* source_code_copy;
  int               source_code_length;
  
  void*            addr_dynalloc_needed;
} DynAllocDesc;

/* ------------------------------------------------------------------------ */

static DynAllocDesc* libbf_interpreter_dynalloc_find_desc(void* data_ptr)
{
  long pagesize = libbf_getpagesize();
  void* data_page = (void*)((long)data_ptr & ~(pagesize-1));
    
  void* cur_page = data_page;
  DynAllocDesc* desc = NULL;
  /* Current page must not be unlocked */
  /* Following pages are OK to be unlocked */
  if (DEBUG_DYNALLOC) fprintf(stderr, "data page is 0x%lX\n", (long)data_page);
    
  while(1)
  {
    int ret;
    cur_page = (void*)((long)cur_page + pagesize);
    
    ret = libbf_mprotect(cur_page, pagesize, PROT_READ);
    if (ret != 0) fatal("mprotect failed\n");
    
    desc = (DynAllocDesc*)cur_page;
    if (desc->signature1 == SIGNATURE_1 &&
        desc->signature2 == SIGNATURE_2)
      break;
  }
  
  if (DEBUG_DYNALLOC) fprintf(stderr, "cur_page=0x%lX\n", (long)cur_page);
  
  return desc;
}

#if defined(WIN32)
static void libbf_interpreter_dynalloc_handler(int signum, siginfo_t* info, void* ptr);

EXCEPTION_DISPOSITION libbf_interpreter_dynalloc_handler_win32(struct _EXCEPTION_RECORD *exception_record,
                                                   void * EstablisherFrame,
                                                   struct _CONTEXT *ContextRecord,
                                                   void * DispatcherContext)
{
  if (DEBUG_DYNALLOC) fprintf(stderr, "in libbf_dynalloc_handler_win32\n");
  
  /* If the exception is an access violation */ 
  if (exception_record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION &&
      exception_record->NumberParameters >= 2) 
  { 
    int accessMode = (int)exception_record->ExceptionInformation[0]; 
    void* adr = (void *)exception_record->ExceptionInformation[1]; 
    siginfo_t s_info;
    
    if (DEBUG_DYNALLOC)
    {
      fprintf(stderr, "access mode = %s\n", (accessMode & PAGE_NOACCESS) ? "No access" : "other");
    }
    s_info.si_code = SEGV_ACCERR;
    s_info.si_addr= adr;
    libbf_interpreter_dynalloc_handler(SIGSEGV, &s_info, NULL);
    return ExceptionContinueExecution; 
  }
  else
  {
    fprintf(stderr, "not handled exception : %X\n", (int)exception_record->ExceptionCode);
    fflush(stderr);
    return ExceptionContinueSearch;
  }
} 

#endif

static
void libbf_interpreter_dynalloc_handler(int signum, siginfo_t* info, void* ptr)
{
#if defined(__FreeBSD__)
  if (signum == SIGBUS)
#else  
  if (signum == SIGSEGV /*&& info->si_code == SEGV_ACCERR*/)
#endif  
  {
    long pagesize = libbf_getpagesize();

    DynAllocDesc* desc = libbf_interpreter_dynalloc_find_desc(info->si_addr);
    
    long fault_page = (long)info->si_addr & ~(pagesize-1);
    int ret;
    void* current_mem;
    int nb_pages;
    int i;
      
    if (DEBUG_DYNALLOC) fprintf(stderr, "illegal access to 0x%lX\n", (long)info->si_addr);
      
    /* Unprotecting upper reserved page (will be protected again) */
    ret = libbf_mprotect(desc, pagesize, PROT_READ | PROT_WRITE);
    if (ret != 0) fatal("mprotect failed\n");
      
    if (fault_page >= (long)desc->current_mem &&
        fault_page < (long)desc->current_mem + desc->count_low_pages * pagesize)
    {
      if (DEBUG_DYNALLOC) fprintf(stderr, "before current memory\n");
      desc->increase_from_low =
          ((long)desc->current_mem +
          desc->count_low_pages * pagesize -
          fault_page) / pagesize;
    }
    else if (fault_page >= (long)desc->current_mem + COUNT_LOW_ACT_PAGES(desc) * pagesize &&
             fault_page < (long)desc->current_mem + COUNT_LOW_ACT_HIGH_PAGES(desc) * pagesize)
    {
      if (DEBUG_DYNALLOC) fprintf(stderr, "after current memory\n");
      desc->increase_from_high =
          1 + (fault_page - ((long)desc->current_mem +
          COUNT_LOW_ACT_PAGES(desc) * pagesize)) / pagesize;
    }
    else
    {
      fatal("dynamic memory allocator strangly failed !");
    }
      
    if (desc->max_total_pages >= 0 && 
        desc->count_active_pages +
        desc->increase_from_low +
        desc->increase_from_high > desc->max_total_pages)
    {
      fatal("Cannot dynamically alloc more than %d pages\n", desc->max_total_pages);
    }

    current_mem = desc->current_mem;
    nb_pages = COUNT_LOW_ACT_HIGH_PAGES(desc);

    /* Patch the whole source code, such that the next executed instruction */
    /* will be the special BF_DYNALLOC_REALLOC_NEEDED */
    for(i=0;i<desc->source_code_length;i++)
    {
      desc->source_code_src[i].ou_instr = BF_DYNALLOC_REALLOC_NEEDED;
      desc->source_code_src[i].ou_interpreter_addr = desc->addr_dynalloc_needed;
    }
    
    ret = libbf_mprotect(desc, pagesize, PROT_NONE);
    if (ret != 0) fatal("mprotect failed\n");

    if (DEBUG_DYNALLOC) fprintf(stderr, "dynamic memory allocator can go on\n");

    /* Unprotected all user memory except reserved page */
    ret = libbf_mprotect(current_mem, nb_pages * pagesize, PROT_READ | PROT_WRITE);
    if (ret != 0) fatal("mprotect failed\n");
    
  }
  else
  {
    fatal("unexpected sigsegv at 0x%lX, info->si_code = %d (SEGV_ACCERR=%d)\n", (long)info->si_addr, info->si_code, SEGV_ACCERR);
  }
}

void libbf_interpreter_dynalloc_init (OFFSET_t min_delta_offset,
                                 OFFSET_t max_delta_offset,
                                 int sizeof_my_type,
                                 OptimizationUnit* source_code,
                                 const void* addr_dynalloc_needed,
                                 void** p_void_ptr_old_action,
                                 void** p_void_data_ptr)
{
  int ret;
  long pagesize = libbf_getpagesize();
  int count_low_pages = (min_delta_offset * sizeof_my_type + pagesize - 1) / pagesize; /* ceil rounding */
  int count_high_pages = (max_delta_offset * sizeof_my_type + pagesize - 1) / pagesize; /* ceil rounding */
  int count_active_pages = NB_MIN_ACTIVE_PAGES; /* number of initial regular pages */
  DynAllocDesc dynAllocDesc; 
  OptimizationUnit* ptr;
  void* mem;
  int reserved_size = (count_low_pages + count_active_pages + count_high_pages + RESERVED_PAGE) * pagesize;
  DynAllocDesc* dynAllocDescUpperMem;
    
#if !defined(WIN32)
  struct sigaction **p_old_action = (struct sigaction **)p_void_ptr_old_action;
  struct sigaction action;
  *p_old_action = alloc0(struct sigaction, 1);
  action.sa_sigaction = libbf_interpreter_dynalloc_handler;
  sigemptyset(&(action.sa_mask));
  action.sa_flags = SA_SIGINFO;
#if defined(__FreeBSD__)
  ret = sigaction(SIGBUS, &action, *p_old_action);
#else
  ret = sigaction(SIGSEGV, &action, *p_old_action);
#endif
  if (ret != 0) fatal("sigaction failed");
#endif
    
  mem = libbf_alloc_aligned(pagesize, reserved_size);
  if (mem == NULL) fatal("alloc_aligned failed");
  memset(mem, 0, reserved_size);
  
  dynAllocDesc.signature1 = SIGNATURE_1;
  dynAllocDesc.signature2 = SIGNATURE_2;
  dynAllocDesc.sizeof_elt = sizeof_my_type;
  dynAllocDesc.increase_from_low = 0;
  dynAllocDesc.increase_from_high = 0;
  dynAllocDesc.current_mem = mem;
  dynAllocDesc.count_low_pages = count_low_pages;
  dynAllocDesc.count_active_pages = count_active_pages;
  dynAllocDesc.count_high_pages = count_high_pages;
  dynAllocDesc.max_total_pages = -1; /* unlimited */
  dynAllocDesc.source_code_src = source_code;
  dynAllocDesc.source_code_length = 1;
  dynAllocDesc.addr_dynalloc_needed = (void*)addr_dynalloc_needed;
  
  /* Store a copy of the orignal source code */
  
  ptr = source_code;
  while(ptr->ou_instr != BF_END)
  {
    dynAllocDesc.source_code_length++;
    ptr++;
  }
  
  dynAllocDesc.source_code_copy = alloc0(OptimizationUnit, dynAllocDesc.source_code_length);
  memcpy(dynAllocDesc.source_code_copy, source_code, dynAllocDesc.source_code_length * sizeof(OptimizationUnit));
  
  dynAllocDescUpperMem =
      (DynAllocDesc*)((long)mem + COUNT_LOW_ACT_HIGH_PAGES((&dynAllocDesc)) * pagesize);
  
  *dynAllocDescUpperMem = dynAllocDesc;
  
  *p_void_data_ptr = (void*)((long)mem + count_low_pages * pagesize);

  /* Lock low and high pages, upper reserved page and checker description page */
  ret = libbf_mprotect(mem, pagesize * count_low_pages, PROT_NONE);
  ret |= libbf_mprotect((void*)((long)mem + (count_low_pages + count_active_pages) * pagesize),
                        (count_high_pages + RESERVED_PAGE) * pagesize, PROT_NONE);
  if (ret != 0) fatal("mprotect failed");

}

long libbf_interpreter_dynalloc_realloc(void* data_ptr)
{
  long pagesize = libbf_getpagesize();
  DynAllocDesc* desc;
  int old_count_active_pages;
  int ret;
  void* old_mem;
  long delta_mem;
  DynAllocDesc desc_copy;
  int i;
    
  if (DEBUG_DYNALLOC)
  {
    fprintf(stderr, "entering libbf_interpreter_dynalloc_realloc\n");
  }
    
  desc = libbf_interpreter_dynalloc_find_desc(data_ptr);
  
  old_count_active_pages = desc->count_active_pages;
      
  /* Unprotecting all - old - user memory, including upper reserved page, for any latter reuse */
  ret = libbf_mprotect(desc->current_mem,
                 (COUNT_LOW_ACT_HIGH_PAGES(desc) + RESERVED_PAGE) * pagesize,
                 PROT_READ | PROT_WRITE);
  if (ret != 0) fatal("mprotect failed\n");

  if (DEBUG_DYNALLOC)
  {
    fprintf(stderr, "old memory range from [0x%lX,0x%lX]\n",
            (long)desc->current_mem,
            (long)desc->current_mem + (COUNT_LOW_ACT_HIGH_PAGES(desc)) * pagesize - 1);
  }
  
  if (desc->increase_from_low)
  {
    if (DEBUG_DYNALLOC) fprintf(stderr, "increase from low : %d\n", desc->increase_from_low);
    assert(desc->increase_from_low > 0);
    desc->count_active_pages += desc->increase_from_low;
  }
  else if (desc->increase_from_high)
  {
    if (DEBUG_DYNALLOC) fprintf(stderr, "increase from high : %d\n", desc->increase_from_high);
    assert(desc->increase_from_high > 0);
    desc->count_active_pages += desc->increase_from_high;
  }
  else
  {
    fatal("libbf_interpreter_dynalloc_realloc failed\n");
  }
  
  old_mem = desc->current_mem;
      
  /* Allocating new user memory */
      
  desc->current_mem = libbf_alloc_aligned(pagesize,
                                    (COUNT_LOW_ACT_HIGH_PAGES(desc) + RESERVED_PAGE) * pagesize);
  if (desc->current_mem == 0)
  {
    return 0;
  }
      
  if (DEBUG_DYNALLOC)
  {
    fprintf(stderr, "new memory range from [0x%lX,0x%lX]\n",
      (long)desc->current_mem,
      (long)desc->current_mem + (COUNT_LOW_ACT_HIGH_PAGES(desc)) * pagesize - 1);
  }
  
  memset(desc->current_mem,
         0, (COUNT_LOW_ACT_HIGH_PAGES(desc) + RESERVED_PAGE) * pagesize);


  /* Copying old user memory into old user memory, */
  /* and compute offset between them (expressed in sizeof_elt) */
  delta_mem = 0;
  if (desc->increase_from_high)
  {
    memcpy((void*)((long)desc->current_mem + desc->count_low_pages * pagesize),
            (void*)((long)old_mem + desc->count_low_pages * pagesize),
           (desc->increase_from_high + old_count_active_pages) * pagesize);
    delta_mem = ((long)desc->current_mem - (long)old_mem) / desc->sizeof_elt;
    desc->increase_from_high = 0;
  }
  else if (desc->increase_from_low)
  {
    memcpy((void*)((long)desc->current_mem + desc->count_low_pages * pagesize),
            (void*)((long)old_mem + (desc->count_low_pages - desc->increase_from_low) * pagesize),
           (desc->increase_from_low + old_count_active_pages) * pagesize);
    delta_mem = ((long)desc->current_mem - (long)old_mem + desc->increase_from_low * pagesize ) / desc->sizeof_elt;
    desc->increase_from_low = 0;
  }
  else
  {
    fatal("realloc failed\n");
  }

  memcpy(&desc_copy, desc, sizeof(desc_copy));
  desc = (DynAllocDesc*)((long)desc->current_mem + COUNT_LOW_ACT_HIGH_PAGES(desc) * pagesize);
  *desc= desc_copy;
  
  /* Discarding now old memory */
  libbf_free_aligned(old_mem);
  
  for(i=0;i<desc->source_code_length;i++)
  {
    desc->source_code_src[i].ou_instr = desc->source_code_copy[i].ou_instr;
    desc->source_code_src[i].ou_interpreter_addr = desc->source_code_copy[i].ou_interpreter_addr;
  }  
  /* Protecting new low and high user pages (including reserved upper page) */

  ret = libbf_mprotect(desc->current_mem,
                 desc->count_low_pages * pagesize,
                 PROT_NONE);
  if (ret != 0) fatal("mprotect failed\n");
  ret = libbf_mprotect((void*)((long)desc->current_mem + desc->count_low_pages * pagesize),
                        desc->count_active_pages * pagesize,
                        PROT_READ | PROT_WRITE);
  if (ret != 0) fatal("mprotect failed\n");
  ret = libbf_mprotect((void*)((long)desc->current_mem + COUNT_LOW_ACT_PAGES(desc) * pagesize),
                      (desc->count_high_pages + RESERVED_PAGE) * pagesize,
                      PROT_NONE);
  if (ret != 0) fatal("mprotect failed\n");

  if (DEBUG_DYNALLOC) fprintf(stderr, "libbf_interpreter_dynalloc2_realloc [OK]\n");

  return delta_mem;
}

void libbf_interpreter_dynalloc_end(void* void_ptr_old_action, void* data_ptr)
{
  long pagesize = libbf_getpagesize();
  
  DynAllocDesc* desc = libbf_interpreter_dynalloc_find_desc(data_ptr);
  int ret;
    
#if !defined(WIN32)
  struct sigaction *p_old_action = (struct sigaction *)void_ptr_old_action;
#if defined(__FreeBSD__)
  sigaction(SIGBUS, p_old_action, 0);
#else
  sigaction(SIGSEGV, p_old_action, 0);
#endif
  free(p_old_action);
#endif

  /* Restore regular protection for user data pages */
  ret = libbf_mprotect(desc->current_mem,
           (COUNT_LOW_ACT_HIGH_PAGES(desc) + RESERVED_PAGE) * pagesize, PROT_READ | PROT_WRITE);
  if (ret != 0) fatal("mprotect failed\n");

  if (DEBUG_DYNALLOC)
  {
    fprintf(stderr, "%d pages have been allocated\n", desc->count_active_pages);
    fprintf(stderr, "freeing from 0x%lX\n", (long)desc->current_mem);
  }
  
  free(desc->source_code_copy);
  
  /* Free user data : low protection pages, regular pages, high protection pages, upper reserved page */
  libbf_free_aligned(desc->current_mem);
}

#endif
