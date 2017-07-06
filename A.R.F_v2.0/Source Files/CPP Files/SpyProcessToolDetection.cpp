#include "SpyProcessToolDetection.h"



SpyProcessToolDetection::SpyProcessToolDetection() { }

//------------------------------------------------------------------------------------------------

bool SpyProcessToolDetection::SpyLDRModDetect(DWORD moduleNameChksum)
{

DWORD spyToolDetected = 0;

	__asm{  

 
	        mov eax, dword ptr fs:[0x30]  // get PEB address
	        mov eax, dword ptr ds:[eax + 0x0c]  // pointer to LDR_DATA structure
	        add eax, 0x0C  // pointer to _LDR_DATA_TABLE_ENTRY structure of first module

_nextmodule:	   
	        mov eax, dword ptr ds:[eax] //address of _LDR_DATA_TABLE_ENTRY structure
	        push eax  //save eax because contains the address of the pointer to the next module
	        add eax, 0x30 // pointer to current module name
	        mov eax, dword ptr ds:[eax]
			test eax, eax
			je _allmoduleschecked
			xor edi,edi

_counter: 
	       mov ecx,4
_nameloop:    
	           mov bl, byte ptr ds:[eax]
		       cmp bl, 65
		       jl _continue
		       cmp bl,122
		       jg _continue
		       cmp bl, 97
		       jl _checknextcase
			   jmp _continue

_checknextcase: cmp bl, 90
				jg _continue
				add bl, 32 //convert to lower case

_continue:	    rol ebx, 8
	            add eax, 2 //assuming Unicode so increasing by 2
	            cmp byte ptr ds:[eax], 0
	            je _lastbytescalc
		        loop _nameloop
		        mov edx, ebx
		        shl ebx, 2
				add bh,bl
		        xor ebx, edx
		        xor edi, ebx
		        jmp _counter

_lastbytescalc: 
	            mov edx, ebx
		        shl ebx, 2
				add bh,bl
		        xor ebx, edx
                xor edi, ebx

_checksumcheck: 
				cmp edi, moduleNameChksum //check module name checksum
	            je _spystudiodll
				xor ebx,ebx
				xor edi,edi
				xor edx,edx
	            pop eax
				jmp _nextmodule

_spystudiodll:
			    pop eax
				mov eax, 1
				mov spyToolDetected, eax
				jmp _finish

_allmoduleschecked:
				    pop eax
				    mov eax, 0
					mov spyToolDetected, eax

_finish:

	}

	if(spyToolDetected == 1)
	{
		return true;
	}


	return false;
}


//-------------------------------------------------------------------------------------------------------

bool SpyProcessToolDetection::SpyLDRGenericDetect(DWORD modulesNumber)
{

DWORD countedModules = 0;

__asm{
	        push eax
	        mov eax, dword ptr fs:[0x30]  // get PEB address
	        mov eax, dword ptr ds:[eax + 0x0c]  // pointer to LDR_DATA structure
	        add eax, 0x0C  // pointer to _LDR_DATA_TABLE_ENTRY structure of first module

_nextmodule:	   
	        mov eax, dword ptr ds:[eax] //address of _LDR_DATA_TABLE_ENTRY structure
	        push eax  //save eax because contains the address of the pointer to the next module
	        add eax, 0x30 // pointer to current module name
	        mov eax, dword ptr ds:[eax]
			test eax, eax
			je _allmoduleschecked
			inc countedModules
			pop eax
			jmp _nextmodule


_allmoduleschecked: pop eax
					pop eax
}

if(countedModules > modulesNumber)
{
	return true;
}


return false;
}