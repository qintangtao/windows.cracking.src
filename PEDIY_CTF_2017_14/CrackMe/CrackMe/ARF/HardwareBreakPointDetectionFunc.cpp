#include "stdafx.h"




 HardwareBreakPointDetection::HardwareBreakPointDetection() { };


//------------------------------------------------------------------------------------------------------------




int HardwareBreakPointDetection::HwdBreakPoint()
{

CONTEXT context; // initialize CONTEXT structure

context.ContextFlags = CONTEXT_DEBUG_REGISTERS; // set context flags accordingly to have access to the registers we need

DWORD threadId = GetCurrentThreadId(); // obtain the identifier of the current thread

HANDLE thread = OpenThread(THREAD_GET_CONTEXT,FALSE,threadId); /* obtain a handle to the current thread with
                                                                
															      THREAD_GET_CONTEXT access rights.	 
															  */

 if(thread!=NULL) //if we managed to take a valid handle to the current thread
 {
   if(GetThreadContext(thread,&context)) /* since we are only interested in the Debug registers we do not have 

										     to suspend the examined thread.*/
  {

	 if(context.Dr0!=0 || context.Dr1!=0 || context.Dr2!=0 || context.Dr3!=0)
	 {

		 CloseHandle(thread);

         
		 return 1; //hardware bp detected


	 }


	 else{

		 CloseHandle(thread);


		 return 0; //no hardware bp detected
	 }

 }

 else{

	 CloseHandle(thread);

	 return -2; // if the GetThreadContext function fails
 }



 }



 return -1; //if OpenThread function failed to obtain a valid handle to the current thread
 

}



//------------------------------------------------------------------------------------------------------------


DWORD check; //this variable will hold the final result after checking the values of DR0-DR3 registers

DWORD ExceptFilter(EXCEPTION_POINTERS * ptr)
{

	check = 0;

	check |= ptr->ContextRecord->Dr0;
	check |= ptr->ContextRecord->Dr1;
	check |= ptr->ContextRecord->Dr2;
	check |= ptr->ContextRecord->Dr3;

	return EXCEPTION_EXECUTE_HANDLER;

}


bool HardwareBreakPointDetection::HWdBreakPointSeh()
{

 __try{

	 __asm xor eax,eax
	 __asm mov dword ptr[eax],ebx //in this case I am forcing a memory access violation exception, but you can customize this to generate other type of exceptions.
}


 __except(ExceptFilter(GetExceptionInformation()))
 {

	 if(check!=0) //if not equal to zero -> HW BP detected so we return true
    {
	  check = 0; //reset - we don't need it anymore
	  return true;
   }

return false; // if no HW BP was found, return false


 }


 /* If we reach this location, it means that the exception was handled by something else, maybe a debugger or another reversing or analysis
    tool, so we return true for security purposes, but not because a HW BP was detected.  */
 return true; 

}