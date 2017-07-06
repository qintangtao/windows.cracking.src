#include "SehDebuggerDetection.h"



SehDbgDetection::SehDbgDetection() { };


//----------------------------------------------------------------------------------------------------------------------------


bool SehDbgDetection::CloseHandleExcepDetection(HANDLE invalid)
{


__try{

	CloseHandle(invalid);
}

__except(EXCEPTION_EXECUTE_HANDLER)
{


	return true;

}


return false;


}



//---------------------------------------------------------------------------------------------------------------------------

bool SehDbgDetection::SingleStepExcepDetection()
{

	__try{

	      __asm {
           PUSHFD //push the flag registers to the stack so we can manipulate them
           OR BYTE PTR[ESP+1], 1 // Set the trap flag
           POPFD //Restore the flag registers after manipulation
           NOP // try to execute next command to generate the exception
        }

	}


__except (EXCEPTION_EXECUTE_HANDLER) 
{


return false;

}


return true;





}



//---------------------------------------------------------------------------------------------------------------------------


bool SehDbgDetection::OutputDebugStringExcepDetection()
{


OutputDebugStringA("test"); //you can modify the string at will
 
__try{
 
	__asm mov ebx, dword ptr [eax] /*if not debugged it will raise an exception cause eax will be 0 or 1 depending on Windows Version (XP->1, Vista/7 -> 0)
								     you can of course modify this instruction with another one of your one that produces the same effect.*/
 
	return true;
}
	 
	   __except(EXCEPTION_EXECUTE_HANDLER)
    {
	    return false;  //if we get here debugger not detected
	
	}



}