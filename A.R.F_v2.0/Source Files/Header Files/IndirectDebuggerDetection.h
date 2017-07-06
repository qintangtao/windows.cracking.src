#ifndef INDIRECT_DEBUGGER_DETECTION_H_

#define INDIRECT_DEBUGGER_DETECTION_H_

/*

This class contains methods that will try to indirectly detect the presence of

a user-mode debugger.

For example,by making a call to a win API and checking the value of a cpu register afterwards,

or by trying to obtain a handle to a vital system process such as services.exe with process_all_access rights.

This trick works in case the debugger has enabled the SeDebugPrivilege to our process which for sure

our process does not have by default. So if we manage to obtain a valid handle to that process with these

rights, then our process is being debugged.


*/

#include <windows.h>
#include <Tlhelp32.h>
#include <string>


using namespace std;


class IndirectDebuggerDetection{


public:

	  IndirectDebuggerDetection(); //constructor

	  bool DebugString(); /*This method will try to take advantage of the value of eax
						  
						   after calling OutputDebugStringA with a valid ascii string.
						   
If no debugger is present the value of eax will be 0 or 1 when returning from that function,
otherwise it will not be 0 or 1.*/

	 int OpenServicesProcess(); /*This method will try to obtain a valid handle through openprocess
							 
							 with process_all_access rights to services.exe. See explanation above...*/




};




#endif