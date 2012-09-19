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

#include "w64wow64.h"

void PrintUsage()
{
	printf( "HeavenInjector v0.1 By George Nicolaou "
		"( nicolaou.george{at}gmail{dot}com )\nUsage:\n\tHeavenInjector.exe" 
		"<pid> <inject library>" );
}

int main( int argc, char * argv[] )
{
	unsigned long ulProcessId = NULL;
	char * lpszInjectLibraryPath = NULL;
	HANDLE hToken;
	void * vpResult;
	void * lpPayloadLib = NULL;
	void * lpInjectProc = NULL;

	if( argc != 3 ) {
		PrintUsage();
		return 0;
	}
	ulProcessId = atoi( argv[1] );
	lpszInjectLibraryPath = argv[2];

	InitializeW64oWoW64();

	lpPayloadLib = LoadLibrary64A( "payload.dll" );

	lpInjectProc = GetProcAddress64( lpPayloadLib, "InjectLibrary" );

	X64Call( lpInjectProc, 2, (DWORD64)ulProcessId, 
		(DWORD64)lpszInjectLibraryPath );
	return 1;
}