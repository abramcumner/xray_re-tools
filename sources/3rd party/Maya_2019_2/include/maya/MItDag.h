#ifndef _MItDag
#define _MItDag
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
// CLASS:    MItDag
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
// CLASS DECLARATION (MItDag)

//! \ingroup OpenMaya
//! \brief DAG Iterator. 
/*!
Use the DAG iterator to traverse the DAG either depth first or breadth
first, visiting each node and, if desired, retrieving the node (as an
MObject).  The DAG iterator provides a basic filtering capability, so
that DAG node retrieval can be limited to a  specific type (MFn::Type)
of node.  With filtering enabled the iterator checks to see if the node
is compatible with the type of Function Set specified by the filter.
See MFn::Type for a list of all valid Function set types.

Since each object, if retrieved, is returned as an MObject, the
MObject::hasFn() method can be used to further check for compatible
function set types since an MObjects may be compatible with more than
one function set).

Any compatible Function Set can be attached to the retrieved object to
query or or edit it.  Often you will want to use the DAG node Function
Set (MFnDagNode), which is compatible with all DAG objects, to perform
basic queries on each node as the iterator traverses the DAG.

The iterator also provides the capability to reset the root of the
iteration, the type of traversal, and the filter.

Additionally, the iterator can be queried for the root, mode and type
of traversal, and to determine if the the traversal has been completed.
*/
class OPENMAYA_EXPORT MItDag
{
public:
    //! Order in which nodes should be traversed.
	enum TraversalType {
		kInvalidType = 0,	//!< \nop
		kDepthFirst,		//!< Visit a node's children before moving on to its siblings.
		kBreadthFirst		//!< Visit a node's siblings before moving on to its children. Note: using kBreadthFirst instead of kDepthFirst can be significantly slower, especially when traversing whole hierarchies.
	};
	MItDag( TraversalType = kDepthFirst,
			MFn::Type = MFn::kInvalid,
			MStatus * ReturnStatus = NULL );
	MItDag( MIteratorType& dagInfoObject,
			TraversalType = kDepthFirst,
			MStatus* ReturnStatus = NULL);
	MStatus       reset();
	MStatus       reset( const MObject & object,
						 TraversalType = kDepthFirst,
						 MFn::Type = MFn::kInvalid );
	MStatus       reset( const MDagPath & path,
						 TraversalType = kDepthFirst,
						 MFn::Type = MFn::kInvalid );
	MStatus	      reset(MIteratorType& dagInfoObject,
						const MObject* object,
						const MDagPath* path,
						TraversalType = kDepthFirst);

    // obsolete
    OPENMAYA_DEPRECATED(2019, "Use the MItDag::currentItem method instead.")
	MObject       item( MStatus * ReturnStatus = NULL );
	MObject       currentItem( MStatus * ReturnStatus = NULL );
	MStatus       next();

	MStatus       prune();
	bool          isDone( MStatus * ReturnStatus = NULL ) const;
	MObject       root( MStatus * ReturnStatus = NULL );
	unsigned int      depth( MStatus * ReturnStatus = NULL ) const;
	TraversalType getType( MStatus * ReturnStatus = NULL ) const;

	MStatus       getPath( MDagPath& path ) const;
	MStatus       getAllPaths( MDagPathArray& paths ) const;
	MString       fullPathName(MStatus *ReturnStatus = NULL) const;
    MString       partialPathName(MStatus *ReturnStatus = NULL) const;
	bool          isInstanced( bool indirect = true,
                               MStatus * ReturnStatus = NULL ) const;
	unsigned int  instanceCount( bool total,
				                 MStatus * ReturnStatus = NULL ) const;
	MStatus	      traverseUnderWorld( bool flag );
	bool          willTraverseUnderWorld( MStatus * ReturnStatus = NULL ) const;

	virtual ~MItDag();

	static const char* className();

protected:
// No protected members

private:
	bool objectHasFnOnFilterOrList(const MObject& mobject);
	MStatus filterObject(const MObject & object, MItDag::TraversalType type);
	MStatus filterDagPath(const MDagPath & path, MItDag::TraversalType type);
	void*		iterator_data;
	MFn::Type	type_filter;
    bool        df;

	MIntArray	filterList;
	bool		useFilterList;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItDag */
