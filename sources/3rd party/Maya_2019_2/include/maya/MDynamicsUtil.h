#ifndef _MDynamicsUtil

#define _MDynamicsUtil
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
// CLASS:    MDynamicsUtil
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MApiNamespace.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDynamicsUtil)

//! \ingroup OpenMayaFX
//! \brief Utility class for Maya dynamics 
/*!
	MDynamicsUtil provides utility methods related to Maya dynamics (eg.
	particles and fluids).
*/
class OPENMAYAFX_EXPORT MDynamicsUtil
{
public:

	static bool			hasValidDynamics2dTexture( const MObject& node,
								   const MObject& texAttr,
								   MStatus* status = NULL );
	static MStatus		evalDynamics2dTexture( const MObject& node,
									const MObject& texAttr,
									MDoubleArray& uCoords,
									MDoubleArray& vCoords,
									MVectorArray* resultColors,
									MDoubleArray* resultAlphas );
	static bool			inRunup();
	static bool			runupIfRequired();
	static bool			addNodeTypeToRunup(const MString& nodeTypeName);
	static bool			removeNodeTypeFromRunup(const MString& nodeTypeName);

protected:
// No protected members
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDynamicsUtil */
