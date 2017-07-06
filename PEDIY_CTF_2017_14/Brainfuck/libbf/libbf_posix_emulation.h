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

#ifndef LIBBF_POSIX_EMULATION_H
#define LIBBF_POSIX_EMULATION_H 

/* Emulation of POSIX functions and enum */

#if defined(WIN32)
  enum
  {
    PROT_NONE = 0,
    PROT_READ = 1 << 0,
    PROT_WRITE = 1 << 1,
    PROT_EXEC = 1 << 2,
  };

  /* Fake siginfo struct to use the POSIX sigaction handler */
  typedef struct
  {
    int si_code;
    void* si_addr;
  } siginfo_t;

  #define SEGV_ACCERR 0  
  #define SIGBUS 0
  
#elif defined(__FreeBSD__)
  #include <sys/mman.h>
  #define SEGV_ACCERR 0  
  
#elif defined(__linux__)  || defined(__OpenBSD__) ||defined(__CYGWIN__) ||defined(sun)
  #include <sys/mman.h>
#endif

void* libbf_alloc_aligned(int alignment, int size);
void  libbf_free_aligned(void* ptr);
int   libbf_mprotect(void *ptr, int size, int protFlags);
int   libbf_getpagesize();


#if defined(__linux__)
  #if defined(__i386__)
    #define GET_IP(context) context->uc_mcontext.gregs[REG_EIP]
    #define GET_AX(context) context->uc_mcontext.gregs[REG_EAX]
    #define GET_BX(context) context->uc_mcontext.gregs[REG_EBX]
    #define GET_CX(context) context->uc_mcontext.gregs[REG_ECX]
    #define GET_DX(context) context->uc_mcontext.gregs[REG_EDX]
    #define GET_SI(context) context->uc_mcontext.gregs[REG_ESI]
    #define GET_DI(context) context->uc_mcontext.gregs[REG_EDI]
    #define GET_FL(context) context->uc_mcontext.gregs[REG_EFL]
    #define GET_BP(context) context->uc_mcontext.gregs[REG_EBP]
    #define GET_SP(context) context->uc_mcontext.gregs[REG_ESP]
  #elif defined(__x86_64__)
    #define GET_IP(context) context->uc_mcontext.gregs[REG_RIP]
    #define GET_AX(context) context->uc_mcontext.gregs[REG_RAX]
    #define GET_BX(context) context->uc_mcontext.gregs[REG_RBX]
    #define GET_CX(context) context->uc_mcontext.gregs[REG_RCX]
    #define GET_DX(context) context->uc_mcontext.gregs[REG_RDX]
    #define GET_SI(context) context->uc_mcontext.gregs[REG_RSI]
    #define GET_DI(context) context->uc_mcontext.gregs[REG_RDI]
    #define GET_R12(context) context->uc_mcontext.gregs[REG_R12]
    #define GET_R13(context) context->uc_mcontext.gregs[REG_R13]
    #define GET_R15(context) context->uc_mcontext.gregs[REG_R15]
    #define GET_FL(context) context->uc_mcontext.gregs[REG_EFL]
    #define GET_BP(context) context->uc_mcontext.gregs[REG_RBP]
    #define GET_SP(context) context->uc_mcontext.gregs[REG_RSP]
  #elif defined(__powerpc__)
    #define GET_IP(context) context->uc_mcontext.uc_regs->gregs[PT_NIP]
    #define GET_R14(context) context->uc_mcontext.uc_regs->gregs[PT_R14]
    #define GET_R15(context) context->uc_mcontext.uc_regs->gregs[PT_R15]
  #endif
#elif defined(__FreeBSD__)
  #if defined(__i386__)
    #define GET_IP(context) context->uc_mcontext.mc_eip
    #define GET_AX(context) context->uc_mcontext.mc_eax
    #define GET_BX(context) context->uc_mcontext.mc_ebx
    #define GET_CX(context) context->uc_mcontext.mc_ecx
    #define GET_DX(context) context->uc_mcontext.mc_edx
    #define GET_SI(context) context->uc_mcontext.mc_esi
    #define GET_DI(context) context->uc_mcontext.mc_edi
    #define GET_FL(context) context->uc_mcontext.mc_eflags
    #define GET_BP(context) context->uc_mcontext.mc_ebp
    #define GET_SP(context) context->uc_mcontext.mc_esp
  #elif defined(__x86_64__)
    #define GET_IP(context) context->uc_mcontext.mc_rip
    #define GET_AX(context) context->uc_mcontext.mc_rax
    #define GET_BX(context) context->uc_mcontext.mc_rbx
    #define GET_CX(context) context->uc_mcontext.mc_rcx
    #define GET_DX(context) context->uc_mcontext.mc_rdx
    #define GET_SI(context) context->uc_mcontext.mc_rsi
    #define GET_DI(context) context->uc_mcontext.mc_rdi
    #define GET_R12(context) context->uc_mcontext.mc_r12
    #define GET_R13(context) context->uc_mcontext.mc_r13
    #define GET_R15(context) context->uc_mcontext.mc_r15
    #define GET_FL(context) context->uc_mcontext.mc_rflags
    #define GET_BP(context) context->uc_mcontext.mc_rbp
    #define GET_SP(context) context->uc_mcontext.mc_rsp
  #endif
#elif defined(__OpenBSD__)
  #if defined(__i386__)
    #define GET_IP(context) context->sc_eip
    #define GET_AX(context) context->sc_eax
    #define GET_BX(context) context->sc_ebx
    #define GET_CX(context) context->sc_ecx
    #define GET_DX(context) context->sc_edx
    #define GET_SI(context) context->sc_esi
    #define GET_DI(context) context->sc_edi
    #define GET_FL(context) context->sc_eflags
    #define GET_BP(context) context->sc_ebp
    #define GET_SP(context) context->sc_esp
  #endif
#elif defined(sun) && defined(__sparc__)
  #define GET_O4(context) context->uc_mcontext.gregs[REG_O4]
  #define GET_O5(context) context->uc_mcontext.gregs[REG_O5]
  #define GET_IP(context) context->uc_mcontext.gregs[REG_PC]
#elif defined(WIN32)
  #define GET_IP(context) context->Eip
  #define GET_AX(context) context->Eax
  #define GET_BX(context) context->Ebx
  #define GET_CX(context) context->Ecx
  #define GET_DX(context) context->Edx
  #define GET_SI(context) context->Esi
  #define GET_DI(context) context->Edi
  #define GET_FL(context) context->EFlags
  #define GET_BP(context) context->Ebp
  #define GET_SP(context) context->Esp
#endif

#endif
