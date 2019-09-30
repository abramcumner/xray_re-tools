#ifndef _MFStream
#define _MFStream
//-
// Copyright 2015 Autodesk, Inc. All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
//+
//
// CLASS:    MFStream
//
// ****************************************************************************

/*!
	The MFStream.h header file was created to centralize the including of the
	fstream header.  
*/

#include <maya/MIOStreamFwd.h>

	#include <fstream>
	using std::endl;
	using std::streambuf;
	using std::filebuf;
	using std::ifstream;
	using std::ofstream;
	using std::fstream;

#endif // _MFStream
