#ifndef _MDeprecate
#define _MDeprecate
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
// ****************************************************************************
/*!
	\ingroup OpenMaya

	OpenMaya deprecation system

	Macros to allow code to be selectively removed from the build based on
	deprecation criteria.

	Here is a list of the currently supported deprecation flags. To build your
	plug-in to be compatible with deprecated functions removed just add the
	following defines to your compile flags:

	<ul>
		<li> _OPENMAYA_DEPRECATION_DISABLE_WARNING		: Silent any deprecation warning </li>
		<li> _OPENMAYA_DEPRECATION_ERROR_2019			: Treat call to deprecated methods from 2019 as error </li>
		<li> _OPENMAYA_DEPRECATION_ERROR_2018			: Treat call to deprecated methods from 2018 as error </li>
		<li> _OPENMAYA_DEPRECATION_ERROR_ALL			: Treat call to any deprecated methods as error </li>
	</ul>

	Defined attributes:
	<ul>
		<li> OPENMAYA_DEPRECATED(version, message)	: Methods and classes marked with this attributes are deprecated from "version", see "message" for replacement.
	</ul>

	Defined helper directives:
	<ul>
		<li> _OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING	: Disable the deprecation warning until the _OPENMAYA_POP_WARNING directive </li>
		<li> _OPENMAYA_POP_WARNING				: Restore the deprecation warning </li>
	</ul>

	If you are building with a customized compiler that does not have a proper support
	it is safe to disable or edit this header file.
*/
// ****************************************************************************


#if defined(_OPENMAYA_DEPRECATION_ERROR_ALL) || defined(_OPENMAYA_DEPRECATION_ERROR_2019)
#	define obsolete_2019	private
#else
#	define obsolete_2019	public
#endif

#if defined(_OPENMAYA_DEPRECATION_ERROR_ALL) || defined(_OPENMAYA_DEPRECATION_ERROR_2018)
#	define obsolete_2018	private
#else
#	define obsolete_2018	public
#endif

// Detect compiler support for function attributes, e.g. __declspec or __attribute__
#if defined(SWIG) || defined(DOXYGEN) || defined(_OPENMAYA_DEPRECATION_DISABLE_WARNING)
// SWIG and DOXYGEN does not support macro and attributes well enough, force them to ignore this
#else // For real compilers
#if defined(__clang__)			// Clang

    // Apple has different version numbers with normal LLVM
    // Apple Clang version 6.0 roughly equals LLVM Clang version 3.4+ (3.5 SVN)
	// Apple Clang version 4.0 roughly equals LLVM Clang version 2.8+ (2.9 SVN)
    #if defined(__apple_build_version__)
        #define _OPENMAYA_APPLE_CLANG_VER	(__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
    #else
        #define _OPENMAYA_LLVM_CLANG_VER	(__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
    #endif

    #if ((_OPENMAYA_LLVM_CLANG_VER >= 30400) || (_OPENMAYA_APPLE_CLANG_VER >= 60000)) && __has_feature(cxx_attributes)
		// Clang needs flag -Wno-c++1y-extensions, to build without extra warnings
        #pragma clang diagnostic ignored "-Wc++1y-extensions"
        #define _OPENMAYA_CPP_ATTRIBUTE
    #elif (_OPENMAYA_LLVM_CLANG_VER >= 20800) || (_OPENMAYA_APPLE_CLANG_VER >= 40000)
		#define _OPENMAYA_GNUC_ATTRIBUTE_SUPPORT_DEPRECATION_MESSAGE
		#define _OPENMAYA_GNUC_ATTRIBUTE
	#endif // Clang < 2.8

	#define _OPENMAYA_PUSH_WARNING _Pragma("clang diagnostic push")
	#define _OPENMAYA_DEPRECATION_DISABLE_WARNING_BELOW _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
	#define _OPENMAYA_POP_WARNING _Pragma("clang diagnostic pop")

#elif defined(__INTEL_COMPILER)	// ICC
	#if __INTEL_COMPILER >= 1600
		#define _OPENMAYA_CPP_ATTRIBUTE
	#elif __INTEL_COMPILER >= 1100
		#define _OPENMAYA_DECLSPEC_ATTRIBUTE
	#endif

	#define _OPENMAYA_PUSH_WARNING __pragma(warning push)
	#define _OPENMAYA_DEPRECATION_DISABLE_WARNING_BELOW __pragma(warning disable 1786) 
	#define _OPENMAYA_POP_WARNING __pragma(warning pop)

#elif defined(__GNUC__)			// GCC
	#define _OPENMAYA_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
	#if (_OPENMAYA_GCC_VERSION >= 50000) && (__cplusplus >= 201103L)
		#define _OPENMAYA_CPP_ATTRIBUTE
	#else
		// Deprecation message is supported after gcc 4.5
		#if (_OPENMAYA_GCC_VERSION >= 40500)
			#define _OPENMAYA_GNUC_ATTRIBUTE_SUPPORT_DEPRECATION_MESSAGE
		#endif
		#define _OPENMAYA_GNUC_ATTRIBUTE
	#endif 

	#if (_OPENMAYA_GCC_VERSION >= 40600)
		#define _OPENMAYA_PUSH_WARNING _Pragma("GCC diagnostic push")
		#define _OPENMAYA_DEPRECATION_DISABLE_WARNING_BELOW _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
		#define _OPENMAYA_POP_WARNING _Pragma("GCC diagnostic pop")
	#else
		// Note, There is no "push warning / pop warning" for GCC < 4.6
		// For maximized binary compatibility
		// We strongly suggest plugin writers to use the compiler listed in "VFX Reference Platform"
		#define _OPENMAYA_PUSH_WARNING
		#define _OPENMAYA_DEPRECATION_DISABLE_WARNING_BELOW _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
		#define _OPENMAYA_DEPRECATION_ENABLE_WARNING _Pragma("GCC diagnostic warning \"-Wdeprecated-declarations\"")
		#define _OPENMAYA_POP_WARNING
	#endif

#elif defined(_MSC_VER)			// MSVC
	// Visual Studio 2017+ : [[attribute]] cannot be applied to constructors in Visual Studio 2015
	#if _MSC_VER >= 1910
		#define _OPENMAYA_CPP_ATTRIBUTE
	#else
		#define _OPENMAYA_DECLSPEC_ATTRIBUTE
	#endif
	
	#define _OPENMAYA_PUSH_WARNING __pragma(warning(push))
	#define _OPENMAYA_DEPRECATION_DISABLE_WARNING_BELOW __pragma(warning(disable:4996)) 
	#define _OPENMAYA_POP_WARNING __pragma(warning(pop))
#else	// Unknown complier
#endif	// Compilers
#endif	// Swig and Doxygen

/*!
\brief Help on Open Maya deprecation system
## To treat deprecation as an error, use the following compiler 
Define preprocessor macro
	_OPENMAYA_DEPRECATION_ERROR_2018

Or use the following compiler switch:

- MSVC	: /We"4996" /D_CRT_NONSTDC_NO_DEPRECATE
- ICC	: -diag-error 1786
- Clang	: -Werror=deprecated-declarations
- GCC	: -Werror=deprecated-declarations

## To silence the deprecation warnings

Define preprocessor macro
	_OPENMAYA_DEPRECATION_DISABLE_WARNING

Or use the following compiler switch:

- MSVC	: /Wd"4996"
- ICC	: -diag-disable 1786
- Clang	: -Wno-error=deprecated-declarations
- GCC	: -Wno-error=deprecated-declarations
*/
#if defined(_OPENMAYA_CPP_ATTRIBUTE)
	#define OPENMAYA_DEPRECATED(version, message) [[deprecated(message)]]
#elif defined(_OPENMAYA_DECLSPEC_ATTRIBUTE)
	#define OPENMAYA_DEPRECATED(version, message) __declspec(deprecated(message))
#elif defined(_OPENMAYA_GNUC_ATTRIBUTE)
	#if defined(_OPENMAYA_GNUC_ATTRIBUTE_SUPPORT_DEPRECATION_MESSAGE)
		#define OPENMAYA_DEPRECATED(version, message) __attribute__((deprecated (message)))
	#else
		#define OPENMAYA_DEPRECATED(version, message) __attribute__((deprecated))
	#endif
#else
#define OPENMAYA_DEPRECATED(version, message)
#endif

// Fall back to empty if no available implementation
#if !defined(_OPENMAYA_DEPRECATION_DISABLE_WARNING_BELOW)
	#define _OPENMAYA_DEPRECATION_DISABLE_WARNING_BELOW
	#define _OPENMAYA_POP_WARNING
	#define _OPENMAYA_PUSH_WARNING
#endif

#define _OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING \
	_OPENMAYA_PUSH_WARNING \
	_OPENMAYA_DEPRECATION_DISABLE_WARNING_BELOW

#if !defined(_OPENMAYA_DEPRECATION_POP_WARNING)
#define _OPENMAYA_DEPRECATION_POP_WARNING _OPENMAYA_POP_WARNING
#endif

// ==================================================================
// Copyright 2018 Autodesk, Inc.  All rights reserved.
// 
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
#endif // _MDeprecate
