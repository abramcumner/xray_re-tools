#ifndef _MPxTransform
#define _MPxTransform
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MPxTransform)
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MObjectArray.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MPxTransformationMatrix.h>
#include <maya/MFnTransform.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxTransform)

//! \ingroup OpenMaya MPx
//! \brief Base class for user defined transforms. 
/*!
MPxTransform allows the creation of user defined transform nodes. User defined
transform nodes can introduce new transform types or change the transformation
order. They are designed to be an extension of the standard Maya transform
node and include all of the normal transform attributes. Standard behaviors
such as limit enforcement and attribute locking are managed by this class,
but may be overriden in derived classes.

In general, a complete implementation of user defined transforms will
require the deriving from two classes; MPxTransform defines the node
while MPxTransformationMatrix describes the mathematical functions of
the user defined transform.

The MPxTransform class is registered using the MFnPlugin::registerTransform()
method. Both the MPxTransform and the MPxTransformationMatrix classes are
registered in the same method. This allows for a clear association between a
MPxTransform and a MPxTransformationMatrix. Both the MPxTransform and the
MPxTransformationMatrix classes need unique MTypeIds.

MPxTransform nodes are DAG nodes and therefore a change to one element
will affect every node beneath the changed node. Since this can involve
quite a bit of calculation, DAG nodes internally handle clean and dirty
differently than other nodes. What this means is that the
updateMatrixAttrs() method should be used when getting one of the matrix
attributes from a method on this node. Additionally, after a value is
changed, the appropriate dirty method (i.e. dirtyTranslate(),
dirtyRotate(), etc.) should be called. When in doubt, dirtyMatrix() will
flag everything as needing to be updated.

It is up to each transform node to determine if it will obey limits or not.
Since transform attributes may have limits or may be involved in some sort
of constraint, there needs to be a way to accept, reject, or modify a value
when a plug is set on the node. The mustCallValidateAndSet() method allows
for this kind of control. When an attribute is flagged with the
mustCallValidateAndSet() method in the initialize() method, every plug
change will call the validateAndSetValue() method for approval. From the
validateAndSetValue() method things like limits and value locking can
be enforced. It is important to note that for new attributes on the
transform node, any locking or limits are left as an implementation detail.

If any of the public methods are affected by the addition of transform
components, or by the order of computation, they should be overriden
in the derived class. Many of the public methods are used by internal
Maya code and exist for more than just convenience.

The createTransformationMatrix() class must be overloaded if a
transformation matrix other than the default MPxTransformationMatrix
is used.

NOTES:
1) The setDependentsDirty() virtual method is available in this class since
MPxTransform derives from MPxNode.  During a call to
MPxTransform::setDependentsDirty(), a plug-in should not invoke any of the
dirty*() or updateMatrixAttrs() calls of this class.  For example, the
methods dirtyMatrix(), dirtyTranslation() or updateMatrixAttrs()
should not be called.
2) Updating world space attributes is an expensive operation.  Maya
updates world space attributes on demand such as in the case of
a getAttr being issued or a connection exists for the attribute.
*/
class OPENMAYA_EXPORT MPxTransform : public MPxNode
{
public:
	MPxTransform();

	MPxTransform(const MPxTransform&)/* = default*/;
	MPxTransform& operator=(const MPxTransform&)/* = default*/;

	virtual ~MPxTransform();
	virtual	void	postConstructor();
	virtual MPxNode::Type type() const;

	virtual MPxTransformationMatrix *createTransformationMatrix();

	virtual bool			isBounded		() const;
	virtual MBoundingBox	boundingBox		() const;
	virtual bool			treatAsTransform() const;

	const MPxTransformationMatrix &	transformationMatrix	() const;
	MPxTransformationMatrix *		transformationMatrixPtr	() const;

	virtual void  resetTransformation (const MMatrix &);
	virtual void  resetTransformation (MPxTransformationMatrix *);

	virtual MStatus	compute(const MPlug& plug, MDataBlock& data);
	virtual MStatus computeLocalTransformation (MPxTransformationMatrix *,
												MDataBlock &);
	virtual MStatus		clearLimits	();
	bool    isLimited	(MFnTransform::LimitType, MStatus * ReturnStatus = NULL ) const;
	double	limitValue	(MFnTransform::LimitType, MStatus * ReturnStatus = NULL ) const;
	MStatus setLimit	(MFnTransform::LimitType, double value);
	MStatus enableLimit	(MFnTransform::LimitType, bool flag);

	//  Method to validate input data.
	//
	virtual MStatus	validateAndSetValue(const MPlug&, const MDataHandle&);

	//	Matrix methods
	//
	virtual MMatrix getMatrix		(MStatus *ReturnStatus = NULL);
	virtual MMatrix getMatrixInverse(MStatus *ReturnStatus = NULL);

	//	Translation methods
	//
	virtual MVector getTranslation	(MSpace::Space = MSpace::kTransform, MStatus *ReturnStatus = NULL);
	virtual MStatus	translateTo		(const MVector&, MSpace::Space = MSpace::kTransform);
	virtual MStatus translateBy		(const MVector&, MSpace::Space = MSpace::kTransform);

	//	Rotation methods
	//
	virtual MQuaternion		getRotation		(MSpace::Space = MSpace::kTransform, MStatus *ReturnStatus = NULL);
	virtual MEulerRotation	getEulerRotation(MSpace::Space = MSpace::kTransform, MStatus *ReturnStatus = NULL);
	virtual MStatus			rotateTo		(const MQuaternion&, MSpace::Space = MSpace::kTransform);
	virtual MStatus			rotateBy		(const MQuaternion&, MSpace::Space = MSpace::kTransform);
	virtual MStatus			rotateTo		(const MEulerRotation&, MSpace::Space = MSpace::kTransform);
	virtual MStatus			rotateBy		(const MEulerRotation&, MSpace::Space = MSpace::kTransform);

	//	Scale methods
	//
	virtual MVector getScale(MSpace::Space = MSpace::kTransform, MStatus *ReturnStatus = NULL);
	virtual MStatus	scaleTo	(const MVector&, MSpace::Space = MSpace::kTransform);
	virtual MStatus scaleBy	(const MVector&, MSpace::Space = MSpace::kTransform);

	//	Shear methods
	//
	virtual MVector getShear(MSpace::Space = MSpace::kTransform, MStatus *ReturnStatus = NULL);
	virtual MStatus	shearTo	(const MVector&, MSpace::Space = MSpace::kTransform);
	virtual MStatus shearBy	(const MVector&, MSpace::Space = MSpace::kTransform);

	//	Pivot methods
	//
	virtual MPoint	getScalePivot			 (MSpace::Space = MSpace::kTransform, MStatus *ReturnStatus = NULL);
	virtual MPoint	getRotatePivot			 (MSpace::Space = MSpace::kTransform, MStatus *ReturnStatus = NULL);
	virtual MVector	getScalePivotTranslation (MSpace::Space = MSpace::kTransform, MStatus *ReturnStatus = NULL);
	virtual MVector	getRotatePivotTranslation(MSpace::Space = MSpace::kTransform, MStatus *ReturnStatus = NULL);
	virtual MStatus	setScalePivot			 (const MPoint &, MSpace::Space = MSpace::kTransform, bool balance = true);
	virtual MStatus	setScalePivotTranslation (const MVector &, MSpace::Space = MSpace::kTransform);
	virtual MStatus	setRotatePivot			 (const MPoint &, MSpace::Space = MSpace::kTransform, bool balance = true);
	virtual MStatus	setRotatePivotTranslation(const MVector &, MSpace::Space = MSpace::kTransform);

	//  Rotation order methods
	//
	virtual MTransformationMatrix::RotationOrder getRotationOrder();
	virtual	MStatus								 setRotationOrder(MTransformationMatrix::RotationOrder ro, bool reorder = true);

	//  Rotation orientation methods
	//
	virtual MQuaternion	getRotateOrientation(MSpace::Space apiSpace = MSpace::kTransform, MStatus *ReturnStatus = NULL);
	virtual MStatus		setRotateOrientation(const MQuaternion &q, MSpace::Space apiSpace = MSpace::kTransform, bool balance = true);

	MStatus	updateMatrixAttrs();
	MStatus updateMatrixAttrs(MObject &attr);

	static void mustCallValidateAndSet(MObject &);

	static MStatus	setNonAffineMatricesEnabled(bool);
	static bool isNonAffineMatricesEnabled(MStatus *ReturnStatus);
	virtual void copyInternalData( MPxNode* );

	// SCRIPT USE ONLY.
	MStatus _dirtyMatrix()									{ return dirtyMatrix(); }
	MStatus _dirtyTranslation(const MVector &v)				{ return dirtyTranslation(v); }
	MStatus _dirtyRotation(const MEulerRotation &e)			{ return dirtyRotation(e); }
	MStatus _dirtyScale(const MVector &v)					{ return dirtyScale(v); }
	MStatus _dirtyShear(const MVector &v)					{ return dirtyShear(v); }
	MStatus _dirtyRotateOrientation(const MEulerRotation &e){ return dirtyRotateOrientation(e); }
	MStatus _dirtyScalePivot(const MPoint &p)				{ return dirtyScalePivot(p); }
	MStatus _dirtyRotatePivot(const MPoint &p)				{ return dirtyRotatePivot(p); }
	MStatus _dirtyScalePivotTranslation(const MVector &v)	{ return dirtyScalePivotTranslation(v); }
	MStatus _dirtyRotatePivotTranslation(const MVector &v)	{return dirtyRotatePivotTranslation(v); }
	//

protected:
	//
	//	Methods for clamping and locking enforcement
	//
	virtual MVector			applyTranslationLimits			(const MVector &unclampedT, MDataBlock &, MStatus *ReturnStatus = NULL);
	virtual	MVector			applyTranslationLocks			(const MVector &toTest, const MVector &savedT, MStatus *ReturnStatus = NULL);
	virtual MEulerRotation	applyRotationLimits				(const MEulerRotation &unclampedR, MDataBlock &, MStatus *ReturnStatus = NULL);
	virtual	MEulerRotation	applyRotationLocks				(const MEulerRotation &toTest, const MEulerRotation &savedR, MStatus *ReturnStatus = NULL);
	virtual MVector			applyScaleLimits				(const MVector &unclampedS, MDataBlock &, MStatus *ReturnStatus = NULL);
	virtual	MVector			applyScaleLocks					(const MVector &toTest, const MVector &savedS, MStatus *ReturnStatus = NULL);
	virtual	MVector			applyShearLocks					(const MVector &toTest, const MVector &savedSh, MStatus *ReturnStatus = NULL);
	virtual	MEulerRotation	applyRotateOrientationLocks		(const MEulerRotation &toTest, const MEulerRotation &savedRO, MStatus *ReturnStatus = NULL);
	virtual	MVector			applyScaleLocksPivot			(const MPoint &toTest, const MPoint &savedSP, MStatus *ReturnStatus = NULL);
	virtual	MVector			applyRotatePivotLocks			(const MPoint &toTest, const MPoint &savedRP, MStatus *ReturnStatus = NULL);
	virtual	MVector			applyScaleLocksPivotTranslate	(const MVector &toTest, const MVector &savedSPT, MStatus *ReturnStatus = NULL);
	virtual	MVector			applyRotatePivotLocksTranslate	(const MVector &toTest, const MVector &savedRPT, MStatus *ReturnStatus = NULL);
	//
	//	Methods for marking the matrix dirty.
	//

	MStatus dirtyMatrix					();							//!< USE _dirtyMatrix() IN SCRIPT
	MStatus	dirtyTranslation			(const MVector &);			//!< USE _dirtyTranslation() IN SCRIPT
	MStatus	dirtyRotation				(const MEulerRotation &);	//!< USE _dirtyRotation() IN SCRIPT
	MStatus dirtyScale					(const MVector &);			//!< USE _dirtyScale() IN SCRIPT
	MStatus dirtyShear					(const MVector &);			//!< USE _dirtyShear() IN SCRIPT
	MStatus dirtyRotateOrientation		(const MEulerRotation &);	//!< USE _dirtyRotateOrientation() IN SCRIPT
	MStatus dirtyScalePivot				(const MPoint &);			//!< USE _dirtyScalePivot() IN SCRIPT
	MStatus dirtyRotatePivot			(const MPoint &);			//!< USE _dirtyRotatePivot() IN SCRIPT
	MStatus dirtyScalePivotTranslation	(const MVector &);			//!< USE _dirtyScalePivotTranslation() IN SCRIPT
	MStatus dirtyRotatePivotTranslation	(const MVector &);			//!< USE _dirtyRotatePivotTranslation() IN SCRIPT

	//
	//	checkAndSet methods. These are called by the compute method.
	//
	virtual	MStatus	checkAndSetTranslation			 (MDataBlock &, const MPlug&, const MVector&, MSpace::Space = MSpace::kTransform);
	virtual	MStatus checkAndSetRotation				 (MDataBlock &, const MPlug&, const MEulerRotation&, MSpace::Space = MSpace::kTransform);
	virtual	MStatus checkAndSetScale				 (MDataBlock &, const MPlug&, const MVector&, MSpace::Space = MSpace::kTransform);
	virtual	MStatus checkAndSetShear				 (MDataBlock &, const MPlug&, const MVector&, MSpace::Space = MSpace::kTransform);
	virtual	MStatus checkAndSetRotateOrientation	 (MDataBlock &, const MPlug&, const MEulerRotation&, MSpace::Space = MSpace::kTransform, bool balance = true);
	virtual	MStatus	checkAndSetRotatePivot			 (MDataBlock &, const MPlug&, const MPoint&, MSpace::Space = MSpace::kTransform, bool balance = true);
	virtual	MStatus checkAndSetRotatePivotTranslation(MDataBlock &, const MPlug&, const MVector&, MSpace::Space = MSpace::kTransform);
	virtual	MStatus	checkAndSetScalePivot			 (MDataBlock &, const MPlug&, const MPoint&, MSpace::Space = MSpace::kTransform, bool = true);
	virtual	MStatus checkAndSetScalePivotTranslation (MDataBlock &, const MPlug&, const MVector&, MSpace::Space = MSpace::kTransform);

public:
	// Default attributes.
	static MObject nodeBoundingBox;				//!<bounding box attribute
		static MObject nodeBoundingBoxMin;		//!<	bounding box minimum point
			static MObject nodeBoundingBoxMinX;	//!<		X component of nodeBoundingBoxMin
			static MObject nodeBoundingBoxMinY;	//!<		Y component of nodeBoundingBoxMin
			static MObject nodeBoundingBoxMinZ;	//!<		Z component of nodeBoundingBoxMin
		static MObject nodeBoundingBoxMax;		//!<	bounding box maximum point
			static MObject nodeBoundingBoxMaxX;	//!<		X component of nodeBoundingBoxMax
			static MObject nodeBoundingBoxMaxY;	//!<		Y component of nodeBoundingBoxMax
			static MObject nodeBoundingBoxMaxZ;	//!<		Z component of nodeBoundingBoxMax
		static MObject nodeBoundingBoxSize;		//!<	bounding box size vector
			static MObject nodeBoundingBoxSizeX;//!<		X component of nodeBoundingBoxSize
			static MObject nodeBoundingBoxSizeY;//!<		Y component of nodeBoundingBoxSize
			static MObject nodeBoundingBoxSizeZ;//!<		Z component of nodeBoundingBoxSize
	static MObject center;						//!<object center attribute
		static MObject boundingBoxCenterX;		//!<	X component of the bounding box center
		static MObject boundingBoxCenterY;		//!<	Y component of the bounding box center
		static MObject boundingBoxCenterZ;		//!<	Z component of the bounding box center
	static MObject matrix;						//!<matrix attribute 
	static MObject inverseMatrix;				//!<inverse matrix attribute
	static MObject worldMatrix;					//!<world matrix attribute
	static MObject worldInverseMatrix;			//!<Inverse world matrix attribute
	static MObject parentMatrix;				//!<Parent matrix attribute
	static MObject parentInverseMatrix;			//!<Inverse parent matrix attribute
	static MObject visibility;					//!<Visibility attribute
	static MObject intermediateObject;			//!<Intermediate object attribute
	static MObject isTemplated;					//!<Template attribute
	static MObject ghosting;					//!<Ghosting attribute
	static MObject instObjGroups;				//!<Instances object group info attribute
		static MObject objectGroups;			//!<Object groups attributes
			static MObject objectGrpCompList;	//!<	Component in object groups attribute
			static MObject objectGroupId;		//!<	Group id attribute
			static MObject objectGroupColor;	//!<	Group color attribute
	static MObject useObjectColor;				//!<Controls choice of wireframe dormant object color
	static MObject objectColor;					//!<The per object dormant wireframe color

	static MObject drawOverride;				//!<Draw override attribute
		static MObject overrideDisplayType;		//!<	Draw override display type attribute
		static MObject overrideLevelOfDetail;	//!<	Draw override LOD attribute
		static MObject overrideShading;			//!<	Draw override shading attribute
		static MObject overrideTexturing;		//!<	Draw override texturing attribute
		static MObject overridePlayback;		//!<	Draw override playback attribute
		static MObject overrideEnabled;			//!<	Draw override enabled attribute
		static MObject overrideVisibility;		//!<	Draw override visibility attribute
		static MObject overrideColor;			//!<	Draw override color attribute
	static MObject lodVisibility;				//!<LOD visibility attribute
	static MObject renderInfo;					//!< \obsolete Obsolete attribute
		static MObject identification;			//!<	\obsolete Obsolete attribute
		static MObject layerRenderable;			//!<	\obsolete Obsolete attribute
		static MObject layerOverrideColor;		//!<	\obsolete Obsolete attribute
	static MObject renderLayerInfo;				//!< Render layer Info attribute
	static MObject renderLayerId;				//!< Render layer ID attribute
	static MObject renderLayerRenderable;		//!< Render layer Renderable attribute
	static MObject renderLayerColor;			//!< Render layer Color attribute
	static MObject translate;					//!<translate attribute
		static MObject translateX;				//!<	translateX attribute
		static MObject translateY;				//!<	translateY attribute
		static MObject translateZ;				//!<	translateZ attribute
	static MObject rotate;						//!<rotate attribute
		static MObject rotateX;					//!<	rotateX attribute
		static MObject rotateY;					//!<	rotateY attribute
		static MObject rotateZ;					//!<	rotateZ attribute
	static MObject rotateOrder;					//!<Rotate order attribute
	static MObject scale;						//!<scale attribute
		static MObject scaleX;					//!<	scaleX attribute
		static MObject scaleY;					//!<	scaleY attribute
		static MObject scaleZ;					//!<	scaleZ attribute
	static MObject shear;						//!<shear attribute
		static MObject shearXY;					//!<	shearXY attribute
		static MObject shearXZ;					//!<	shearXZ attribute
		static MObject shearYZ;					//!<	shearYZ attribute
	static MObject rotatePivot;					//!<rotate pivot attribute
		static MObject rotatePivotX;			//!<	rotate pivot X attribute
		static MObject rotatePivotY;			//!<	rotate pivot Y attribute
		static MObject rotatePivotZ;			//!<	rotate pivot Z attribute
	static MObject rotatePivotTranslate;		//!<rotate pivot translate attribute
		static MObject rotatePivotTranslateX;	//!<	rotate pivot translate X attribute
		static MObject rotatePivotTranslateY;	//!<	rotate pivot translate Y attribute
		static MObject rotatePivotTranslateZ;	//!<	rotate pivot translate Z attribute
	static MObject scalePivot;					//!<scale pivot attribute
		static MObject scalePivotX;				//!<	scale pivot X attribute
		static MObject scalePivotY;				//!<	scale pivot Y attribute
		static MObject scalePivotZ;				//!<	scale pivot Z attribute
	static MObject scalePivotTranslate;			//!<Scale pivot translate attribute
		static MObject scalePivotTranslateX;	//!<	scale pivot translate X attribute
		static MObject scalePivotTranslateY;	//!<	scale pivot translate Y attribute
		static MObject scalePivotTranslateZ;	//!<	scale pivot translate Z attribute
	static MObject rotateAxis;					//!<Rotate orientation attribute
		static MObject rotateAxisX;				//!<	rotate orientation X attribute
		static MObject rotateAxisY;				//!<	rotate orientation Y attribute
		static MObject rotateAxisZ;				//!<	rotate orientation Z attribute
	static MObject transMinusRotatePivot;		//!<translate minus rotate pivot attribute
		static MObject transMinusRotatePivotX;	//!<	translateX minus rotate pivotX attribute
		static MObject transMinusRotatePivotY;	//!<	translateY minus rotate pivotY attribute
		static MObject transMinusRotatePivotZ;	//!<	translateZ minus rotate pivotZ attribute
	static MObject minTransLimit;				//!<Minumum translation limits attribute
		static MObject minTransXLimit;			//!<	minimum translate X limit attribute
		static MObject minTransYLimit;			//!<	minimum translate Y limit attribute
		static MObject minTransZLimit;			//!<	minimum translate Z limit attribute
	static MObject maxTransLimit;				//!<Maximum translation limits attribute
		static MObject maxTransXLimit;			//!<	maximum translate X limit attribute
		static MObject maxTransYLimit;			//!<	maximum translate Y limit attribute
		static MObject maxTransZLimit;			//!<	maximum translate Z limit attribute
	static MObject minTransLimitEnable;			//!<Enable the mimimum translation limits attribute
		static MObject minTransXLimitEnable;	//!<	enable the minimum translate X limit attribute
		static MObject minTransYLimitEnable;	//!<	enable the minimum translate Y limit attribute
		static MObject minTransZLimitEnable;	//!<	enable the minimum translate Z limit attribute
	static MObject maxTransLimitEnable;			//!<Enable the maximum translation limits attribute
		static MObject maxTransXLimitEnable;	//!<	enable the maximum translate X limit attribute
		static MObject maxTransYLimitEnable;	//!<	enable the maximum translate Y limit attribute
		static MObject maxTransZLimitEnable;	//!<	enable the maximum translate Z limit attribute
	static MObject minRotLimit;					//!<Minimum rotation limits attribute
		static MObject minRotXLimit;			//!<	minimum rotate X limit attribute
		static MObject minRotYLimit;			//!<	minimum rotate Y limit attribute
		static MObject minRotZLimit;			//!<	minimum rotate Z limit attribute
	static MObject maxRotLimit;					//!<Maximum rotation limits attribute
		static MObject maxRotXLimit;			//!<	maximum rotate X limit attribute
		static MObject maxRotYLimit;			//!<	maximum rotate Y limit attribute
		static MObject maxRotZLimit;			//!<	maximum rotate Z limit attribute
	static MObject minRotLimitEnable;			//!<Enable minimum rotation limits attribute
		static MObject minRotXLimitEnable;		//!<	enable minimum rotate X limit attribute
		static MObject minRotYLimitEnable;		//!<	enable minimum rotate Y limit attribute
		static MObject minRotZLimitEnable;		//!<	enable minimum rotate Z limit attribute
	static MObject maxRotLimitEnable;			//!<Enable maximum rotation limits attribute
		static MObject maxRotXLimitEnable;		//!<	enable maximum rotate X limit attribute
		static MObject maxRotYLimitEnable;		//!<	enable maximum rotate Y limit attribute
		static MObject maxRotZLimitEnable;		//!<	enable maximum rotate Z limit attribute
	static MObject minScaleLimit;				//!<Minimum scale limit attribute
		static MObject minScaleXLimit;			//!<	minimum scale X limit attribute
		static MObject minScaleYLimit;			//!<	minimum scale Y limit attribute
		static MObject minScaleZLimit;			//!<	minimum scale Z limit attribute
	static MObject maxScaleLimit;				//!<Maximum scale limit attribute
		static MObject maxScaleXLimit;			//!<	maximum scale X limit attribute
		static MObject maxScaleYLimit;			//!<	maximum scale Y limit attribute
		static MObject maxScaleZLimit;			//!<	maximum scale Z limit attribute
	static MObject minScaleLimitEnable;			//!<Enable minimum scale limit attribute
		static MObject minScaleXLimitEnable;	//!<	enable minimum scale X limit attribute
		static MObject minScaleYLimitEnable;	//!<	enable minimum scale Y limit attribute
		static MObject minScaleZLimitEnable;	//!<	enable minimum scale Z limit attribute
	static MObject maxScaleLimitEnable;			//!<Enable maximum scale limit attribute
		static MObject maxScaleXLimitEnable;	//!<	enable maximum scale X limit attribute
		static MObject maxScaleYLimitEnable;	//!<	enable maximum scale Y limit attribute
		static MObject maxScaleZLimitEnable;	//!<	enable maximum scale Z limit attribute
	static MObject geometry;					//!<Geometry attribute
	static MObject xformMatrix;					//!<Xform matrix attribute
	static MObject selectHandle;				//!<Selection handle attribute
		static MObject selectHandleX;			//!<	Selection handle X attribute
		static MObject selectHandleY;			//!<	Selection handle Y attribute
		static MObject selectHandleZ;			//!<	Selection handle Z attribute
	static MObject inheritsTransform;			//!<Inherits transform attribute
	static MObject displayHandle;				//!<Display handle attribute
	static MObject displayScalePivot;			//!<Display scale pivot attribute
	static MObject displayRotatePivot;			//!<Display rotate pivot attribute
	static MObject displayLocalAxis;			//!<Display local axis attribute
	static MObject dynamics;					//!<Dynamics attribute
	static MObject showManipDefault;			//!<Show manipulator default attribute
	static MObject specifiedManipLocation;		//!<Specified manipulator location attribute
	static MObject rotateQuaternion;			//!<Rotate quaternion attribute
		static MObject rotateQuaternionX;		//!<	Rotate quaternion X attribute
		static MObject rotateQuaternionY;		//!<	Rotate quaternion Y attribute
		static MObject rotateQuaternionZ;		//!<	Rotate quaternion Z attribute
		static MObject rotateQuaternionW;		//!<	Rotate quaternion W attribute
	static MObject rotationInterpolation;		//!<Rotation interporlation attribute

	static const char*	    className();

private:
	static void				initialSetup();
	static MEulerRotation	getEulerRotationFromAttrs(MDataBlock& block);
	static MEulerRotation::RotationOrder getEulerRotationOrderFromAttrs(MDataBlock& block);

protected:
	//! \brief Cached MPxTransformationMatrix for normal-context (same as returned by transformationMatrix() method in normal-context).
	//! \warning This member does not respect evaluation context.
	//! Any use of it may result wrong result in non-normal context.
	//! \note The value of this pointer is filled by Maya after this class' postConstructor got called. 
	//! It should be treat as read-only field, modifying it may lead to incorrect result.
	OPENMAYA_DEPRECATED(2019, "This member is not valid during background evaluation. Use transformationMatrixPtr() instead.")
	MPxTransformationMatrix	* /*const*/ baseTransformationMatrix;

public:
	OPENMAYA_DEPRECATED(2019, "Use default-constructor instead, Maya will manage the creation of MPxTransformationMatrix on demand.")
	MPxTransform(MPxTransformationMatrix *);

public:
	//======================================================================
	//
	// Obsolete versions - use the method overloads without context instead
	//

	OPENMAYA_DEPRECATED(2018, "Override validateAndSetValue(const MPlug&, const MDataHandle&) instead. If needed, use MDGContext::current() to get the context.")
	virtual MStatus	validateAndSetValue(const MPlug&, const MDataHandle&, const MDGContext&);

	OPENMAYA_DEPRECATED(2018, "Call updateMatrixAttrs() instead.")
	MStatus updateMatrixAttrs(const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Call updateMatrixAttrs(MObject &) instead.")
	MStatus updateMatrixAttrs(MObject &attr, const MDGContext&);

	OPENMAYA_DEPRECATED(2018, "Call getMatrix(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MMatrix getMatrix		(const MDGContext&, MStatus *ReturnStatus = NULL);

	OPENMAYA_DEPRECATED(2018, "Call getMatrixInverse(MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MMatrix getMatrixInverse(const MDGContext&, MStatus *ReturnStatus = NULL);



	OPENMAYA_DEPRECATED(2018, "Use getTranslation(MSpace::Space, MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MVector getTranslation	(MSpace::Space, const MDGContext&, MStatus *ReturnStatus = NULL);
	
	OPENMAYA_DEPRECATED(2018, "Use translateTo(const MVector&, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus	translateTo		(const MVector&, MSpace::Space, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use translateBy(const MVector&, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus translateBy		(const MVector&, MSpace::Space, const MDGContext&);


	OPENMAYA_DEPRECATED(2018, "Use getRotation(MSpace::Space, MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MQuaternion		getRotation		(MSpace::Space, const MDGContext&, MStatus *ReturnStatus = NULL);
	
	OPENMAYA_DEPRECATED(2018, "Use getEulerRotation(MSpace::Space, MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MEulerRotation	getEulerRotation(MSpace::Space, const MDGContext&, MStatus *ReturnStatus = NULL);
	
	OPENMAYA_DEPRECATED(2018, "Use rotateTo(const MQuaternion&, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus			rotateTo		(const MQuaternion&, MSpace::Space, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use rotateBy(const MQuaternion&, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus			rotateBy		(const MQuaternion&, MSpace::Space, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use rotateTo(const MEulerRotation&, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus			rotateTo		(const MEulerRotation&, MSpace::Space, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use rotateBy(const MEulerRotation&, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus			rotateBy		(const MEulerRotation&, MSpace::Space, const MDGContext&);


	OPENMAYA_DEPRECATED(2018, "Use getScale(MSpace::Space, MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MVector getScale(MSpace::Space, const MDGContext&, MStatus *ReturnStatus = NULL);
	
	OPENMAYA_DEPRECATED(2018, "Use scaleTo(const MVector&, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus	scaleTo	(const MVector&, MSpace::Space, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use scaleBy(const MVector&, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus scaleBy	(const MVector&, MSpace::Space, const MDGContext&);


	OPENMAYA_DEPRECATED(2018, "Use getShear(MSpace::Space, MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MVector getShear(MSpace::Space, const MDGContext&, MStatus *ReturnStatus = NULL);
	
	OPENMAYA_DEPRECATED(2018, "Use shearTo(const MVector&, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus	shearTo	(const MVector&, MSpace::Space, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use shearBy(const MVector&, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus shearBy	(const MVector&, MSpace::Space, const MDGContext&);


	OPENMAYA_DEPRECATED(2018, "Use getScalePivot(MSpace::Space, MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MPoint	getScalePivot			 (MSpace::Space, const MDGContext&, MStatus *ReturnStatus = NULL);
	
	OPENMAYA_DEPRECATED(2018, "Use getRotatePivot(MSpace::Space, MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MPoint	getRotatePivot			 (MSpace::Space, const MDGContext&, MStatus *ReturnStatus = NULL);
	
	OPENMAYA_DEPRECATED(2018, "Use getScalePivotTranslation(MSpace::Space, MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MVector	getScalePivotTranslation (MSpace::Space, const MDGContext&, MStatus *ReturnStatus = NULL);
	
	OPENMAYA_DEPRECATED(2018, "Use getRotatePivotTranslation(MSpace::Space, MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MVector	getRotatePivotTranslation(MSpace::Space, const MDGContext&, MStatus *ReturnStatus = NULL);
	
	OPENMAYA_DEPRECATED(2018, "Use setScalePivot(const MPoint &, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus	setScalePivot			 (const MPoint &, MSpace::Space, bool balance, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use setScalePivotTranslation(const MVector &, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus	setScalePivotTranslation (const MVector &, MSpace::Space, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use setRotatePivot(const MPoint &, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus	setRotatePivot			 (const MPoint &, MSpace::Space, bool balance, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use setRotatePivotTranslation(const MVector &, MSpace::Space) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus	setRotatePivotTranslation(const MVector &, MSpace::Space, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use getRotationOrder() instead. If needed, use MDGContextGuard to switch the context.")
	virtual MTransformationMatrix::RotationOrder getRotationOrder(const MDGContext &);

	OPENMAYA_DEPRECATED(2018, "Use setRotationOrder(MTransformationMatrix::RotationOrder, bool reorder) instead. If needed, use MDGContextGuard to switch the context.")
	virtual	MStatus	setRotationOrder	(MTransformationMatrix::RotationOrder, bool reorder, const MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use getRotateOrientation(MSpace::Space, MStatus*) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MQuaternion getRotateOrientation(MSpace::Space apiSpace, const MDGContext&apiContext, MStatus *ReturnStatus = NULL);
	
	OPENMAYA_DEPRECATED(2018, "Use setRotateOrientation(const MQuaternion&, MSpace::Space, bool balance) instead. If needed, use MDGContextGuard to switch the context.")
	virtual MStatus		setRotateOrientation(const MQuaternion &q, MSpace::Space, bool balance, const MDGContext&);
	//
	//======================================================================
};

// ****************************************************************************
// CLASS DECLARATION (MPxTransform_BoundingBox)

//! \ingroup OpenMaya MPx
//! \brief Base class for user defined transforms.
/*!
MPxTransform_BoundingBox is a class derived from MPxTransform that will only be present in Maya 2019 update 2 (2019.2)
and any subsequent updates to Maya 2019.

When MPxTransform::treatAsTransform() returns false, MPxTransform::boundingBox() should be called from
commands such as 'viewfit'. However, boundingBox() is never called regardless of treatAsTransform's return value.

MPxTransform_BoundingBox has been introduced as a temporary fix for this issue.

In order for boundingBox to be called from commands,
you will need to change your MPxTransform derived classes to MPxTransform_BoundingBox derived classes.

This class is only meant to help maintain behaviour in Maya 2019 updates. It
will be removed in the next major version when the behaviour of MPxTransform::treatAsTransform()
will be fixed.
*/
class OPENMAYA_EXPORT MPxTransform_BoundingBox : public MPxTransform
{
public:
	MPxTransform_BoundingBox();
	MPxTransform_BoundingBox(const MPxTransform_BoundingBox&)/* = default*/;
	MPxTransform_BoundingBox& operator=(const MPxTransform_BoundingBox&)/* = default*/;

	virtual ~MPxTransform_BoundingBox();

	virtual bool			isBounded() const;
	virtual MBoundingBox	boundingBox() const;
	virtual bool			treatAsTransform() const;

};


OPENMAYA_NAMESPACE_CLOSE

#endif /* _MPxTransform */
