#ifndef _MItSelectionList
#define _MItSelectionList
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
// CLASS:    MItSelectionList
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MStringArray.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS


// ****************************************************************************
// CLASS DECLARATION (MItSelectionList)

//! \ingroup OpenMaya
//! \brief Iterate over the items in the selection list. 
/*!
Class for iterating over the items in an MSelection list.
A filter can be specified so that only those items of interest on a selection
list can be obtained.

If a filter is specified then the children of DAG selection items will be
searched if the selection item does not match the filter.
For example, if  <i>filter = MFn::kNurbsCurve</i>  and a transform is selected
then the underlying shape for the transform will be in the iteration if it
is a nurbs curve.

<b>Example:</b> (of a simple traversal)

\code
    MSelectionList activeList;
    MGlobal::activeSelectionList(activeList);
    MItSelectionList iter( activeList );

    for ( ; !iter.isDone(); iter.next() )
    {
        MDagPath item;
        MObject component;
        iter.getDagPath( item, component );

        // do something with it
    }
\endcode


<b>Example:</b> (of a traversal to find all active cameras)

\code
    MSelectionList activeList;
    MGlobal::activeSelectionList(activeList);
    MItSelectionList iter( activeList, MFn::kCamera );

    for ( ; !iter.isDone(); iter.next() )
    {
        MDagPath camera;
        iter.getDagPath( camera );

        MFnCamera cameraFn( camera );

        // ...
   }
\endcode
*/
class OPENMAYA_EXPORT MItSelectionList
{
public:
	//! Types of items which can appear on a selection list.
	enum selItemType{
		kUnknownItem = -1,
		kDagSelectionItem,	//!< DAG node or component
		kAnimSelectionItem,	//!< keyset
		kDNselectionItem,	//!< dependency node
		kPlugSelectionItem	//!< plug
	};
	bool		isDone( MStatus * ReturnStatus = NULL );
	MStatus		next();
	MStatus		reset();
	MStatus		getDependNode( MObject &depNode );
	MStatus		getDagPath( MDagPath &dagPath, MObject &component );
	MStatus     getDagPath( MDagPath &dagPath );
	MStatus     getPlug( MPlug &plug );
	MStatus     getStrings( MStringArray & array );
	selItemType itemType( MStatus * ReturnStatus = NULL );
	MStatus		setFilter( MFn::Type filter );
	bool		hasComponents( MStatus * ReturnStatus = NULL ) const;
	MItSelectionList( 	const MSelectionList & list,
						MFn::Type = MFn::kInvalid,
						MStatus * ReturnStatus = NULL );
	virtual ~MItSelectionList();

	static const char* 	className();

protected:
// No protected members

private:
	MObject			findMObject(void *);
	MStatus				resetInner();
	void*				f_main_iter;
	void*				f_component_iter;
	void*				list_data;
	MFn::Type			f_filter;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MItSelectionList */
