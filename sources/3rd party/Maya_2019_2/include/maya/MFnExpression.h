#ifndef _MFnExpression
#define _MFnExpression
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
// CLASS:    MFnExpression
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MString.h>

// ****************************************************************************
// DECLARATIONS

class MPtrBase;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnExpression)

//! \ingroup OpenMaya MFn
//! \brief Expression function set. 
/*!
  This class is used to create, edit, and query expression nodes.
*/
class OPENMAYA_EXPORT MFnExpression : public MFnDependencyNode
{
	declareMFn( MFnExpression, MFnDependencyNode );

public:
	//! Unit conversion modes.
	enum UnitConversion {
		kAll,				//!< Convert all units.
		kNone,				//!< Do not do any unit conversion.
		kAngularOnly		//!< Only convert angular units.
	};

	MObject  		create( const MString & expression,
							MObject & object = MObject::kNullObj,
							MStatus * ReturnStatus = NULL );

	MString			expression( MStatus * ReturnStatus = NULL);
	MStatus			setExpression( const MString & expression );

	MStatus			getDefaultObject( MObject & object );
	MStatus			setDefaultObject( MObject & object );
	bool			isAnimated( MStatus * ReturnStatus = NULL );
	MStatus			setAnimated( bool value = false );

	MStatus			evaluate( MDoubleArray & result );

	UnitConversion  unitConversion( MStatus * ReturnStatus = NULL ) const;
	MStatus         setUnitConversion( UnitConversion conversion );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnExpression, MFnDependencyNode );
	//!		NO SCRIPT SUPPORT
	MStatus			getExpression( MString & expression );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnExpression */
