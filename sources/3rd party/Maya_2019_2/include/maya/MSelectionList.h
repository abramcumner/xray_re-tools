#ifndef _MSelectionList
#define _MSelectionList
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
// CLASS:    MSelectionList
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MSelectionList)

//! \ingroup OpenMaya
//! \brief A list of MObjects. 
/*!
This class implements a list of MObjects.

The global selection list is a special case where the objects on the list
are also active objects in Maya.

Besides the usual list methods, this class also provides an add method
which retrieves objects from Maya, such as dependency nodes, by name.
*/
class OPENMAYA_EXPORT MSelectionList
{
public:
	//! Specifies how to merge objects with those already in the list.
	enum MergeStrategy {
		//! If the object is not already on the list, add it.
		kMergeNormal=0,

		//! Exclusive OR: if the object is already on the list,
		//! remove it, otherwise add it.
		kXORWithList,

		//! Remove the object from the list.
		kRemoveFromList
	};

	MSelectionList();
	MSelectionList( const MSelectionList & src );

	virtual ~MSelectionList();

	MStatus			clear	();
	bool			isEmpty	( MStatus * ReturnStatus = NULL ) const;
	unsigned int	length	( MStatus * ReturnStatus = NULL ) const;
	MStatus		    getDependNode ( unsigned int index, MObject &depNode ) const;
	MStatus		    getDagPath    ( unsigned int index, MDagPath &dagPath,
								    MObject &component = MObject::kNullObj
									) const;
	MStatus		    getPlug	( unsigned int index, MPlug &plug ) const;

	MStatus			add		( const MObject & object,
							  const bool mergeWithExisting = false );
	MStatus			add		( const MDagPath & object,
							  const MObject & component = MObject::kNullObj,
							  const bool mergeWithExisting = false );
	MStatus         add     ( const MString & matchString,
							  const bool searchChildNamespacesToo = false );
	MStatus			add		( const MPlug & plug,
							  const bool mergeWithExisting = false );
	MStatus			add		( const MUuid & uuid, 
							  bool mergeWithExisting = false );

	MStatus			remove	( unsigned int index );
	MStatus			replace	( unsigned int index, const MObject & item );
	MStatus			replace	( unsigned int index,
							  const MDagPath& item,
							  const MObject& component = MObject::kNullObj );
	MStatus			replace	( unsigned int index, const MPlug & plug );

	bool			hasItem ( const MObject & item,
							  MStatus* ReturnStatus = NULL ) const;
	bool			hasItem ( const MDagPath& item,
							  const MObject& component = MObject::kNullObj,
							  MStatus* ReturnStatus = NULL ) const;
	bool			hasItem ( const MPlug & plug,
							  MStatus* ReturnStatus = NULL ) const;

	bool			hasItemPartly ( const MDagPath& item,
									const MObject& component,
									MStatus* ReturnStatus = NULL ) const;
	MStatus			toggle ( const MDagPath& item,
							 const MObject& component = MObject::kNullObj );

	MSelectionList& operator =( const MSelectionList& other );

	MStatus			merge( const MSelectionList& other,
						   const MergeStrategy strategy = kMergeNormal );
	MStatus			merge( const MDagPath& object,
						   const MObject& component = MObject::kNullObj,
						   const MergeStrategy strategy = kMergeNormal );

	MStatus			intersect ( const MSelectionList& other,
							    bool expandToLeaves = false );

	MStatus         getSelectionStrings( MStringArray & array ) const;
	MStatus         getSelectionStrings( unsigned int index,
										 MStringArray & array ) const;

	static const char* className();

protected:
// No protected members

private:
	MSelectionList( void * );
	void merge( const void*, const MergeStrategy strategy );
	void * list_data;
	bool fOwn;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MSelectionList */
