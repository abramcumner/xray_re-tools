#ifndef _MNativeWindowHdl
#define _MNativeWindowHdl
//-
// Copyright 2015 Autodesk, Inc. All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//+
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>

// ****************************************************************************
// DECLARATIONS

/*! \file
    \brief Defines the native window handle type.
*/

#if defined(LINUX)
	typedef unsigned long XID;
	typedef XID Window;
	typedef Window	MNativeWindowHdl;
#elif defined(_WIN32)
	typedef HWND	MNativeWindowHdl;
#elif defined(OSMac_)
	//class NSView;
	typedef void * MNativeWindowHdl;
#elif defined(DOCS_ONLY)
	/*! \brief Native window handle type.

	  'type' is defined as follows on each supported platform:

	  \code
	  Linux			Window
	  OS X (64-bit)		void* (cast to NSView* before using)
	  Microsoft Windows	HWND
	  \endcode
	*/
	typedef type MNativeWindowHdl;
#else
	#error Unsupported platform.
#endif

#endif /* __cplusplus */
#endif /* _MNativeWindowHdl */
