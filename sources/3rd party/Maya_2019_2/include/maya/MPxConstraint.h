#ifndef _MPxConstraint
#define _MPxConstraint
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
// CLASS:    MPxConstraint
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


class TdataGeometry;

#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MEulerRotation.h>
#include <maya/MQuaternion.h>
#include <maya/MPxTransform.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxConstraint)

//! \ingroup OpenMayaAnim MPx
//! \brief Proxy constraint node 
/*!
  MPxConstraint is the parent class for user defined constraint nodes.
  Position, orientation or scale of an object can be constrained by
  other objects. This class works in conjunction with the
  MPxConstraintCommand class.
*/
class OPENMAYAANIM_EXPORT MPxConstraint : public MPxNode
{
public:

	//! The type of up vector
	enum UpVectorType
	{
		kScene = 0,			//!< Scene up vector
		kObject,			//!< Object up vector
		kObjectRotation,	//!< Object rotation up vector
		kVector,			//!< Specified vector
		kLast				//!< Last value, used for counting
	};

	MPxConstraint();
	virtual ~MPxConstraint();


	virtual const MObject weightAttribute() const;
    virtual const MObject targetAttribute() const;
    virtual const MObject constraintRotateOrderAttribute() const;

	//! Node attribute: enableRestPosition/erp - boolean
	static MObject enableRestPosition;

	//! Node attribute: lockOutput/lo - boolean
	static MObject lockOutput;

	static const char*	    className();

protected:
    virtual const MObject passiveOutputAttribute() const;
	virtual void getOutputAttributes(MObjectArray& attributeArray);

	// Utility methods

	MStatus	closestPoint( const MObject& dataGeometryObject,
								const MPoint& toThisPoint,
								MPoint& theClosestPoint ) const;
	MStatus closestNormal( const MObject& dataGeometryObject,
								const MPoint& toThisPoint,
								MVector& theNormal ) const;
	MStatus closestTangent( const MObject& dataGeometryObject,
								 const MPoint& toThisPoint,
								 MVector& theTangent ) const;

	MEulerRotation computeAim( const MMatrix& parentInverseMatrix,
							   const MVector& targetVector,
							   const MVector& aimVector,
							   const MVector& upVector,
							   const MVector& wupVector,
							   MEulerRotation::RotationOrder order = MEulerRotation::kXYZ,
							   const MQuaternion& jointOrient = MQuaternion::identity,
							   MStatus* ResultStatus = NULL ) const;
	MVector worldUpVector( MPxConstraint::UpVectorType upType,
								const MVector& upVector,
								const MMatrix& upMatrix,
								const MPoint& constrPoint,
								MStatus* ResultStatus = NULL ) const;
	MPoint worldConstraintPoint( const MMatrix& parentInverseMatrix,
									const MVector& translate,
									const MVector& rotatePivot,
									const MVector& rotatePivotTranslate,
									MStatus* ResultStatus = NULL ) const;

	void* instance;

private:
	static void				initialSetup();


	TdataGeometry *convert( const MObject& dataGeometryObject ) const;
	void setData(void *ptr);
	void setConstraintData(void *ptr);
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxConstraint */
