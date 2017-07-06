#include "ModuleDebuggerDetection.h"




ModuleDebuggerDetection::ModuleDebuggerDetection() { };


//-----------------------------------------------------------------------------------------------------------------------

string * ModuleDebuggerDetection::SetModulesList()
{

	moduleslist = new string[GetListSize()];



	//ollydbg standard plugins/modules

	moduleslist[0] = "Cmdline.dll"; 

	moduleslist[1] = "BOOKMARK.DLL";


	//PEiD standard plugins/modules

	moduleslist[2] = "pluzina1.dll";

	moduleslist[3] = "pluzina2.dll";

	moduleslist[4] = "pluzina3.dll";

	moduleslist[5] = "pluzina4.dll";


	//LordPE standard plugins/modules

	moduleslist[6] = "procs.dll";

	moduleslist[7] = "realign.dll";

	moduleslist[8] = "16Edit.DLL";



	//IDA Pro standard plugins/modules

	moduleslist[9] = "win32_user.plw";

	moduleslist[10] = "win32_stub.plw";

	moduleslist[11] = "linux_stub.plw";

	moduleslist[12] = "wince_stub.plw";

	moduleslist[13] = "mac_stub.plw";




	//SpyStudio v2.0

	moduleslist[14] = "DeviareCOM.dll";

	moduleslist[15] = "Nektra.Deviare2.dll";



	
	//Sandboxie v3.70

	moduleslist[16] = "SbieDll.dll";



	//API Monitor v2.0
	
	moduleslist[17] = "apimonitor-drv-x86.sys";


    return moduleslist;

}


//----------------------------------------------------------------------------------------------------------------------


int ModuleDebuggerDetection::ModuleDetection(string * arraymemlocation , int listsize)
{


string * list = arraymemlocation;

string modulename;

string currentExaminedModule = "";

WORD * ptrToModuleName = NULL;

HANDLE procSnap = INVALID_HANDLE_VALUE;

HANDLE modSnap = INVALID_HANDLE_VALUE;

PROCESSENTRY32W pe32; // initialize PROCESSENTRY32 structure

MODULEENTRY32W me32; // initialize MODULEENTRY32 structure


ZeroMemory(&pe32, sizeof(PROCESSENTRY32W)); // clear

ZeroMemory(&me32, sizeof(MODULEENTRY32W)); // clear


pe32.dwSize = sizeof(PROCESSENTRY32W);

me32.dwSize = sizeof(MODULEENTRY32W);


procSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ); //take snapshot of all running processes


if(procSnap != INVALID_HANDLE_VALUE) // if we succesfully took a snapshot of running processes  
{


	modulename = *list; // retrieve name of the first suspec module from the list



	if(Process32FirstW(procSnap, &pe32)) //if we managed to get info about first running process 
	{


		modSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pe32.th32ProcessID); //take snapshot of all modules of currently examined process
		

		     if(Module32FirstW(modSnap, &me32)) //if we managed to retrieve info about the 1st module of the currently examined process
		      {


				  ptrToModuleName = (WORD *)&me32.szModule;

			     
				  while( * ptrToModuleName != 0x00 ) // store name of currently examined module
				  {

					  currentExaminedModule+= (char *)ptrToModuleName;


					 ptrToModuleName++;

				  }
				  

				  ptrToModuleName = NULL; //reset
				  


			     for(int i=0; i<listsize; i++) // loop for all suspect modules list
			    {

  
				     if(!strcmp(currentExaminedModule.c_str(), modulename.c_str())) // check if the currently examined module matches any of the suspect
				     {

					      CloseHandle(procSnap);

					      CloseHandle(modSnap);

						  delete [] moduleslist; 

						  moduleslist = NULL;

						  list = NULL;

					      return 1; // suspect module detected
				     }



				           if(i<listsize-1) // in order to avoid Bad Ptr
	                       {

	                         list++; // increase pointer to retrieve next suspect process name from our dynamic array

	                          modulename = *list; // get name of next suspect process name from our dynamic array
				         }
	   


			}


			while(Module32NextW(modSnap, &me32)) // go through all modules of the currently examined process
			{

				 list = arraymemlocation; // reset pointer to indicate first suspect module from our dynamic array each time

                 modulename = *list; // retrieve name of first suspect module from our dynamic array


				 currentExaminedModule = ""; //reset


				   ptrToModuleName = (WORD *)&me32.szModule;

			     
				  while( * ptrToModuleName != 0x00 ) // store name of currently examined module
				  {

					  currentExaminedModule+= (char *)ptrToModuleName;


					 ptrToModuleName++;

				  }



				  ptrToModuleName = NULL; //reset
				 


				 for(int i =0; i<listsize; i++)
		           {

		           if(!strcmp(currentExaminedModule.c_str(), modulename.c_str()))
		           {

			              CloseHandle(procSnap);

						  CloseHandle(modSnap);

						  delete [] moduleslist; 

						  moduleslist = NULL;

						  list = NULL;

			               return 1; //suspect module detected
		            }

		                  if(i<listsize-1) // in order to avoid Bad Ptr
		                  {

		                        list++; // increase pointer to retrieve next suspect module name from our dynamic array

		                        modulename = *list; // get name of next suspect module name from our dynamic array

		                  }


                 }



			}



		}


		while(Process32NextW(procSnap, &pe32)) //go through all processes
		{


			   modSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pe32.th32ProcessID);


			    list = arraymemlocation; // reset pointer to indicate first suspect module from our dynamic array each time

                
				modulename = *list; // retrieve name of first suspect module from our dynamic array


			if(Module32FirstW(modSnap, &me32)) //if we managed to retrieve info about the 1st module of the currently examined process
		{

			currentExaminedModule = ""; //reset

			  ptrToModuleName = (WORD *)&me32.szModule;

			     
				  while( * ptrToModuleName != 0x00 ) // store name of currently examined module
				  {

					  currentExaminedModule+= (char *)ptrToModuleName;


					 ptrToModuleName++;

				  }

				  ptrToModuleName = NULL; //reset


			for(int i=0; i<listsize; i++) // loop for all suspect modules list
			{


				if(!strcmp(currentExaminedModule.c_str(), modulename.c_str())) // check if the currently examined module matches any of the suspect
				{

					CloseHandle(procSnap);

					CloseHandle(modSnap);

					delete [] moduleslist;

					moduleslist = NULL;

					list = NULL;

					return 1; // suspect module detected
				}



				     if(i<listsize-1) // in order to avoid Bad Ptr
	                {

	                   list++; // increase pointer to retrieve next suspect module name from our dynamic array

	                    modulename = *list; // get name of next suspect module name from our dynamic array
				    }
	   


			}


			while(Module32NextW(modSnap, &me32)) // go through all modules of the currently examined process
			{

				 list = arraymemlocation; // reset pointer to indicate first suspect module from our dynamic array each time

                 modulename = *list; // retrieve name of first suspect module from our dynamic array

				 currentExaminedModule = "";


				   ptrToModuleName = (WORD *)&me32.szModule;

			     
				  while( * ptrToModuleName != 0x00 ) // store name of currently examined module
				  {

					  currentExaminedModule+= (char *)ptrToModuleName;


					 ptrToModuleName++;

				  }


				  ptrToModuleName = NULL; //reset


				 for(int i =0; i<listsize; i++)
		           {

		           if(!strcmp(currentExaminedModule.c_str(), modulename.c_str()))
		           {

			              CloseHandle(procSnap);

						  CloseHandle(modSnap);

						  delete [] moduleslist; 

						  moduleslist = NULL;

						  list = NULL;

			               return 1; //suspect module detected
		            }

		                  if(i<listsize-1) // in order to avoid Bad Ptr
		                  {

		                        list++; // increase pointer to retrieve next suspect module name from our dynamic array

		                        modulename = *list; // get name of next suspect module name from our dynamic array

		                  }


                    }



			}



		 }


 }

		CloseHandle(procSnap);

		CloseHandle(modSnap);

		delete [] moduleslist; 

		moduleslist = NULL;

		list = NULL;


		return 0; // if no suspect module detected
		       

		} 

	else{


		CloseHandle(procSnap);

		delete [] moduleslist; 

		moduleslist = NULL;

		list = NULL;

		return -2; // if we failed to take info about the first running process 
	}


}


delete [] moduleslist; 

moduleslist = NULL;

list = NULL;

return -1; // if we failed to take a snapshot of all running processes



}


