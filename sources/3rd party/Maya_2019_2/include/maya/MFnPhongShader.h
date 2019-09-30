#ifndef _MFnPhongShader
#define _MFnPhongShader
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
// CLASS:    MFnPhongShader
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
// CLASS DECLARATION (MFnPhongShader)

//! \ingroup OpenMaya MFn
//! \brief Manage Phong Shaders. 
/*!
 MFnPhongShader facilitates the creation and manipulation of dependency graph
 nodes representing Phong shaders.
*/
class OPENMAYA_EXPORT MFnPhongShader : public MFnReflectShader
{
	declareMFn( MFnPhongShader, MFnReflectShader );

public:
	MObject 	create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	float       cosPower( MStatus * ReturnStatus = NULL ) const;
	MStatus     setCosPower( const float& cos_power );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnPhongShader, MFnReflectShader );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnPhongShader */
