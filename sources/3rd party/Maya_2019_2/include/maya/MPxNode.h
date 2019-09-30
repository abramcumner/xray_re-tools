#ifndef _MPxNode
#define _MPxNode
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
// CLASS:    MPxNode
//
// ****************************************************************************

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>

#include <maya/MTypeId.h>
#include <maya/MDataHandle.h>
#include <maya/MDataBlock.h>
#include <maya/MPlug.h>
#include <maya/MString.h>
#include <maya/MDGContext.h>
#include <maya/MDeprecate.h>




OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxNode)

//! \ingroup OpenMaya MPx
//! \brief Base class for user defined dependency nodes. 
/*!
  MPxNode is the the parent class for user defined dependency nodes.
  A dependency node is an object that resides in the dependency graph.
  It computes output attributes based on a set of input attributes.
  When an input changes, the compute method is called for each
  dependent output.

  The dependency graph is made up of nodes that have connections
  between their attributes. When an attribute changes, recomputation
  propagates through the graph until all affected values have been
  updated.

  When writing a dependency node, there is a very basic rule that
  should be observed. The outputs should be calculated only using the
  values of the inputs. All information about the world outside the
  node should come from input attributes. If this rule is not
  observed, then the results may be unpredictable.

  All dependency nodes have four basic attributes. Documentation for
  them may be found with the documentation for the dependency nodes in
  Maya. Only one attribute requires special attention by node
  developers. Developers must decide whether to support the
  <i>HasNoEffect</i> setting of the <i>state</i> attribute. {\em
  HasNoEffect} means that a node should pass through all data without
  performing computations on it. For example, a deformer node will
  pass geometry through unmodified when <i>state</i> is set to
  <i>HasNoEffect</i>. It is up the the plug-in writer to observe the
  <i>HasNoEffect mode</i> if it is relevant to the type of node.
*/

class OPENMAYA_EXPORT MPxNode
{
public:

	//! Defines the type of node
	enum Type {
		kDependNode,					//!< Custom node derived from MPxNode
		kLocatorNode,					//!< Custom locator derived from MPxLocatorNode
		kDeformerNode,					//!< Custom deformer derived from MPxDeformerNode
		kManipContainer,				//!< Custom container derived from MPxManipContainer
		kSurfaceShape,					//!< Custom shape derived from MPxSurfaceShape
		kFieldNode,						//!< Custom field derived from MPxFieldNode
		kEmitterNode,					//!< Custom emitter derived from MPxEmitterNode
		kSpringNode,					//!< Custom spring derived from MPxSpringNode
		kIkSolverNode,					//!< Custom IK solver derived from MPxIkSolverNode
		kHardwareShader,				//!< Custom shader derived from MPxHardwareShader
		kHwShaderNode,					//!< Custom shader derived from MPxHwShaderNode
		kTransformNode,					//!< Custom transform derived from MPxTransform
		kObjectSet,						//!< Custom set derived from MPxObjectSet
		kFluidEmitterNode,				//!< Custom fluid emitter derived from MpxFluidEmitterNode
		kImagePlaneNode,				//!< Custom image plane derived from MPxImagePlane
		kParticleAttributeMapperNode,	//!< Custom particle attribute mapper derived from MPxParticleAttributeMapperNode
		kCameraSetNode,					//!< Custom director derived from MPxCameraSet
		kConstraintNode,				//!< Custom constraint derived from MPxConstraint
		kManipulatorNode,				//!< Custom manipulator derived from MPxManipulatorNode
		kMotionPathNode,				//!< Custom motionPath derived from MPxMotionPathNode
		kClientDeviceNode,				//!< Custom threaded device derived from MPxThreadedDeviceNode
		kThreadedDeviceNode,			//!< Custom threaded device node
		kAssembly,     				    //!< Custom assembly derived from MPxAssembly
		kSkinCluster,					//!< Custom deformer derived from MPxSkinCluster
		kGeometryFilter,				//!< Custom deformer derived from MPxGeometryFilter
		kBlendShape,					//!< Custom deformer derived from MPxBlendShape
		kLast							//!< Last value, used for counting
	};

	MPxNode();
	virtual ~MPxNode();
	virtual void	postConstructor();
	virtual MStatus	compute( const MPlug&, MDataBlock& );

	virtual bool	getInternalValue ( const MPlug&, MDataHandle& );	
	virtual bool	setInternalValue ( const MPlug&, const MDataHandle& );
	virtual int		internalArrayCount	( const MPlug& ) const;

	virtual void	copyInternalData	( MPxNode* );

	virtual MStatus	legalConnection		( const MPlug&, const MPlug&, bool asSrc, bool& isLegal ) const;
	virtual MStatus	legalDisconnection	( const MPlug&, const MPlug&, bool asSrc, bool& isLegal ) const;
	virtual MStatus	setDependentsDirty	( const MPlug& plug, MPlugArray& plugArray);

	virtual MStatus		preEvaluation( const  MDGContext& context, const MEvaluationNode& evaluationNode );
    enum PostEvaluationEnum {
        kEvaluatedIndirectly,
        kEvaluatedDirectly,
        kLeaveDirty,
        kPostEvaluationTypeLast
    };
    typedef unsigned int PostEvaluationType;
	virtual MStatus			postEvaluation( const  MDGContext& context, const MEvaluationNode& evaluationNode, PostEvaluationType evalType ); 
    
	//! Defines the degree of parallelism of a node.
    enum SchedulingType
    {
		//! This schedulingType indicates that the node can be evaluated concurrently to any other nodes without restrictions
		kParallel,
		//! Groups are formed for nodes having this ShedulingType when they are directly connected to each other. 
		//! Within a same group nodes are guaranteed to not to be concurrently evaluated.
		//! However nodes in distinct groups can still be concurently be evaluated.
		kSerial,
		//! Nodes having the kGloballySerial SchedulingType are guaranteed not to be evaluated concurrently to any other node having the same schedulingType.
		kGloballySerial,
		//! Untrusted nodes are guaranteed not to be evaluately concurrently to any other node
		kUntrusted,
		//!< Last value, used for counting
		kSchedulingTypeLast,
		//----------------------------------------------------------------------
		kDefaultScheduling = kSerial,			//!< \obsolete as of Maya2017, use kSerial
		kSerialize         = kSerial,			//!< \obsolete as of Maya2017, use kSerial
		kGloballySerialize = kGloballySerial	//!< \obsolete as of Maya2017, use kGloballySerial
    };

    virtual SchedulingType schedulingType()const;

	virtual MStatus			connectionMade			( const MPlug&, const MPlug&, bool asSrc );
	virtual MStatus			connectionBroken		( const MPlug&, const MPlug&, bool asSrc );
	virtual MStatus			dependsOn				( const MPlug&, const MPlug&, bool& depends ) const;
	virtual bool			isPassiveOutput			( const MPlug& ) const;
	virtual MStatus			shouldSave				( const MPlug&, bool& isSaving );
	virtual MPlug			passThroughToOne		( const MPlug& ) const;
	virtual bool			passThroughToMany		( const MPlug&, MPlugArray& ) const;

	virtual Type            type					() const;
	virtual bool			isAbstractClass			() const;

	virtual bool			isTrackingTopology		() const;

	static MStatus          addAttribute			( const MObject & attr );
	static MStatus		    inheritAttributesFrom	( const MString & parentClassName );
	static MStatus          attributeAffects		( const MObject & whenChanges, const MObject & isAffected );
	static MStatus          attributeAffects		( const MObject & whenChanges, const MObject & isAffected , bool affectsTopology );
	
	virtual MStringArray	getFilesToArchive( bool shortName = false,
											   bool unresolvedName = false,
											   bool markCouldBeImageSequence = false ) const;
											   
	virtual	void			getExternalContent			 ( MExternalContentInfoTable& table ) const;
	bool					addExternalContentForFileAttr( MExternalContentInfoTable&, const MObject& attr ) const;
	bool					setExternalContentForFileAttr( const MObject& attr, const MExternalContentLocationTable& );
	virtual	void			setExternalContent			 ( const MExternalContentLocationTable& );
	// SCRIPT USE ONLY
	MDataBlock				_forceCache		()				{ return forceCache(); }
	void					_setDoNotWrite	( bool flag )	{ setDoNotWrite(flag); }
	bool					_doNotWrite		()				{ return doNotWrite(); }
	//

	static MObject	message;					//!< message attribute
	static MObject	isHistoricallyInteresting;	//!< is historically interesting attribute
	static MObject	caching;					//!< caching attribute
	static MObject	state;						//!< state attribute
	static MObject	frozen;						//!< frozen attribute

	static const char*	    className();

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// DO NOT OVERRIDE in your derived class
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	virtual MTypeId			typeId		() const /*final*/;
	virtual MString			typeName	() const /*final*/;
	virtual MString			name		() const /*final*/;
	virtual MObject         thisMObject	() const /*final*/;
	
	virtual MStatus			setExistWithoutInConnections	( bool flag ) /*final*/;
	virtual bool			existWithoutInConnections		( MStatus* ReturnStatus = NULL ) const /*final*/;
	virtual MStatus			setExistWithoutOutConnections	( bool flag ) /*final*/;
	virtual bool			existWithoutOutConnections		( MStatus* ReturnStatus = NULL ) const /*final*/;

BEGIN_NO_SCRIPT_SUPPORT:
protected:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// DO NOT OVERRIDE in your derived class
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	//!	USE _forceCache() IN SCRIPT
	virtual MDataBlock		forceCache() /*final*/;
	//!	USE _setDoNotWrite() IN SCRIPT
	virtual MStatus			setDoNotWrite ( bool flag ) /*final*/;
	//!	USE _doNotWrite() IN SCRIPT
	virtual bool			doNotWrite( MStatus *ReturnStatus = NULL ) const /*final*/;

	OPENMAYA_DEPRECATED(2019, "Override schedulingType() to set thread safety instead.")
	virtual void			setMPSafe ( bool isMPSafe ) /*final*/;

	OPENMAYA_DEPRECATED(2018, "Use the method forceCache() instead. If needed, use MDGContextGuardto switch the context.")
	virtual MDataBlock		forceCache	( const MDGContext& ) /*final*/;
END_NO_SCRIPT_SUPPORT:

protected:

	void*			instance;
	SchedulingType	fSchedulingType;

private:
	static void		initialSetup();
	static void*	initClass;

public:
	//**********************************************************************
	// Obsolete variations of methods

	OPENMAYA_DEPRECATED(2019, "Override schedulingType() to set thread safety instead.")
	void					_setMPSafe		( bool isMPSafe );

	OPENMAYA_DEPRECATED(2018, "Use the method _forceCache() instead. If needed, use MDGContextGuard to switch the context.")
	MDataBlock				_forceCache	( const MDGContext& );

	OPENMAYA_DEPRECATED(2018, "Use the method getInternalValue(const MPlug&, MDataHandle&) instead. If needed, use MDGContext::current() to get the context.")
	virtual bool			getInternalValueInContext ( const MPlug&, MDataHandle&, MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use the method setInternalValue(const MPlug&, const MDataHandle&) instead. If needed, use MDGContext::current() to get the context.")
    virtual bool			setInternalValueInContext ( const MPlug&, const MDataHandle&, MDGContext&);
	
	OPENMAYA_DEPRECATED(2018, "Use the method internalArrayCount(const MPlug&) instead.")
    virtual int				internalArrayCount		  ( const MPlug&, const MDGContext&) const;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MPxNode */
