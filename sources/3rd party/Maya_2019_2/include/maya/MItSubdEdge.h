#ifndef _MItSubdEdge
#define _MItSubdEdge
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
// CLASS:    MItSubdEdge
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MItSubdEdge)

//! \ingroup OpenMaya
//! \brief Subdiv edge iterator. 
/*!
	This class is the iterator for subdiv edges.
*/
class OPENMAYA_EXPORT MItSubdEdge
{
public:
	MItSubdEdge( MObject & subdObject, MStatus * ReturnStatus = NULL );
	virtual ~MItSubdEdge();

	MStatus		reset();
	MStatus		next();
	bool		isDone( MStatus * ReturnStatus = NULL );
	bool isValid(MStatus * ReturnStatus = NULL) const;

	int level(MStatus * ReturnStatus = NULL) const;
	void setLevel( int level, MStatus * ReturnStatus = NULL );

	MUint64		index( MStatus * ReturnStatus = NULL );

	bool		isSharp( MStatus * ReturnStatus = NULL );
	void		setSharpness( bool sharp, MStatus * ReturnStatus = NULL );
	bool		isBoundary( MStatus * ReturnStatus = NULL );

	static const char* 	className();

protected:

private:
	void     *       f_it;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItSubdEdge */



