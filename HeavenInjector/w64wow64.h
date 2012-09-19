#ifndef __W64WOW64_H_
#define __W64WOW64_H_

__declspec(dllexport) unsigned __int64 X64Call( void * lvpFunctionPtr, 
	int nArgc, ... );
__declspec(dllexport) void * GetProcAddress64( void * lvpBaseAddress, 
	char * lpszProcName );
__declspec(dllexport) void * LoadLibrary64A( char * lpcLibraryName );
extern __declspec(dllexport) BOOL InitializeW64oWoW64(void);
#endif