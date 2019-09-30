#ifndef _MArgList
#define _MArgList
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
// CLASS:    MArgList
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MString.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MArgList)

//! \ingroup OpenMaya
//! \brief Create and retrieve argument lists. 
/*!
  This class is used to create and retrieve argument lists that can be
  passed to API class methods that require arglists.

  Do not use MArgList to parse flags for a plug-in command as the flags
  will not work if the command is called from Python. Use MArgParser or
  MArgDatabase instead.
*/
class OPENMAYA_EXPORT MArgList
{
public:
	static const unsigned int kInvalidArgIndex;
					MArgList();
					MArgList( const MArgList& other );
					~MArgList();
	unsigned int		length( MStatus* ReturnStatus= NULL ) const;
	bool			asBool(		unsigned int index, MStatus* ret=NULL ) const;
	int				asInt(		unsigned int index, MStatus* ret=NULL ) const;
	double			asDouble(	unsigned int index, MStatus* ret=NULL ) const;
	MString			asString(	unsigned int index, MStatus* ret=NULL ) const;
	MDistance		asDistance(	unsigned int index, MStatus* ret=NULL ) const;
	MAngle			asAngle(	unsigned int index, MStatus* ret=NULL ) const;
	MTime			asTime(		unsigned int index, MStatus* ret=NULL ) const;
	MVector			asVector(	unsigned int& index,
								unsigned int numElements=3,
								MStatus* ReturnStatus=NULL ) const;
	MPoint			asPoint(	unsigned int& index,
							 unsigned int numElements=3, MStatus* ReturnStatus=NULL ) const;
	MMatrix			asMatrix( unsigned int& index, MStatus* ret=NULL ) const;
	MIntArray		asIntArray( unsigned int& index, MStatus* ReturnStatus=NULL ) const;
	MDoubleArray		asDoubleArray( unsigned int& index, MStatus *ReturnStatus=NULL ) const;
	const MStringArray	asStringArray( unsigned int& index, MStatus *ReturnStatus=NULL ) const;
	MArgList&		operator =( const MArgList& other );

	// Argument list helper methods
	unsigned int		flagIndex (	const char *shortFlag,
								const char *longFlag = NULL ) const;

	// Command argument construction methods

	MStatus			addArg( bool arg );
	MStatus			addArg( int arg );
	MStatus			addArg( double arg );
	MStatus 		addArg( const MString & arg );
	MStatus 		addArg( const MDistance & arg );
	MStatus 		addArg( const MAngle & arg );
	MStatus 		addArg( const MTime & arg );
	MStatus			addArg( const MVector & arg );
	MStatus			addArg( const MPoint & arg );

BEGIN_NO_SCRIPT_SUPPORT:
	//! \noscript
	MStatus			get( unsigned int index, bool& ret ) const;
	//! \noscript
	MStatus			get( unsigned int index, int& ret ) const;
	//! \noscript
	MStatus			get( unsigned int index, double& ret ) const;
	//! \noscript
	MStatus			get( unsigned int index, MString& ret ) const;
	//! \noscript
	MStatus			get( unsigned int index, MDistance& ret ) const;
	//! \noscript
	MStatus			get( unsigned int index, MAngle& ret ) const;
	//! \noscript
	MStatus			get( unsigned int index, MTime& ret ) const;
	//! \noscript
	MStatus			get( unsigned int& index, MVector& ret,
						 unsigned int numElements=3 ) const;
	//! \noscript
	MStatus			get( unsigned int& index, MPoint& ret,
						 unsigned int numElements=3 ) const;
	//! \noscript
	MStatus			get( unsigned int& index, MMatrix& ret ) const;
	//! \noscript
	MStatus			get( unsigned int& index, MIntArray& ret ) const;
	//! \noscript
	MStatus			get( unsigned int& index, MDoubleArray& ret ) const;
	//! \noscript
	MStatus         get( unsigned int& index, MStringArray& ret ) const;
	//! \noscript
	friend OPENMAYA_EXPORT std::ostream&    operator<<(std::ostream& os, const MArgList& args);
END_NO_SCRIPT_SUPPORT:
	static	const char*		className();

protected:
// No protected members

private:
	MArgList ( const void*, unsigned int offset );
	const void*		fArgList;
	unsigned int		fOffset;
	bool			fOwn;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MArgList */
