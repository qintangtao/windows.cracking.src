#ifndef VIRTUAL_MACHINE_DETECTION_H_
#define VIRTUAL_MACHINE_DETECTION_H_


/* This class contains methods dedicated to the detection of VMs such as VBOX, VMWare, VirtualPC
  and even more generic ones.*/

#include <Windows.h>

class VirtualMachineDetection{


public:

	    VirtualMachineDetection(); //constructor

		bool VMWareDetect(); // detect if our process is running inside VMWare
        bool VMWareRegKeyDetect();
	
        bool VirtualPCDetect(); // detect if our process is running inside VirtualPC
		bool VirtualPCRegKeyDetect();
		
        bool VirtualBoxDetect(); // detect if our process is running inside VirtualBox
		bool VirtualBoxRegKeyDetect();

		bool VMRegKeyDetect(char * VMname); /* Detect a VM from the virtual HD name
										  
	      Example of VMname param: "VMware"
		  
		                           "DiskVirtual"     (for VPC) 
								  
								   "DiskVBOX"        (for VBOX)

								   "Virtual"  (some VMs will include this word to the virtual HD name. More generic, but use at your own risk) */

		bool VMGenericDetect(); /* Detect-by-assumption a VM by analysing the system resources along with the Windows Version.
								
		 Example:
		           A system that runs WinVista or Win7 and above with less that 2G of Ram and a single core CPU would be a quite rare
				   case in nowadays. This method it is expected to have some false positives, but still it worths it since most people
				   use the minimum required resources to boot a VM in order to test applications etc..
				   I recommend you use this one mostly for testing, unless you know what you are really doing.*/		
};

#endif