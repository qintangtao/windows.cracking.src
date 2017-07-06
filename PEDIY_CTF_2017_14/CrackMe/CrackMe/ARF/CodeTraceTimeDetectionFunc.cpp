#include "stdafx.h"




CodeTraceTimeDetection::CodeTraceTimeDetection() { };




//--------------------------------------------------------------------------------------------------


DWORD CodeTraceTimeDetection::StartExecutionTime()
{

   return GetTickCount();

}


//--------------------------------------------------------------------------------------------------


DWORD CodeTraceTimeDetection::EndExecutionTime()
{

   return GetTickCount();

}


//------------------------------------------------------------------------------------------------------



bool CodeTraceTimeDetection::IsCodeBeingTaced()
{



	DWORD timelimit = GetTimeLimit(); // retrieve time execution limit

	DWORD totaltime = GetTotalTime(); // retrieve total time

 
	if(totaltime > timelimit)
	{

		return true; // we are being traced
	}


	else{

		return false; //everything runs smoothly...
	}


}