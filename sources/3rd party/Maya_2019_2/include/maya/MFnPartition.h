#ifndef _MFnPartition
#define _MFnPartition
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
// CLASS:    MFnPartition
//

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MString.h>
#include <maya/MObject.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnPartition)

//! \ingroup OpenMaya MFn
//! \brief Function set for partitions of objects. 
/*!

  MFnPartition is the function set that is used for manipulating
  partitions of objects.

  A partition is a list of dependency nodes and dag nodes.  Partitions
  are useful for keeping track of lists of objects for many purposes
  such as selection or applying common rendering parameters.
  Partitions in Maya are also dependency nodes, so it is possible for
  one partition to contain others.
*/
class OPENMAYA_EXPORT MFnPartition : public MFnDependencyNode
{
	declareMFn(MFnPartition, MFnDependencyNode);

public:

	enum Restriction {
		kNone,
		kVerticesOnly,
		kEdgesOnly,
		kFacetsOnly,
		kEditPointsOnly,
		kRenderableOnly
	};

	MObject     create( bool isRenderPartition = false,
						MStatus * ReturnStatus = NULL );
	bool        isRenderPartition( MStatus * ReturnStatus = NULL ) const;
	MStatus     addMember( const MObject &set );
	MStatus     removeMember( const MObject &set );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnPartition, MFnDependencyNode );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnPartition */
