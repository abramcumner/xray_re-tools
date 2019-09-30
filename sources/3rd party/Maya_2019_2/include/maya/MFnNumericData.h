#ifndef _MFnNumericData
#define _MFnNumericData
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
// CLASS:    MFnNumericData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnNumericData)

//! \ingroup OpenMaya MFn
//! \brief Numeric data function set. 
/*!
  MFnNumericData allows the creation and manipulation of numeric data objects
  for use in the dependency graph.  Normally, data objects are not required
  for the transmission of numeric data.  The graph supports numeric types
  directly (see the methods of MDataHandle).

  Numeric data objects are useful if you have an attribute that
  accepts generic data.  A generic attribute can accept multiple types
  of data, so you cannot hardwire it to accept a specific type of
  numeric data.  So, generic attributes can only accept numeric data
  in the form of actual data objects.

  This function set only supports numeric data with multiple components,
  such as a pair of floats or a triple of integers. Single numeric values
  can be retrieved directly using MPlug or MDataHandle.
*/
class OPENMAYA_EXPORT MFnNumericData : public MFnData
{
	declareMFn(MFnNumericData, MFnData);

public:

	//! Supported numerical types.
	enum Type {
		kInvalid,			//!< Invalid data.
		kBoolean,			//!< Boolean.
		kByte,				//!< One byte.
		kChar,				//!< One character.
		kShort,				//!< One short.
		k2Short,			//!< Two shorts.
		k3Short,			//!< Three shorts.
		kLong,				//!< One long. Same as int since "long" is not platform-consistent.
		kInt = kLong,		//!< One int.
		k2Long,				//!< Two longs. Same as 2 ints since "long" is not platform-consistent.
		k2Int = k2Long,		//!< Two ints.
		k3Long,				//!< Three longs. Same as 3 ints since "long" is not platform-consistent.
		k3Int = k3Long,		//!< Three ints.
		kInt64,				//!< One 64-bit int.
		kFloat,				//!< One float.
		k2Float,			//!< Two floats.
		k3Float,			//!< Three floats.
		kDouble,			//!< One double.
		k2Double,			//!< Two doubles.
		k3Double,			//!< Three doubles.
		k4Double,			//!< Four doubles.
        kAddr,				//!< An address.

		/*! Last value. Does not represent a real type, but can be
		  used to loop on all possible types.
		*/
		kLast								
    };

	MObject create( Type dataType, MStatus* ReturnStatus = NULL );

	Type numericType( MStatus* ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:
	//!     NO SCRIPT SUPPORT
	MStatus getData( short& val1, short& val2 );
	//!     NO SCRIPT SUPPORT
	MStatus getData( int& val1, int& val2 );
	//!     NO SCRIPT SUPPORT
	MStatus getData( float& val1, float& val2 );
	//!     NO SCRIPT SUPPORT
	MStatus getData( double& val1, double& val2 );
	//!     NO SCRIPT SUPPORT
	MStatus getData( short& val1, short& val2, short& val3 );
	//!     NO SCRIPT SUPPORT
	MStatus getData( int& val1, int& val2, int& val3 );
	//!     NO SCRIPT SUPPORT
	MStatus getData( float& val1, float& val2, float& val3 );
	//!     NO SCRIPT SUPPORT
	MStatus getData( double& val1, double& val2, double& val3 );
	//!     NO SCRIPT SUPPORT
	MStatus getData( double& val1, double& val2, double& val3, double& val4 );

	//!     NO SCRIPT SUPPORT
	MStatus setData( short val1, short val2 );
	//!     NO SCRIPT SUPPORT
	MStatus setData( int val1, int val2 );
	//!     NO SCRIPT SUPPORT
	MStatus setData( float val1, float val2 );
	//!     NO SCRIPT SUPPORT
	MStatus setData( double val1, double val2 );
	//!     NO SCRIPT SUPPORT
	MStatus setData( short val1, short val2, short val3 );
	//!     NO SCRIPT SUPPORT
	MStatus setData( int val1, int val2, int val3 );
	//!     NO SCRIPT SUPPORT
	MStatus setData( float val1, float val2, float val3 );
	//!     NO SCRIPT SUPPORT
	MStatus setData( double val1, double val2, double val3 );
	//!     NO SCRIPT SUPPORT
	MStatus setData( double val1, double val2, double val3, double val4 );

	//!     NO SCRIPT SUPPORT
 	declareMFnConstConstructor( MFnNumericData, MFnData );

END_NO_SCRIPT_SUPPORT:

	MStatus getData2Short( short& val1, short& val2 );
	MStatus getData2Int( int& val1, int& val2 );
	MStatus getData2Float( float& val1, float& val2 );
	MStatus getData2Double( double& val1, double& val2 );
	MStatus getData3Short( short& val1, short& val2, short& val3 );
	MStatus getData3Int( int& val1, int& val2, int& val3 );
	MStatus getData3Float( float& val1, float& val2, float& val3 );
	MStatus getData3Double( double& val1, double& val2, double& val3 );
	MStatus getData4Double( double& val1, double& val2, double& val3, double& val4 );

	MStatus setData2Short( short val1, short val2 );
	MStatus setData2Int( int val1, int val2 );
	MStatus setData2Float( float val1, float val2 );
	MStatus setData2Double( double val1, double val2 );
	MStatus setData3Short( short val1, short val2, short val3 );
	MStatus setData3Int( int val1, int val2, int val3 );
	MStatus setData3Float( float val1, float val2, float val3 );
	MStatus setData3Double( double val1, double val2, double val3 );
	MStatus setData4Double( double val1, double val2, double val3, double val4 );


protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnNumericData */
