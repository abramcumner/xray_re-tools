#ifndef _MCppCompat
#define _MCppCompat

// ===========================================================================
// Copyright 2017 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
// ****************************************************************************
//
// FILE:    MCppCompat.h
//
// ****************************************************************************
//
// DESCRIPTION (MCppCompat)
//
//      This file is a workaround to maintain compatibility for legacy C++ compilers
//      regarding c++11 keywords. These will be defined here.
//
//      These are the C++ standards and what value you should be able to expect in __cplusplus:
//          C++ pre-C++98: __cplusplus is 1.
//          C++98: __cplusplus is 199711L.
//          C++98 + TR1: This reads as C++98 and there is no way to check that I know of.
//          C++11: __cplusplus is 201103L.
//          C++14: __cplusplus is 201402L
//
// ****************************************************************************

#if defined __cplusplus

#if __cplusplus < 201103L
    // Includes: C++98, C++ pre-C++98, and hopefully C++98 + TR1
    // Keywords taken from : http://en.cppreference.com/w/cpp/keyword
    
	#ifndef nullptr
		#define nullptr NULL
	#endif // !nullptr

	#ifndef override
		#define override
	#endif // !override

	#ifndef final
		#define final
	#endif // !final

		// Other keywords of C++11, not being used in public header files:
		//		char16_t 
		//		char32_t     
		//		alignas
		//		alignof
		//		constexpr
		//		noexcept
		//		static_assert
		//		thread_local
		//		_Pragma
#endif

#if __cplusplus < 201402L
    // Includes: C++98, C++ pre-C++98, and hopefully C++98 + TR1, C++11
    // No new keywords. Table 4 (Keywords) in N3936 (C++14) is same as Table 4 in N3337 (C++11)
#endif



#endif /* __cplusplus */
#endif /* _MCppCompat */
