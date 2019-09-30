#ifndef _MObjectListFilter
#define _MObjectListFilter
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
// CLASS:    MObjectListFilter
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MObjectListFilter)
//
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MString.h>

// ****************************************************************************
// DECLARATIONS

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MObjectListFilter)
/*! \ingroup OpenMayaUI

	\brief Class for defining a scene list filter

	MObjectListFilter provides an interface to 
	define a list of selection items which can be used to filter
	the display of items for interactive 3D scene rendering.

	The selection list can either have the meaning of rendering
	only the items in that list (an inclusion list) or only rendering items 
	which are not in the list (an exclusion list).

	Programmers using this interface can derive from this class 
	and implement the required methods. The derived class is responsible for 

	- Registering and deregistering the filter via the registerFilter() and deregisterFilter()
	  methods.
	- Maintaining the selection list to be used for filtering.
	- Indicating when this list has been modified via the requireListUpdate() method.
	  Note that it is valid to always mark the list as being modified.
	- Return the selection list when asked via the getList() method.
*/
class OPENMAYAUI_EXPORT MObjectListFilter
{
public:
	//! Type of filter list
	typedef enum {
		kInclusionList = 0,		//!< Include only items on the list
		kExclusionList,			//!< Exclude only items on the list.
		kNumberOfFilterTypes	//!< Not to be used. This is the number of filter types.
	} MFilterType;

	MObjectListFilter( const MString &name );
	virtual ~ MObjectListFilter();

	// Query to determine if the list requires updating
	virtual bool requireListUpdate() = 0;

	// The list for filtering
	virtual MStatus getList(MSelectionList &list) = 0;

	//! Type of scene update
	typedef enum {
		kNone = 0,					//!< List update is not dependent on scene changes
		kAddRemoveObjects = 1<<(0)	//!< List update is dependent on addition or removal of dag objects from the scene
	} MSceneUpdateType;
	// Query if the list update is dependent on scene updates
	virtual MSceneUpdateType dependentOnSceneUpdates();

	// Get / set filter type
	void setFilterType( MFilterType filterType );
	MFilterType filterType() const;

	// Name identifier
	const MString &name() const;

	// UI visible name
	const MString &UIname() const;
	void setUIName( const MString &name );

	// Register and deregister a filter
	static MStatus registerFilter( const MObjectListFilter & filter );
	static MStatus deregisterFilter( const MObjectListFilter & filter );        

	// Class name
	static const char* className();

protected:
	MString         mName;
	MString			mUIName;
	MFilterType     mFilterType;
private:
	// Private methods
	MObjectListFilter();
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MObjectListFilter */
