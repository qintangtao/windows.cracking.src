#ifndef API_BREAKPOINT_DETECTION_H_

#define API_BREAKPOINT_DETECTION_H_


/* This class contains a method through which we can detect if someone has placed a breakpoint to the 
   beginning of a win API and more specifically to its first 5 bytes.*/



#include <windows.h>




class ApiBreakPointDetection{

public: 
	
    ApiBreakPointDetection(); //constructor

	int ApiBreakPoint(char * DLL, char * API);

};


#endif