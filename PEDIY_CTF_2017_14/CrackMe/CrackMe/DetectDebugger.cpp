#include "stdafx.h"

bool AmIBeingDebugged1()
// Returns true if the current process is being debugged (either running under the debugger or has a debugger attached post facto).
{
#ifdef _DEBUG
	return false;
#else
	bool r = false;
	DirectDebuggerDetection *d1 = new DirectDebuggerDetection();
	IndirectDebuggerDetection *d2 = new IndirectDebuggerDetection();
	HardwareBreakPointDetection *d3 = new HardwareBreakPointDetection();
	ApiBreakPointDetection *d4 = new ApiBreakPointDetection();
	SehDbgDetection *d5 = new SehDbgDetection();
	r |= (d1->RemoteDebuggerPresent() == 1);
	r |= (d2->OpenServicesProcess() == 1);
	r |= d3->HWdBreakPointSeh();
	r |= (d4->ApiBreakPoint("KERNEL32.DLL", "GetCommandLineW") == 1) || (d4->ApiBreakPoint("KERNEL32.DLL", "ReadConsoleW") == 1) || (d4->ApiBreakPoint("KERNEL32.DLL", "WriteConsoleW") == 1) || (d4->ApiBreakPoint("KERNEL32.DLL", "IsDebuggerPresent") == 1) || (d4->ApiBreakPoint("KERNEL32.DLL", "LoadResource") == 1) || (d4->ApiBreakPoint("KERNEL32.DLL", "GetProcAddress") == 1);
	r |= d5->CloseHandleExcepDetection((HANDLE)0x90909090);
	return r;
#endif
}
bool AmIBeingDebugged2()
// Returns true if the current process is being debugged (either running under the debugger or has a debugger attached post facto).
{
#ifdef _DEBUG
	return true;
#else
	bool r = true;
	DirectDebuggerDetection *d1 = new DirectDebuggerDetection();
	r &= (d1->RemoteDebuggerPresent() == 1) && d1->DebuggerPresent();
	return r;
#endif
}

