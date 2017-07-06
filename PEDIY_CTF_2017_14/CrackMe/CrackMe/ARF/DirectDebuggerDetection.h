#ifndef DIRECT_DEBUGGER_DETECTION_H_

#define DIRECT_DEBUGGER_DETECTION_H_

/* The simplest method to detect any user-mode debugger attached to the process of our application,

 by using a direct call to IsDebuggerPresent and/or CheckRemoteDebuggerPresent APIs.

*/

#include <windows.h>


class DirectDebuggerDetection{

	

public: 

    DirectDebuggerDetection(); //constructor

	bool DebuggerPresent(); // detection through IsDebuggerPresent API

    int RemoteDebuggerPresent(); // detection through CheckRemoteDebuggerPresent API

								  

	 

};

#endif