#ifndef SEH_DEBUGGER_DETECTION_H_

#define SEH_DEBUGGER_DETECTION_H_


/*This class contains methods that enables us to detect the presence of a debugger through exception generation.
 
  In other words by monitoring what happens after that an exception has been raised or not we can determine if our process
  
  is under debugging or not.
  
  **** These methods may work or not depending on the debugger's configuration regarding exceptions.****
  */

#include <Windows.h>


class SehDbgDetection{


public:

	    SehDbgDetection(); //constructor


		bool CloseHandleExcepDetection(HANDLE invalid); /* In this method we are using CloseHandle win API by pushing and invalid

		                                          handle to it. If there is no debugger attached to our process
												  
												  CloseHandle will just return an error code and the execution will continue.

												  However, if a debugger is attached then an exception will be raised.
												  */

	  bool SingleStepExcepDetection(); /*In this method we trigger a single step exception by pushing the EFLAGS to the stack
									     and then setting the trap flag bit. Finally we restore them back with the trap flag bit set.
										 If a debugger is attached it will intercept the exception so execution will never reach our
										 SEH.
	                                   */



	  bool OutputDebugStringExcepDetection(); /* In this method we send a string to the debugger. If a debugger is present the return address in EAX will be a valid
											     VA inside the process address space, otherwise if the process in not being debugger a memory access violation will occur
												 and we will know that no debugger is present.*/
		
};



#endif