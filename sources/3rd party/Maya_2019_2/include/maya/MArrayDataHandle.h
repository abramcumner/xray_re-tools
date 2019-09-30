#ifndef _MArrayDataHandle
#define _MArrayDataHandle
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
// CLASS:    MArrayDataHandle
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <string.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MArrayDataHandle)

//! \ingroup OpenMaya
//! \brief Data block handle for array data. 
/*!
  An MArrayDataHandle is a smart pointer into a data block (MDataBlock) that
  handles access to array data.  MArrayDataHandle supports attributes that
  can have an array of data.

  The array indices do not have to be continuous.  The array can be
  sparse.  Thus, an array could have four elements with the indicies
  1, 6, 8, and 10.

  MArrayDataHandle methods cannot be assumed to be threadsafe. The
  jumpToElement() method sets an internal pointer so definitely cannot
  be called by multiple threads simultaneously.

*/
class OPENMAYA_EXPORT MArrayDataHandle {

public:
	MArrayDataHandle( const MDataHandle & in, MStatus * ReturnStatus = NULL );

	MDataHandle       inputValue(MStatus *ReturnStatus = NULL);
	MDataHandle       outputValue(MStatus *ReturnStatus = NULL);
	MArrayDataHandle  inputArrayValue( MStatus * ReturnStatus = NULL );
	MArrayDataHandle  outputArrayValue( MStatus * ReturnStatus = NULL );
    MStatus           next();
	unsigned int          elementCount(MStatus *ReturnStatus = NULL);
	unsigned int          elementIndex(MStatus *ReturnStatus = NULL);
    MStatus           jumpToElement( unsigned int index );
    MStatus           jumpToArrayElement( unsigned int position );
    MStatus           setClean();
    MStatus           setAllClean();
	MArrayDataBuilder builder(MStatus *ReturnStatus = NULL);
	MStatus           set( MArrayDataBuilder &builder );

    MArrayDataHandle( const MArrayDataHandle &other );
	MArrayDataHandle& operator=( const MArrayDataHandle& other );

	static	const char*	className();

protected:
// No protected members

private:
	friend class MDataBlock;
	friend class MArrayDataBuilder;
	MArrayDataHandle( void* );
	MArrayDataHandle();
	char data[40];
	bool fIsNull;
};

inline MArrayDataHandle::MArrayDataHandle( const MArrayDataHandle &other )
{
	memcpy( this, &other, sizeof(MArrayDataHandle) );
}

inline MArrayDataHandle& MArrayDataHandle::operator=(
											const MArrayDataHandle &other )
{
	memcpy( this, &other, sizeof(MArrayDataHandle) );
	return *this;
}

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MArrayDataHandle */
