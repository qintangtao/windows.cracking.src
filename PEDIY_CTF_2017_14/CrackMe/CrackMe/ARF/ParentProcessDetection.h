#ifndef PARENT_PROCESS_DETECTION_H_

#define PARENT_PROCESS_DETECTION_H_


/* This class contains a method that will check if the parent process of the process of our application is
   the windows explorer,otherwise if it is not it means that most probably the process of our application
   has been launched through a debugger or a loader.

  **** This method must not be used if you plan to design an application that will be launched from another
   one that your already know that it is legitimate.****
*/

#include <windows.h>
#include <Tlhelp32.h>
#include <string>


using namespace std;

class ParentProcessDetection{


public:

	 ParentProcessDetection(); //constructor

	 int CheckParentProcess(); /* This method will retrieve the parent process id of our process and	
	                              then will verify that this id corresponds to the process id of windows
								  explorer.
	 
	                         */

};


#endif