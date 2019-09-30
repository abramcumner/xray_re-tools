#ifndef _MFnMotionPath
#define _MFnMotionPath
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
// CLASS:    MFnMotionPath
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnMotionPath)

//! \ingroup OpenMayaAnim MFn
//! \brief Motion path animation function set 
/*!

  This class is used for constructing and manipulating motion path
  animation.

  Motion path animation requires a curve (or surface) and one or more
  other objects.  During the animation, the objects will be moved
  along the curve.

  Setting "follow" for the motion path aligns the object(s) local axis
  to the tangent of the motion path. Banking can also be applied to
  objects.

  Motion path markers are points along the path where the orientation
  and position for the object(s) travelling along the path can be
  specified.
*/
class OPENMAYAANIM_EXPORT MFnMotionPath : public MFnDependencyNode
{

	declareMFn(MFnMotionPath, MFnDependencyNode);

public:
	//! Available axes.
	enum Axis {
		kXaxis,		//!< \nop
		kYaxis,		//!< \nop
		kZaxis		//!< \nop
	};
	MObject		create( const MDagPath & pathObject,
						const MDagPath & objectToAnimate,
						MTime & timeStart, MTime & timeEnd,
						MDGModifier * modifier = NULL,
						MStatus * ReturnStatus = NULL );
	MStatus		setPathObject( const MDagPath & pathObject,
								 MDGModifier * modifier = NULL );
	MDagPath	pathObject( MStatus * ReturnStatus = NULL );
	MStatus		addAnimatedObject( const MDagPath & objectToAnimate,
									MDGModifier * modifier = NULL );
	MStatus		getAnimatedObjects( MDagPathArray & array );
	MStatus		setFollow( bool on, MDGModifier * modifier = NULL );
	bool		follow( MStatus * ReturnStatus = NULL ) const;
	MStatus		setFollowAxis( Axis axis );
	Axis		followAxis( MStatus * ReturnStatus = NULL );
	MStatus		setUpAxis( Axis axis );
	Axis		upAxis( MStatus * ReturnStatus = NULL );
	MStatus		setBank( bool bank );
	bool		bank( MStatus * ReturnStatus  = NULL) const;
	MStatus		setBankScale( double bankScale );
	double		bankScale( MStatus * ReturnStatus = NULL );
	MStatus		setBankThreshold( double bankThreshold );
	double		bankThreshold( MStatus * ReturnStatus = NULL );
	MStatus		setUseNormal( bool use );
	bool		useNormal( MStatus * ReturnStatus = NULL );
	MStatus		setInverseNormal( bool invert );
	bool		inverseNormal( MStatus * ReturnStatus = NULL );
	MStatus		setUStart( double start );
	MStatus		setUEnd( double end );

	double		uStart( MStatus * ReturnStatus = NULL );
	double		uEnd( MStatus * ReturnStatus = NULL );
	MStatus		setUTimeStart( MTime & start );
	MStatus		setUTimeEnd( MTime & end );
	MTime		uTimeStart( MStatus * ReturnStatus = NULL );
	MTime		uTimeEnd( MStatus * ReturnStatus = NULL );
	unsigned int	numPositionMarkers( MStatus * ReturnStatus = NULL );
	MObject		getPositionMarker( unsigned int, MStatus * ReturnStatus = NULL );
	unsigned int	numOrientationMarkers( MStatus * ReturnStatus = NULL );
	MObject		getOrientationMarker( unsigned int, MStatus * ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnMotionPath, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
    void *         getCurve();
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnMotionPath */
