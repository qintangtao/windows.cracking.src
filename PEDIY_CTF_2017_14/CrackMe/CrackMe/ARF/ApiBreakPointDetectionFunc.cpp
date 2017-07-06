#include "stdafx.h"

ApiBreakPointDetection::ApiBreakPointDetection() {
};

// -------------------------------------------------------------------------------------------------------------

int ApiBreakPointDetection::ApiBreakPoint(char * DLL, char * API) {

	HMODULE hmodule = LoadLibraryA(DLL);

	DWORD ApiAddress = 0;

	const BYTE bp = 0xCC; // opcode of sw breakpoint (INT 3)

	BYTE * ptr = NULL;

	if (hmodule != NULL) {

		ApiAddress = (DWORD)GetProcAddress(hmodule, API);

		if (ApiAddress != NULL) {

			ptr = (BYTE*)ApiAddress;

			for (int i = 0; i < 5; i++) {

				if (*ptr == bp) {

					FreeLibrary(hmodule); // unload dll

					return 1; // Api Breakpoint detected in the first 5 bytes
				}

				ptr++;
			}

			FreeLibrary(hmodule); // unload dll

			return 0; // if no breakpoint detected in the first 5 bytes

		}

		else {

			FreeLibrary(hmodule); // unload dll

			return-2; // if we failed to get the address of the selected API
		}

	}

	return-1; // if we failed to get a handle to the selected dll in the first place

}
