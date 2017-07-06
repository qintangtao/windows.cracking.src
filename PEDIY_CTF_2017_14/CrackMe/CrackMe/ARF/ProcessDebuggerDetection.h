#ifndef PROCESS_DEBUGGER_DETECTION_H_

#define PROCESS_DEBUGGER_DETECTION_H_


#include <windows.h>
#include <Tlhelp32.h>
#include <string>


using namespace std;

/*This class contains methods that will try to detect a running instance of a debugger or a reversing tool from

  its process name and the parent process ID. Normally the parent process ID must the be the ID of windows explorer.exe,

  otherwise it more likely that the process is currently being  debugged.


*/

class ProcessDebuggerDetection{


private:      
	
	     string * processlist; 

		  int listsize;


  
public:

	    ProcessDebuggerDetection(); // constructor

		
		string * SetProcessList(); /* this method will fill the processlist dynamic array and it will 

								      return a pointer to it. 

		                              You can modify this list at will, but it you do so, don't forget to write the correct
									  
									  size to the SetListSize method bellow.*/


		void SetListSize() { listsize = 35; } // set list size


		int GetListSize() { SetListSize(); return listsize;} // retrieve list size


		int ProcessDetection(string * arraymemlocation , int listsize); /* Detection through process name list of

	   most popular debuggers/reversing tools.

It will use the pointer to the processlist dynamic array returned by the function SetProcessList() above.

 This function serves for multiple detection of most well-known reversing tools through their process name.

*/



};




#endif