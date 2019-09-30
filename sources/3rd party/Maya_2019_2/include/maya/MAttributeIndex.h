#ifndef _MAttributeIndex
#define _MAttributeIndex
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
// CLASS:    MAttributeIndex
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MAttributeIndex)

//! \ingroup OpenMaya
//! \brief The index information for an attribute specification. 
/*!
Class that provides access to the index part of an attribute
specification (MAttributeSpec).

See MAttributeSpec for more information.
*/
class OPENMAYA_EXPORT MAttributeIndex {
public:
	//! Data types for attribute indices.
	enum MIndexType {
		kInteger,	//!< Integer index (e.g. mesh.cp[5])
		kFloat		//!< Floating-poing index (e.g. curve.u[1.3])
	};

	MAttributeIndex();
	~MAttributeIndex();
	MAttributeIndex( const MAttributeIndex & other );
	MAttributeIndex( int value );
	MAttributeIndex( double value );

public:
	MIndexType		type() const;
	bool			hasRange() const;
	bool			hasValidRange() const;

	bool			hasLowerBound() const;
	bool			hasUpperBound() const;
	MStatus			getLower( int & value ) const;
	MStatus			getLower( double & value ) const;
	MStatus			getUpper( int & value ) const;
	MStatus			getUpper( double & value ) const;

	bool			isBounded() const;
	MStatus			getValue( int & value ) const;
	MStatus			getValue( double & value ) const;

public:
	MStatus			setType( MIndexType type);
	MStatus			setValue( int value );
	MStatus			setValue( double value );
	MStatus			setLower( int value );
	MStatus			setLower( double value );
	MStatus			setUpper( int value );
	MStatus			setUpper( double value );

public:
	MAttributeIndex & operator=( const MAttributeIndex & other );
	bool			operator==( const MAttributeIndex & other ) const;
	bool			operator!=( const MAttributeIndex & other ) const;

	static const char* className();

private:
	MAttributeIndex( void* );
	friend class MAttributeSpec;
	void*	data;
	bool	own;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MAttributeIndex */
