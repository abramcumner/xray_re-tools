#ifndef adskCommon_h
#define adskCommon_h

//----------------------------------------------------------------------
//
// This file contains some simple definitions that are of potential
// use in any of this component's files.
//
//----------------------------------------------------------------------

// Library import/export declarations

// Define 64-bit types and class exports in a platform-independent way
#if WIN32
	typedef long long int64_t;
	typedef unsigned long long uint64_t;

#	if defined( METADATA_DLL )
#		define METADATA_EXPORT _declspec( dllexport )
#	else
#		define METADATA_EXPORT _declspec( dllimport )
#	endif

#else // WIN32

#	include <stdint.h>
#	define METADATA_EXPORT __attribute__ ((visibility("default")))

#endif // WIN32

// Define the 2-member macro merge in a platform-independent way
//   i.e. name2(A,B) is preprocessed into AB
#ifndef name2
#	if defined(__STDC__) || defined(__ANSI_CPP__)
#		define name2(a,b)		_name2_aux(a,b)
#		define _name2_aux(a,b)	a##b
#	else
#		define name2(a,b)		a/**/b
#	endif
#endif

//=========================================================
//-
// Copyright  2012  Autodesk,  Inc.  All  rights  reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================
#endif // adskCommon_h
