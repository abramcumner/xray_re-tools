#ifndef _MFnParticleSystem
#define _MFnParticleSystem
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
// CLASS:    MFnParticleSystem
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MFnDagNode.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnParticleSystem)

//! \ingroup OpenMayaFX MFn
//! \brief Class for obtaining information about a particle system. 
/*!
	Particle object access class.

	\li <b>isValid()</b>: data is valid (predicate)
	\li <b>renderType()</b>: object render type
	\li <b>count()</b>: particle count at current frame


	Use this chart to determine which methods to call based on the
	render type of the particle object:

	<ul>
	<li> Valid Methods for render type kCloud:
		<ul>
		<li> <b>position()</b>: particle position
		<li> <b>radius()</b>:	particle radius
		<li> <b>surfaceShading()</b>: object surface shading value
		<li> <b>betterIllum()</b>: invoke thick cloud sampling
		<li> <b>threshold()</b>: object threshold
		<li> <b>disableCloudAxis()</b>: for internal use; do not call
		<li> <b>flatShaded()</b>: cloud is flat shaded in modeling view
		</ul>

	<li> Valid Methods for render type kTube:
		<ul>
		<li> <b>position0()</b>: particle start position
		<li> <b>position1()</b>: particle end position
		<li> <b>radius0()</b>: particle start radius
		<li> <b>radius1()</b>: particle end radius
		<li> <b>tailSize()</b>: length scale factor
		</ul>

	<li> Valid Methods for render type kBlobby:
		<ul>
		<li> <b>position()</b>: particle position
		<li> <b>radius()</b>:	particle radius
		<li> <b>threshold()</b>: object threshold
		</ul>

	<li> Valid Methods for render type kHardware:
		<ul>
		<li> <b>position()</b>: particle position
		<li> <b>radius()</b>:	particle radius
		</ul>
    </ul>
*/
class OPENMAYAFX_EXPORT MFnParticleSystem : public MFnDagNode
{
    declareDagMFn(MFnParticleSystem, MFnDagNode);

public:
	//! Ways in which particles can be rendered.
	enum RenderType
	{
		kCloud,		//!< \nop
		kTube,		//!< \nop
		kBlobby,	//!< \nop
		kMultiPoint,	//!< \nop
		kMultiStreak,	//!< \nop
		kNumeric,	//!< \nop
		kPoints,	//!< \nop
		kSpheres,	//!< \nop
		kSprites,	//!< \nop
		kStreak		//!< \nop
	};

	MObject			create                  ( MStatus* status = NULL );
	MObject			create                  ( MObject parent, MStatus* status = NULL );
	MStatus                 emit                    ( const MPoint& position );
	MStatus                 emit                    ( const MPointArray& positionArray );
	MStatus                 emit                    ( const MPoint& position, const MVector& velocity );
	MStatus                 emit                    ( const MPointArray& positionArray, const MVectorArray& velocityArray );
	MStatus                 saveInitialState        () const;
	void			evaluateDynamics        ( MTime &to, bool runupFromStart );
	bool			isValid			() const;
	MString			particleName	        () const;
	unsigned int	        count			() const;
	void	                setCount		( unsigned int ) const;
	RenderType		renderType		() const;
	void			position		( MVectorArray& ) const;
	void			velocity		( MVectorArray& ) const;
	void			acceleration		( MVectorArray& ) const;
	void			position0		( MVectorArray& ) const;
	void			position1		( MVectorArray& ) const;
	void			radius			( MDoubleArray& ) const;
	void			radius0			( MDoubleArray& ) const;
	void			radius1			( MDoubleArray& ) const;
	double			surfaceShading	        () const;
	double			threshold		() const;
	bool			betterIllum		() const;
	bool			flatShaded		() const;
	bool			disableCloudAxis        () const;
	double			tailSize		() const;
	void			particleIds		( MIntArray& ) const;
	void			age			( MDoubleArray& ) const;
	void			lifespan		( MDoubleArray& ) const;
	void			rgb			( MVectorArray& ) const;
	void			opacity			( MDoubleArray& ) const;
	void			mass			( MDoubleArray& ) const;
	void			emission		( MVectorArray& ) const;
	bool			hasLifespan		() const;
	bool			hasRgb			() const;
	bool			hasOpacity		() const;
	bool			hasEmission		() const;
	bool			primaryVisibility	() const;
	bool			visibleInReflections    () const;
	bool			visibleInRefractions    () const;
	bool			castsShadows		() const;
	bool			receiveShadows		() const;
	unsigned int            getPerParticleAttribute ( const MString &attrName, MIntArray&, MStatus* status = NULL ) const;
	unsigned int            getPerParticleAttribute ( const MString &attrName, MVectorArray&, MStatus* status = NULL ) const;
	unsigned int            getPerParticleAttribute ( const MString &attrName, MDoubleArray&, MStatus* status = NULL ) const;
	void                    setPerParticleAttribute ( const MString &attrName, MVectorArray&, MStatus* status = NULL ) const;
	void                    setPerParticleAttribute ( const MString &attrName, MDoubleArray&, MStatus* status = NULL ) const;
	bool                    isPerParticleIntAttribute ( const MString &attrName, MStatus* status = NULL ) const;
	bool                    isPerParticleDoubleAttribute ( const MString &attrName, MStatus* status = NULL ) const;
	bool                    isPerParticleVectorAttribute ( const MString &attrName, MStatus* status = NULL ) const;
	bool                    isDeformedParticleShape ( MStatus* status = NULL ) const;
	MObject                 deformedParticleShape   ( MStatus* status = NULL ) const;
	MObject                 originalParticleShape   ( MStatus* status = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnParticleSystem, MFnDagNode );

END_NO_SCRIPT_SUPPORT:

protected:

	virtual bool	objectChanged( MFn::Type, MStatus * );

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnParticleSystem */
