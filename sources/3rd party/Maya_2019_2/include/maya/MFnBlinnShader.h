#ifndef _MFnBlinnShader
#define _MFnBlinnShader
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
// CLASS:    MFnBlinnShader
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnReflectShader.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnBlinnShader)

//! \ingroup OpenMaya MFn
//! \brief Manage Blinn shaders.
/*!
 MFnBlinnShader facilitates creation and manipulation of dependency graph
 nodes representing Blinn shaders.
*/
class OPENMAYA_EXPORT MFnBlinnShader : public MFnReflectShader
{
	declareMFn( MFnBlinnShader, MFnReflectShader );

public:
	MObject 	create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	float       eccentricity( MStatus * ReturnStatus = NULL ) const;
	MStatus     setEccentricity( const float& eccentricity );
	float       specularRollOff( MStatus * ReturnStatus = NULL ) const;
	MStatus     setSpecularRollOff( const float& specular_rolloff );

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	declareMFnConstConstructor( MFnBlinnShader, MFnReflectShader );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnBlinnShader */
