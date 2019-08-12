/*---------------------------------------------------------------\
| OS.H                                                           |
| Author: Ateeq Sharfuddin                                       |
| Last Revised: 2002.2.09                                        |
| -------------------------------------------------------------- |
| Description: os headers                                        |
| -------------------------------------------------------------- |
| Reproduction or distribution of this code or the binaries      |
| without consent is prohibited.                                 |
| -------------------------------------------------------------- |
| Copyright (c) 2002.                                            |
\---------------------------------------------------------------*/

#ifndef __OS_H__
#define __OS_H__

#if _MSC_VER >= 1020
   #pragma once
#endif // _MSC_VER >= 1020

/*
strict typing
and make compilations faster
*/
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOIME
#define NOMCX
#define NOPROFILER
#define NOHELP
#define NOKANJI
#define NOCOMM
#define NOWH
#define NOWINOFFSETS
#define NODEFERWINDOWPOS
#define NOTEXTMETRIC
#define NOSERVICE
#define NOOPENFILE
#define NOMINMAX
#define NOMETAFILE
#define NOMEMMGR
#define NONLS
#define NODRAWTEXT
#define NOCLIPBOARD
#define NORASTEROPS
#define NOSYSCOMMANDS
#define NOGDICAPMASKS

#include <winsock2.h>
#include <winnt.h>
#include <mswsock.h>
#include <wtypes.h>
#include <shellapi.h>
#include <objidl.h>
#include <tchar.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <crtdbg.h>

/*
 goto Statements that provide the functionality
   of try{}catch(...){} w/o the overhead.
*/
#define FREE(x) if ((x)) { free(x); x = NULL; }
#define HRC(x) if (FAILED(hr = x)) { goto Cleanup; }
#define HRCEX(x,y) if (!(x)) { hr = y; goto Cleanup; } hr = S_OK;
#define HRCMEM(x) if (!(x)) { hr = E_OUTOFMEMORY; goto Cleanup; } hr = S_OK;
#define RETURN(x) hr = x; goto Cleanup;

#ifdef NDEBUG
	#define RETURNHR(hr) return hr;
#else /* !NDEBUG */

#define RETURNHR(hr)															\
	if (FAILED(hr))																\
	{																			\
		LPTSTR lpBuf=0;															\
		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER	|						\
						FORMAT_MESSAGE_FROM_SYSTEM		|						\
						FORMAT_MESSAGE_IGNORE_INSERTS,							\
						NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
						lpBuf,													\
						0,NULL);												\
		MessageBox( NULL, lpBuf, _T("TicTacToe"), MB_OK | MB_ICONEXCLAMATION );	\
		LocalFree( lpBuf );														\
		return S_FALSE;															\
	}																			\
	return hr;

#endif /* NDEBUG */

#endif /* __OS_H__ */