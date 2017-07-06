#ifndef ANTI_ATTACH_H_

#define ANTI_ATTACH_H_


/*This class contains methods through which we can forbid to a user mode debugger to attach to our process while
  our process is running.

  More specifically, since it is known that each time a user mode debugger attaches to our process, there will be a call

  to the APIs DebugUiRemoreBreakin and DbgBreakPoint from our process, we can alter these 2 functions in order forbid to
  
  the debugger to succesfully attach to our process by changing their entry point with a ret instruction.
  
  The second method uses a more sophisticated trick. It will actually create a child process and it will debug it on run time.
  
  The execution of the code will take place on the child process in which we will not be able to attach a user mode debugger
  
  because it will be already being debugged by its parent process.
  
  
  ***NOTE***: Read the documentation about these 2 methods before using them.
		
		*/



#include <windows.h>



class AntiAttach{

private:
		  PROCESS_INFORMATION pi;

		  STARTUPINFO si;

		  DEBUG_EVENT de;



   
public : 

	    AntiAttach(); //constructor

		int AntiAttachSet(); // set anti-attach to for our process

		void AntiAttachSelfDebug(); // 




};





#endif