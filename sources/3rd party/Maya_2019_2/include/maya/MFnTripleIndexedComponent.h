#ifndef _MFnTripleIndexedComponent
#define _MFnTripleIndexedComponent
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
// CLASS:    MFnTripleIndexedComponent
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
// CLASS DECLARATION (MFnTripleIndexedComponent)

//! \ingroup OpenMaya MFn
//! \brief Triple indexed component function set. 
/*!
This function set allows you to create, edit, and query triple indexed
components. Triple indexed components store 3 dimensional index
values.
*/
class OPENMAYA_EXPORT MFnTripleIndexedComponent : public MFnComponent
{
	declareMFn( MFnTripleIndexedComponent, MFnComponent );

public:

	// Create a triple indexed component of the given type.
	// Allowable types are
	//
	//    MFn::klatticeComponent
	//
	MObject		create( MFn::Type compType, MStatus * ReturnStatus = NULL );

    MStatus 	addElement( int sIndex, int tIndex, int uIndex );
    MStatus 	addElements( const MIntArray& sIndexArray,
							 const MIntArray& tIndexArray,
							 const MIntArray& uIndexArray );

    MStatus		getElement( int index,
							int & sIndex, int & tIndex, int & uIndex ) const;
    MStatus 	getElements( MIntArray& sIndexArray,
							 MIntArray& tIndexArray,
							 MIntArray& uIndexArray ) const;

	MStatus		setCompleteData( int maxS, int maxT, int maxU );
	MStatus		getCompleteData( int & maxS, int & maxT, int & maxU ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnTripleIndexedComponent, MFnComponent );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnTripleIndexedComponent */
