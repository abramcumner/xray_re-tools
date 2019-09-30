#ifndef _MFnSingleIndexedComponent
#define _MFnSingleIndexedComponent
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
// CLASS:    MFnSingleIndexedComponent
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MString.h>
#include <maya/MFnComponent.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnSingleIndexedComponent)

//! \ingroup OpenMaya MFn
//! \brief Single indexed component function set. 
/*!
This function set allows you to create, edit, and query single indexed
components. Single indexed components store 1 dimensional index
values.
*/
class OPENMAYA_EXPORT MFnSingleIndexedComponent : public MFnComponent
{
	declareMFn( MFnSingleIndexedComponent, MFnComponent );

public:

	// Create a single indexed component of the given type.
	// Allowable types are
	//
	//    MFn::kCurveCVComponent
	//    MFn::kCurveEPComponent
	//    MFn::kCurveKnotComponent
	//    MFn::kMeshEdgeComponent
	//    MFn::kMeshPolygonComponent
	//    MFn::kMeshVertComponent
	//	  MFn::kMeshMapComponent
	//
	MObject		create( MFn::Type compType, MStatus * ReturnStatus = NULL );

    MStatus 	addElement( int element );
    MStatus 	addElements( MIntArray& elements );

    int			elementMax( MStatus * ReturnStatus = NULL ) const;
    int			element( int index, MStatus * ReturnStatus = NULL ) const;
    MStatus		getElements( MIntArray& elements ) const;

	MStatus		setCompleteData( int numElements );
	MStatus		getCompleteData( int & numElements ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnSingleIndexedComponent, MFnComponent );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnSingleIndexedComponent */
