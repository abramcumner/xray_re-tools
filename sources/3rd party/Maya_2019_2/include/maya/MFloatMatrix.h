#ifndef _MFloatMatrix
#define _MFloatMatrix
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
// CLASS:    MFloatMatrix
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS

#define MFloatMatrix_kTol	1.0e-5F

// ****************************************************************************
// CLASS DECLARATION (MFloatMatrix)

//! \ingroup OpenMaya
//! \brief A matrix math class for 4x4 matrices of floats.
/*!
This class provides access to Maya's internal matrix math library allowing
matrices to be handled easily, and in a manner compatible with internal
Maya data structures.
*/
class OPENMAYA_EXPORT MFloatMatrix
{

public:
						MFloatMatrix();
						MFloatMatrix( const MFloatMatrix & src );
						MFloatMatrix( const double m[4][4] );
						MFloatMatrix( const float m[4][4] );
						~MFloatMatrix();
 	MFloatMatrix&		operator = (const MFloatMatrix &);
	float				operator()(unsigned int row, unsigned int col ) const;
	const float* 		operator[]( unsigned int row ) const;
	MStatus				get( double dest[4][4] ) const;
	MStatus				get( float dest[4][4] ) const;
 	MFloatMatrix     	transpose() const;
 	MFloatMatrix &   	setToIdentity();
 	MFloatMatrix &   	setToProduct( const MFloatMatrix & left,
									  const MFloatMatrix & right );
 	MFloatMatrix &   	operator+=( const MFloatMatrix & right );
 	MFloatMatrix    	operator+ ( const MFloatMatrix & right ) const;
 	MFloatMatrix &   	operator-=( const MFloatMatrix & right );
 	MFloatMatrix  		operator- ( const MFloatMatrix & right ) const;
 	MFloatMatrix &   	operator*=( const MFloatMatrix & right );
 	MFloatMatrix     	operator* ( const MFloatMatrix & right ) const;
 	MFloatMatrix &   	operator*=( float );
 	MFloatMatrix     	operator* ( float ) const;
 	bool          		operator==( const MFloatMatrix & other ) const;
 	bool           		operator!=( const MFloatMatrix & other ) const;
 	MFloatMatrix     	inverse() const;
 	MFloatMatrix     	adjoint() const;
 	MFloatMatrix     	homogenize() const;
 	float       		det4x4() const;
 	float         		det3x3() const;
 	bool           		isEquivalent( const MFloatMatrix & other,
									  float tolerance = MFloatMatrix_kTol )
 									  const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	inline float& operator()(unsigned int row, unsigned int col )
	{
		return matrix[row][col];
	}

	//!	NO SCRIPT SUPPORT
	inline float* operator[]( unsigned int row )
	{
		return matrix[row];
	}

	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MFloatMatrix	operator* ( float,
												const MFloatMatrix & right );
	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream& operator<< ( std::ostream& os,
												const MFloatMatrix& m );

END_NO_SCRIPT_SUPPORT:

	//! the matrix data
 	float matrix[4][4];

	static const char* className();

protected:
// No protected members

private:
};

/*!
	Index operator.
	Given row and column indices, it will return the value at
	the specified location in the matrix.

	\param[in] row index of the row to access
	\param[in] col index of the column to access

	\return
	\li the value at the specified location in the matrix.
*/
inline float MFloatMatrix::operator()(unsigned int row, unsigned int col ) const
{
	return matrix[row][col];
}

/*!
	Index operator.
	Returns an entire row of the matrix as an array of floats.

	\param[in] row index of the row to access

	\return
	\li float array containing the values of the specified row
*/
inline const float* MFloatMatrix::operator[]( unsigned int row ) const
{
	return matrix[row];
}

/*!
  \fn float& MFloatMatrix::operator()(unsigned int row, unsigned int col )

	Index operator.
	Given row and column indices, it will return the value at
	the specified location in the matrix.

	\param[in] row index of the row to access
	\param[in] col index of the column to access

	\return
	\li the value at the specified location in the matrix.
*/


/*!
\fn float* MFloatMatrix::operator[](unsigned int row)

	Index operator.
	Returns an entire row of the matrix as an array of floats.

	\param[in] row index of the row to access

	\return
	\li float array containing the values of the specified row
*/

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFloatMatrix */
