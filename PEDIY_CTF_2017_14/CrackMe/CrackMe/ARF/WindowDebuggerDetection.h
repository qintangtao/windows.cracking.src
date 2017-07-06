#ifndef WINDOW_DEBUGGER_DETECTION_H_

#define WINDOW_DEBUGGER_DETECTION_H_

#include <windows.h>

#include <string>

using namespace std;

/* 

This class contains methods that will attempt to identify if a specific debugger

or a specific reversing tool is currently running by trying to obtain a valid handle to

its top-level window either by using the window's title or by its class name.

*/

class WindowDebuggerDetection{

private: 

	 string * reversetoolslist;

	 int listsize;


public:

  WindowDebuggerDetection(); //constructor

  bool SpecificWindowNameDetection(string windowname); //detection throught window title

  
  bool SpecificWindowClassDetection(string classname); //detection through window class name

  
  
  string * SetReverseToolsList(); /* this function  will fill the reversetoolslist dynamic array and it will
  
                                  return a pointer to it

                                  You can modify this list at will, but it you do so, don't forget to write the correct
									  
							      size to the SetListSize method bellow.*/




  void SetListSize() { listsize = 13;} // set list size

  
  int GetListSize() { SetListSize(); return listsize;} // retrieve list size


  bool ListWindowClassDetection(string * arraymemlocation , int listsize); /* detection through a predefined list

  
																		   
 of windows class names of popular reversing tools.

 It will use the pointer to the reversetoolslist dynamic array returned by the function SetReverseToolsList() above.

 This function serves for multiple detection of most well-known reversing tools.

*/




};


#endif