#include "stdafx.h"




ProcessDebuggerDetection::ProcessDebuggerDetection() { };


//----------------------------------------------------------------------------------------------------------------




string * ProcessDebuggerDetection::SetProcessList()
{


	processlist = new string[GetListSize()];



	processlist[0] = "OLLYDBG.EXE";

    processlist[1] = "PEiD.exe";

	processlist[2] = "ollydbg.exe";
	
	processlist[3] = "OllyDbg.exe";

	processlist[4] = "LordPE.exe";

	processlist[5] = "LordPE.exe";

	processlist[6] = "ImportREC.exe";

	processlist[7] = "CiM's.exe";

	processlist[8] = "DeFixed.exe";

	processlist[9] = "YGS-DOX.exe";

	processlist[10] = "OllyICE.exe";

	processlist[11] = "HanOlly_English.exe";

	processlist[12] = "HanOlly.exe";

	processlist[13] = "HanOlly_Korean.exe";

	processlist[14] = "W32DSM89.EXE";

	processlist[15] = "WinHex.exe";

	processlist[16] = "HIEW32.EXE"; 

    processlist[17] = "XVI32.exe";

    processlist[18] = "idag.exe";

    processlist[19] = "hiew32.exe";

    processlist[20] = "PROCDUMP.exe";

    processlist[21] = "FILEMON.EXE";

    processlist[22] = "FILEMON.exe";

    processlist[23] = "PROCDUMP.EXE";
  
    processlist[24] = "Regmon.exe";

    processlist[25] = "ResHacker.exe";

    processlist[26] = "exeinfope.exe";

	processlist[27] = "eXeScope.exe";

	processlist[28] = "DiE.exe";

	processlist[29] = "protection_id.exe";

	processlist[30] = "EvO_DBG.exe";

	processlist[31] = "SbieCtrl.exe"; //sandboxie

	processlist[32] = "SpyStudio.exe";

	processlist[33] = "SbieSvc.exe"; //sandboxie

	processlist[34] = "apimonitor-x86.exe";


    return processlist;



}



//-------------------------------------------------------------------------------------------------------------------------



int ProcessDebuggerDetection::ProcessDetection(string * arraymemlocation , int listsize)
{

string * list = arraymemlocation;

string processname;

string currentExaminedProc = "";

WORD * ptrToProcName = NULL;

HANDLE procSnap = NULL;

PROCESSENTRY32W pe32; // initialize PROCESSENTRY32 structure


ZeroMemory(&pe32, sizeof(PROCESSENTRY32W));


pe32.dwSize = sizeof(PROCESSENTRY32W); // set size of the structure


procSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ); // take a snapshot of all running processes


if(procSnap != INVALID_HANDLE_VALUE) // if snapshot has been taken
{


processname = *list; // retrieve name of first suspect process from our dynamic array


if(Process32FirstW(procSnap, &pe32)) // if we managed to take info about the first running process
{


		         ptrToProcName = (WORD *)&pe32.szExeFile;

			     
				  while( * ptrToProcName != 0x00 ) // store name of currently examined running process
				  {

					  currentExaminedProc+= (char *)ptrToProcName;


					 ptrToProcName++;

				  }



				  ptrToProcName = NULL; //reset

	



   for(int i=0; i<listsize; i++)
   {

	   if(!strcmp(currentExaminedProc.c_str(),processname.c_str()))
	{

        CloseHandle(procSnap);

		delete [] processlist;

		processlist = NULL;

		list = NULL;

		return 1; // suspect process detected

	}
	   if(i<listsize-1) // in order to avoid Bad Ptr
	   {

	  list++; // increase pointer to retrieve next suspect process name from our dynamic array

	  processname = *list; // get name of next suspect process name from our dynamic array

	   }

   }





	while(Process32NextW(procSnap, &pe32)) // repeat for all remaining running processes
	{

		
       list = arraymemlocation; // reset pointer to indicate first suspect process from our dynamic array each time

        processname = *list; // retrieve name of first suspect process from our dynamic array


		 currentExaminedProc = ""; //reset

		         
		 ptrToProcName = (WORD *)&pe32.szExeFile;

			     
				  while( * ptrToProcName != 0x00 ) // store name of currently examined running process
				  {

					  currentExaminedProc+= (char *)ptrToProcName;


					 ptrToProcName++;

				  }


				  ptrToProcName = NULL; //reset


        
		for(int i =0; i<listsize; i++)
		{

		
		    if(!strcmp(currentExaminedProc.c_str(),processname.c_str()))
		   {

			    CloseHandle(procSnap);

				delete [] processlist;

				processlist = NULL;

				list = NULL;

			    return 1; //suspect process detected
		     }

		            if(i<listsize-1) // in order to avoid Bad Ptr
		           {

		            list++; // increase pointer to retrieve next suspect process name from our dynamic array

		            processname = *list; // get name of next suspect process name from our dynamic array

		        }


         }

}


	delete [] processlist; //1

	processlist = NULL;

	list = NULL;
  
	return 0; // if no suspect process detected


}

else{

	CloseHandle(procSnap);

	delete [] processlist;

	list = NULL;

	processlist = NULL;

	return -2; // if we failed to take info about the first running process 
}



}


else{


delete [] processlist;

processlist = NULL;

list = NULL;

return -1; // if we failed to take a snapshot of all running processes

}


}
