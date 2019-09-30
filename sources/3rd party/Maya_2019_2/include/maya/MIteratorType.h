#ifndef _MIteratorType
#define _MIteratorType
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
// CLASS:    MIteratorType
//
// ****************************************************************************

#include <maya/MIntArray.h>
#include <maya/MTypes.h>
#include <maya/MStatus.h>
#include <maya/MFn.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMaya
//! \brief MIteratorType class 
/*!
The MIteratorType class is used on iterators where more than one type
of filters can be specified. It also provides functionalities to set and
get the filter list or individual types of filter. This class should be
used in conjunction with DAG/DG/DependencyNodes iterators for using filter
list (list of MFn::Type objects) on them, thus enabling faster traversal
thro' iterators.

Also, the class has functionalities for specifying the type of object the
iterator will be reset to. This could be an MObject, an MPlug or an MDagPath.
*/
class OPENMAYA_EXPORT MIteratorType
{
public:

	//! Type of object the iterator deals with.
	enum objFilterType
	{
		//! This is to indicate that the root of the iterator has to be reset to an MObject
		kMObject,
		//! This is to indicate that the root of the iterator has to be reset to an MDagPath object.
		kMDagPathObject,
		//! This is to indicate that the root of the iterator has to be reset to an MPlug object.
		kMPlugObject
	};

						MIteratorType(MStatus* ReturnStatus = NULL);
						MIteratorType( const MIteratorType &, MStatus* ReturnStatus = NULL);
						~MIteratorType();
	void				setFilterType(MFn::Type type, MStatus* ReturnStatus = NULL);
	void				setFilterList(MIntArray& listOfFilters, MStatus* ReturnStatus = NULL);
	void				setObjectType(objFilterType objType, MStatus* ReturnStatus = NULL);
	MFn::Type			getFilterType(MStatus* ReturnStatus = NULL);
	MStatus				getFilterList(MIntArray& listOfFilters);
	MIteratorType::objFilterType
						getObjectType(MStatus* ReturnStatus = NULL);
	bool				filterListEnabled();

private:
	MFn::Type			filterType;
	MIntArray			filterList;
	bool				useFilterList;
	objFilterType		objType;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* _MIteratorType */
