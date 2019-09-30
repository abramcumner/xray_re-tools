#ifndef _MFnLayeredShader
#define _MFnLayeredShader
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
// CLASS:    MFnLayeredShader
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
// CLASS DECLARATION (MFnLayeredShader)

//! \ingroup OpenMaya MFn
//! \brief Manage Layered shaders. 
/*!
 MFnLayeredShader facilitates creation and manipulation of dependency graph
 nodes representing layered shaders.
*/
class OPENMAYA_EXPORT MFnLayeredShader : public MFnDependencyNode
{
	declareMFn( MFnLayeredShader, MFnDependencyNode );

public:
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	short		compositingFlag( MStatus * ReturnStatus = NULL ) const;
	MStatus     setCompositingFlag( const short & cflag );
	MColor      color( unsigned int lIndex = 0, MStatus * ReturnStatus = NULL ) const;
	MStatus     setColor( const MColor & col, unsigned int lIndex = 0 );
	MColor      transparency( unsigned int lIndex = 0, MStatus * ReturnStatus = NULL ) const;
	MStatus     setTransparency( const MColor & transp, unsigned int lIndex = 0 );
	MColor      glowColor( unsigned int lIndex = 0, MStatus * ReturnStatus = NULL ) const;
	MStatus     setGlowColor( const MColor & glow_color, unsigned int lIndex = 0 );
	MColor      hardwareColor( MStatus * ReturnStatus = NULL ) const;
	MStatus     setHardwareColor( const MColor & hardware_color );
	MColor       hardwareShader( MStatus * ReturnStatus = NULL ) const;
	MStatus     setHardwareShader( const MColor& hardware_shader );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnLayeredShader, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnLayeredShader */
