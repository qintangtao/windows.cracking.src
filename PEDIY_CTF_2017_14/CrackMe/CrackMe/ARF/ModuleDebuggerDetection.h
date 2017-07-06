#ifndef MODULE_DEBUGGER_DETECTION_H_

#define MODULE_DEBUGGER_DETECTION_H_


#include <windows.h>
#include <Tlhelp32.h>
#include <string>


using namespace std;

class ModuleDebuggerDetection{

	private: 
		
		      string * moduleslist; // we are going to use unicode in this case 

			  int listsize;

		       


	      


public:

	    ModuleDebuggerDetection(); //constructor


	    string * SetModulesList(); /* this method will fill the moduleslist dynamic array and it will 

								      return a pointer to it.  
									  
									  You can modify this list at will, but it you do so, don't forget to write the correct
									  
									  size to the SetListSize method bellow.*/


        void SetListSize() { listsize = 18; } // set list size


		int GetListSize() { SetListSize(); return listsize;} // retrieve list size

	     
	    int ModuleDetection(string * arraymemlocation, int listsize); /* This method will take a list of all running processes and then
											                              for each process will go through its loaded modules in order to
																		  detect a debugger or a reversing tools through its own loaded modules
																		  and/or plugins.
																	   .
								  It will use the pointer to the moduleslist dynamic array returned by the function SetProcessList() above in

								  order to check if any of the loaded modules matches any of those in the list.

		*/




};




#endif
