#ifndef _MItSubdFace
#define _MItSubdFace
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
// CLASS:    MItSubdFace
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MColor.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MVectorArray.h>
#include <maya/MColorArray.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MItSubdFace)

//! \ingroup OpenMaya
//! \brief Subdiv face iterator. 
/*!
	This class is the iterator for subdiv faces. It will iterate over all
	of the surface's faces.
*/
class OPENMAYA_EXPORT MItSubdFace
{
public:
	MItSubdFace(MObject & subdObject, MStatus * ReturnStatus = NULL);
	virtual ~MItSubdFace();
	MStatus		reset();
	MStatus		next();
	bool		isDone(MStatus * ReturnStatus = NULL);
	bool		isValid(MStatus * ReturnStatus = NULL) const;
	unsigned int level(MStatus * ReturnStatus = NULL) const;
	void setLevel(unsigned int level, MStatus * ReturnStatus = NULL);
	MUint64		index(MStatus * ReturnStatus = NULL);

	static const char* 	className();

protected:

private:
	void     *       f_it;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItSubdFace */



