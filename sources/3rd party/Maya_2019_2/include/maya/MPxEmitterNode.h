#ifndef _MPxEmitterNode
#define _MPxEmitterNode
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
// CLASS:    MPxEmitterNode
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MPxEmitterNode)
//
// MPxEmitterNode allows the creation and manipulation of dependency graph
// nodes representing emitters.  This is the top level of a hierarchy of
// emitter node function sets.  It permits manipulation of the attributes
// common to all types of emitters.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/M3dView.h>

#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MTime.h>
#include <maya/MBoundingBox.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxEmitterNode)

//! \ingroup OpenMayaFX MPx
//! \brief Base class for user defined particle emitters
/*!
  MPxEmitterNode allows the creation and manipulation of dependency
  graph nodes representing particle emitters.
*/
class OPENMAYAFX_EXPORT MPxEmitterNode : public MPxNode
{
public:
	MPxEmitterNode();
	virtual ~MPxEmitterNode();
	virtual MPxNode::Type	type() const;
	virtual MStatus			compute(const MPlug& plug, MDataBlock& dataBlock);


	virtual void    draw(  M3dView  & view, const  MDagPath  & path,  M3dView::DisplayStyle  style, M3dView:: DisplayStatus );

	// ************************************************************
	// Inherited attributes
	// ************************************************************

	// General attributes for every emitter.
	//
	//! emitter type attribute
	static MObject 	mEmitterType;

	//! Types of emitters.
	enum MEmitterType
	{
		kDirectional = 0,	//!< \nop
		kOmni,			//!< \nop
		kSurface,		//!< \nop
		kCurve,			//!< \nop
		kVolume			//!< \nop
	};

	//! rate attribute
	static MObject	mRate;
	//! speed attribute
	static MObject	mSpeed;
	//! direction attribute
	static MObject	mDirection;
	//! X component of mDirection
	static MObject	mDirectionX;
	//! Y component of mDirection
	static MObject	mDirectionY;
	//! Z component of mDirection
	static MObject	mDirectionZ;

	// Attributes for ownership.
	// Connections made with the owner object.
	//
	//! owner postion attribute, multi
	static MObject	mOwnerPosData;
	//! owner velocity attribute, multi
	static MObject	mOwnerVelData;
	//! owner centroid attribute
	static MObject	mOwnerCentroid;
	//! X component of mOwnerCentroid
	static MObject	mOwnerCentroidX;
	//! Y component of mOwnerCentroid
	static MObject	mOwnerCentroidY;
	//! Z component of mOwnerCentroid
	static MObject	mOwnerCentroidZ;

	//! input swept geometry attribute
	static MObject	mSweptGeometry;

	//! world matrix attribute
	static MObject	mWorldMatrix;

	// Attributes for target particle object.
	// Connections with the target particle object.
	//
	//! start time attribute
	static MObject	mStartTime;
	//! delta time attribute
	static MObject	mDeltaTime;
	//! flag for maximum count attribute
	static MObject	mIsFull;
	//! inherit factor attribute
	static MObject	mInheritFactor;
	//! random seed attribute
	static MObject	mSeed;
	//! random state attribute
	static MObject  mRandState;
	static MObject  mRandStateX;
	static MObject  mRandStateY;
	static MObject  mRandStateZ;

	// Current time attribute.
	// Connection with time node.
	//
	//! current time attribute
	static MObject	mCurrentTime;

	// Output Attribute.
	// Connection with the target particle object.
	//
	//! output particle attribute, multi
	static MObject mOutput;

	//! min distance attribute
	static MObject mMinDistance;

	//! max distance attribute
	static MObject mMaxDistance;

	//
	MEmitterType getEmitterType( MDataBlock& block );
	MObject	getOwnerShape();

	MTime	getCurrentTime( MDataBlock& block );
	MTime	getStartTime( int plugIndex, MDataBlock& block );
	MTime	getDeltaTime( int plugIndex, MDataBlock& block );

	int		getRandomSeed( int plugIndex, MDataBlock& block );
	void	getRandomState( int plugIndex, MDataBlock& block );
	void	setRandomState( int plugIndex, MDataBlock& block );
	void	resetRandomState( int plugIndex, MDataBlock& block );
	double	randgen();

	double 	getRate( MDataBlock& block );
	double	getMinDistance( MDataBlock& block );
	double	getMaxDistance( MDataBlock& block );

	MPoint	getWorldPosition();
	MMatrix getWorldMatrix();

	bool	volumePrimitiveBoundingBox( MBoundingBox& box );
	bool	volumePrimitivePointInside( const MPoint& worldPoint, const MMatrix& emitterWorldMatrix );
	double 	volumePrimitiveDistanceFromAxis( const MPoint& worldPoint, const MMatrix& emitterWorldMatrix );

	bool	hasValidEmission2dTexture( const MObject& texAttr, MStatus *status = NULL );
	MStatus	evalEmission2dTexture( const MObject& texAttr,
							 MDoubleArray& uCoords,
							 MDoubleArray& vCoords,
							 MVectorArray* resultColors,
							 MDoubleArray* resultAlphas );

	static const char*	className();

protected:

	int		randState[3];

private:

	static void			initialSetup();

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxEmitterNode */

