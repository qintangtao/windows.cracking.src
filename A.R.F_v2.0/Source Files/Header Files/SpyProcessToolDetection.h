#ifndef SPY_PROCESS_TOOL_DETECTION_H_
#define SPY_PROCESS_TOOL_DETECTION_H_

/*This class contains methods dedicated to the detection of process spy tools
  used to monitor the process on runtime for API calls etc....*/

#include <Windows.h>


class SpyProcessToolDetection{
     

public:

	   static const DWORD spyStudioChksum = 0x8D9D78EA; // SpyStudio (nektra.com)

	   static const  DWORD apiMonitorChksum = 0x3E0BD34C; // API Monitor (rohitab.com) 



	      SpyProcessToolDetection(); //constructor

		  bool SpyLDRModDetect(DWORD moduleNameChksum); /* detect if the process is being monitored by locating the injected dll 
														*** 32-bit apps only *** */

		  bool SpyLDRGenericDetect(DWORD modulesNumber); /* generic detection of any process monitoring tool through the
														 verification of the number of the loaded modules.
														 *** 32-bit apps only *** */
		  
};

#endif