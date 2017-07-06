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

#define _XOPEN_SOURCE 600

#include <unistd.h>
#include <stdlib.h>

#include "libbf_posix_emulation.h"
#include "libbf_internal.h"

#if defined(WIN32)
  #include <windows.h>
  int libbf_getpagesize() 
  { 
    SYSTEM_INFO SysInfo; 
    GetSystemInfo(&SysInfo); 
    return (size_t)SysInfo.dwPageSize; 
  } 

  int libbf_mprotect(void *Addr, int Size, int ProtFlags) 
  { 
    int rc; 
    DWORD Flags, OldFlags; 
      
    if (ProtFlags & PROT_WRITE)
    { 
      Flags = PAGE_READWRITE; 
    } 
    else if (ProtFlags & PROT_READ)
    { 
      Flags = PAGE_READONLY; 
    } 
    else
    { 
      Flags = PAGE_NOACCESS; 
    } 
      
    rc = VirtualProtect(Addr, Size, Flags, &OldFlags);
  
    return (rc != 0) ? 0 : -1; 
  }

  void* libbf_alloc_aligned(int alignment, int size)
  {
    void *addr = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
    if ((((long)addr) & (~(alignment-1))) != ((long)addr))
    {
      fatal("Bad alignment\n");
    }
    return addr;
  }
   
  void libbf_free_aligned(void* ptr)
  {
    int ret = VirtualFree(ptr, 0, MEM_RELEASE);
    if (ret == 0) fatal("VirtualFree failed");
  }

#else
  #if defined(__FreeBSD__)  || defined(__OpenBSD__)
    #include <sys/mman.h>
    /* In FreeBSD, allocations greeter than one page are page aligned. pretty cool ! */
    void* libbf_alloc_aligned(int alignment, int size)
    {
      return malloc(size);
    }

  #elif defined(__CYGWIN__) || defined(sun)
    void* libbf_alloc_aligned(int alignment, int size)
    {
      void* addr = memalign(alignment, size);
      if ((((long)addr) & (~(alignment-1))) != ((long)addr))
      {
        fatal("Bad alignment\n");
      }
      return addr;
    }    
  #elif defined(__linux__)
    #include <sys/mman.h>
    void* libbf_alloc_aligned(int alignment, int size)
    {
      void* addr = NULL;
      posix_memalign(&addr, alignment, size);
      if ((((long)addr) & (~(alignment-1))) != ((long)addr))
      {
        fatal("Bad alignment\n");
      }
      return addr;
    }
  
    #if 0
    #define _GNU_SOURCE
    #include <ucontext.h>
    #endif
    
  #else
  #error "Unhandled OS"
  #endif
    
  void libbf_free_aligned(void* ptr)
  {
    free(ptr);
  }
    
  int libbf_mprotect(void *ptr, int size, int protFlags)
  {
    return mprotect(ptr, size, protFlags);
  }
    
  int libbf_getpagesize()
  {
    return getpagesize();
  }    
#endif
