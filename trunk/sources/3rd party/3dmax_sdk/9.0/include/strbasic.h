
/*******************************************************************
 *
 *    DESCRIPTION:      Basic string definitions header file
 *
 *    AUTHOR:           Tom Hudson
 *
 *    HISTORY:          File created 9/6/94
 *
 *******************************************************************/

#ifndef _STRBASICS_
#define _STRBASICS_

#define WIN95STUFF

// To set up Max to use Unicode, define _UNICODE, and don't define _MBCS
// To set up Max to use multi-byte character sets, define _MBCS and 
//              don't define _UNICODE
// To set up Max to use single-byte ANSI character strings, don't define
//              either _UNICODE or _MBCS

// #define _UNICODE     // turn on Unicode support

#ifndef _MBCS
#define _MBCS   // if Unicode is off, turn on multi-byte character support
#endif


#ifdef _UNICODE

#ifdef _MBCS
#undef _MBCS    // can't have both Unicode and MBCS at once -- Unicode wins
#endif

#undef UNICODE
#define UNICODE

#undef STRCONST
#define STRCONST L

//#define RWSTR RWWString
//#define RWTOKENIZER RWWTokenizer
// Here's a macro to get a const char * from a RWWString object -- It
// temporarily constructs a RWCString object to hold the 1-byte wide
// character string output by the toAcsii() operator.  Don't store
// this pointer!  Copy it to a new allocation, because it might go
// away.
#undef NARROW
#define NARROW(s) ((const char *)((s).toAscii()))

#else

//#define RWSTR RWCString
//#define RWTOKENIZER RWCTokenizer
//#define NARROW(s) (s)

#endif

// Bring in the generic international text header file
#include <tchar.h>

// Starting with VS2005 the default is to have wchar_t defined as a type, so that's
// what we'll use as well (plus it coincides with the VS2005 Max9 release)
typedef __wchar_t       mwchar_t;

//
// MAX is not compiled in UNICODE (yet).  TCHAR has been changed to MCHAR where
// appropriate, so you can compile in UNICODE or not, and not have Max's headers
// change.
// 

#define MCHAR           char
#define LPCMSTR         const MCHAR*
#define _M(s)           (s)

#endif // _STRBASICS_
