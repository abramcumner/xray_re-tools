#ifndef _MPxFieldNode
#define _MPxFieldNode
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
// CLASS:    MPxFieldNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/M3dView.h>

#if defined(OSMac_MachO_)
#	include <OpenGL/gl.h>
#else
#	include <GL/gl.h>
#endif



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxFieldNode)

//! \ingroup OpenMayaFX MPx
//! \brief Base class for user defined fields
/*!
 MPxFieldNode allows the creation and manipulation of dependency graph nodes
 representing fields.  This is the top level of a hierarchy of field
 node function sets.  It permits manipulation of the attributes common to
 all types of fields.
*/
class OPENMAYAFX_EXPORT MPxFieldNode : public MPxNode
{
public:
	MPxFieldNode();
	virtual ~MPxFieldNode();
	virtual MPxNode::Type type() const;
	virtual MStatus		compute( const MPlug& plug, MDataBlock& dataBlock );
    virtual MStatus getForceAtPoint(const MVectorArray&  point,
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
                            double deltaTime);
	virtual MStatus iconSizeAndOrigin(	GLuint& width,
						GLuint& height,
						GLuint& xbo,
						GLuint& ybo   );
	virtual MStatus iconBitmap(GLubyte* bitmap);

	virtual void    draw(  M3dView  & view, const  MDagPath  & path,  M3dView::DisplayStyle  style, M3dView:: DisplayStatus );

	virtual double  falloffCurve(const double param, MStatus *ReturnStatus = NULL);
	virtual bool    isFalloffCurveConstantOne(MStatus *ReturnStatus = NULL);

	// ************************************************************
	// Inherited attributes
	// ************************************************************

	// General attributes for every field.
	//
	//! magnitude attribute
	static MObject	mMagnitude;
	//! attenuation attribute
	static MObject	mAttenuation;
	//! maximum distance attribute
	static MObject	mMaxDistance;
	//! flag for maximum distance attribute
	static MObject	mUseMaxDistance;
	//! flag for per vertex attribute
	static MObject	mApplyPerVertex;

	// Attributes for input force data.
	//
	//! input attribute compound, multi
	static MObject	mInputData;
	//! child attribute, multi
	static MObject	mInputPositions;
	//! child attribute, multi
	static MObject	mInputVelocities;
	//! child attribute, multi
	static MObject	mInputMass;
	//! child attribute
	static MObject	mDeltaTime;

	// Attribute for input forces.
	//
	//! forces input attribute, multi
	static MObject	mInputForce;

	// Attribute for output forces.
	//
	//! forces output attribute, multi
	static MObject	mOutputForce;

	// Attributes for ownership.
	//
	//! X component of mOwnerCentroid
	static MObject	mOwnerCentroidX;
	//! Y component of mOwnerCentroid
	static MObject	mOwnerCentroidY;
	//! Z component of mOwnerCentroid
	static MObject	mOwnerCentroidZ;
	//! owner centroid attribute
	static MObject	mOwnerCentroid;
	//! owner position attribute, multi
	static MObject	mOwnerPosData;
	//! owner velocity attribute, multi
	static MObject	mOwnerVelData;

	// Attribute for world position.
	//
	//! world matrix attribute
	static MObject	mWorldMatrix;

	//! Attribute for input pp data from particleShape
	static MObject  mInputPPData;

	//! Attribute for input pp data from owner particleShape
	static MObject  mOwnerPPData;

	static const char*	className();

protected:
// No protected members

private:

	static void			initialSetup();


};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxFieldNode */
