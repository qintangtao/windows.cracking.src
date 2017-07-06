#ifndef CHECKSUM_STRING_CALCULATOR_H_
#define CHECKSUM_STRING_CALCULATOR_H_

#include <Windows.h>

class CheckSumCalculator{
 


public:            
	        CheckSumCalculator(); //constructor

			DWORD CalculateNameCheckSum(wchar_t * moduleName);
	         
};


#endif