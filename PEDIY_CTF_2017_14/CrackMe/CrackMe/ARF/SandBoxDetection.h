#ifndef SANDBOX_DETECTION_H_
#define SANDBOX_DETECTION_H_

/*This class contains methods dedicated to the detection of various Sandboxes.*/


#include <Windows.h>
#include <Tlhelp32.h>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

class SandBoxDetection{


public:

	     
	     static const DWORD sandBoxieChksum = 0xC3CD42B0; // detect Sandboxie's injected DLL



	     SandBoxDetection(); //constructor

		 bool SandBoxLDRModDetect(DWORD moduleNameChksum);  /*detect Sandbox injected DLL through PEB_LDR_DATA structure
		                                                      *** 32-bit apps only *** */
		 
};

#endif