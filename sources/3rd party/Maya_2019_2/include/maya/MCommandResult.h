#ifndef _MCommandResult
#define _MCommandResult
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
// CLASS:    MCommandResult
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MTypes.h>


// ****************************************************************************
// FORWARD DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MCommandResult)

//! \ingroup OpenMaya
//! \brief Result returned from executing a command. 
/*!
  MCommandResult collects the result returned by MGlobal::executeCommand.
  It can either be an int or an array of int or a double or an array of
  double or a string or an array of string. Use resultType to find out
  its type and use the appropriate getResult method to retrieve the result.
*/
class OPENMAYA_EXPORT MCommandResult {

public:
    //! The types of results that MEL commands can return.
    enum Type {
	  kInvalid = 0,		//!< \nop
	  kInt,			//!< \nop
	  kInt64,			//!< \nop
	  kIntArray,		//!< \nop
	  kInt64Array,		//!< \nop
	  kDouble,		//!< \nop
	  kDoubleArray,		//!< \nop
	  kString,		//!< \nop
	  kStringArray,		//!< \nop
	  kVector,		//!< \nop
      kVectorArray,		//!< \nop
      kMatrix,			//!< \nop
      kMatrixArray		//!< \nop
	};

    MCommandResult(MStatus* ReturnStatus = NULL );
	virtual         ~MCommandResult();
	Type            resultType(MStatus* ReturnStatus = NULL) const;
	MStatus         getResult( int& result) const;
	MStatus         getResult( MInt64& result) const;
    MStatus         getResult( MIntArray& result) const;
    MStatus         getResult( MInt64Array& result) const;
	MStatus         getResult( double& result) const;
	MStatus         getResult( MDoubleArray& result) const;
	MString			stringResult( MStatus *ReturnResult=NULL) const;
BEGIN_NO_SCRIPT_SUPPORT:
	//!     NO SCRIPT SUPPORT
	MStatus         getResult( MString& result) const;
END_NO_SCRIPT_SUPPORT:
	MStatus         getResult( MStringArray& result) const;
	MStatus         getResult( MVector& result) const;
	MStatus         getResult( MVectorArray& result) const;
	MStatus         getResult( MDoubleArray& result,
							   int &numRows, int &numColumns) const;
    static	const char* className();

protected:
// No protected members

private:
    void *fResult;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MCommandResult */
