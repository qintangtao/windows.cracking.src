// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma warning(disable: 4503 4166)

#include "targetver.h"
#include <windows.h>

#include "resource.h"
#include "asmlib.h"
#include "MetaRandom.h"
#include "MetaString4.h"
#include "ObfuscatedCall.h"
#include "DetectDebugger.h"
#include "ObfuscatedCallWithPredicate.h"
#include "ARF/AntiAttach.h"
#include "ARF/ApiBreakPointDetection.h"
#include "ARF/CheckSumCalculator.h"
#include "ARF/CodeTraceTimeDetection.h"
#include "ARF/DirectDebuggerDetection.h"
#include "ARF/HardwareBreakPointDetection.h"
#include "ARF/IndirectDebuggerDetection.h"
#include "ARF/ModuleDebuggerDetection.h"
#include "ARF/ParentProcessDetection.h"
#include "ARF/ProcessDebuggerDetection.h"
#include "ARF/SandBoxDetection.h"
#include "ARF/SehDebuggerDetection.h"
#include "ARF/SpyProcessToolDetection.h"
#include "ARF/VirtualMachineDetection.h"
#include "ARF/WindowDebuggerDetection.h"
