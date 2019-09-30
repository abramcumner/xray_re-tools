#ifndef _MFnPhongEShader
#define _MFnPhongEShader
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
// CLASS:    MFnPhongEShader
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnDependencyNode.h>

// ****************************************************************************
// DECLARATIONS

class MFltVector;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnPhongEShader)

//! \ingroup OpenMaya MFn
//! \brief Manage phongE shaders.
/*!
 MFnPhongEShader facilitates creation and manipulation of dependency graph
 nodes representing phongE shaders.
*/
class OPENMAYA_EXPORT MFnPhongEShader : public MFnDependencyNode
{
	declareMFn( MFnPhongEShader, MFnDependencyNode );

public:
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	float       roughness( MStatus * ReturnStatus = NULL ) const;
	MStatus     setRoughness( const float &roughness );
	float       highlightSize( MStatus * ReturnStatus = NULL ) const;
	MStatus     setHighlightSize( const float & highlight_size );
	MColor      whiteness( MStatus * ReturnStatus = NULL ) const;
	MStatus     setWhiteness( const MColor & whiteness );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnPhongEShader, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnPhongEShader */
