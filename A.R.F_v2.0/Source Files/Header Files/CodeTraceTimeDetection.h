#ifndef CODE_TRACE_TIME_DETECTION_H_

#define CODE_TRACE_TIME_DETECTION_H_

/*This class contains methods through which we are able to calculate how much time has

 passed for the execution of a certain block of code. If the amount of time it is bigger
 
 that the expected, then it means that someone is analyzing our code and he executed the
 
 commands through a debugger by taking his time.

 So the code of block that we want to protect must be between the calculation of the start execution time

 and the end execution time.
 
 */

#include <windows.h>

class CodeTraceTimeDetection{

private: 
	     DWORD startTime; //when the execution of the code block we are interested too begins
         DWORD endTime; // when the execution of the block ends
         DWORD totalTime; // total time may be retrieved by subtracting start time from end time
		 DWORD timeLimit; // if amount of time exceeds our limit then we are being traced

public:

	CodeTraceTimeDetection(); // constructor

	DWORD StartExecutionTime(); //calculate value when execution of block starts

	DWORD EndExecutionTime(); //calculate value when execution of block ends

	void SetStartTime() { startTime = StartExecutionTime(); }

	void SetEndTime() { endTime = EndExecutionTime(); } 

	void SetTimeLimit() { timeLimit = 1000; } /*we set 1000ms by default we may increase it if
												the code block is too big.*/
    DWORD GetTimeLimit() { SetTimeLimit(); return timeLimit; }

	void SetTotalTime() { totalTime = endTime - startTime; } // calculate total execution time

	DWORD GetTotalTime() { SetTotalTime(); return totalTime; } //returns total execution time

	bool IsCodeBeingTaced(); // check if someone is tracing our code

};


#endif