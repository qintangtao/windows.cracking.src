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

#ifndef NO_COMPILER

#if ((defined(__linux__) || defined(WIN32) || defined(__FreeBSD__) || defined(__OpenBSD__)) && (defined(__x86_64__) || defined(__i386__))) || (defined(__linux__) && defined(__powerpc__) ) || (defined(sun) && defined(__sparc__))

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if defined(__FreeBSD__) || defined(sun)
#include <ucontext.h>
#endif

#if defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__)
  #include <sys/time.h>
#endif

#if defined(WIN32)
  #include <process.h>
#endif

#include "libbf_internal.h"
#include "libbf_compiler.h"
#include "libbf_compiler_dynalloc.h"
#include "libbf_posix_emulation.h"

#define COUNT_LOW_ACT_HIGH_PAGES(x) (x->count_low_pages + x->count_active_pages + x->count_high_pages)
#define COUNT_LOW_ACT_PAGES(x) (x->count_low_pages + x->count_active_pages)
 
#define NB_MIN_ACTIVE_PAGES   1

#if defined(__i386__) && (defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(WIN32))
  #define GET_DATA_PTR_REG GET_SI
#elif defined(__x86_64__) && (defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__))
  #define GET_DATA_PTR_REG GET_BX
#elif defined(__powerpc__)
  #define GET_DATA_PTR_REG GET_R15
#elif defined(sun) && defined(__sparc__)
  #define GET_DATA_PTR_REG GET_O5
#endif

#if defined(__linux__) || defined(sun)
#define cast_ptr_to_context(ptr, context) struct ucontext* context = (struct ucontext*)ptr
#elif defined(__FreeBSD__)
#define cast_ptr_to_context(ptr, context) ucontext_t* context = (ucontext_t*)ptr
#elif defined(__OpenBSD__)
#define cast_ptr_to_context(ptr, context) struct sigcontext* context = (struct sigcontext*)ptr
#elif defined(WIN32)
#define cast_ptr_to_context(ptr, context) struct _CONTEXT *context = (struct _CONTEXT *)ptr
#endif

/* ------------------------------------------------------------------------ */

typedef struct
{
  ExecutableCodeData_t  executableCodeData;
  
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
  
  void*            current_executable_code;
  int              size_of_executable_code;
  
  long             delta_mem;
#if !defined(WIN32)
  struct sigaction old_action_sigbus;
  struct sigaction old_action_sigsegv;
#endif
  int              isInterrupted;
  int              sigalarm_set;
  int              sigalarm_active;
#if (defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__)) && (defined(__i386__) || defined(__x86_64__))
  struct sigaction old_action_sigalrm;
  struct sigaction saved_action_sigalrm;
  struct timeval   start_tv;
#elif defined(WIN32)
  DWORD            start_time;
  int              hasStarted;
#endif

  AdvancedOptions_t* options;
} DynAllocDesc;

/* ------------------------------------------------------------------------ */

#if defined(WIN32)

static void libbf_compiler_dynalloc_handler(int signum, siginfo_t* info, void* ptr);

EXCEPTION_DISPOSITION libbf_compiler_dynalloc_handler_win32(
    struct _EXCEPTION_RECORD *exception_record,
    void * EstablisherFrame,
    struct _CONTEXT *ContextRecord,
    void * DispatcherContext)
{
  if (DEBUG_DYNALLOC) fprintf(stderr, "in libbf_compiler_dynalloc_handler_win32\n");
  
  /* If the exception is an access violation */ 
  if (exception_record->ExceptionCode == EXCEPTION_ACCESS_VIOLATION &&
      exception_record->NumberParameters >= 2) 
  { 
    int accessMode = (int)exception_record->ExceptionInformation[0]; 
    void* adr = (void *)exception_record->ExceptionInformation[1]; 
    
    if (DEBUG_DYNALLOC)
    {
      fprintf(stderr, "access mode = %s\n", (accessMode & PAGE_NOACCESS) ? "No access" : "other");
    }
    siginfo_t s_info;
    s_info.si_code = SEGV_ACCERR;
    s_info.si_addr= adr;
    libbf_compiler_dynalloc_handler(SIGSEGV, &s_info, ContextRecord);
    return ExceptionContinueExecution; 
  }
  else
  {
    return ExceptionContinueSearch;
  }
} 

#endif

#if defined(__powerpc__)
static int libbf_compiler_desasm(int sizeofData, unsigned char* code)
{
  if (code[0] == 0x80 && code[1] == 0x6f)
  {
    if (DEBUG_DYNALLOC) fprintf(stderr, "lwz     r3,0x%X(r15)\n", (int)*(short*)(code + 2));
  }
  else if (code[0] == 0x90 && code[1] == 0x6f)
  {
    if (DEBUG_DYNALLOC) fprintf(stderr, "stw     r3,0x%X(r15)\n", (int)*(short*)(code + 2));
  }
  else if (code[0] == 0x80 && code[1] == 0x8f)
  {
    if (DEBUG_DYNALLOC) fprintf(stderr, "lwz     r4,0x%X(r15)\n", (int)*(short*)(code + 2));
  }
  else if (code[0] == 0x90 && code[1] == 0x8f)
  {
    if (DEBUG_DYNALLOC) fprintf(stderr, "stw     r4,0x%X(r15)\n", (int)*(short*)(code + 2));
  }
  else
  {
    SHOULDNT_HAPPEN();
  }
  return 4;
}
#endif

static long libbf_compiler_dynalloc_realloc(DynAllocDesc* desc)
{
  long pagesize = libbf_getpagesize();
  int old_count_active_pages;
  void* old_mem;
  long delta_mem;
  int ret;
  
  if (DEBUG_DYNALLOC)
  {
    fprintf(stderr, "entering libbf_compiler_dynalloc_realloc\n");
  }
    
  old_count_active_pages = desc->count_active_pages;
  
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
    fatal("libbf_compiler_dynalloc_realloc failed\n");
  }
  
  old_mem = desc->current_mem;
      
  /* Allocating new user memory */
      
  desc->current_mem = libbf_alloc_aligned(pagesize,
                                          (COUNT_LOW_ACT_HIGH_PAGES(desc)) * pagesize);
  if (desc->current_mem == 0)
  {
    SHOULDNT_HAPPEN();
  }
      
  if (DEBUG_DYNALLOC)
  {
    fprintf(stderr, "new memory range from [0x%lX,0x%lX]\n",
            (long)desc->current_mem,
            (long)desc->current_mem + (COUNT_LOW_ACT_HIGH_PAGES(desc)) * pagesize - 1);
  }
  
  memset(desc->current_mem,
         0, (COUNT_LOW_ACT_HIGH_PAGES(desc)) * pagesize);


  delta_mem = 0;
  
  /* Copying old user memory into old user memory, */
  if (desc->increase_from_high)
  {
    memcpy((void*)((long)(void*)((long)desc->current_mem + desc->count_low_pages * pagesize)),
            (void*)((long)old_mem + desc->count_low_pages * pagesize),
           old_count_active_pages * pagesize);
    delta_mem = (long)desc->current_mem - (long)old_mem;
    desc->increase_from_high = 0;
  }
  else if (desc->increase_from_low)
  {
    memcpy((void*)((long)desc->current_mem + (desc->count_low_pages + desc->increase_from_low) * pagesize),
            (void*)((long)old_mem + desc->count_low_pages * pagesize),
           old_count_active_pages * pagesize);
    delta_mem = (long)desc->current_mem - (long)old_mem + desc->increase_from_low * pagesize;
    desc->increase_from_low = 0;
  }
  else
  {
    fatal("realloc failed\n");
  }

  /* Protecting new  pages */

  ret = libbf_mprotect(desc->current_mem,
                       desc->count_low_pages * pagesize,
                       PROT_NONE);
  if (ret != 0) fatal("mprotect failed\n");
  ret = libbf_mprotect((void*)((long)desc->current_mem + desc->count_low_pages * pagesize),
                       desc->count_active_pages * pagesize,
                       PROT_READ | PROT_WRITE);
  if (ret != 0) fatal("mprotect failed\n");

  ret = libbf_mprotect((void*)((long)desc->current_mem + COUNT_LOW_ACT_PAGES(desc) * pagesize),
                      (desc->count_high_pages) * pagesize,
                      PROT_NONE);
  if (ret != 0) fatal("mprotect failed\n");  
  
  if (DEBUG_DYNALLOC) fprintf(stderr, "libbf_compiler_dynalloc_realloc [OK]\n");
  
  return delta_mem;
}

#if (defined(__linux__) || defined(__FreeBSD__)  || defined(__OpenBSD__)|| defined(WIN32)) && (defined(__i386__) || defined(__x86_64__))
static void libbf_save_state(DynAllocDesc* desc, void* ptr);
#endif

static
void libbf_compiler_dynalloc_handler(int signum, siginfo_t* info, void* ptr)
{
  if (signum == SIGBUS || (signum == SIGSEGV /* && info->si_code == SEGV_ACCERR */))
  {
    long pagesize = libbf_getpagesize();
    cast_ptr_to_context(ptr, context);
    long fault_page;
    long regIP;
    void* old_mem;
    int old_size;
    long delta_mem;
    int ret;
      
#if defined(__powerpc__)
    DynAllocDesc* desc = (DynAllocDesc*)GET_R14(context);
#elif defined(sun) && defined(__sparc__)
    DynAllocDesc* desc = (DynAllocDesc*)GET_O4(context);
#else
    void** esp = (void**)GET_SP(context);
    DynAllocDesc* desc = (DynAllocDesc*)esp[0];
    assert(esp[1] == (void*)SIGNATURE_1);
    assert(esp[2] == (void*)SIGNATURE_2);
#endif
    if (DEBUG_DYNALLOC) fprintf(stderr, "illegal access to 0x%lX\n", (long)info->si_addr);
    
    assert(desc->signature1 == SIGNATURE_1);
    assert(desc->signature2 == SIGNATURE_2);
    
#if  (defined(__linux__) || defined(__FreeBSD__)  || defined(__OpenBSD__) || defined(WIN32)) && (defined(__i386__) || defined(__x86_64__))
    if (desc->isInterrupted)
    {
      libbf_save_state(desc, ptr);
      return;
    }
#endif

    fault_page = (long)info->si_addr & ~(pagesize-1);

    if (fault_page >= (long)desc->current_mem &&
        fault_page < (long)desc->current_mem + desc->count_low_pages * pagesize)
    {
      if (DEBUG_DYNALLOC) fprintf(stderr, "before current memory\n");
      desc->increase_from_low =
          ((long)desc->current_mem + desc->count_low_pages * pagesize - (long)fault_page) / pagesize;
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

    regIP = (long)GET_IP(context);

    if (DEBUG_DYNALLOC) 
      fprintf(stderr, "0x%lX [0x%lX,0x%lX[\n", regIP, (long)desc->current_executable_code, (long)desc->current_executable_code + desc->size_of_executable_code);
    
    if (!(regIP >= (long)desc->current_executable_code && regIP < (long)desc->current_executable_code + desc->size_of_executable_code))
    {
      fatal("!(regIP >= desc->current_executable_code && regIP < desc->current_executable_code + desc->size_of_executable_code)\n");
    }
    
#if defined(__powerpc__)
    libbf_compiler_desasm(desc->sizeof_elt, (char*)regIP);
#endif
        
    old_mem = desc->current_mem;
    old_size = (COUNT_LOW_ACT_HIGH_PAGES(desc)) * pagesize;
        
    delta_mem = libbf_compiler_dynalloc_realloc(desc);

    GET_DATA_PTR_REG(context) += delta_mem;
    desc->executableCodeData.base_data_ptr = (void*)((long)desc->executableCodeData.base_data_ptr + delta_mem);

    if (DEBUG_DYNALLOC) fprintf(stderr, "dynamic memory allocator can go on\n");
        
    /* Unprotecting all - old - user memory, including upper reserved page, for any latter reuse */
    ret = libbf_mprotect(old_mem,
                         old_size,
                         PROT_READ | PROT_WRITE);
    if (ret != 0) fatal("mprotect failed\n");
    
    /* Discarding now old memory */
    libbf_free_aligned(old_mem);
  
  }
  else
  {
    fatal("unexpected sigsegv at 0x%lX\n", (long)info->si_addr);
  }
}

#if (defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(WIN32)) && (defined(__i386__) || defined(__x86_64__))

static void libbf_save_state(DynAllocDesc* desc, void* ptr)
{
  cast_ptr_to_context(ptr, context);

  long pagesize = libbf_getpagesize();
  
  long regIP = (long)GET_IP(context);
        
  int relative_ip = regIP - (long)desc->current_executable_code;
      
  void* data_ptr = (void*)GET_DATA_PTR_REG(context);
  void* base_data_ptr = desc->executableCodeData.base_data_ptr;
  int relative_data_ptr = (long)data_ptr - (long)base_data_ptr;
      
  /* Restore regular protection for user data pages */
  int ret = libbf_mprotect(desc->current_mem,
                           (COUNT_LOW_ACT_HIGH_PAGES(desc)) * pagesize, PROT_READ | PROT_WRITE);
  if (ret != 0) fatal("mprotect failed\n");
      
  assert (regIP >= (long)desc->current_executable_code &&
          regIP < (long)desc->current_executable_code + desc->size_of_executable_code);

#if defined(__i386__)
{
  int eax = GET_AX(context);
  int ebx = GET_BX(context);
  int ecx = GET_CX(context);
  int edx = GET_DX(context);
  int flags = GET_FL(context);
  int i;
  unsigned char* c = (unsigned char*) desc->current_executable_code;

  FILE* f;
  if (desc->options->suspend_file &&  (f = fopen(desc->options->suspend_file, "wb")) != NULL)
  {
    fwrite(desc->current_executable_code, desc->size_of_executable_code, 1, f);
    fwrite(&relative_ip, sizeof(int), 1, f);
    fwrite(&eax, sizeof(int), 1, f);
    fwrite(&ebx, sizeof(int), 1, f);
    fwrite(&ecx, sizeof(int), 1, f);
    fwrite(&edx, sizeof(int), 1, f);
    fwrite(&flags, sizeof(int), 1, f);
    fwrite(&relative_data_ptr, sizeof(int), 1, f);
    fwrite(&desc->count_active_pages, sizeof(int), 1, f);
    fwrite(base_data_ptr, desc->count_active_pages * pagesize, 1, f);
    fclose(f);
  }
  else
  {
    warning("Can't write in suspend file\n");
  }

  /* seek : 83 c4 0c                add    $12,%esp */
  for(i=desc->size_of_executable_code-3-1;i>=0;i--)
  {
    if (c[i] == 0x83 && c[i+1] == 0xc4 && c[i+2] == 4*3)
    {
      GET_IP(context) = (int)(c + i);
      return;
    }
  }
  SHOULDNT_HAPPEN();
}
#else
{
  long rax = GET_AX(context);
  long rdi = GET_DI(context);
  long rsi = GET_SI(context);
  long rcx = GET_CX(context);
  long rdx = GET_DX(context);
  long flags = GET_FL(context);
  int i;
  unsigned char* c = (unsigned char*) desc->current_executable_code;
  
  FILE* f;
  if (desc->options->suspend_file &&  (f = fopen(desc->options->suspend_file, "wb")) != NULL)
  {
    fwrite(desc->current_executable_code, desc->size_of_executable_code, 1, f);
    fwrite(&relative_ip, sizeof(int), 1, f);
    fwrite(&rax, sizeof(rax), 1, f);
    fwrite(&rdi, sizeof(rdi), 1, f);
    fwrite(&rsi, sizeof(rsi), 1, f);
    fwrite(&rcx, sizeof(rcx), 1, f);
    fwrite(&rdx, sizeof(rdx), 1, f);
    fwrite(&flags, sizeof(flags), 1, f);
    fwrite(&relative_data_ptr, sizeof(int), 1, f);
    fwrite(&desc->count_active_pages, sizeof(int), 1, f);
    fwrite(base_data_ptr, desc->count_active_pages * pagesize, 1, f);
    fclose(f);
  }
  else
  {
    warning("Can't write in suspend file\n");
  }

  /* seek : 48 83 c4 18                add    $24,%rsp */
  for(i=desc->size_of_executable_code-4-1;i>=0;i--)
  {
    if (c[i] == 0x48 && c[i+1] == 0x83 && c[i+2] == 0xc4 && c[i+3] == 8*3)
    {
      GET_IP(context) = (long)(c + i);
      return;
    }
  }
  SHOULDNT_HAPPEN();
}
#endif
}
#endif

#if (defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__)) && (defined(__i386__) || defined(__x86_64__))
#define TIMER_RESOLUTION (10 * 1000) /* in usec */


static void libbf_set_itimer()
{
  struct itimerval tv;
  tv.it_interval.tv_sec = 0;
  tv.it_interval.tv_usec = 0;
  tv.it_value.tv_sec = 0;
  tv.it_value.tv_usec = TIMER_RESOLUTION;
  setitimer(ITIMER_REAL, &tv, NULL);
}

static
void libbf_compiler_timer_handler(int unused_signum, siginfo_t* unused_info, void* ptr)
{
  cast_ptr_to_context(ptr, context);
  
  int load = 0;
  
  long pagesize = libbf_getpagesize();
  
  void** esp = (void**)GET_SP(context);
  
  int inc_esp = 0;
  
  if (DEBUG_DYNALLOC) 
    fprintf(stderr, "in libbf_compiler_timer_handler\n");
  
  while (!(esp[1] == (void*)SIGNATURE_1 &&
           esp[2] == (void*)SIGNATURE_2))
  {
    inc_esp++;
    if (inc_esp == 100)
    {
      fprintf(stderr, "should probably not happen. continue...\n");
      break;
      /* SHOULDNT_HAPPEN(); */
    }
    esp++;
  }
  if (esp[1] == (void*)SIGNATURE_1 &&
      esp[2] == (void*)SIGNATURE_2)
  {
    DynAllocDesc* desc = (DynAllocDesc*)esp[0];
    struct timeval tv;
    int ms;
    
    assert(desc->signature1 == SIGNATURE_1);
    assert(desc->signature2 == SIGNATURE_2);
    
    load = desc->options->average_load;

    gettimeofday(&tv, NULL);
    ms = (tv.tv_sec - desc->start_tv.tv_sec) * 1000 + (tv.tv_usec - desc->start_tv.tv_usec) / 1000;

    if (desc->options->stop_after_ms > 0 && ms > desc->options->stop_after_ms)
    {
      long regIP;
      if (inc_esp > 0)
      {
        int i;
        for(i=1;i<=3;i++)
        {
          regIP = (long)esp[-i];
          if (regIP >= (long)desc->current_executable_code &&
              regIP < (long)desc->current_executable_code + desc->size_of_executable_code)
          {
            int ret;
            desc->isInterrupted = TRUE;
    
            ret = libbf_mprotect(desc->current_mem,
                                (COUNT_LOW_ACT_HIGH_PAGES(desc)) * pagesize, PROT_NONE);
            if (ret != 0) fatal("mprotect failed\n");
            break;
          }
        }
        if (!desc->isInterrupted)
        {
          SHOULDNT_HAPPEN();
        }
        return;
      }
      else
      {
        libbf_save_state(desc, ptr);
      }
    }
  }
  
  if (load > 0)
  {
    int usec;
    /* (100000 usec / (100000 usec + x)) = load / 100 */
    if (load > 100) load = 100;
    usec = TIMER_RESOLUTION * 100 / load - TIMER_RESOLUTION;
    if (usec >= 0) usleep(usec);
  }

  libbf_set_itimer();

}
#endif

#if defined(WIN32)
__stdcall unsigned int libbf_compiler_timer_handler_win32(void* pvoid)
{
  DynAllocDesc* dynAllocDesc = (DynAllocDesc*)pvoid;
  dynAllocDesc->hasStarted = TRUE;
  while(GetTickCount() - dynAllocDesc->start_time < dynAllocDesc->options->stop_after_ms)
  {
    Sleep(100);
  }
  
  dynAllocDesc->isInterrupted = TRUE;
  
  long pagesize = libbf_getpagesize();
  int ret;
  ret = libbf_mprotect(dynAllocDesc->current_mem,
                       (COUNT_LOW_ACT_HIGH_PAGES(dynAllocDesc)) * pagesize, PROT_NONE);
  if (ret != 0) fatal("mprotect failed\n");
  
  return 0;
}
#endif

void* libbf_compiler_dynalloc_init(ExecutableCodeData_t* executableCodeData,
                                  OFFSET_t min_delta_offset,
                                  OFFSET_t max_delta_offset,
                                  int sizeof_my_type,
                                  void* executable_code,
                                  int size_of_executable_code,
                                  void* init_data_ptr,
                                  int count_active_pages,
                                  AdvancedOptions_t* options)
{
#if !defined(WIN32)
  struct sigaction action;
  sigset_t block_mask;
#endif
  int ret;
  
  long pagesize = libbf_getpagesize();
  
  DynAllocDesc* dynAllocDesc = alloc0(DynAllocDesc, 1); 
  void* mem;
  int reserved_size;
    
  int count_low_pages = (min_delta_offset * sizeof_my_type + pagesize - 1) / pagesize; /* ceil rounding */
  int count_high_pages = (max_delta_offset * sizeof_my_type + pagesize - 1) / pagesize; /* ceil rounding */
  if (count_active_pages <= 0)
    count_active_pages = NB_MIN_ACTIVE_PAGES; /* number of initial regular pages */
  
  reserved_size = (count_low_pages + count_active_pages + count_high_pages) * pagesize;
  mem = libbf_alloc_aligned(pagesize, reserved_size);
  if (mem == NULL) fatal("alloc_aligned failed");
  memset(mem, 0, reserved_size);
  
  if (init_data_ptr)
    memcpy((void*)((long)mem + count_low_pages * pagesize), init_data_ptr, count_active_pages * pagesize);
  
  dynAllocDesc->executableCodeData = *executableCodeData;
  dynAllocDesc->executableCodeData.input_output_data = dynAllocDesc;
  dynAllocDesc->executableCodeData.base_data_ptr = (void*)((long)mem + count_low_pages * pagesize);
  dynAllocDesc->signature1 = SIGNATURE_1;
  dynAllocDesc->signature2 = SIGNATURE_2;
  dynAllocDesc->sizeof_elt = sizeof_my_type;
  dynAllocDesc->increase_from_low = 0;
  dynAllocDesc->increase_from_high = 0;
  dynAllocDesc->current_mem = mem;
  dynAllocDesc->count_low_pages = count_low_pages;
  dynAllocDesc->count_active_pages = count_active_pages;
  dynAllocDesc->count_high_pages = count_high_pages;
  dynAllocDesc->max_total_pages = -1; /* unlimited */
  dynAllocDesc->current_executable_code = executable_code;
  dynAllocDesc->size_of_executable_code = size_of_executable_code;
  dynAllocDesc->options = options;
  dynAllocDesc->isInterrupted = FALSE;
  dynAllocDesc->sigalarm_set = FALSE;
  dynAllocDesc->sigalarm_active = FALSE;
  
#if !defined(WIN32)
  action.sa_sigaction = libbf_compiler_dynalloc_handler;
  sigemptyset(&(action.sa_mask));
  action.sa_flags = SA_SIGINFO;
  sigemptyset(&block_mask);
  sigaddset(&block_mask, SIGALRM);
  action.sa_mask = block_mask;

  ret = sigaction(SIGBUS, &action, &dynAllocDesc->old_action_sigbus);
  if (ret != 0) fatal("sigaction failed");
  
  ret = sigaction(SIGSEGV, &action, &dynAllocDesc->old_action_sigsegv);
  if (ret != 0) fatal("sigaction failed");
#endif

#if defined(WIN32)
  if (options && options->stop_after_ms > 0)
  {
    dynAllocDesc->hasStarted = FALSE;
    dynAllocDesc->start_time = GetTickCount();
    _beginthreadex(NULL, 0, libbf_compiler_timer_handler_win32, dynAllocDesc, 0, 0);
    while(dynAllocDesc->hasStarted == FALSE) Sleep(100);
  }
#elif (defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__)) && (defined(__i386__) || defined(__x86_64__))
  if (options && (options->stop_after_ms > 0 || options->average_load > 0))
  {
    struct itimerval tv;
    tv.it_interval.tv_sec = 0;
    tv.it_interval.tv_usec = 0;
    if (options->average_load == 0)
    {
      tv.it_value.tv_sec = options->stop_after_ms / 1000;
      tv.it_value.tv_usec = (options->stop_after_ms % 1000) * 1000;
    }
    else
    {
      tv.it_value.tv_sec = 0;
      tv.it_value.tv_usec = TIMER_RESOLUTION;
    }
    setitimer(ITIMER_REAL, &tv, NULL);
    action.sa_sigaction = libbf_compiler_timer_handler;
    sigemptyset(&(action.sa_mask));
    action.sa_flags = SA_SIGINFO;
      
    memcpy(&dynAllocDesc->saved_action_sigalrm, &action, sizeof(struct sigaction));
    
    ret = sigaction(SIGALRM, &action, &dynAllocDesc->old_action_sigalrm);
    if (ret != 0) fatal("sigaction failed");
      
    gettimeofday(&dynAllocDesc->start_tv, NULL);
    
    dynAllocDesc->sigalarm_set = TRUE;
    dynAllocDesc->sigalarm_active = TRUE;
  }
#endif

  /* Lock low and high pages, upper reserved page and checker description page */
  ret = libbf_mprotect(mem, pagesize * count_low_pages, PROT_NONE);
  ret |= libbf_mprotect((void*)((long)mem + (count_low_pages + count_active_pages) * pagesize),
                        (count_high_pages) * pagesize, PROT_NONE);
  if (ret != 0) fatal("mprotect failed");
  
  return dynAllocDesc;
}

void libbf_compiler_dynalloc_toggle_suspend_or_resume(void* ptr)
{
#if (defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__)) && (defined(__i386__) || defined(__x86_64__))
  DynAllocDesc* desc = (DynAllocDesc*)ptr;
  assert(desc->signature1 == SIGNATURE_1);
  assert(desc->signature2 == SIGNATURE_2);
  
  if (desc->sigalarm_set)
  {
    if (desc->sigalarm_active)
    {
      struct sigaction tmp;
      tmp.sa_handler = SIG_IGN;
      sigemptyset(&(tmp.sa_mask));
      tmp.sa_flags = 0;
      sigaction(SIGALRM, &tmp, &desc->saved_action_sigalrm);
    }
    else
    {
      sigaction(SIGALRM, &desc->saved_action_sigalrm, NULL);
      libbf_set_itimer();
    }
    desc->sigalarm_active = !desc->sigalarm_active;
  }
#endif
}

void libbf_compiler_dynalloc_end(void* ptr)
{
  long pagesize = libbf_getpagesize();
  int ret;
  
  DynAllocDesc* desc = (DynAllocDesc*)ptr;
  assert(desc->signature1 == SIGNATURE_1);
  assert(desc->signature2 == SIGNATURE_2);
  
#if !defined(WIN32)
  sigaction(SIGBUS, &desc->old_action_sigbus, (struct sigaction*)NULL);
  sigaction(SIGSEGV, &desc->old_action_sigsegv, (struct sigaction*)NULL);
#endif

#if (defined(__linux__) || defined(__FreeBSD__) || defined(__OpenBSD__)) && (defined(__i386__) || defined(__x86_64__))
  if (desc->sigalarm_set)
  {
    sigaction(SIGALRM, &desc->old_action_sigalrm, (struct sigaction*)NULL);
  }
#endif

  /* Restore regular protection for user data pages */
  ret = libbf_mprotect(desc->current_mem,
                           (COUNT_LOW_ACT_HIGH_PAGES(desc)) * pagesize, PROT_READ | PROT_WRITE);
  if (ret != 0) fatal("mprotect failed\n");

  if (DEBUG_DYNALLOC)
  {
    fprintf(stderr, "%d pages have been allocated\n", desc->count_active_pages);
    fprintf(stderr, "freeing from 0x%lX\n", (long)desc->current_mem);
  }
  
  /* Free user data : low protection pages, regular pages, high protection pages */
  libbf_free_aligned(desc->current_mem);
  
  free(desc);
}


#endif

#endif
