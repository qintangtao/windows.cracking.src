//
//  DetectDebugger.h
//  ADVobfuscator
//
//  Created by sebastien on 22/09/14.
//  Copyright (c) 2014 Andrivet. All rights reserved.
//

#ifndef ADVobfuscator_DetectDebugger_h
#define ADVobfuscator_DetectDebugger_h

bool AmIBeingDebugged1();
struct DetectDebugger1 { bool operator()() { return AmIBeingDebugged1(); } };
bool AmIBeingDebugged2();
struct DetectDebugger2 { bool operator()() { return !AmIBeingDebugged2(); } };
#endif
