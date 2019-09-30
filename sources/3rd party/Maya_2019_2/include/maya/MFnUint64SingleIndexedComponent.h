#ifndef _MFnUint64SingleIndexedComponent
#define _MFnUint64SingleIndexedComponent
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
// CLASS:    MFnUint64SingleIndexedComponent
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MString.h>
#include <maya/MFnComponent.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// *****************************************************************************
// CLASS DECLARATION (MFnUint64SingleIndexedComponent)

//! \ingroup OpenMaya MFn
//! \brief Single MUint64 indexed component function set. 
/*!
This function set allows you to create, edit, and query single MUint64
indexed components. Single indexed components store 1 dimensional index
values, where each value is an MUint64.
*/
class OPENMAYA_EXPORT MFnUint64SingleIndexedComponent : public MFnComponent 
{
	declareMFn( MFnUint64SingleIndexedComponent, MFnComponent );

public:

	// Create a single indexed component of the given type.
	// Allowable types are
	//
	//    MFn::kSubdivCVComponent
	//    MFn::kSubdivEdgeComponent
	//    MFn::kSubdivFaceComponent
	//
	MObject		create( MFn::Type compType, MStatus * ReturnStatus = NULL );

    MStatus 	addElement( MUint64 element );
    MStatus 	addElements( const MUint64Array& elements );

    MUint64		element( int index, MStatus * ReturnStatus = NULL ) const;
    MStatus		getElements( MUint64Array& elements ) const;

	MStatus		setCompleteData( MUint64 numElements );
	MStatus		getCompleteData( MUint64 & numElements ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnUint64SingleIndexedComponent, MFnComponent );
	
END_NO_SCRIPT_SUPPORT: 

protected:
// No protected members

private:
// No private members
 
};

// *****************************************************************************

OPENMAYA_NAMESPACE_CLOSE
#endif /* __cplusplus */
#endif /* _MFnUint64SingleIndexedComponent */
