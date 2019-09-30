#ifndef _MIOStream
#define _MIOStream
//-
// Copyright 2015 Autodesk, Inc. All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//+
//
// CLASS:    MIOStream
//
// ****************************************************************************

/*!
	The MIOStream.h header file was created to centralize the including of the
	iostream header.  
*/

#include <maya/MIOStreamFwd.h>

	#include <iostream>

	using std::cout;
	using std::cin;
	using std::cerr;
	using std::clog;

	using std::endl;
	using std::flush;
	using std::ws;

	using std::streampos;

	using std::iostream;
	using std::ostream;
	using std::istream;
	using std::ios;

#endif // _MIOStream
