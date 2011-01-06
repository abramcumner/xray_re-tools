//**************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// Created by Dan Silva, 1994
//**************************************************************************/
 
// No sentry -- if we include this file, we may want to re-define assert for instance.
// #if !defined(_MAX_ASSERT_H_)
// #define _MAX_ASSERT_H_

#include "maxheap.h"

#ifndef __UTILEXPORT__H
#  include "utilexp.h"
#endif

#ifndef BUILDVER_H
#   include "buildver.h"        // Pull in ALPHABETA
#endif

#undef assert
#undef MaxAssert
#undef DbgVerify
#undef MaxAssert64
#undef DbgAssert64
#undef assert64

// Enable this symbol in order to turn On evaluation of DbgAsserts in Release builds
// We'll turn it on automatically for pre-release versions.

#undef DBG_ASSERT_ACTIVE_IN_RELEASE_BUILD
#undef REL_ASSERT_ACTIVE_IN_RELEASE_BUILD

// You can do a global override to always, always, always prevent 'release asserts'
// using /DMAX_DISABLE_RELEASE_ASSERT.
// 
// is: set CL=/DMAX_DISABLE_RELEASE_ASSERT
#ifndef MAX_DISABLE_RELEASE_ASSERT
#   ifdef ALPHABETA
#       define DBG_ASSERT_ACTIVE_IN_RELEASE_BUILD
#       define REL_ASSERT_ACTIVE_IN_RELEASE_BUILD
#   endif
#endif

// SR NOTE: Finally disabling asserts in release builds, unless you're asking for them
// by setting REL_ASSERT_ACTIVE_IN_RELEASE_BUILD.
// 
#if !defined( NDEBUG ) || defined( REL_ASSERT_ACTIVE_IN_RELEASE_BUILD )
#  define assert( expr ) ( expr || assert1( /*#expr,*/ __LINE__, __FILE__ ) )
#  define MaxAssert( expr ) ( (expr) || assert1( __LINE__, __FILE__ ) )
#else    
#  define assert( expr )      ((void *)0)
#  define MaxAssert( expr )   ((void *)0)
#endif   

extern UtilExport int assert1( /*char *expr,*/ int line, char *file );

#if !defined( NDEBUG ) || defined( DBG_ASSERT_ACTIVE_IN_RELEASE_BUILD )
   #define DbgAssert( expr ) ( (expr) || assert1( __LINE__, __FILE__ ) )
   #define DbgVerify( expr ) ( (expr) || assert1( __LINE__, __FILE__ ) )
#else 
   #define DbgAssert( expr )
   #define DbgVerify( expr ) ( expr )
#endif   

#if defined( _WIN64 )
// Verify that a condition holds only in x64 builds.
// typical usage would be DbgAssert64( size_t_value == static_cast<DWORD>(size_t_value) );
#  define DbgAssert64   DbgAssert
#  define MaxAssert64   MaxAssert
#  define assert64      assert
#else
#  define DbgAssert64( expr )   ((void*)0)
#  define MaxAssert64( expr )   ((void*)0)
#  define assert64( expr )      ((void*)0)
#endif

//-----------------------------------------------------------------------------
//-- Assert callback mechanism. (GG:06/27/02)

#ifndef CASSERTCB_CLASS
#define CASSERTCB_CLASS

enum {
   ASSERTCB_IGNORE,     //-- Ignore the assert and continue
   ASSERTCB_CANCEL,     //-- Cancel the whole thing. Max will be brought down.
   ASSERTCB_CONTINUE    //-- Proceed with the normal assert code (dialogue and whatnot)
};

class CAssertCB: public MaxHeapOperators {
   public:
      virtual  int Assert(int line, char* file)=0;
};

UtilExport CAssertCB* SetAssertCB(CAssertCB* cb);

#endif   // CASSERTCB_CLAS

// #endif // _MAX_ASSERT_H_
