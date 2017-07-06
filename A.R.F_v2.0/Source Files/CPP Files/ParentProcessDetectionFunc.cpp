#include "ParentProcessDetection.h"




ParentProcessDetection::ParentProcessDetection() { };


//---------------------------------------------------------------------------------------------------------------------



int ParentProcessDetection::CheckParentProcess()
{

DWORD procId = 0; //the id of our process

DWORD parentProcId = 0; // the id of the parent process

DWORD explorerId = 0;

WORD * ptrToProcName = NULL;

string currentExaminedProc = "";

HANDLE procSnap = NULL;

string legitimateParent = "explorer.exe";


PROCESSENTRY32W pe32; // initialize PROCESSENTRY32 structure


ZeroMemory(&pe32, sizeof(PROCESSENTRY32W)); // clear


pe32.dwSize = sizeof(PROCESSENTRY32W); // set size of the structure


procId = GetCurrentProcessId();


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




	if(!strcmp(currentExaminedProc.c_str(), legitimateParent.c_str())) // if it is explorer.exe store its id
	 {

		 explorerId = pe32.th32ProcessID;

	 }

	 else if(pe32.th32ProcessID == procId) //if it is our process store parent id
	 {


		 parentProcId = pe32.th32ParentProcessID;

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


	 if(!strcmp(currentExaminedProc.c_str(), legitimateParent.c_str())) // if it is explorer.exe store its process id
	 {

		 explorerId = pe32.th32ProcessID;

	 }

	 else if(pe32.th32ProcessID == procId) //if it is our process store parent id
	 {


		 parentProcId = pe32.th32ParentProcessID;

	 }
        




	 }


/* when processes enumeration ends check if the parent process
   id of our process is the same with the process id of windows
   explorer.... */

	if(parentProcId != explorerId) //...if it is not
	{
								   
        CloseHandle(procSnap);

		return 1; // debugger detected
	}

	else{

		CloseHandle(procSnap);

		return 0; // no debugger detected

	}
	
	
}

	else{


	CloseHandle(procSnap);

	return -2; // if we failed to take info about the first running process 

	}




}

else{


	return -1; // if we failed to take a snapshot of all running processes
}


}