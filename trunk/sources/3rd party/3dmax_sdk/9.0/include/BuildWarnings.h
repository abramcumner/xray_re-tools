/*****************************************************************************
* FILE:              BuildWarnings.h
* DESCRIPTION: Enables\Disables build warnings
* CREATED BY:     Attila Szabo
* HISTORY:  
* - 2003.Oct.14 - created 
******************************************************************************
* Copyright (c) 2003 Autodesk, All Rights Reserved.
******************************************************************************/

#ifndef _BUILD_WARNINGS_H_
#define _BUILD_WARNINGS_H_

//*****************************************************************************
// WARNINGS
//*****************************************************************************
// - Enabling certain level 4 warnings can help to automatically detect defects in the code. 
// - To disable warning number N in your code (only if absolutely necessary!), 
// You can do the following:
// #pragma warning( push )
// #pragma warning( disable : N )
// void YourFunction() {
// }
// #pragma warning( pop )
//
// - The following level 4 warnings have been promoted to level 3:
// + C4130 - 'operator' : logical operation on address of string constant
#pragma warning (3 : 4130 )

//*****************************************************************************
// COMPILER SPECIFIC DEFINES
//*****************************************************************************
#if (_MSC_VER >= 1300)  // Visual Studio .NET

// SS 6/7/2002: globally turning off certain warnings we now receive from
// the VC7 compiler. This may only be temporary.
// NH 02/12/03 Added this to R6 but I only kept the first pragma.  This was to get 
// a nicer looking build and so we could concentrate on the easier fixes.  
// We may want to address this again
// Temporarily re-enabled the other two pragmas to help find bigger problems in the
// build logs, 020319  --prs.

// Conversion from XXX to YYY, possible loss of data
#pragma warning(disable: 4244)

// Signed/unsigned mismatch (for ==, >, etc.)
#pragma warning(disable: 4018) 

#endif

// Macro used to shield us from changes to the syntax of the deprecated keyword, 
// and to make the deprecated declarations more succinct
#define MAX_DEPRECATED __declspec(deprecated)

#endif // _BUILD_WARNINGS_H_

