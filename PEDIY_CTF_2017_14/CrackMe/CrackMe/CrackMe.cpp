// CrackMe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lfsr.h"
#include <iostream>
#include <string>

using namespace std;
using namespace andrivet::ADVobfuscator;

static char key[12]; //ToBeOrNot2Be

void notice(void)
{
	cout << OBFUSCATED4("Please enter the key: ");
}

void key_right(void)
{
	cout << OBFUSCATED4("Yeah! You've got it") << endl;
}

void key_wrong(void)
{
	cout << OBFUSCATED4("Oops, wrong key..") << endl;
}

int get_input(void)
{
	int i;
	string input;
	getline(cin, input);
	if (input.length() != 12) return FAILURE;
	for(i=0;i<12;i++)
	{
		key[i] = input.at(i);
	}
	return SUCCESS;
}

int __cdecl main()
{
	int result=1;
#ifdef NDEBUG
	AntiAttach *d = new AntiAttach();
	d->AntiAttachSet();
	//d->AntiAttachSelfDebug();
#endif
	while (result) {
		OBFUSCATED_CALL_P0(DetectDebugger1, notice);
		result = OBFUSCATED_CALL_RET_P0(DetectDebugger1, int, get_input);
		if (result) {
			OBFUSCATED_CALL_P0(DetectDebugger1, key_wrong);
		}
		else {
			result = OBFUSCATED_CALL_RET_P(DetectDebugger1, int, lfsr_bf, &key[0]);
			if (result) {
				OBFUSCATED_CALL_P0(DetectDebugger1, key_wrong);
			}
			else {
				OBFUSCATED_CALL_P0(DetectDebugger1, key_right);
			}
		}
	}
	return result;
}
