/*
HeavenInjector
Copyright (C) 2012  George Nicolaou <nicolaou.george[at]gmail.[dot]com>

This file is part of HeavenInjector.

HeavenInjector is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

HeavenInjector is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with HeavenInjector.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <Windows.h>
#include <stdio.h>
typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;


BOOL WindowsErrorPrint(void) {
	DWORD dwErrorId = NULL;
	LPSTR lpaszErrorMessage[512];
	FILE * f;
	dwErrorId = GetLastError();
	FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM + FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwErrorId, NULL, (LPSTR)&lpaszErrorMessage, 512, NULL );
	MessageBoxA( 0, (LPCSTR)lpaszErrorMessage, "Error", 0 );
	return FALSE;
}

__declspec( dllexport ) BOOL  WINAPI InjectLibrary( int nProcessId, 
	char * lpszInjectLibraryPath )
{
	HANDLE hProcess = NULL;
	void * lpLibraryNameAddr;
	SIZE_T stInectLibraryLength = 0;
	HMODULE hKernel32 = NULL;
	FARPROC lpfprocLoadLibraryW = NULL;

	if( ( hProcess = OpenProcess( PROCESS_QUERY_INFORMATION + 
		PROCESS_CREATE_THREAD + PROCESS_VM_OPERATION + PROCESS_VM_WRITE, NULL, 
		(DWORD)nProcessId) ) == NULL ) {
			return WindowsErrorPrint();
	}
	stInectLibraryLength = strlen( lpszInjectLibraryPath ) + 1 ;

	//Write Library Name into remote process memory
	if( ( lpLibraryNameAddr = VirtualAllocEx( hProcess, NULL, 
		stInectLibraryLength, MEM_COMMIT, PAGE_READWRITE) ) == NULL ) {
			return WindowsErrorPrint();
	}

	if( WriteProcessMemory( hProcess, lpLibraryNameAddr, lpszInjectLibraryPath,
		stInectLibraryLength, NULL ) == 0 ) {
			return WindowsErrorPrint();
	}

	hKernel32 = GetModuleHandleA( "kernel32" );
	if( ( lpfprocLoadLibraryW = GetProcAddress( hKernel32, "LoadLibraryA" ) ) == 
		NULL ) {
			return WindowsErrorPrint();
	}

	if( CreateRemoteThread( hProcess, NULL, 0, 
		(LPTHREAD_START_ROUTINE)lpfprocLoadLibraryW, lpLibraryNameAddr, 0, NULL) 
		== NULL ) {
			return WindowsErrorPrint();
	}
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, 
	LPVOID lpReserved )
{
	return TRUE;
}

