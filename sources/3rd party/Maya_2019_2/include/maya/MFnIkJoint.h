#ifndef _MFnIkJoint
#define _MFnIkJoint
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
// CLASS:    MFnIkJoint
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnTransform.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnIkJoint)

//! \ingroup OpenMayaAnim MFn
//! \brief Function set for joints 
/*!
	This is the function set for joints.

	The transformation matrix for a joint node is below.


	\li matrix = [S] * [RO] * [R] * [JO] * [IS] * [T]


	(where '*' denotes matrix multiplication).

	These matrices are defined as follows:


	\li <b>[S]</b> : scale
	\li <b>[RO]</b> : rotateOrient (attribute name is rotateAxis)
	\li <b>[R]</b> : rotate
	\li <b>[JO]</b> : jointOrient
	\li <b>[IS]</b> : parentScaleInverse
	\li <b>[T]</b> : translate


	The methods to get the value of these matrices are:


	\li <b>[S]</b> : getScale
	\li <b>[RO]</b> : getScaleOrientation
	\li <b>[R]</b> : getRotation
	\li <b>[JO]</b> : getOrientation
	\li <b>[IS]</b> : (the inverse of the getScale on the parent transformation matrix)
	\li <b>[T]</b> : translation
*/
class OPENMAYAANIM_EXPORT MFnIkJoint : public MFnTransform
{
	declareDagMFn( MFnIkJoint, MFn::kJoint );

public:
	enum Axis {
		kXAxis,
		kYAxis,
		kZAxis,
		kNone };
	MObject create( MObject parent = MObject::kNullObj,
					MStatus * ReturnStatus = NULL );
	MStatus getOrientation( MQuaternion &quaternion ) const;
	MStatus setOrientation( const MQuaternion &quaternion );
	MStatus getOrientation( MEulerRotation &rotation ) const;
	MStatus setOrientation( const MEulerRotation &rotation );
	MStatus getOrientation( double rotation[3],
							MTransformationMatrix::RotationOrder &order )
		                    const;
	MStatus setOrientation( const double rotation[3],
							MTransformationMatrix::RotationOrder order );
	MStatus getScaleOrientation	( MQuaternion &quaternion ) const;
	MStatus setScaleOrientation	( const MQuaternion &quaternion );
	MStatus getScaleOrientation	( double rotation[3],
								  MTransformationMatrix::RotationOrder &order ) const;
	MStatus setScaleOrientation	( const double rotation[3],
								  MTransformationMatrix::RotationOrder order );
	MStatus getSegmentScale		( double scale[3] ) const;
	MStatus setSegmentScale		( const double scale[3] );
	MStatus getStiffness		( double stiffness[3] ) const;
	MStatus setStiffness		( const double stiffness[3] );
	MStatus getPreferredAngle	( double rotation[3] ) const;
	MStatus setPreferredAngle	( const double rotation[3] );
	MStatus getDegreesOfFreedom	( bool &freeInX, bool &freeInY,
								  bool &freeInZ ) const;
	MStatus setDegreesOfFreedom	( bool freeInX, bool freeInY,
								  bool freeInZ );
	double  minRotateDampXRange ( MStatus * ReturnStatus = NULL ) const;
	double  minRotateDampYRange ( MStatus * ReturnStatus = NULL ) const;
	double  minRotateDampZRange ( MStatus * ReturnStatus = NULL ) const;
	double  maxRotateDampXRange ( MStatus * ReturnStatus = NULL ) const;
	double  maxRotateDampYRange ( MStatus * ReturnStatus = NULL ) const;
	double  maxRotateDampZRange ( MStatus * ReturnStatus = NULL ) const;
	MStatus setMinRotateDampXRange( double angle );
	MStatus setMinRotateDampYRange( double angle );
	MStatus setMinRotateDampZRange( double angle );
	MStatus setMaxRotateDampXRange( double angle );
	MStatus setMaxRotateDampYRange( double angle );
	MStatus setMaxRotateDampZRange( double angle );
	double  minRotateDampXStrength( MStatus * ReturnStatus = NULL ) const;
	double  minRotateDampYStrength( MStatus * ReturnStatus = NULL ) const;
	double  minRotateDampZStrength( MStatus * ReturnStatus = NULL ) const;
	double  maxRotateDampXStrength( MStatus * ReturnStatus = NULL ) const;
	double  maxRotateDampYStrength( MStatus * ReturnStatus = NULL ) const;
	double  maxRotateDampZStrength( MStatus * ReturnStatus = NULL ) const;
	MStatus setMinRotateDampXStrength( double angle );
	MStatus setMinRotateDampYStrength( double angle );
	MStatus setMinRotateDampZStrength( double angle );
	MStatus setMaxRotateDampXStrength( double angle );
	MStatus setMaxRotateDampYStrength( double angle );
	MStatus setMaxRotateDampZStrength( double angle );
	MString hikJointName( MStatus * ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnIkJoint, MFn::kJoint );

	//!     NO SCRIPT SUPPORT
	MStatus getHikJointName( MString& name ) const;

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members

public:
	//! Obsolete - use correctly spelled getPreferredAngle
	MStatus getPreferedAngle( double rotation[3] ) const { return getPreferredAngle( rotation ); }
	//! Obsolete - use correctly spelled setPreferredAngle
	MStatus setPreferedAngle( const double rotation[3] ) { return setPreferredAngle( rotation ); }
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MFnIkJoint */
