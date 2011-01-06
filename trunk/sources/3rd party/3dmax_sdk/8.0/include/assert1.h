/**********************************************************************
 *<
	FILE: assert1.h

	DESCRIPTION:

	CREATED BY: Dan Silva

	HISTORY:

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/
#if !defined(_MAX_ASSERT_H_)
#define _MAX_ASSERT_H_

#ifdef assert
#undef assert
#endif

// Enable this symbol in order to turn On evaluation of DbgAsserts in Release builds
//#define DBG_ASSERT_ACTIVE_IN_RELEASE_BUILD

#define assert( expr ) ( expr || assert1( /*#expr,*/ __LINE__, __FILE__ ) )

#define MaxAssert( expr ) ( (expr) || assert1( __LINE__, __FILE__ ) )

extern int UtilExport assert1( /*char *expr,*/ int line, char *file );

#if !defined(NDEBUG)
	#define DbgAssert( expr ) ( (expr) || assert1( __LINE__, __FILE__ ) )
	#define DbgVerify( expr ) ( (expr) || assert1( __LINE__, __FILE__ ) )
#elif defined(DBG_ASSERT_ACTIVE_IN_RELEASE_BUILD)
	#pragma message("=!= Debug Asserts are ON in Release builds =!=")
	#define DbgAssert( expr ) ( (expr) || assert1( __LINE__, __FILE__ ) )
	#define DbgVerify( expr ) ( (expr) || assert1( __LINE__, __FILE__ ) )
#else	
	#define DbgAssert( expr )
	#define DbgVerify( expr ) ( expr )
#endif	


//-----------------------------------------------------------------------------
//-- Assert callback mechanism. (GG:06/27/02)

#ifndef CASSERTCB_CLASS
#define CASSERTCB_CLASS

enum {
	ASSERTCB_IGNORE,		//-- Ignore the assert and continue
	ASSERTCB_CANCEL,		//-- Cancel the whole thing. Max will be brought down.
	ASSERTCB_CONTINUE		//-- Proceed with the normal assert code (dialogue and whatnot)
};

class CAssertCB {
	public:
		virtual	int Assert(int line, char* file)=0;
};

UtilExport CAssertCB* SetAssertCB(CAssertCB* cb);
#endif

#endif // _MAX_ASSERT_H_
