#ifndef _MFnComponent
#define _MFnComponent
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
// CLASS:    MFnComponent
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MString.h>
#include <maya/MWeight.h>
#include <maya/MDeprecate.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnComponent)

//! \ingroup OpenMaya MFn
//! \brief Base class for component function sets.
/*!
This is the base class for all function sets which deal with
component objects.

Components are MObjects which hold index information for shapes.
The component object does not contain any information about the
shape, it just contains index values and type information.

Components can be marked as complete meaning that the component
represents a complete set of indices from 0 to numElements-1;

Components come is 3 varieties based upon the dimensions of the
index. The types are single, double, and triple indexed.
Examples of these types are mesh vertices (single indexed),
nurbs surface CVs (double indexed), and lattice points (triple indexed).

To create, edit, or query a specific type of component, use the
derived classes MFnSingleIndexedComponent, MFnDoubleIndexedComponent,
and MFnTripleIndexedComponent.
*/
class OPENMAYA_EXPORT MFnComponent : public MFnBase
{
	declareMFnNoVoidConstructor( MFnComponent, MFnBase );

public:
	MFnComponent();
	int        elementCount( MStatus* ReturnStatus = NULL ) const;
	MFn::Type	componentType( MStatus * ReturnStatus = NULL ) const;
	bool		isEmpty( MStatus * ReturnStatus = NULL ) const;
	bool		isEqual( MObject &, MStatus * ReturnStatus = NULL ) const;
	bool		isComplete( MStatus * ReturnStatus = NULL ) const;
	MStatus		setComplete( bool );

	bool		hasWeights() const;
	MWeight		weight( int index, MStatus * ReturnStatus = NULL ) const;
	MStatus		setWeight( int index, const MWeight& weight );
	MStatus		setWeights( const MWeight& weight ); 

BEGIN_NO_SCRIPT_SUPPORT:

	declareMFnNoVoidConstructorConstConstructor( MFnComponent, MFnBase );

	//!	Obsolete & no script support
	OPENMAYA_DEPRECATED(2019, "Use the MFnComponent::componentType method instead.")
	MFn::Type	type( MStatus * ReturnStatus = NULL );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnComponent */
