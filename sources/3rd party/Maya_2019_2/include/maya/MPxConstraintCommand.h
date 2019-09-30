#ifndef _MPxConstraintCommand
#define _MPxConstraintCommand
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

//
// CLASS:    MPxConstraintCommand
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MDagPath.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxConstraintCommand)

//! \ingroup OpenMayaAnim MPx
//! \brief Proxy constraint command 
/*!
  MPxConstraintCommand is the base class for user defined commands which
  create constraints. This command gives all of the flags and options of the
  base constraint command and in addition allows user defined flags or
  behaviours. When registering this command, use the
  MFnPlugin::registerConstraintCommand() method. A MPxConstraint is
  also required to be used with MPxConstraintCommand. The
  constraintTypeId() virtual must be implemented to return the correct
  constraint node type id.
*/
class OPENMAYAANIM_EXPORT MPxConstraintCommand : public MPxCommand
{
public:

	//! Defines the type of target object
	enum TargetType
	{
		kTransform,			//!< Transform target
		kGeometryShape,		//!< Geometry shape(or children of) targets
		kLast				//!< Last value, used for counting
	};

						MPxConstraintCommand();
	virtual 			~MPxConstraintCommand();

	virtual MStatus		doIt(const MArgList &argList);
	virtual MStatus		undoIt();
	virtual MStatus		redoIt();

	virtual MStatus	appendSyntax();

	MPxConstraint *constraintNode();

	// FOR PYTHON USE ONLY
	MSyntax	_syntax(MStatus *ReturnStatus = NULL) const
		{ return syntax(ReturnStatus); }

protected:

	virtual MStatus				parseArgs(const MArgList &argList);

	//!	USE _syntax() IN PYTHON
	MSyntax	syntax(MStatus *ReturnStatus = NULL) const;

	virtual MStatus				doEdit();
	virtual MStatus				doCreate();
	virtual MStatus				doQuery();

	virtual void				createdConstraint(MPxConstraint *constraint);

	virtual bool				supportsOffset() const;

	virtual bool				hasVectorFlags() const;

	virtual MTypeId				constraintTypeId() const;
	virtual MPxConstraintCommand::TargetType targetType() const;

	virtual const MObject&		aimVectorAttribute() const;
	virtual const MObject&		upVectorAttribute() const;
	virtual const MObject&		worldUpMatrixAttribute() const;
	virtual const MObject&		worldUpTypeAttribute() const;
	virtual const MObject&		worldUpVectorAttribute() const;
	virtual const MObject&		offsetAttribute() const;
	virtual const MObject&		constraintInstancedAttribute() const;
	virtual const MObject&		constraintOutputAttribute() const;
	virtual const MObject&		constraintRestAttribute() const;
	virtual const MObject&		constraintEnableRestAttribute() const;
	virtual const MObject&		constraintTargetInstancedAttribute() const;

	virtual const MObject&		constraintTargetAttribute() const;
	virtual const MObject&		constraintTargetWeightAttribute() const;
	virtual const MObject&		objectAttribute() const;

	virtual void				getObjectAttributesArray(MObjectArray& array);

	virtual void				handleNewTargets(MObject& dagObject);
	virtual MStatus				handleNewTargets(MDagPath& dagObject);

	virtual MStatus				connectTarget(void *opaqueTarget, int index);
	virtual MStatus				connectTarget(MDagPath &targetPath, int index);

	virtual MStatus				connectObjectAndConstraint(MDGModifier& modifier);
	virtual MStatus				setRestPosition(MDGModifier& modifier);

	MStatus						connectGeometryAttribute(
									void *opaqueTarget,
									int index,
									MObject& constraintAttr
								);
	
	MStatus						connectTargetAttribute(
									void *opaqueTarget,	
									int index, 
									MObject& constraintAttr
								);

	MStatus						connectTargetAttribute(
									MDagPath &targetPath,
									int index, 
									MObject& tarAttr, 
									MObject& constraintAttr, 
									bool instanced = false
								);

	MStatus						connectObjectAttribute(
									const MObject& objectAttr,
									const MObject& constraintAttr,
									bool toConstraint = true,
									bool instanced = false
								);

	const MObject				transformObject();

private:


	void* instance;
	void setData(void *ptr);
	MPxConstraint *fUserNode;
};


/*!
	Returns the custom MPxConstraint-derived node created by this instance
	of the command. This can be useful if the command needs to access methods
	or members of the node.

	\return
	The created node.
*/
inline MPxConstraint* MPxConstraintCommand::constraintNode()
{
	return fUserNode;
}

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxConstraintCommand */
