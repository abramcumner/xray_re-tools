#ifndef _MCommonSystemUtils
#define _MCommonSystemUtils
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
// CLASS:    MCommonSystemUtils
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MCommonSystemUtils)

//! \ingroup OpenMaya
//! \brief OS functionality.  
/*!

 Methods for getting and setting environment variables, make
 directories etc.

  This class provides some standard Operating System functionality that
  is available from MEL.  These methods can be called instead of
  invoking MEL through the MGlobal class.

  NOTE: These methods are provided as a convenience.  It would be more
  efficient to call directly into the operating system than to use
  the methods of this class.
*/
class OPENMAYA_EXPORT MCommonSystemUtils
{
public:
	static MString getEnv( const MString &variable, MStatus *ResultStatus = NULL);
	static MStatus putEnv( const MString &variable, const MString &value );
	static MStatus makeDirectory( const MString &path );

BEGIN_NO_SCRIPT_SUPPORT:
	//!		NO SCRIPT SUPPORT
	static MStatus getEnv( const MString &variable, MString &result );
END_NO_SCRIPT_SUPPORT:

	static const char* 	className();

protected:
// No protected members

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MCommonSystemUtils */
