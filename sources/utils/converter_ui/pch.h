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

#include <malloc.h>
#include "debug_macros.h"

const char CONVERTER_INI[] = "converter.ini";
#pragma managed

#pragma warning(disable:4127)
#pragma warning(disable:4100)

// do not forget to call
// 'cs_free'
// on the block of memory being returned
inline char* to_string(System::String ^string) {
	// Pin memory so GC can't move it while native function is called
	pin_ptr<const wchar_t> wch = PtrToStringChars(string);

	size_t	convertedChars = 0;
	size_t	sizeInBytes = (string->Length + 1) * 2;
	errno_t	err = 0;
	char*	result = (char*) malloc(sizeInBytes);

	err = wcstombs_s(&convertedChars, result, sizeInBytes, wch, sizeInBytes);

	if (err)
		VERIFY(!"[tostring][failed] : wcstombs_s failed");

	return result;
}

inline System::String^ to_string(const char* string) {
	return gcnew System::String(string);
}

#endif // PCH_H