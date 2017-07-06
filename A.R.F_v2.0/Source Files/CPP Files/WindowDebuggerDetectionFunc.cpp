#include "WindowDebuggerDetection.h"

WindowDebuggerDetection::WindowDebuggerDetection() { };

// --------------------------------------------------------------------------------------------------------------

bool WindowDebuggerDetection::SpecificWindowNameDetection(string windowname) {

	HWND window = FindWindowA(NULL, windowname.c_str());
	// This API is located into the user32.dll

	if (window != NULL) {

		return true;
	}

	else {

		return false;
	}

}

// --------------------------------------------------------------------------------------------------------------------------

bool WindowDebuggerDetection::SpecificWindowClassDetection(string classname) {

	HWND window = FindWindowA(classname.c_str(), NULL);

	if (window != NULL) {

		return true;

	}

	else {

		return false;
	}

}

// -------------------------------------------------------------------------------------------------------------

string * WindowDebuggerDetection::SetReverseToolsList() {

	/* if you desire to update/modify this list do not forget to change its size

	in the SetListSize method in the header file.

	 */

	reversetoolslist = new string[GetListSize()]; // windows classes list size

	reversetoolslist[0] = "OLLYDBG"; // Olly debugger main window

	reversetoolslist[1] = "ID"; // Immunity debugger main window

	reversetoolslist[2] = "Tokno_konfig"; // Exeinfo PE

	reversetoolslist[3] = "TDiEfrm"; // Detect it Easy

	reversetoolslist[4] = "MYDEBUG"; // EvO_DBG (modified olly)

	reversetoolslist[5] = "259C91A0"; // ProcDump

	reversetoolslist[6] = "18467-41"; // RegMon

	reversetoolslist[7] = "FileMonClass"; // FileMon

	reversetoolslist[8] = "OWL_Window"; // WDasm

	reversetoolslist[9] = "HANOLLY"; // modified olly

	reversetoolslist[10] = "YPOGEiOS"; // modified olly

	reversetoolslist[11] = "DeFixed"; // modified olly

	reversetoolslist[12] = "TIdaWindow"; // IDA Pro


	return reversetoolslist;

}


//--------------------------------------------------------------------------------------------------------------

bool WindowDebuggerDetection::ListWindowClassDetection
(string * arraymemlocation, int listsize) {

	string * list = arraymemlocation;

	string windowclassname;

	for (int i = 0; i < listsize; i++) // check all the list of windows class names
	{

		windowclassname = *list;

		if (FindWindowA(windowclassname.c_str(), NULL)) {


			delete [] reversetoolslist;

			reversetoolslist = NULL;

			list = NULL;

			return true;
		}

		  if(i < (listsize - 1))  //to avoid bad ptr
	          {

		    list++; // increase the pointer to indicate next string in the dynamic array
                  }  

	}


	delete [] reversetoolslist;

	 reversetoolslist = NULL;

	 list = NULL;

	return false;

}
