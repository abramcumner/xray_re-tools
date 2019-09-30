#ifndef _MItDependencyNodes
#define _MItDependencyNodes
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
// CLASS:    MItDependencyNodes
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MIntArray.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MItDependencyNodes)

//! \ingroup OpenMaya
//! \brief Dependency Node iterator. 
/*!
Use the dependency node iterator to traverse all the nodes in Maya's
Dependency Graph.

With filtering enabled, the iterator checks to see if the node is
compatible with the type specified by the filter.  See MFn::Type for a
list of all valid types.

Since MObjects may be compatible with more than one type (nodes are
organised in a hierarchy) the MObject::hasFn() method can be used to
further check for compatible types.

Any compatible Function Set can be attached to the retrieved object to
query or or edit it.  Often you will want to use the dependency node
function set (MFnDependencyNode), which is compatible with all
dependency nodes, to perform queries on each node as the iterator
Traverses the Dependency Graph.
*/
class OPENMAYA_EXPORT MItDependencyNodes
{
public:
				MItDependencyNodes( MFn::Type filter = MFn::kInvalid,
									MStatus * ReturnStatus = NULL );
				MItDependencyNodes( MIteratorType& infoObject,
									MStatus * ReturnStatus = NULL );
	virtual		~MItDependencyNodes();

	MStatus		reset( MFn::Type filter = MFn::kInvalid );
	MStatus		reset(MIteratorType& infoObject);

	MObject		thisNode( MStatus * ReturnStatus = NULL ) const;
	MStatus		next();
	bool		isDone( MStatus * ReturnStatus = NULL ) const;

    // obsolete
    OPENMAYA_DEPRECATED(2019, "Use the method MItDependencyNodes::thisNode instead.")
	MObject		item( MStatus * ReturnStatus = NULL ) const;

	static const char* className();

protected:
// No protected members

private:
	bool objectHasFnOnFilterOrList(const MObject& mobject);

	void*		f_data;
	MFn::Type	f_filter;

	MIntArray	filterList;
	bool		useFilterList;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItDependencyNodes */
