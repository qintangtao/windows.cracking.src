#ifndef HARDWARE_BREAKPOINT_DETECTION_H_

#define HARDWARE_BREAKPOINT_DETECTION_H_


/*

This class contains methods dedicated to the detection of hardware breakpoint by

evaluating the values of Debug Registers Dr0,Dr1,Dr2,Dr3 of the cpu.

*/

#include <windows.h>


class HardwareBreakPointDetection{



public:

	   HardwareBreakPointDetection(); //constructor

	   int HwdBreakPoint(); /*This method will use GetThreadContext win Api in order to evaluate the values of
							 
							 Debug Registers (DR0-DR3) used for storing hardware breakpoints. If the value of any of them is

							 not 0 then a hardware bp has been detected.
							 */

	   bool HWdBreakPointSeh(); /*This method will attempt to detect HW BPs by forcing an exception and then
								 reading the values DR0-DR3 registers from the ThreadContext structure saved inside the stack memory
								 area.
								 It is considered a more stealthy way, since it does not require the use of any
								 Windows APIs.*/
};


#endif