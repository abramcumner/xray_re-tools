#ifndef _MFnDoubleIndexedComponent
#define _MFnDoubleIndexedComponent
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
// CLASS:    MFnDoubleIndexedComponent
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
// CLASS DECLARATION (MFnDoubleIndexedComponent)

//! \ingroup OpenMaya MFn
//! \brief Double indexed component function set.
/*!
  This function set allows you to create, edit, and query double
  indexed components. Double indexed components store 2 dimensional
  index values.
*/
class OPENMAYA_EXPORT MFnDoubleIndexedComponent : public MFnComponent
{
	declareMFn( MFnDoubleIndexedComponent, MFnComponent );

public:

	// Create a double indexed component of the given type.
	// Allowable types are
	//
	//    MFn::kSurfaceCVComponent
	//    MFn::kSurfaceEPComponent
	//    MFn::kSurfaceKnotComponent
	//    MFn::kMeshVtxFaceComponent
	//
	MObject		create( MFn::Type compType, MStatus * ReturnStatus = NULL );

    MStatus 	addElement( int uIndex, int vIndex );
    MStatus 	addElements( const MIntArray& uIndexArray,
							 const MIntArray& vIndexArray );

    MStatus		getElement( int index, int & uIndex, int & vIndex ) const;
    MStatus 	getElements( MIntArray& uIndexArray,
							 MIntArray& vIndexArray ) const;

	MStatus		setCompleteData( int maxU, int maxV );
	MStatus		getCompleteData( int & maxU, int & maxV ) const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	declareMFnConstConstructor( MFnDoubleIndexedComponent, MFnComponent );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnDoubleIndexedComponent */
