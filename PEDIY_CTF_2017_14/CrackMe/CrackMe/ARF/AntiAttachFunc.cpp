#include "stdafx.h"
#include <cstdlib>


using namespace andrivet::ADVobfuscator;

AntiAttach::AntiAttach() { };



//-----------------------------------------------------------------------------------------------------------



int AntiAttach::AntiAttachSet()
{

HANDLE hproc = NULL; //handle to our process

HMODULE hmodule = NULL; //handle to the DLL

char * DLL = "ntdll";

char * API1 = "DbgUiRemoteBreakin";

char * API2 = "DbgBreakPoint";

DWORD procid = 0; //our process id

DWORD api1Address = 0; //address of API

DWORD api2Address = 0; //address of API2

DWORD byteswritten = 0;


const BYTE ret [] = {0xC3}; // return operation code



procid = GetCurrentProcessId(); //get our process id



hproc = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, false, procid); /*obtain a handle to our process with
																			necessary access rights.*/


if(hproc != NULL) //if we succesfully obtained the handle
{

hmodule = LoadLibraryA(DLL); //obtain a handle to the ntdll

if(hmodule != NULL) // if we succesfully obtained the handle
{

	api1Address = (DWORD)GetProcAddress(hmodule, API1); //obtain address of API1

	api2Address = (DWORD)GetProcAddress(hmodule, API2); //obtain address of API2

	if(api1Address != NULL && api2Address!= NULL) //if we succesfully obtained addresses of both APIs
	{

      if(WriteProcessMemory(hproc, (LPVOID)api1Address, ret, 0x01, &byteswritten) && WriteProcessMemory(hproc, (LPVOID)api2Address, ret, 0x01, &byteswritten))
	  {


		  CloseHandle(hproc);

		  FreeLibrary(hmodule);
         
           return 1; // anti-attach set succesfully

	  }

	  else{


		  CloseHandle(hproc);

		  FreeLibrary(hmodule);


		  return -4; // if we failed to place a ret instruction to the entry point of one of the 2 APIs
	  }



	}

	else{


		

		FreeLibrary(hmodule);

		return -3; //if we failed to obtain the addresses of both APIs
	}


}

else{


     CloseHandle(hproc);

	 return -2; // if we failed to obtain a handle to ntdll
}






}


return -1; // if we failed to obtain a handle to our process in the first place




}


//------------------------------------------------------------------------------------------------------------------------------


void AntiAttach::AntiAttachSelfDebug()
{

BYTE buf [] = {0xC3}; // the RET byte

DWORD byteswriten = 0;

bool IsChildAlive = true;

HANDLE thr = NULL;

HANDLE proc2 = NULL;

HANDLE proc = NULL;

DWORD stack = 0;

DWORD * stackcontent = 0;

DWORD methodaddress = 0;

DWORD exitcode = 0;



ZeroMemory( &si, sizeof(STARTUPINFO) );

ZeroMemory( &pi, sizeof(PROCESS_INFORMATION));

si.cb = sizeof(STARTUPINFO);

si.dwFlags = STARTF_USESHOWWINDOW;

si.wShowWindow = SW_SHOWNORMAL;

//OBFUSCATED_CALL_P(DetectDebugger1, exit, 1);

	if(CreateProcess(NULL, GetCommandLine(), NULL, NULL, TRUE, CREATE_SUSPENDED|DEBUG_PROCESS, NULL, NULL, &si, &pi))
	{

	   proc2 = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, pi.dwProcessId);


	   if(proc2 != 0)
		{

		void(AntiAttach::*ptr) (); // function pointer of the same type of this method


		 ptr = &AntiAttach::AntiAttachSelfDebug; /*retrieve address of this method

		                                            in memory*/

		stack = (DWORD)&ptr; // retrieve address of stack where contect of ptr is stored

		
		stackcontent = (DWORD *) stack; /*create a DWORD * pointer that points to the address of stack
		                                where content of ptr is stored which is the address of this method*/


	    methodaddress = *stackcontent; // and finally store the address of this method to a DWORD variable

		                                    // so that we can push it as a parameter in WriteProcessMemory





		 if( WriteProcessMemory(proc2, (LPVOID)methodaddress, buf, 1, &byteswriten)) /*write RET instruction at the beggining of 
			                                                                         the method*/
		  {


			 thr = OpenThread(THREAD_SUSPEND_RESUME , FALSE, pi.dwThreadId); //handle to the main thread of child process

			  ResumeThread(pi.hThread); //resume its execution

			  CloseHandle(thr); // close the handle
		  }


		}




	while(IsChildAlive) //while the child process is alive we keep debugging it
	{


	WaitForDebugEvent(&de, INFINITE); //wait for a debug event

	if(de.dwDebugEventCode != EXCEPTION_DEBUG_EVENT)
	{

	  ContinueDebugEvent(pi.dwProcessId, pi.dwThreadId, DBG_CONTINUE); //continue execution of child process

	}


	else if((de.dwDebugEventCode == EXCEPTION_DEBUG_EVENT))
	{

		 ContinueDebugEvent(pi.dwProcessId, pi.dwThreadId, DBG_EXCEPTION_NOT_HANDLED); //continue execution of child process

	}
	

	if(de.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT) /*if the child process returns debug event code 5 it means that
		                           its process has been terminated*/
	{

		IsChildAlive = false; //so we set this variable to false to exit the loop

	}

  }


  //and the parent must die too...



  	 proc = OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_TERMINATE , FALSE , GetCurrentProcessId());

     GetExitCodeProcess(proc, &exitcode);

	 TerminateProcess(proc,exitcode);

	

}

	


}