#include "stdafx.h"





DirectDebuggerDetection::DirectDebuggerDetection() {  };



//----------------------------------------------------------------------------------------------------------------------



bool DirectDebuggerDetection::DebuggerPresent() 
{ 


if(IsDebuggerPresent()) //this is a win API located in kernel32.dll in older win version and in kernelbase.dll in newer versions
{
	return true;
} 

else {
	
	return false;

}

}




//-----------------------------------------------------------------------------------------------------------------


int DirectDebuggerDetection::RemoteDebuggerPresent()
{

BOOL remotedbgpresent = FALSE; // set initially to false


DWORD processId = GetCurrentProcessId(); //obtain our process identifier
	
	
HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION ,FALSE,processId); // obtain a handle to our process


if(process!=NULL) //if we managed to obtain a valid handle
{

	if(CheckRemoteDebuggerPresent(process,&remotedbgpresent)) //win api located in kernel32.dll
	{

		if(remotedbgpresent) //if debuggerdetected
		{

		CloseHandle(process);

		return 1;
		}
	


		
		CloseHandle(process);

		return 0; //if debugger not detected

	


	}

	    CloseHandle(process);

        return -2; // if CheckRemoteDebuggerPresent Api failed


}


    return -1; //if we failed to obtain a valid handle to our process in the first place
	

}