////////////////////////////////////////////////////////////////////////////
//	Module 		: pch.h
//	Created 	: 23.01.2008
//  Modified 	: 25.02.2015
//	Author		: Dmitriy Iassenev
//	Description : preocmpiled header creator for editor controls library
////////////////////////////////////////////////////////////////////////////
#ifndef PCH_H
#define PCH_H

#include <stdlib.h>
#include <vcclr.h>
#include <stdio.h>

#pragma unmanaged
typedef unsigned int u32;

typedef char*		LPSTR;
typedef char const* LPCSTR;

#include <malloc.h>
#include "debug_macros.h"

const char BUILD_DATE[] = __DATE__;
const char DEFAULT_FS_SPEC[] = "fsconverter.ltx";
#pragma managed

#pragma warning(disable:4127)
#pragma warning(disable:4100)

// do not forget to call
// 'cs_free'
// on the block of memory being returned
inline LPSTR to_string(System::String ^string) {
	// Pin memory so GC can't move it while native function is called
	pin_ptr<const wchar_t> wch = PtrToStringChars(string);

	size_t	convertedChars = 0;
	size_t	sizeInBytes = (string->Length + 1) * 2;
	errno_t	err = 0;
	LPSTR	result = (LPSTR) malloc(sizeInBytes);

	err = wcstombs_s(&convertedChars, result, sizeInBytes, wch, sizeInBytes);

	if (err)
		VERIFY(!"[tostring][failed] : wcstombs_s failed");

	return result;
}

inline System::String^ to_string(LPCSTR string) {
	return gcnew System::String(string);
}

#endif // PCH_H