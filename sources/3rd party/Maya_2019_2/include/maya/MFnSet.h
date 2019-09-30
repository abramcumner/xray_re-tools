#ifndef _MFnSet
#define _MFnSet
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
// CLASS:    MFnSet
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MFnSet)
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MString.h>
#include <maya/MObject.h>

// ****************************************************************************
// DECLARATIONS

class TsetCmd;
class Tstring;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnSet)

//! \ingroup OpenMaya MFn
//! \brief Function Set for Sets of Objects. 
/*!
  MFnSet is the function set that is used for manipulating sets of objects.
  Sets in Maya are dependency nodes, so it is possible for one set to
  contain others.

  A set is a list of dependency nodes and dag nodes.  Sets are useful for
  keeping track of lists of objects for many purposes such as selection
  or applying common rendering parameters.  Sets in Maya are also dependency
  nodes, so it is possible for one set to contain others.
*/
class OPENMAYA_EXPORT MFnSet : public MFnDependencyNode
{
	declareMFn(MFnSet, MFnDependencyNode);

public:
	//! Set membership restrictions.
	enum Restriction {
		kNone,			//!< \nop
		kVerticesOnly,		//!< \nop
		kEdgesOnly,		//!< \nop
		kFacetsOnly,		//!< \nop
		kEditPointsOnly,	//!< \nop
		kRenderableOnly		//!< \nop
	};

	MObject     create( const MSelectionList & members,
						         Restriction restriction = kNone,
						         MStatus * ReturnStatus = NULL );

	MStatus     getUnion( const MObject & withSet, MSelectionList & result );
	MStatus     getUnion( const MObjectArray & setList,
						  MSelectionList & result );
	MStatus     getIntersection( const MObject & withSet,
								 MSelectionList & result );
	MStatus     getIntersection( const MObjectArray & setList,
								 MSelectionList & result );

	MStatus     clear();
	MStatus     getMembers( MSelectionList &members, bool flatten ) const;
	MStatus     addMember( const MObject &obj );
	MStatus     addMember( const MDagPath &obj,
						   const MObject &component = MObject::kNullObj );
	MStatus     addMember( const MPlug &plug );
	MStatus     addMembers( const MSelectionList &list );
	MStatus     removeMember( const MObject &obj );
	MStatus     removeMember( const MDagPath &obj, const MObject &component );
	MStatus     removeMember( const MPlug &plug );
	MStatus     removeMembers( const MSelectionList &list );

	bool        isMember( const MObject &object,
						  MStatus * ReturnStatus = NULL ) const;
	bool        isMember( const MDagPath &object,
						  const MObject &component = MObject::kNullObj,
						  MStatus * ReturnStatus = NULL ) const;
	bool        isMember( const MPlug &plug,
						  MStatus * ReturnStatus = NULL ) const;

	bool        intersectsWith( const MObject & otherSet,
								 MStatus * ReturnStatus = NULL ) const;
	bool        hasRestrictions( MStatus * ReturnStatus = NULL ) const;
	Restriction restriction( MStatus * ReturnStatus = NULL ) const;
	MString     annotation( MStatus * ReturnStatus = NULL ) const;
	MStatus     setAnnotation( const MString &annotation );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnSet, MFnDependencyNode );

	// Obsolete, no script support
	MObject     create( const MSelectionList & members,
						         Restriction restriction = kNone,
						         bool isLayer = false,
						         MStatus * ReturnStatus = NULL );

END_NO_SCRIPT_SUPPORT:

protected:
	// for use for api implementation of classes derived from setCmd
	//
	virtual		Tstring setCommandString();
	virtual		TsetCmd* setCommand();
private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnSet */
