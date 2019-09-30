#ifndef _MStringResource
#define _MStringResource
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
// CLASS:    MStringResource
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>




OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MStringResource)

//! \ingroup OpenMaya
//! \brief Localizable string resources.
/*!
  The MStringResource class implements methods for registering and
  retrieving string values from externally defined string resources.
  This allows the plug-in to provide localized string resources.

  The lookup is done using MStringResourceId values.
*/
class OPENMAYA_EXPORT MStringResource
{

public:
				MStringResource();
	static const MString getString( const MStringResourceId &id,
									MStatus &status);
	static MStatus registerString( const MStringResourceId &id );
	        	~MStringResource();
protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MStringResource */
