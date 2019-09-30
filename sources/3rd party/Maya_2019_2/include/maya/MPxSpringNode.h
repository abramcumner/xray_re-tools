#ifndef _MPxSpringNode
#define _MPxSpringNode
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
// CLASS:    MPxSpringNode
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



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxSpringNode)

//! \ingroup OpenMayaFX MPx
//! \brief Base class for user defined spring law 
/*!
	MPxSpringNode allows the creation and manipulation of dependency
	graph nodes representing a spring law.  The class is a DAG node
	and Maya manages the drawing, creation, and selection of springs.

    A user defined spring node is a DAG node that can have attributes
    and a applySpringLaw() method. To derive the full benefit
    of the MPxSpringNode class, it is suggested that you do not write
    your own compute() method. Instead, write the applySpringLaw() method.
    All of the parameters passed into this method will be supplied by Maya.

    To create a user defined spring node, derive from this class
    and override the applySpringLaw() method. The other methods of
    the parent class MPxNode may also be overridden to perform
    dependency node capabilities.
*/
class OPENMAYAFX_EXPORT MPxSpringNode : public MPxNode
{
public:
	MPxSpringNode();
	virtual ~MPxSpringNode();

	virtual MPxNode::Type	type() const;

	virtual MStatus	applySpringLaw( double stiffness,
						double damping, double restLength,
						double endMass1, double endMass2,
						const MVector &endP1, const MVector &endP2,
						const MVector &endV1, const MVector &endV2,
						MVector &forceV1, MVector &forceV2 );


	// ************************************************************
	// Inherited attributes
	// ************************************************************

	// General attributes for spring.
	//
	//! from end weight attribute
	static MObject	mEnd1Weight;
	//! to end weight attribute
	static MObject	mEnd2Weight;

	//! delta time attribute
	static MObject	mDeltaTime;

	static const char*	className();

protected:
// No protected members

private:
// No private members

	static void			initialSetup();

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxSpringNode */

