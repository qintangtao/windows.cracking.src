#include <Windows.h>
#include <winnt.h>
#include <excpt.h>
#include <wchar.h>
#include <stdio.h>
#include "Injector.h"
#include "Encrypter.h"

#define BB(x) __asm _emit x

#define STRING_COMPARE(str1, str2) \
    __asm push str1 \
    __asm call get_string_length \
    __asm push eax \
    __asm push str1 \
    __asm mov eax, str2 \
    __asm push eax \
    __asm call strings_equal

#pragma code_seg(".pediy")

void __declspec(naked) injection_stub(void) {
	//Fake pushad
	BB(0x60) BB(0xE8) BB(0x03) BB(0x00) BB(0x00) BB(0x00) BB(0xE9) BB(0xEB) BB(0x05) BB(0x5D) BB(0x90) BB(0x45) BB(0x55) BB(0xC3) BB(0xE8) BB(0x01) BB(0x00) BB(0x00)
	BB(0x00) BB(0xEB) BB(0xF8) BB(0x73) BB(0x01) BB(0xEB) BB(0x58) BB(0x8B) BB(0xC8) BB(0x83) BB(0xC0) BB(0x0F) BB(0x50) BB(0x51) BB(0xC3) BB(0xEB) BB(0x61)
	__asm { //Prologue, stub entry point
		sub esp, 0x30			//Save stack place for esp law
		push ecx
		push edi
		push esi
		call ExpHandle
		mov edi, fs:[0]
		pushad                  //Save context of entry point
		push ebp
		mov fs : [0], esp
		add fs : [0], 0x4
		mov ebp, esp
		sub esp, 0x200          //Space for local variables

	}
	PIMAGE_DOS_HEADER target_image_base;
	PIMAGE_DOS_HEADER kernel32_image_base;
	__asm {
		call get_module_list    //Get PEB
		mov ebx, eax
		push 0
		push ebx
		call get_dll_base       //Get image base of process
		mov[target_image_base], eax
		push 2
		push ebx
		call get_dll_base       //Get kernel32.dll image base
		mov[kernel32_image_base], eax
	}
	__asm {
		jmp stubback
		push eax
		push fs : [0]
		mov fs : [0], esp
	}
	BB(0xEB)
		stub:
	__asm {
		push ebp
		mov ebp, esp
		sub esp, 0x20
		fldz
		fnstenv[esp - 0xC]		//Stub check start
		pop eax
		mov esp, ebp
		pop ebp
		pop ebx
		inc ebx
		inc ebx
		jmp ebx				//ret addr+2
	}
	//eax + 0xF
	BB(0x6A) BB(0xAC) BB(0x8E)
	BB(0xAA) BB(0xCD) BB(0x0D) BB(0x2C) BB(0xCD) BB(0x8C) BB(0x8D) BB(0xAC) BB(0x8C)
	BB(0xA8) BB(0x0F) BB(0x6C) BB(0xAC) BB(0x0E) BB(0x8E) BB(0x2D) BB(0xED) BB(0xCD)
	BB(0xC8) BB(0x2D) BB(0x8D) BB(0x8E) BB(0xAC) BB(0x4E)
	BB(0)
	BB(0x10)	//Checksum stub
	//toplevelhandler	eax + 0xF + 0x1D
	__asm {
		push ebp
		mov ebp, esp
		sub esp, 0x200
	}
	struct _EXCEPTION_POINTERS *ExceptionInfo;
	DWORD DecAddr;
	__asm {
		mov eax, [ebp + 0x8]
		mov ExceptionInfo, eax
		call getjmpaddr
	getjmpaddr :
		pop eax
		jmp getjmpaddrend
		jmp epilogue
		getjmpaddrend :
		add eax, 0x3
		mov DecAddr, eax
	}
	if (ExceptionInfo->ContextRecord->Dr0 || ExceptionInfo->ContextRecord->Dr1 || ExceptionInfo->ContextRecord->Dr2 || ExceptionInfo->ContextRecord->Dr3)
	{
		DecAddr += ExceptionInfo->ContextRecord->Dr7;
	}
	ExceptionInfo->ContextRecord->Esp -= 4;
	*(DWORD*)(ExceptionInfo->ContextRecord->Esp) = ExceptionInfo->ContextRecord->Eip + 2;
	ExceptionInfo->ContextRecord->Eip = DecAddr;
	__asm{
		mov esp, ebp
		pop ebp
		mov eax, EXCEPTION_CONTINUE_EXECUTION
		ret 0x4
	}
	stubback:
	__asm{
		push 0
	}
	BB(0xCD)
	BB(0x3)
	__asm { //Dummy Code
		mov ecx, 0x23333
		l:
		call get_module_list
		inc eax
		loop l
		popad
	}
	BB(0xEB)
	BB(0x2F)
	BB(0xE8)
	BB(0x56)
	BB(0xFF)
    __asm {			//Decrypt all sections
        push kernel32_image_base
        push target_image_base
    }
	__asm{
		push 1
		int 3
	}
	__asm{			//Dummy code
		mov esp, ebp
		pop ebp
		popad
		jmp section_name
		cld
		xor eax, eax
		mov eax, [eax]
		pushad
		pushfd
	}
	BB(0xFF)
	BB(0xCD)
	BB(0xEB)
	__asm{
    	call decrypt_sections
		push ebp
		//int 0x2
		//jmp epilogue
//Will OVERWRITE eax, ebx, ecx & edx!

rdtsc
mov ebx, edx
je Node1
jne Node2
_emit 0xE9
Node23call:
pop eax
jne Node23
Node23je:
popad
je Node23
_emit 0xE9
Node23:
mov ecx, 0xFF7E2B
Node23loop:
push eax
pop eax
inc eax
loop Node23loop
jne Node23ne
pushad
je Node27je
_emit 0xE8
Node23ne:
call Node28call
_emit 0x73
Node32call:
pop eax
jne Node32
Node32je:
popad
je Node32
_emit 0x75
Node32:
mov ecx, 0xFF1FBF
Node32loop:
push eax
pop eax
inc eax
loop Node32loop
jne Node32ne
pushad
je Node36je
_emit 0xE8
Node32ne:
call Node37call
_emit 0x72
Node8call:
pop eax
jne Node8
Node8je:
popad
je Node8
_emit 0x75
Node8:
mov ecx, 0xFF67A5
Node8loop:
push eax
pop eax
inc eax
loop Node8loop
jne Node8ne
pushad
je Node12je
_emit 0xE8
Node8ne:
call Node9call
_emit 0xEB
Node48call:
pop eax
jne Node48
Node48je:
popad
je Node48
_emit 0xE9
Node48:
mov ecx, 0xFF4BFE
Node48loop:
push eax
pop eax
inc eax
loop Node48loop
jne Node48ne
pushad
je Node54je
_emit 0x73
Node48ne:
call Node52call
_emit 0xE9
Node34call:
pop eax
jne Node34
Node34je:
popad
je Node34
_emit 0x72
Node34:
mov ecx, 0xFF3CAA
Node34loop:
push eax
pop eax
inc eax
loop Node34loop
jne Node34ne
pushad
je Node40je
_emit 0x75
Node34ne:
call Node38call
_emit 0xF
Node28call:
pop eax
jne Node28
Node28je:
popad
je Node28
_emit 0xE9
Node28:
mov ecx, 0xFF44A5
Node28loop:
push eax
pop eax
inc eax
loop Node28loop
jne Node28ne
pushad
je Node31je
_emit 0x72
Node28ne:
call Node30call
_emit 0x73
Node54call:
pop eax
jne Node54
Node54je:
popad
je Node54
_emit 0x73
Node54:
mov ecx, 0xFF5597
Node54loop:
push eax
pop eax
inc eax
loop Node54loop
jne Node54ne
pushad
je Node59je
_emit 0x75
Node54ne:
call Node58call
_emit 0xE9
Node58call:
pop eax
jne Node58
Node58je:
popad
je Node58
_emit 0xE9
Node58:
mov ecx, 0xFF71A6
Node58loop:
push eax
pop eax
inc eax
loop Node58loop
jne Node58ne
pushad
je Node64je
_emit 0xF
Node58ne:
call Node65call
_emit 0x72
Node61call:
pop eax
jne Node61
Node61je:
popad
je Node61
_emit 0xE8
Node61:
mov ecx, 0xFF278F
Node61loop:
push eax
pop eax
inc eax
loop Node61loop
jne Node61ne
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0xF
Node61ne:
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0x72
Node38call:
pop eax
jne Node38
Node38je:
popad
je Node38
_emit 0x73
Node38:
mov ecx, 0xFF14F5
Node38loop:
push eax
pop eax
inc eax
loop Node38loop
jne Node38ne
pushad
je Node43je
_emit 0x72
Node38ne:
call Node44call
_emit 0x74
Node41call:
pop eax
jne Node41
Node41je:
popad
je Node41
_emit 0xF
Node41:
mov ecx, 0xFF62F9
Node41loop:
push eax
pop eax
inc eax
loop Node41loop
jne Node41ne
pushad
je Node46je
_emit 0x74
Node41ne:
call Node43call
_emit 0x73
Node30call:
pop eax
jne Node30
Node30je:
popad
je Node30
_emit 0x74
Node30:
mov ecx, 0xFF710A
Node30loop:
push eax
pop eax
inc eax
loop Node30loop
jne Node30ne
pushad
je Node34je
_emit 0x74
Node30ne:
call Node33call
_emit 0xF
Node51call:
pop eax
jne Node51
Node51je:
popad
je Node51
_emit 0xF
Node51:
mov ecx, 0xFF21FE
Node51loop:
push eax
pop eax
inc eax
loop Node51loop
jne Node51ne
pushad
je Node56je
_emit 0xEB
Node51ne:
call Node57call
_emit 0x72
Node6call:
pop eax
jne Node6
Node6je:
popad
je Node6
_emit 0xEB
Node6:
mov ecx, 0xFF7F62
Node6loop:
push eax
pop eax
inc eax
loop Node6loop
jne Node6ne
pushad
je Node10je
_emit 0xF
Node6ne:
call Node9call
_emit 0x72
Node9call:
pop eax
jne Node9
Node9je:
popad
je Node9
_emit 0xE9
Node9:
mov ecx, 0xFF270F
Node9loop:
push eax
pop eax
inc eax
loop Node9loop
jne Node9ne
pushad
je Node13je
_emit 0x73
Node9ne:
call Node14call
_emit 0x74
Node43call:
pop eax
jne Node43
Node43je:
popad
je Node43
_emit 0xEB
Node43:
mov ecx, 0xFF36FE
Node43loop:
push eax
pop eax
inc eax
loop Node43loop
jne Node43ne
pushad
je Node48je
_emit 0xE8
Node43ne:
call Node49call
_emit 0x75
Node11call:
pop eax
jne Node11
Node11je:
popad
je Node11
_emit 0xE9
Node11:
mov ecx, 0xFF2B5E
Node11loop:
push eax
pop eax
inc eax
loop Node11loop
jne Node11ne
pushad
je Node15je
_emit 0x73
Node11ne:
call Node16call
_emit 0xE9
Node21call:
pop eax
jne Node21
Node21je:
popad
je Node21
_emit 0x74
Node21:
mov ecx, 0xFF62E3
Node21loop:
push eax
pop eax
inc eax
loop Node21loop
jne Node21ne
pushad
je Node25je
_emit 0xE8
Node21ne:
call Node23call
_emit 0x72
Node47call:
pop eax
jne Node47
Node47je:
popad
je Node47
_emit 0xF
Node47:
mov ecx, 0xFF3D60
Node47loop:
push eax
pop eax
inc eax
loop Node47loop
jne Node47ne
pushad
je Node52je
_emit 0x73
Node47ne:
call Node48call
_emit 0xF
Node18call:
pop eax
jne Node18
Node18je:
popad
je Node18
_emit 0x75
Node18:
mov ecx, 0xFF0898
Node18loop:
push eax
pop eax
inc eax
loop Node18loop
jne Node18ne
pushad
je Node22je
_emit 0xF
Node18ne:
call Node23call
_emit 0xF
Node24call:
pop eax
jne Node24
Node24je:
popad
je Node24
_emit 0x75
Node24:
mov ecx, 0xFF2F52
Node24loop:
push eax
pop eax
inc eax
loop Node24loop
jne Node24ne
pushad
je Node29je
_emit 0x73
Node24ne:
call Node27call
_emit 0xE8
Node37call:
pop eax
jne Node37
Node37je:
popad
je Node37
_emit 0x74
Node37:
mov ecx, 0xFF5EAB
Node37loop:
push eax
pop eax
inc eax
loop Node37loop
jne Node37ne
pushad
je Node42je
_emit 0x75
Node37ne:
call Node39call
_emit 0xF
Node20call:
pop eax
jne Node20
Node20je:
popad
je Node20
_emit 0x75
Node20:
mov ecx, 0xFF15B0
Node20loop:
push eax
pop eax
inc eax
loop Node20loop
jne Node20ne
pushad
je Node25je
_emit 0xEB
Node20ne:
call Node26call
_emit 0xEB
Node56call:
pop eax
jne Node56
Node56je:
popad
je Node56
_emit 0x74
Node56:
mov ecx, 0xFF532C
Node56loop:
push eax
pop eax
inc eax
loop Node56loop
jne Node56ne
pushad
je Node62je
_emit 0x74
Node56ne:
call Node58call
_emit 0x72
Node40call:
pop eax
jne Node40
Node40je:
popad
je Node40
_emit 0x72
Node40:
mov ecx, 0xFF55BF
Node40loop:
push eax
pop eax
inc eax
loop Node40loop
jne Node40ne
pushad
je Node45je
_emit 0xEB
Node40ne:
call Node41call
_emit 0x75
Node62call:
pop eax
jne Node62
Node62je:
popad
je Node62
_emit 0xE8
Node62:
mov ecx, 0xFF2230
Node62loop:
push eax
pop eax
inc eax
loop Node62loop
jne Node62ne
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0x75
Node62ne:
call Node65call
_emit 0x75
Node64call:
pop eax
jne Node64
Node64je:
popad
je Node64
_emit 0xEB
Node64:
mov ecx, 0xFF4404
Node64loop:
push eax
pop eax
inc eax
loop Node64loop
jne Node64ne
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0x75
Node64ne:
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0xE9
Node22call:
pop eax
jne Node22
Node22je:
popad
je Node22
_emit 0x73
Node22:
mov ecx, 0xFF7C50
Node22loop:
push eax
pop eax
inc eax
loop Node22loop
jne Node22ne
pushad
je Node26je
_emit 0xE9
Node22ne:
call Node23call
_emit 0xEB
Node31call:
pop eax
jne Node31
Node31je:
popad
je Node31
_emit 0xE8
Node31:
mov ecx, 0xFF52CB
Node31loop:
push eax
pop eax
inc eax
loop Node31loop
jne Node31ne
pushad
je Node35je
_emit 0x75
Node31ne:
call Node33call
_emit 0xEB
Node1call:
pop eax
jne Node1
Node1je:
popad
je Node1
_emit 0xE9
Node1:
mov ecx, 0xFF3D27
Node1loop:
push eax
pop eax
inc eax
loop Node1loop
jne Node1ne
pushad
je Node3je
_emit 0xE8
Node1ne:
call Node2call
_emit 0xF
Node63call:
pop eax
jne Node63
Node63je:
popad
je Node63
_emit 0x72
Node63:
mov ecx, 0xFF50A2
Node63loop:
push eax
pop eax
inc eax
loop Node63loop
jne Node63ne
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0xE9
Node63ne:
call Node65call
_emit 0x74
Node52call:
pop eax
jne Node52
Node52je:
popad
je Node52
_emit 0x73
Node52:
mov ecx, 0xFF31D8
Node52loop:
push eax
pop eax
inc eax
loop Node52loop
jne Node52ne
pushad
je Node54je
_emit 0xE9
Node52ne:
call Node53call
_emit 0x72
Node5call:
pop eax
jne Node5
Node5je:
popad
je Node5
_emit 0xEB
Node5:
mov ecx, 0xFF36DB
Node5loop:
push eax
pop eax
inc eax
loop Node5loop
jne Node5ne
pushad
je Node8je
_emit 0xF
Node5ne:
call Node9call
_emit 0xF
Node57call:
pop eax
jne Node57
Node57je:
popad
je Node57
_emit 0xEB
Node57:
mov ecx, 0xFF6F85
Node57loop:
push eax
pop eax
inc eax
loop Node57loop
jne Node57ne
pushad
je Node63je
_emit 0x75
Node57ne:
call Node61call
_emit 0x73
Node27call:
pop eax
jne Node27
Node27je:
popad
je Node27
_emit 0x75
Node27:
mov ecx, 0xFF080D
Node27loop:
push eax
pop eax
inc eax
loop Node27loop
jne Node27ne
pushad
je Node30je
_emit 0xF
Node27ne:
call Node29call
_emit 0xE9
Node44call:
pop eax
jne Node44
Node44je:
popad
je Node44
_emit 0x75
Node44:
mov ecx, 0xFF39D9
Node44loop:
push eax
pop eax
inc eax
loop Node44loop
jne Node44ne
pushad
je Node50je
_emit 0x73
Node44ne:
call Node51call
_emit 0x72
Node35call:
pop eax
jne Node35
Node35je:
popad
je Node35
_emit 0xE8
Node35:
mov ecx, 0xFF3E93
Node35loop:
push eax
pop eax
inc eax
loop Node35loop
jne Node35ne
pushad
je Node40je
_emit 0x73
Node35ne:
call Node37call
_emit 0xF
Node42call:
pop eax
jne Node42
Node42je:
popad
je Node42
_emit 0x73
Node42:
mov ecx, 0xFF797F
Node42loop:
push eax
pop eax
inc eax
loop Node42loop
jne Node42ne
pushad
je Node47je
_emit 0xEB
Node42ne:
call Node46call
_emit 0xE9
Node3call:
pop eax
jne Node3
Node3je:
popad
je Node3
_emit 0xE8
Node3:
mov ecx, 0xFF4B0C
Node3loop:
push eax
pop eax
inc eax
loop Node3loop
jne Node3ne
pushad
je Node5je
_emit 0xF
Node3ne:
call Node6call
_emit 0xE9
Node15call:
pop eax
jne Node15
Node15je:
popad
je Node15
_emit 0x73
Node15:
mov ecx, 0xFF42A9
Node15loop:
push eax
pop eax
inc eax
loop Node15loop
jne Node15ne
pushad
je Node19je
_emit 0x74
Node15ne:
call Node18call
_emit 0x73
Node60call:
pop eax
jne Node60
Node60je:
popad
je Node60
_emit 0x74
Node60:
mov ecx, 0xFF22D9
Node60loop:
push eax
pop eax
inc eax
loop Node60loop
jne Node60ne
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0x74
Node60ne:
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0x74
Node7call:
pop eax
jne Node7
Node7je:
popad
je Node7
_emit 0xE8
Node7:
mov ecx, 0xFF6B8A
Node7loop:
push eax
pop eax
inc eax
loop Node7loop
jne Node7ne
pushad
je Node11je
_emit 0x75
Node7ne:
call Node10call
_emit 0x75
Node17call:
pop eax
jne Node17
Node17je:
popad
je Node17
_emit 0xF
Node17:
mov ecx, 0xFF5EA3
Node17loop:
push eax
pop eax
inc eax
loop Node17loop
jne Node17ne
pushad
je Node21je
_emit 0xE9
Node17ne:
call Node18call
_emit 0xF
Node10call:
pop eax
jne Node10
Node10je:
popad
je Node10
_emit 0x75
Node10:
mov ecx, 0xFF32DC
Node10loop:
push eax
pop eax
inc eax
loop Node10loop
jne Node10ne
pushad
je Node14je
_emit 0x72
Node10ne:
call Node11call
_emit 0x73
Node19call:
pop eax
jne Node19
Node19je:
popad
je Node19
_emit 0x75
Node19:
mov ecx, 0xFF68D2
Node19loop:
push eax
pop eax
inc eax
loop Node19loop
jne Node19ne
pushad
je Node24je
_emit 0x75
Node19ne:
call Node21call
_emit 0xEB
Node45call:
pop eax
jne Node45
Node45je:
popad
je Node45
_emit 0x74
Node45:
mov ecx, 0xFF765B
Node45loop:
push eax
pop eax
inc eax
loop Node45loop
jne Node45ne
pushad
je Node50je
_emit 0x73
Node45ne:
call Node47call
_emit 0xE8
Node36call:
pop eax
jne Node36
Node36je:
popad
je Node36
_emit 0x74
Node36:
mov ecx, 0xFF3C16
Node36loop:
push eax
pop eax
inc eax
loop Node36loop
jne Node36ne
pushad
je Node41je
_emit 0xE8
Node36ne:
call Node38call
_emit 0xE9
Node46call:
pop eax
jne Node46
Node46je:
popad
je Node46
_emit 0x74
Node46:
mov ecx, 0xFF7947
Node46loop:
push eax
pop eax
inc eax
loop Node46loop
jne Node46ne
pushad
je Node52je
_emit 0xE8
Node46ne:
call Node53call
_emit 0xE8
Node25call:
pop eax
jne Node25
Node25je:
popad
je Node25
_emit 0x74
Node25:
mov ecx, 0xFF31E2
Node25loop:
push eax
pop eax
inc eax
loop Node25loop
jne Node25ne
pushad
je Node30je
_emit 0xE8
Node25ne:
call Node28call
_emit 0xEB
Node39call:
pop eax
jne Node39
Node39je:
popad
je Node39
_emit 0x73
Node39:
mov ecx, 0xFF3AE6
Node39loop:
push eax
pop eax
inc eax
loop Node39loop
jne Node39ne
pushad
je Node44je
_emit 0xE8
Node39ne:
call Node43call
_emit 0x73
Node12call:
pop eax
jne Node12
Node12je:
popad
je Node12
_emit 0xF
Node12:
mov ecx, 0xFF60EC
Node12loop:
push eax
pop eax
inc eax
loop Node12loop
jne Node12ne
pushad
je Node16je
_emit 0xF
Node12ne:
call Node14call
_emit 0x73
Node16call:
pop eax
jne Node16
Node16je:
popad
je Node16
_emit 0xF
Node16:
mov ecx, 0xFF5D00
Node16loop:
push eax
pop eax
inc eax
loop Node16loop
jne Node16ne
pushad
je Node20je
_emit 0x73
Node16ne:
call Node21call
_emit 0x73
Node59call:
pop eax
jne Node59
Node59je:
popad
je Node59
_emit 0xF
Node59:
mov ecx, 0xFF0BAF
Node59loop:
push eax
pop eax
inc eax
loop Node59loop
jne Node59ne
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0x74
Node59ne:
call Node60call
_emit 0xE8
Node55call:
pop eax
jne Node55
Node55je:
popad
je Node55
_emit 0x75
Node55:
mov ecx, 0xFF1A61
Node55loop:
push eax
pop eax
inc eax
loop Node55loop
jne Node55ne
pushad
je Node60je
_emit 0x74
Node55ne:
call Node61call
_emit 0xEB
Node29call:
pop eax
jne Node29
Node29je:
popad
je Node29
_emit 0xE9
Node29:
mov ecx, 0xFF1EEA
Node29loop:
push eax
pop eax
inc eax
loop Node29loop
jne Node29ne
pushad
je Node33je
_emit 0x74
Node29ne:
call Node30call
_emit 0xEB
Node26call:
pop eax
jne Node26
Node26je:
popad
je Node26
_emit 0x75
Node26:
mov ecx, 0xFF4D5B
Node26loop:
push eax
pop eax
inc eax
loop Node26loop
jne Node26ne
pushad
je Node31je
_emit 0xF
Node26ne:
call Node32call
_emit 0x74
Node49call:
pop eax
jne Node49
Node49je:
popad
je Node49
_emit 0xEB
Node49:
mov ecx, 0xFF401E
Node49loop:
push eax
pop eax
inc eax
loop Node49loop
jne Node49ne
pushad
je Node55je
_emit 0x73
Node49ne:
call Node50call
_emit 0xE8
Node14call:
pop eax
jne Node14
Node14je:
popad
je Node14
_emit 0x72
Node14:
mov ecx, 0xFF0642
Node14loop:
push eax
pop eax
inc eax
loop Node14loop
jne Node14ne
pushad
je Node18je
_emit 0x73
Node14ne:
call Node15call
_emit 0xEB
Node50call:
pop eax
jne Node50
Node50je:
popad
je Node50
_emit 0x74
Node50:
mov ecx, 0xFF6C13
Node50loop:
push eax
pop eax
inc eax
loop Node50loop
jne Node50ne
pushad
je Node52je
_emit 0x74
Node50ne:
call Node51call
_emit 0xEB
Node13call:
pop eax
jne Node13
Node13je:
popad
je Node13
_emit 0x72
Node13:
mov ecx, 0xFF31EE
Node13loop:
push eax
pop eax
inc eax
loop Node13loop
jne Node13ne
pushad
je Node17je
_emit 0xE9
Node13ne:
call Node15call
_emit 0xE9
Node4call:
pop eax
jne Node4
Node4je:
popad
je Node4
_emit 0x73
Node4:
mov ecx, 0xFF2553
Node4loop:
push eax
pop eax
inc eax
loop Node4loop
jne Node4ne
pushad
je Node7je
_emit 0xEB
Node4ne:
call Node5call
_emit 0x73
Node2call:
pop eax
jne Node2
Node2je:
popad
je Node2
_emit 0x74
Node2:
mov ecx, 0xFF503C
Node2loop:
push eax
pop eax
inc eax
loop Node2loop
jne Node2ne
pushad
je Node4je
_emit 0x73
Node2ne:
call Node3call
_emit 0xEB
Node53call:
pop eax
jne Node53
Node53je:
popad
je Node53
_emit 0x75
Node53:
mov ecx, 0xFF32AD
Node53loop:
push eax
pop eax
inc eax
loop Node53loop
jne Node53ne
pushad
je Node58je
_emit 0xE9
Node53ne:
call Node56call
_emit 0xF
Node33call:
pop eax
jne Node33
Node33je:
popad
je Node33
_emit 0xE8
Node33:
mov ecx, 0xFF3A8D
Node33loop:
push eax
pop eax
inc eax
loop Node33loop
jne Node33ne
pushad
je Node38je
_emit 0x72
Node33ne:
call Node39call
_emit 0x74
Node65call:
pop eax
jne Node65
Node65je:
popad
je Node65
_emit 0xF
Node65:
mov ecx, 0xFF6DB4
Node65loop:
push eax
pop eax
inc eax
loop Node65loop
jne Node65ne
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0xE8
Node65ne:
rdtscp
cmp edx, ebx
jne Nodedone
int 0x02
_emit 0xE8
Nodedone:
int 0x3
	}
	BB(0xE8)
    ///////////////////////////////////////////////////////////////////
    //Gets the module list
    //Preserves no registers, PEB_LDR_DATA->PPEB_LDR_DATA->InLoadOrderModuleList returned in EAX
    ///////////////////////////////////////////////////////////////////
    __asm {
    get_module_list:
            mov eax, fs:[0x30]  //PEB
            mov eax, [eax+0xC]  //PEB_LDR_DATA->PPEB_LDR_DATA
            mov eax, [eax+0xC]  //PEB_LDR_DATA->PPEB_LDR_DATA->InLoadOrderModuleList
            retn
    }
    ///////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////
    //Gets the DllBase member of the InLoadOrderModuleList structure
    //Call as void *get_dll_base(void *InLoadOrderModuleList, int index)
    ///////////////////////////////////////////////////////////////////
    __asm {
    get_dll_base:
        push ebp
        mov ebp, esp
        mov eax, [ebp+0x8]      //PEB->PPEB_LDR_DATA->InLoadOrderModuleList address
        mov ecx, [ebp+0xC]      //Set loop index
        cmp ecx, 0x0			//Initial zero check
        je done
        traverse_list:
            mov eax, [eax]      //Go to next entry
        loop traverse_list
        done:
            mov eax, [eax+0x18] //PEB->PPEB_LDR_DATA->InLoadOrderModuleList.DllBase
            mov esp, ebp
            pop ebp
            ret 0x8
    }
    ///////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////
    //Gets the length of the string passed as the parameter
    //Call as int get_string_length(char *str)
    ///////////////////////////////////////////////////////////////////
    __asm {
    get_string_length:
        push ebp
        mov ebp, esp
        mov edi, [ebp+0x8]      //String held here
        mov eax, 0x0            //EAX holds size of the string
        counting_loop:
            cmp byte ptr[edi], 0x0//Current byte is null-terminator?
            je string_done      //Done, leave loop
            inc edi             //Go to next character
            inc eax             //size++
            jmp counting_loop
        string_done:
            mov esp, ebp
            pop ebp
            retn
    }
    ///////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////
    //String comparison function, checks for equality of two strings
    //Call as bool strings_equal(char *check_string, char *known_string, int known_string_length)
    ///////////////////////////////////////////////////////////////////
    __asm {
    strings_equal:
        push ebp
        mov ebp, esp

        mov edi, [ebp+0xC]      //EDI gets known_string
        mov ecx, [ebp+0x10]     //ECX gets known_string_length
		cmp ecx, 0x0
		je comp

    restore_str:
		mov al, [edi]
		rol al, 0x3
		mov [edi], al
		inc edi
		loop restore_str

	comp:
        mov ecx, [ebp+0x10]     //ECX gets known_string_length
        mov edi, [ebp+0xC]      //EDI gets known_string
        mov esi, [ebp+0x8]      //ESI gets check_string
        cld                     //Forward comparison
    	repe cmpsb              //Start comparing
        mov esi, 0x0            //Assume unequal
        jne end1
        mov esi, 0x1            //Strings equal
	end1:
        mov edi, [ebp+0xC]      //EDI gets known_string
        mov ecx, [ebp+0x10]     //ECX gets known_string_length
		cmp ecx, 0x0
		je end2

    destore_str:
		mov al, [edi]
		rol al, 0x5
		mov [edi], al
		inc edi
		loop destore_str

    end2:
		mov eax, esi
        mov esp, ebp
        pop ebp
        ret 0xC
    }
    ///////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////
    //Implementation of GetProcAddress
    //Call as FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
    ///////////////////////////////////////////////////////////////////
    get_proc_address:
        __asm {
            push ebp
            mov ebp, esp
            sub esp, 0x200
        }
        PIMAGE_DOS_HEADER kernel32_dos_header;
        PIMAGE_NT_HEADERS kernel32_nt_headers;
        PIMAGE_EXPORT_DIRECTORY kernel32_export_dir;
        unsigned short *ordinal_table;
        unsigned long *function_table;
        FARPROC function_address;
        int function_names_equal;
        __asm { //Initializations
            mov eax, [ebp+0x8]
            mov kernel32_dos_header, eax
            mov function_names_equal, 0x0
        }
        kernel32_nt_headers = (PIMAGE_NT_HEADERS)((DWORD_PTR)kernel32_dos_header + kernel32_dos_header->e_lfanew);
        kernel32_export_dir = (PIMAGE_EXPORT_DIRECTORY)((DWORD_PTR)kernel32_dos_header +
            kernel32_nt_headers->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
        for(unsigned long i = 0; i < kernel32_export_dir->NumberOfNames; ++i) {
            char *eat_entry = (*(char **)((DWORD_PTR)kernel32_dos_header + kernel32_export_dir->AddressOfNames + i * sizeof(DWORD_PTR)))
                + (DWORD_PTR)kernel32_dos_header;   //Current name in name table
            STRING_COMPARE([ebp+0xC], eat_entry) //Compare function in name table with the one we want to find
            __asm mov function_names_equal, eax
            if(function_names_equal == 1) {
                ordinal_table = (unsigned short *)(kernel32_export_dir->AddressOfNameOrdinals + (DWORD_PTR)kernel32_dos_header);
                function_table = (unsigned long *)(kernel32_export_dir->AddressOfFunctions + (DWORD_PTR)kernel32_dos_header);
                function_address = (FARPROC)((DWORD_PTR)kernel32_dos_header + function_table[ordinal_table[i]]);
                break;
            }
        }
        __asm {
            mov eax, function_address
            mov esp, ebp
            pop ebp
            ret 0x8
        }
    ///////////////////////////////////////////////////////////////////
    //Decrypts sections in the image, including .data/.text/section_name
    //Call as void decrypt_sections(void *image_base, void *kernel32_base)
    ///////////////////////////////////////////////////////////////////
    decrypt_sections:
        __asm {
            push ebp
            mov ebp, esp
            sub esp, 0x200
        }
        typedef BOOL (WINAPI *pVirtualProtect)(LPVOID lpAddress, SIZE_T dwSize, DWORD flNewProtect,
            PDWORD lpflOldProtect);
        char *str_virtualprotect;
        char *str_section_name;
        char *str_data_name;
        char *str_text_name;
        PIMAGE_DOS_HEADER target_dos_header;
        int section_offset;
        int section_names_equal;
        unsigned long old_protections;
        pVirtualProtect virtualprotect_addr;
        __asm { //String initializations
            jmp virtualprotect
            virtualprotectback:
                pop esi
                mov str_virtualprotect, esi
            jmp section_name
            section_nameback:
                pop esi
                mov str_section_name, esi
            jmp data_name
            data_nameback:
                pop esi
                mov str_data_name, esi
            jmp text_name
            text_nameback:
                pop esi
                mov str_text_name, esi
        }
        __asm { //Initializations
            mov eax, [ebp+0x8]
            mov target_dos_header, eax
            mov section_offset, 0x0
            mov section_names_equal, 0x0
            push str_virtualprotect
            push [ebp+0xC]
            call get_proc_address
            mov virtualprotect_addr, eax
        }
		LPVOID ESp;
		__asm mov ESp, esp
        virtualprotect_addr(ESp, 0x400, PAGE_READWRITE, &old_protections);
		PIMAGE_NT_HEADERS target_nt_headers;
		target_nt_headers = (PIMAGE_NT_HEADERS)((DWORD_PTR)target_dos_header + target_dos_header->e_lfanew);
        for(unsigned long j = 0; j < target_nt_headers->FileHeader.NumberOfSections; ++j) {
            section_offset = (target_dos_header->e_lfanew + sizeof(IMAGE_NT_HEADERS) +
                (sizeof(IMAGE_SECTION_HEADER) * j));
            PIMAGE_SECTION_HEADER section_header = (PIMAGE_SECTION_HEADER)((DWORD_PTR)target_dos_header + section_offset);
            STRING_COMPARE(str_section_name, section_header)
            __asm mov section_names_equal, eax
            STRING_COMPARE(str_data_name, section_header)
            __asm add section_names_equal, eax
            STRING_COMPARE(str_text_name, section_header)
            __asm add section_names_equal, eax
            if(section_names_equal != 0) {
                unsigned char *current_byte =
                    (unsigned char *)((DWORD_PTR)target_dos_header + section_header->VirtualAddress);
                unsigned char *last_byte =
                    (unsigned char *)((DWORD_PTR)target_dos_header + section_header->VirtualAddress
                    + section_header->SizeOfRawData);
                const unsigned int num_rounds = 32;
                const unsigned int key[4] = {0x12345678, 0xAABBCCDD, 0x10101010, 0xF00DBABE};
                for(current_byte; current_byte < last_byte; current_byte += 8) {
                    virtualprotect_addr(current_byte, sizeof(DWORD_PTR) * 2, PAGE_EXECUTE_READWRITE, &old_protections);
                    unsigned int block1 = (*current_byte << 24) | (*(current_byte+1) << 16) |
                        (*(current_byte+2) << 8) | *(current_byte+3);
                    unsigned int block2 = (*(current_byte+4) << 24) | (*(current_byte+5) << 16) |
                        (*(current_byte+6) << 8) | *(current_byte+7);
                    unsigned int full_block[] = {block1, block2};
                    unsigned int delta = 0x9E3779B9;
                    unsigned int sum = (delta * num_rounds);
                    for (unsigned int i = 0; i < num_rounds; ++i) {
                        full_block[1] -= (((full_block[0] << 4) ^ (full_block[0] >> 5)) + full_block[0]) ^ (sum + key[(sum >> 11) & 3]);
                        sum -= delta;
                        full_block[0] -= (((full_block[1] << 4) ^ (full_block[1] >> 5)) + full_block[1]) ^ (sum + key[sum & 3]);
                    }
                    //virtualprotect_addr(current_byte, sizeof(DWORD_PTR) * 2, old_protections, NULL);
                    *(current_byte+3) = (full_block[0] & 0x000000FF);
                    *(current_byte+2) = (full_block[0] & 0x0000FF00) >> 8;
                    *(current_byte+1) = (full_block[0] & 0x00FF0000) >> 16;
                    *(current_byte+0) = (full_block[0] & 0xFF000000) >> 24;
                    *(current_byte+7) = (full_block[1] & 0x000000FF);
                    *(current_byte+6) = (full_block[1] & 0x0000FF00) >> 8;
                    *(current_byte+5) = (full_block[1] & 0x00FF0000) >> 16;
                    *(current_byte+4) = (full_block[1] & 0xFF000000) >> 24;
                }
            }
            section_names_equal = 0;
        }
        __asm {
            mov esp, ebp
            pop ebp
            ret 0x8
        }

	BB(0xEB)
	epilogue:
	__asm { //Epilogue, stub exit point
		pop ecx
		mov eax, target_image_base
		add eax, 0xCCDDEEFF     //Signature to be replaced by original entry point (OEP)
		mov esp, ebp
		mov[esp + 0x5C], eax     //Store OEP
		mov edi, [esp + 0x4]		//Restore fs:[0]
		add esp, 0xC
		pop ebp
		mov ebx, [esp + 0x4]
		mov edx, [esp + 0x8]
		mov ecx, [esp + 0xC]
		mov eax, [esp + 0x12]
		pop esp
		mov fs : [0], edi
		pop esi
		pop edi
		add esp, 0x30
		//ret                 //Jump to OEP
		pop edi
		add edi, 0x5
		push edi
		add edi, 0x81a
		add edi, 0x15
	}
		//Stolen Bytes(Should NOT use esi and edi, should NOT be jumped to, should not use local variables)
		BB(0x55) BB(0x8B) BB(0xEC) BB(0x83) BB(0xEC) BB(0x14)
	__asm {
		call ExpHandle		//SEH
		push esi
		push fs : [0]
		mov fs:[0], esp
	}
		BB(0x83) BB(0x65) BB(0xF4) BB(0x00)
		BB(0x83) BB(0x65) BB(0xF4) BB(0x00)
		BB(0xA1) BB(0x20) BB(0xF3) BB(0x43) BB(0x00)
		BB(0x56) BB(0x57)
	__asm{
		push 1
		jmp edi				//jmp to OEP
	}
	ExpHandle:
	__asm{
		call GetHAddr
		jmp RealHandler		//Handle check start
		GetHAddr:
		pop esi
		ret
	}
	RealHandler:
	__asm{
		pushad
		push ebp
		mov ebp, esp
		sub esp, 0x200
	}
	DWORD i;
	EXCEPTION_RECORD* excrecord;
	CONTEXT* conrecord;
	DWORD start;
	DWORD stop;
	__asm {
		mov eax, [esp+0x228]
		mov excrecord, eax
		mov eax, [esp+0x230]
		mov conrecord, eax
	}

	i = *(DWORD*) conrecord->Esp;
	if(i==0)
	{
		BB(0x6A)
		BB(0x01)			//push 0x1
		BB(0xCD)
		BB(0x03)			// int 03
		BB(0x91)
		BB(0x64)
		BB(0x74)
		BB(0xEB)
		BB(0xFF)
		BB(0x90)
		BB(0x60)
		BB(0x61)
		BB(0xE9)
		BB(0x12)
		BB(0xCC)
		BB(0x90)
		BB(0xA4)
		BB(0x64)
		BB(0x64)
		BB(0x33)
		BB(0x21)
		BB(0xF0)
		BB(0xE8)
		__asm {
			call stub
		error :
			int 0x2
			push eax
			add ebx, 0x52
			push ebx
			call Checksum
			//xor eax, eax		//Test
			test eax, eax
			jne error
		}
		__asm {
			call get_module_list    //Get PEB
			push 2
			push eax
			call get_dll_base       //Get kernel32.dll image base
			mov ecx, eax
			call stub
			rdtsc
			add eax, 0xF
			push eax
			mov ebx, eax
			push ecx
			call get_proc_address
			add ebx, 0x1D
			push ebx
			call eax
		}
	}
	if(i==0x55AA)
	{
		__asm{
			//TODO
		}
	}

	i = 0x13;
	__asm {
		call ExpHandle
		mov start, esi
		jmp HandleEnd
	}
	BB(0x5D)			//Checksum Handle
	BB(0xEB)
	__asm{
	HandleEndback:
		pop edi
		mov stop, edi	//Stub check stop
	}
	i++;
	__asm{
		mov eax, start
		push eax
		mov eax, stop
		inc eax
		push eax
		call eax		//Check handle
		pop ebx
		pop ebx
		mov ebx, i
		add eax, ebx
		mov i, eax
	}

	if (excrecord->ExceptionCode == EXCEPTION_BREAKPOINT) {
		conrecord->Eip ^= conrecord->Dr0;
		conrecord->Eip ^= conrecord->Dr1;
		conrecord->Eip ^= conrecord->Dr2;
		conrecord->Eip ^= conrecord->Dr3;
		conrecord->Eip += i;
		conrecord->Esp += 4;
		__asm {
			mov esp, ebp
			pop ebp
			popad
			mov eax, ExceptionContinueExecution
			ret
		}
	}
	else
	{
		if(excrecord->ExceptionCode == STATUS_ACCESS_VIOLATION && excrecord->ExceptionInformation[1] != 0xFFFFFFFF && excrecord->ExceptionInformation[0] !=0)
			conrecord->Eip += i + 0xC;
		__asm {
			mov esp, ebp
			pop ebp
			popad
			mov eax, ExceptionContinueSearch
			ret
		}
	}
	__asm{
	HandleEnd:
		call HandleEndback		//Handle check end
	}
	BB(0xEB)
	Checksum:
	__asm {
		push ebp
		mov ebp, esp
		sub esp, 0x200
	}
	DWORD s;
	DWORD ed;
	DWORD counter;
	__asm{
		mov eax, [ebp+0x8]
		mov ed, eax
		mov eax, [ebp+0xC]
		mov s, eax
	}
	counter = ed - s;
	__asm{
		mov ecx, counter;
		inc ecx
		xor eax, eax
		mov edx, eax
		mov eax, s
		test edx, edx
		je checkloop
	}
	BB(0xE8)
	__asm{
	checkloop:
		mov bl, cs:[eax]
		xor dl, bl
		inc eax
		loop checkloop
		//xor edx, edx	//for test
		cmp edx, ecx
		je loopdone
		pop eax
		pop ebx
		mov start, ebx
		inc eax
		push eax
		jmp HandleEndback
	}
	BB(0xEB)
	__asm{
		loopdone:
		mov esp, ebp
		pop ebp
		mov eax, edx
		ret
	}
    __asm {
    virtualprotect:
        call virtualprotectback
        BB(0xCA) BB(0x2D) BB(0x4E) BB(0x8E) BB(0xAE) BB(0x2C) BB(0x8D)
        BB(0x0A) BB(0x4E) BB(0xED) BB(0x8E) BB(0xAC) BB(0x6C) BB(0x8E) BB(0)
    data_name:
        call data_nameback
        BB(0xC5) BB(0x8C) BB(0x2C) BB(0x8E) BB(0x2C) BB(0)
    text_name:
        call text_nameback
        BB(0xC5) BB(0x8E) BB(0xAC) BB(0x0F) BB(0x8E) BB(0)
    section_name:
        call section_nameback
        BB(0xE7) BB(0)
        int 0x3
        int 0x3
        int 0x3
    }
}
#pragma code_seg()
#pragma comment(linker, "/SECTION:.pediy,re")

wchar_t *convert_to_unicode(char *str, unsigned int length) {
    wchar_t *wstr;
    int wstr_length = MultiByteToWideChar(CP_ACP, 0, str, (length + 1), NULL, 0);
    wstr = (wchar_t *)malloc(wstr_length * sizeof(wchar_t));
    wmemset(wstr, 0, wstr_length);
    if (wstr == NULL)
        return NULL;
    int written = MultiByteToWideChar(CP_ACP, 0, str, length, wstr, wstr_length);
    if(written > 0)
        return wstr;
    return NULL;
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage: ./%s <target>\n", argv[0]);
        return -1;
    }
    wchar_t *target_file_name = convert_to_unicode(argv[1], strlen(argv[1]));
    if(target_file_name == NULL) {
        printf("Could not convert %s to unicode\n", argv[1]);
        return -1;
    }

	CopyFile(target_file_name, L"packed.exe", FALSE);
	target_file_name = L"packed.exe";

    pfile_info target_file = file_info_create();
    void (*stub_addr)(void) = injection_stub;
    unsigned int stub_size = get_stub_size(stub_addr);
    unsigned int stub_size_aligned = 0;
    bool map_file_success = map_file(target_file_name, stub_size, false, target_file);
    if(map_file_success == false) {
        wprintf(L"Could not map target file\n");
        return -1;
    }
    PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)target_file->file_mem_buffer;
    PIMAGE_NT_HEADERS nt_headers = (PIMAGE_NT_HEADERS)((DWORD_PTR)dos_header + dos_header->e_lfanew);
    stub_size_aligned = align_to_boundary(stub_size, nt_headers->OptionalHeader.SectionAlignment);
    const char *section_name = ".pediy";
    file_info_destroy(target_file);
    target_file = file_info_create();
    (void)map_file(target_file_name, stub_size_aligned, true, target_file);
    PIMAGE_SECTION_HEADER new_section = add_section(section_name, stub_size_aligned, target_file->file_mem_buffer);
    if(new_section == NULL) {
        wprintf(L"Could not add new section to file");
        return -1;
    }
    write_stub_entry_point(nt_headers, stub_addr);
    copy_stub_instructions(new_section, target_file->file_mem_buffer, stub_addr);
    change_file_oep(nt_headers, new_section);
    encrypt_file(nt_headers, target_file, "?");
    int flush_view_success = FlushViewOfFile(target_file->file_mem_buffer, 0);
    if(flush_view_success == 0)
        wprintf(L"Could not save changes to file");
    file_info_destroy(target_file);
    return 0;
}