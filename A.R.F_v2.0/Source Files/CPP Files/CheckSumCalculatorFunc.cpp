#include "CheckSumCalculator.h"



CheckSumCalculator::CheckSumCalculator() { }

//----------------------------------------------------------------------------------------


DWORD CheckSumCalculator::CalculateNameCheckSum(wchar_t * moduleName)
{

DWORD checksum = 0;



__asm{
	        
	        push edi
			push ebx
			push edx
			push ecx
			push eax
	        xor edi,edi
			xor ebx,ebx
			mov eax, moduleName

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
	            add eax, 2 //Unicode chars so we increase by 2
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
				

_finalchecksum: 
				mov checksum, edi  //check module name checksum
				pop eax
				pop ecx
				pop edx
				pop ebx
				pop edi

      }


return checksum;

}