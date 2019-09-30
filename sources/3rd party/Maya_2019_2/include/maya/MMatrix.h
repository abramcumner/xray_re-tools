#ifndef _MMatrix
#define _MMatrix
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
// CLASS:    MMatrix
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

#define MMatrix_kTol	1.0e-10

// ****************************************************************************
// CLASS DECLARATION (MMatrix)

//! \ingroup OpenMaya
//! \brief A matrix math class for 4x4 matrices of doubles. 
/*!
This class provides access to Maya's internal matrix math library allowing
matrices to be handled easily, and in a manner compatible with internal
Maya data structures.

All methods that query the matrix are threadsafe, all methods that
modify the matrix are not threadsafe.

*/
class OPENMAYA_EXPORT MMatrix
{

public:
					MMatrix();
					MMatrix( const MMatrix & src );
					MMatrix( const double m[4][4] );
					MMatrix( const float m[4][4] );
					~MMatrix();
 	MMatrix&		operator = (const MMatrix &);
	double			operator()(unsigned int row, unsigned int col ) const;
	const double* 		operator[]( unsigned int row ) const;
	MStatus			get( double dest[4][4] ) const;
	MStatus			get( float dest[4][4] ) const;
 	MMatrix     	transpose() const;
 	MMatrix &   	setToIdentity();
 	MMatrix &   	setToProduct( const MMatrix & left,
				 		const MMatrix & right );
 	MMatrix &   	operator+=( const MMatrix & right );
 	MMatrix  		operator+ ( const MMatrix & right ) const;
 	MMatrix &   	operator-=( const MMatrix & right );
 	MMatrix  		operator- ( const MMatrix & right ) const;
 	MMatrix &   	operator*=( const MMatrix & right );
 	MMatrix     	operator* ( const MMatrix & right ) const;
 	MMatrix &   	operator*=( double );
 	MMatrix     	operator* ( double ) const;
 	bool          	operator==( const MMatrix & other ) const;
 	bool           	operator!=( const MMatrix & other ) const;
 	MMatrix     	inverse() const;
 	MMatrix     	adjoint() const;
 	MMatrix     	homogenize() const;
 	double       	det4x4() const;
 	double         	det3x3() const;
 	bool           	isEquivalent( const MMatrix & other,
				 		double tolerance = MMatrix_kTol ) const;
	bool			isSingular() const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	double& operator()(unsigned int row, unsigned int col )
	{
		return matrix[row][col];
	}

	//!	NO SCRIPT SUPPORT
	double* operator[]( unsigned int row )
	{
		return matrix[row];
	}

	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MMatrix operator* ( double, const MMatrix & right );
	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream&	operator<<(std::ostream& os, const MMatrix& m);

END_NO_SCRIPT_SUPPORT:

	//! The matrix data
 	double matrix[4][4];

	//! The identity matrix.
	static const MMatrix identity;

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
inline double MMatrix::operator()(unsigned int row, unsigned int col ) const
{
	return matrix[row][col];
}

/*!
	Index operator.
	Returns an entire row of the matrix as an array of doubles.

	\param[in] row index of the row to access

	\return
	\li double array containing the values of the specified row
*/
inline const double* MMatrix::operator[]( unsigned int row ) const
{
	return matrix[row];
}


/*! \fn double& MMatrix::operator()(unsigned int row, unsigned int col )

	Index operator.
	Given row and column indices, it will return the value at
	the specified location in the matrix.

	\param[in] row index of the row to access
	\param[in] col index of the column to access

	\return
	\li the value at the specified location in the matrix.
*/


/*! \fn double* MMatrix::operator[](unsigned int row)

	Index operator.
	Returns an entire row of the matrix as an array of doubles.

	\param[in] row index of the row to access

	\return
	\li double array containing the values of the specified row
*/

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MMatrix */
