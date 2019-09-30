#ifndef _MFnMatrixAttribute
#define _MFnMatrixAttribute
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
// CLASS:    MFnMatrixAttribute
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnMatrixAttribute)

//! \ingroup OpenMaya MFn
//! \brief Matrix attribute function set.
/*!
  Function object for matrix attributes. These attributes accept
  either a matrix of float values or double values depending on the
  type specified to the create method.
*/
class OPENMAYA_EXPORT MFnMatrixAttribute : public MFnAttribute
{
	declareMFn(MFnMatrixAttribute, MFnAttribute);

public:

	//! Matrix data type.
	enum Type {
		kFloat,			//!< Matrix of float values.
		kDouble			//!< Matrix of double values.
	};

	MObject     create( const MString& fullName,
					    const MString& briefName,
						Type matrixType = kDouble,
					    MStatus* ReturnStatus = NULL );
	MStatus     getDefault( MMatrix & def );
	MStatus     getDefault( MFloatMatrix & def );
	MStatus     setDefault( const MMatrix & def );
	MStatus     setDefault( const MFloatMatrix & def );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnMatrixAttribute, MFnAttribute );

END_NO_SCRIPT_SUPPORT:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnMatrixAttribute */



