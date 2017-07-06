#include "IndirectDebuggerDetection.h"




IndirectDebuggerDetection::IndirectDebuggerDetection() {};


//------------------------------------------------------------------------------------------------------------------------



bool IndirectDebuggerDetection::DebugString()
{

	string dbgmsg = "";

	DWORD eaxvalue = 0; // we set it initially to zero

	OutputDebugStringA(dbgmsg.c_str()); // Api located in kernel32.dll



/*we will use some inline assembly in order to retrieve the value of eax
 
 immediately after returning back from the above function.*/
	
	__asm{  

		   mov eaxvalue, eax     

	}

	if(eaxvalue!=0 && eaxvalue!=1) //if value of eax is not be 1 or 0 depending on Windows version
	{

          return true;  //debugger detected

	}


	else{


		return false;
	}



}


//---------------------------------------------------------------------------------------------------------

int IndirectDebuggerDetection::OpenServicesProcess()
{

DWORD procId = 0; //the id of our process

HANDLE procSnap = NULL;

HANDLE services = NULL;

string currentExaminedProc = "";

WORD * ptrToProcName = NULL;

string systemproc  = "services.exe";

PROCESSENTRY32W pe32; // initialize PROCESSENTRY32 structure


ZeroMemory(&pe32, sizeof(PROCESSENTRY32W)); //clear


pe32.dwSize = sizeof(PROCESSENTRY32W); // set size of the structure


procSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ); // take a snapshot of all running processes


if(procSnap != INVALID_HANDLE_VALUE) // if snapshot has been taken
{

	

    if(Process32FirstW(procSnap, &pe32)) // if we managed to take info about the first running process
{


	            ptrToProcName = (WORD *)&pe32.szExeFile;

			     
				  while( * ptrToProcName != 0x00 )
				  {

					  currentExaminedProc+= (char *)ptrToProcName;


					 ptrToProcName++;

				  }


				  ptrToProcName = NULL; //reset



				  if(!strcmp(systemproc.c_str(), currentExaminedProc.c_str())) // if the first examined process is the one we are interested too
		   {

			   services = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); // try to obtain a valid handle


			   if(services!=NULL) //if we managed to obtain a valid handle
			   {

				   CloseHandle(procSnap);

				   return 1; // debugger detected
			   }

			   else{

				   CloseHandle(procSnap);

				   return 0; // no debugger detected
			   }
	}


	 while(Process32NextW(procSnap, &pe32)) // repeat for all remaining running processes
	{

		       currentExaminedProc = ""; //reset


			   ptrToProcName = (WORD *)&pe32.szExeFile;

			     
				  while( * ptrToProcName != 0x00 )
				  {

					  currentExaminedProc+= (char *)ptrToProcName;


					 ptrToProcName++;

				  }


				  ptrToProcName = NULL; //reset




		if(!strcmp(systemproc.c_str(),currentExaminedProc.c_str())) // if the examined process is the one we are interested too
		   {

			   services = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); // try to obtain a valid handle

			   if(services!=NULL) //if we managed to obtain a valid handle
			   {

				   CloseHandle(procSnap);

				   return 1; // debugger detected
			   }

			    else{

				   CloseHandle(procSnap);

				   return 0; // no debugger detected
			   }


	     }

	 }
}

	else{


      CloseHandle(procSnap);

	return -2; // if we failed to take info about the first running process 

	}



CloseHandle(procSnap);
return 0; // no debugger detected

}

else{

	return -1; // if we failed to take snapshot of all running processes
}


}