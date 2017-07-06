#include "stdafx.h"



VirtualMachineDetection::VirtualMachineDetection() { }; 


//-----------------------------------------------------------------------------------------------------------------

bool VirtualMachineDetection::VMWareDetect()
{

DWORD VMWare = 0;

 __try{

	     __asm{

			     mov eax, 0x564d5868
				 push eax
				 xor ebx, ebx
				 mov ecx, 0x0A
				 mov edx, 0x5658
				 in eax, dx // this is a privileged instruction but it can be executed from Ring3 inside VMWare
				 pop eax
				 xor eax,ebx // if VMWare is present then eax = ebx so after XOring them eax has to be 0
				 mov VMWare, eax
			}


		 if(VMWare!=0)
		 {
			 return false;
		 }

		 else{

			 return true;
		 }

		

	    
 }

 
 // if our application is not running inside VMWare the "in eax,dx" instruction will raise a privileged instruction exception
__except(EXCEPTION_EXECUTE_HANDLER) 
 {

	 return false;

 }

}



//--------------------------------------------------------------------------------------------------------------------


bool VirtualMachineDetection::VirtualPCDetect()
{

DWORD VPC = 0;

 __try{

	     __asm{

			     xor ebx,ebx
				 mov eax, 1
				 __emit(0x0F)
				 __emit(0x3F)
				 __emit(0x07)
				 __emit(0x0B)
				 test ebx,ebx
				 xor eax,eax
				 xor ebx,eax
				 mov VPC, ebx
			}


		 if(VPC!=0)
		 {
			 return false;
		 }

		 else{

			 return true;
		 }

		

	    
 }

 
 // if our application is not running inside VBox the virtual call will raise a privileged instruction exception
__except(EXCEPTION_EXECUTE_HANDLER) 
 {

	 return false;

 }



}



//--------------------------------------------------------------------------------------------------------------------


bool VirtualMachineDetection::VirtualBoxDetect()
{

HWND vboxtray = NULL;

vboxtray = FindWindowA("VBoxTrayToolWndClass",NULL);

if(vboxtray!=NULL)
{

	return true;
}


return false;


}



//------------------------------------------------------------------------------------------------------------------


bool VirtualMachineDetection::VMWareRegKeyDetect()
{

HKEY regkey;

if(RegOpenKeyExA(HKEY_LOCAL_MACHINE,"HARDWARE\\ACPI\\DSDT\\PTLTD_",0,KEY_READ,&regkey) == ERROR_SUCCESS)
{
	RegCloseKey(regkey);

	return true;

}

return false;

}



//------------------------------------------------------------------------------------------------------------------


bool VirtualMachineDetection::VirtualBoxRegKeyDetect()
{

HKEY regkey;

if(RegOpenKeyExA(HKEY_LOCAL_MACHINE,"HARDWARE\\ACPI\\DSDT\\VBOX__",0,KEY_READ,&regkey) == ERROR_SUCCESS)
{
	RegCloseKey(regkey);

	return true;

}

return false;


}



//------------------------------------------------------------------------------------------------------------------


bool VirtualMachineDetection::VirtualPCRegKeyDetect()
{

HKEY regkey;

if(RegOpenKeyExA(HKEY_LOCAL_MACHINE,"HARDWARE\\ACPI\\DSDT\\AMIBI",0,KEY_READ,&regkey) == ERROR_SUCCESS)
{
	RegCloseKey(regkey);

	return true;

}

return false;

}



//------------------------------------------------------------------------------------------------------------------

bool VirtualMachineDetection::VMRegKeyDetect(char * VMname)
{

HKEY regkey;

DWORD type = REG_SZ;

DWORD size = 4096;

DWORD cbdata = size;

char * regvalue = new char[size];

DWORD dwRet;

if(RegOpenKeyExA(HKEY_LOCAL_MACHINE,"SYSTEM\\ControlSet001\\services\\Disk\\Enum",0,KEY_READ,&regkey) == ERROR_SUCCESS)
{

	dwRet = RegQueryValueExA(regkey,"0",NULL,&type,(LPBYTE)regvalue,&cbdata);

	while(dwRet==ERROR_MORE_DATA)
	{
       size+=size;
	   delete [] regvalue;
	   regvalue = new char[size];
	   cbdata = size;
	   dwRet = RegQueryValueExA(regkey,"0",NULL,&type,(LPBYTE)regvalue,&cbdata);
	}

	if(dwRet == ERROR_SUCCESS)
	{

		if(strstr(regvalue,VMname) != NULL)
		{
			
			delete [] regvalue;

			return true;
		}

	}

}

delete [] regvalue;

return false;

}


//------------------------------------------------------------------------------------------------------------------



bool VirtualMachineDetection::VMGenericDetect()
{

// Effective for Windows Vista, Win7, Win8. Read example notes in VirtualMachineDetection.h 

 OSVERSIONINFOA osver;

 SYSTEM_INFO sysinfo;
 
 MEMORYSTATUSEX memstatus;

 ZeroMemory(&osver, sizeof(OSVERSIONINFOA));
 osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

 ZeroMemory(&sysinfo, sizeof(SYSTEM_INFO));

 ZeroMemory(&memstatus, sizeof(memstatus));
 memstatus.dwLength = sizeof(memstatus);


 GetVersionExA(&osver);

 if(osver.dwMajorVersion >= 6)
 {
	 GetSystemInfo(&sysinfo);

	 if(sysinfo.dwNumberOfProcessors < 2)
	 {

		 GlobalMemoryStatusEx(&memstatus);

		 if((memstatus.ullTotalPhys/1024) < 2097152)
		 {

			 return true;
		 }

	 }
 }

 return false;
}
