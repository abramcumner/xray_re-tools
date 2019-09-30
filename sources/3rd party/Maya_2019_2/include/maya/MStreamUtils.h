#ifndef _MStreamUtils
#define _MStreamUtils
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MStreamUtils
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MIOStreamFwd.h>
#include <maya/MApiNamespace.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MStreamUtils)

//! \ingroup OpenMaya
//! \brief Stream functionality. 
/*!
This class provides some standard stream functionality for developers
working in C++ or script. Write methods are provided for writing to
ASCII or binary.  Read methods are only binary.
*/
class OPENMAYA_EXPORT MStreamUtils
{
public:

	static std::ostream& stdErrorStream();
	static std::ostream& stdOutStream();

	static MStatus writeChar( std::ostream& out, const char value, bool binary = false );
	static MStatus writeCharBuffer( std::ostream& out, const char* value, bool binary = false );
	static MStatus writeInt( std::ostream& out, const int value, bool binary = false );
	static MStatus writeFloat( std::ostream& out, const float value, bool binary = false );
	static MStatus writeDouble( std::ostream& out, const double value, bool binary = false );

	static MStatus readChar( std::istream& in, char& value, bool binary = false );
	static MStatus readCharBuffer( std::istream& in, char*& value, unsigned int length, bool binary = false );
	static MStatus readInt( std::istream& in, int& value, bool binary = false );
	static MStatus readFloat( std::istream& in, float& value, bool binary = false );
	static MStatus readDouble( std::istream& in, double& value, bool binary = false );

protected:
// No protected members
private:
// No protected members
};

OPENMAYA_NAMESPACE_CLOSE

#endif  // __cplusplus
#endif // _MStreamUtils
