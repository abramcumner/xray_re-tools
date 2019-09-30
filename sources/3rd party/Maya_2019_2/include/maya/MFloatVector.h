#ifndef _MFloatVector
#define _MFloatVector
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
// CLASS:    MFloatVector
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <math.h>

#define MFloatVector_kTol 1.0e-5F

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFloatVector)

//! \ingroup OpenMaya
//! \brief A vector math class for vectors of floats.
/*!
  This class provides access to Maya's internal vector math library
  allowing vectors to be handled easily, and in a manner compatible
  with internal Maya data structures.

  All methods that query the vector are threadsafe, all methods that
  modify the vector are not threadsafe.

*/
class OPENMAYA_EXPORT MFloatVector
{
public:
						MFloatVector();
						MFloatVector( const MFloatVector&);
						MFloatVector( const MVector&);
						MFloatVector( const MFloatPoint&);
						MFloatVector( const MPoint&);
						MFloatVector( float xx, float yy, float zz = 0.0);
						MFloatVector( const float f[3] );
						MFloatVector( const double d[3] );
						~MFloatVector();
 	MFloatVector&		operator= ( const MFloatVector& src );
 	float   			operator()( unsigned int i ) const;
	float				operator[]( unsigned int i )const;
 	MFloatVector		operator^( const MFloatVector& right) const;
 	MFloatVector&   	operator/=( float scalar );
 	MFloatVector 	    operator/( float scalar ) const;
	MFloatVector& 		operator*=( int scalar );
	MFloatVector& 		operator*=( short scalar );
	MFloatVector& 		operator*=( unsigned int scalar );
	MFloatVector& 		operator*=( unsigned short scalar );
	MFloatVector& 		operator*=( float scalar );
	MFloatVector& 		operator*=( double);
	MFloatVector		operator*( int scalar ) const;
	MFloatVector		operator*( short scalar ) const;
	MFloatVector		operator*( unsigned int scalar ) const;
	MFloatVector		operator*( unsigned short scalar ) const;
 	MFloatVector   		operator*( float scalar ) const;
	MFloatVector		operator*( double scalar ) const;
 	MFloatVector   		operator+( const MFloatVector& other) const;
	MFloatVector&		operator+=( const MFloatVector& other );
 	MFloatVector   		operator-() const;
 	MFloatVector   		operator-( const MFloatVector& other ) const;
	MFloatVector&		operator-=( const MFloatVector& other );
 	MFloatVector  		operator*( const MFloatMatrix&) const;
 	MFloatVector&		operator*=( const MFloatMatrix&);
 	float      		    operator*( const MFloatVector& other ) const;
 	bool       		   	operator!=( const MFloatVector& other ) const;
 	bool       	    	operator==( const MFloatVector& other ) const;
	MStatus				get( float[3] ) const;
 	float      		   	length() const;
 	MFloatVector  		normal() const;
	MStatus				normalize();
 	float      		 	angle( const MFloatVector& other ) const;
	bool				isEquivalent( const MFloatVector& other,
									  float tolerance = MFloatVector_kTol )
									  const;
 	bool       		   	isParallel( const MFloatVector& other,
									float tolerance = MFloatVector_kTol )
									const;
BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	float&     		 	operator()( unsigned int i );

	//!	NO SCRIPT SUPPORT
 	float&     		 	operator[]( unsigned int i );

	//!	NO SCRIPT SUPPORT
	MFloatVector		transformAsNormal( const MFloatMatrix & matrix ) const;

	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MFloatVector	operator*( int,
												   const MFloatVector& );
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MFloatVector	operator*( short,
												   const MFloatVector& );
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MFloatVector	operator*( unsigned int,
												   const MFloatVector& );
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MFloatVector	operator*( unsigned short,
												   const MFloatVector& );
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MFloatVector	operator*( float,
												   const MFloatVector& );
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MFloatVector	operator*( double,
												   const MFloatVector& );
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MFloatVector	operator*( const MFloatMatrix&,
												   const MFloatVector& );
	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream& operator<<( std::ostream& os,
												const MFloatVector& v );

END_NO_SCRIPT_SUPPORT:

	//! The null vector
	static const MFloatVector zero;
	//! The vector <1.0,1.0,1.0>
	static const MFloatVector one;
	//! Unit vector in the positive x direction
	static const MFloatVector xAxis;
	//! Unit vector in the positive y direction
	static const MFloatVector yAxis;
	//! Unit vector in the positive z direction
	static const MFloatVector zAxis;
	//! Unit vector in the negative z direction
	static const MFloatVector xNegAxis;
	//! Unit vector in the negative z direction
	static const MFloatVector yNegAxis;
	//! Unit vector in the negative z direction
	static const MFloatVector zNegAxis;
	//! The x component of the vector
	float x;
	//! The y component of the vector
	float y;
	//! The z component of the vector
	float z;

protected:
// No protected members

private:
// No private members

};

OPENMAYA_NAMESPACE_CLOSE

//	These need to be included *after* the MFloatVector class definition,
//	otherwise we end up with a chicken-and-egg situation due to mutual
//	header inclusions.
//
#ifndef _MFloatPoint
#include <maya/MFloatPoint.h>
#endif
#ifndef _MPoint
#include <maya/MPoint.h>
#endif
#ifndef _MVector
#include <maya/MVector.h>
#endif

OPENMAYA_MAJOR_NAMESPACE_OPEN

/*!
	The default class constructor. Creates a null vector.
*/
inline MFloatVector::MFloatVector()
 	: x(0.0)
	, y(0.0)
	, z(0.0)
{
}

/*!
	The copy constructor.  Create a new vector and initialize it
	to the same values as the given MFloatVector.

	\param[in] src the vector object to copy
*/
inline MFloatVector::MFloatVector(const MFloatVector& src)
 	: x(src.x)
	, y(src.y)
	, z(src.z)
{
}

/*!
	Class constructor.  Create a new vector and initialize it
	to the same values as the given MVector.

	\warning This method converts double precision floating point values to
	         single precision. This will result in a loss of precision and,
			 if the double precision value exceeds the valid range for single
			 precision, the result will be undefined and unusable.

	\param[in] src the vector object to copy
*/
inline MFloatVector::MFloatVector(const MVector& src)
 	: x(static_cast<float>(src.x))
	, y(static_cast<float>(src.y))
	, z(static_cast<float>(src.z))
{
}

/*!
	Class constructor.  Initializes the vector with the
	explicit x, y and z values provided as arguments.

	\param[in] xx the x component of the vector
	\param[in] yy the y component of the vector
	\param[in] zz the z component of the vector.  Defaults to 0.0.
*/
inline MFloatVector::MFloatVector(float xx, float yy, float zz)
 	: x(xx)
	, y(yy)
	, z(zz)
{
}

/*!
	Class constructor.  Initializes the vector with the
	explicit x, y and z values provided in the given float array.

	\param[in] f the 3 element array containing the initial x, y, and z values.
*/
inline MFloatVector::MFloatVector( const float f[3] )
 	: x(f[0])
	, y(f[1])
	, z(f[2])
{
}

/*!
	Class constructor.  Initializes the vector with the
	explicit x, y and z values provided in the given double array.

	\warning This method converts double precision floating point values to
	         single precision. This will result in a loss of precision and,
			 if the double precision value exceeds the valid range for single
			 precision, the result will be undefined and unusable.

	\param[in] d the 3 element array containing the initial x, y, and z values.
*/
inline MFloatVector::MFloatVector( const double d[3] )
 	: x(static_cast<float>(d[0]))
	, y(static_cast<float>(d[1]))
	, z(static_cast<float>(d[2]))
{
}

/*!
	Class constructor.  Create a new vector and initialize it
	to the same x, y, z values as the given MFloatPoint.

	\param[in] src the vector object to copy
*/
inline MFloatVector::MFloatVector(const MFloatPoint& src)
 	: x(src.x)
	, y(src.y)
	, z(src.z)
{
}


/*!
	Class constructor.  Create a new vector and initialize it
	to the same x, y, z values as the given MPoint.

	\warning This method converts double precision floating point values to
	         single precision. This will result in a loss of precision and,
			 if the double precision value exceeds the valid range for single
			 precision, the result will be undefined and unusable.

	\param[in] src the point object to copy
*/
inline MFloatVector::MFloatVector(const MPoint& src)
 	: x(static_cast<float>(src.x))
	, y(static_cast<float>(src.y))
	, z(static_cast<float>(src.z))
{
}

/*!
	Class destructor.
*/
inline MFloatVector::~MFloatVector()
{
}

/*!
	The assignment operator.  Allows assignment between MFloatVectors.

	\param[in] src The vector to copy from.

	\return 
	A reference to the assigned vector.
*/
inline MFloatVector& MFloatVector::operator= (const MFloatVector& src)
{
	x=src.x;
	y=src.y;
	z=src.z;
	return *this;
}

/*!
	The index operator.  If its argument is 0 it will return the
	x component of the vector. If its argument is 1 it will return
	the y component of the vector. Otherwise it will return the
	z component of the vector.

	\param[in] i Value indicating which component to return.

	\return 
	Reference to the vector component.
*/
inline float& MFloatVector::operator()( unsigned int i )
{
	//	switch is more efficient than else-if and can be better optimized
	//	by compilers.
	//
	switch (i)
	{
		default:
		case 2:	return z;
		case 1:	return y;
		case 0:	return x;
	}
}

/*!
	The index operator.  If its argument is 0 it will return the
	x component of the vector.  If its argument is 1 it will return
	the y component of the vector.  Otherwise it will return the
	z component of the vector.

	\param[in] i Value indicating which component to return.

	\return 
	The vector component.
*/
inline float MFloatVector::operator()( unsigned int i ) const
{
	switch (i)
	{
		default:
		case 2:	return z;
		case 1:	return y;
		case 0:	return x;
	}
}

/*!
	The index operator.  If its argument is 0 it will return the
	x component of the vector.  If its argument is 1 it will return
	the y component of the vector.  Otherwise it will return the
	z component of the vector.

	\param[in] i Value indicating which component to return.

	\return 
	Reference to the vector component.
*/
inline float& MFloatVector::operator[]( unsigned int i )
{
	switch (i)
	{
		default:
		case 2:	return z;
		case 1:	return y;
		case 0:	return x;
	}
}

/*!
	The index operator.  If its argument is 0 it will return the
	x component of the vector.  If its argument is 1 it will return
	the y component of the vector.  Otherwise it will return the
	z component of the vector.

	\param[in] i Value indicating which component to return.

	\return 
	The vector component.
*/
inline float MFloatVector::operator[]( unsigned int i ) const
{
	switch (i)
	{
		default:
		case 2:	return z;
		case 1:	return y;
		case 0:	return x;
	}
}

/*!
	The cross product operator.

	\param[in] right Vector to take the cross product with.

	\return 
	The cross product result.
*/
inline MFloatVector MFloatVector::operator^ (const MFloatVector& right) const
{
	return MFloatVector(y*right.z - z*right.y,
						z*right.x - x*right.z,
						x*right.y - y*right.x);
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	A reference to the scaled vector.
*/
inline MFloatVector& MFloatVector::operator*= (int scalar)
{
	x *= (float)scalar;
	y *= (float)scalar;
	z *= (float)scalar;
	return *this;
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	A reference to the scaled vector.
*/
inline MFloatVector& MFloatVector::operator*= (short scalar)
{
	x *= (float)scalar;
	y *= (float)scalar;
	z *= (float)scalar;
	return *this;
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	A reference to the scaled vector.
*/
inline MFloatVector& MFloatVector::operator*= (unsigned int scalar)
{
	x *= (float)scalar;
	y *= (float)scalar;
	z *= (float)scalar;
	return *this;
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	A reference to the scaled vector.
*/
inline MFloatVector& MFloatVector::operator*= (unsigned short scalar)
{
	x *= (float)scalar;
	y *= (float)scalar;
	z *= (float)scalar;
	return *this;
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	A reference to the scaled vector.
*/
inline MFloatVector& MFloatVector::operator*= (float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	A reference to the scaled vector.
*/
inline MFloatVector& MFloatVector::operator*= (double scalar)
{
	x *= (float)scalar;
	y *= (float)scalar;
	z *= (float)scalar;
	return *this;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	The scaled vector.
*/
inline MFloatVector MFloatVector::operator* ( int scalar ) const
{
	MFloatVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	The scaled vector.
*/
inline MFloatVector MFloatVector::operator* ( short scalar ) const
{
	MFloatVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	The scaled vector.
*/
inline MFloatVector MFloatVector::operator* ( unsigned int scalar ) const
{
	MFloatVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	The scaled vector.
*/
inline MFloatVector MFloatVector::operator* ( unsigned short scalar ) const
{
	MFloatVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	The scaled vector.
*/
inline MFloatVector MFloatVector::operator* ( float scalar ) const
{
	MFloatVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.

	\return 
	The scaled vector.
*/
inline MFloatVector MFloatVector::operator* ( double scalar ) const
{
	MFloatVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The in place division operator.

	\param[in] scalar Division factor.

	\return 
	A reference to the scaled vector.
*/
inline MFloatVector& MFloatVector::operator/= (float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

/*!
	The division operator.

	\param[in] scalar Division factor.

	\return 
	The scaled vector.
*/
inline MFloatVector MFloatVector::operator/ ( float scalar ) const
{
	MFloatVector tmp(*this);
	tmp /= scalar;
	return tmp;
}

/*!
	The vector subtraction operator.

	\param[in] other Vector to substract.

	\return 
	The resulting vector.
*/
inline MFloatVector MFloatVector::operator- (const MFloatVector& other) const
{
	return MFloatVector(x-other.x, y-other.y, z-other.z);
}

/*!
	The vector addition operator.

	\param[in] other Vector to add.

	\return 
	The resulting vector.
*/
inline MFloatVector MFloatVector::operator+ (const MFloatVector& other) const
{
	return MFloatVector(x+other.x, y+other.y, z+other.z);
}

/*!
	The in place vector addition operator.

	\param[in] other Vector to add.

	\return 
	A reference to the resulting vector.
*/
inline MFloatVector& MFloatVector::operator+= (const MFloatVector& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

/*!
	The in place vector subtraction operator.

	\param[in] other Vector to substract.

	\return 
	A reference to the resulting vector.
*/
inline MFloatVector& MFloatVector::operator-= (const MFloatVector& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

/*!
	The unary minus operator.  Negates the value of each of the
	x, y, and z components of the vector.

	\return 
	The resulting vector.
*/
inline MFloatVector MFloatVector::operator- () const
{
	return MFloatVector(-x,-y,-z);
}

/*!
	The dot product operator.

	\param[in] right Vector take the dot product with.

	\return 
	Dot product value.
*/
inline float MFloatVector::operator* (const MFloatVector& right) const
{
	return (x*right.x + y*right.y + z*right.z);
}

/*!
	Extracts the x, y, and z components of the vector and places
	them in elements 0, 1, and 2 of the float array passed.

	\param[out] d The array of 3 floats into which the results are placed.

	\return
	MS::kSuccess if d is a non-zero pointer and MS::kFailure otherwise.
*/
inline MStatus MFloatVector::get( float d[3] ) const
{
	if(d != NULL)
	{
		d[0] = x;
		d[1] = y;
		d[2] = z;
		return MStatus::kSuccess ;
	}
	return MStatus::kFailure;
}

/*!
	Performs an in place normalization of the vector

	\return
	Always returns MS::kSuccess.
*/
inline MStatus MFloatVector::normalize()
{
	float lensq = x*x + y*y + z*z;
	if(lensq>1e-20) {
		float factor = 1.0f / sqrtf(lensq);
		x *= factor;
		y *= factor;
		z *= factor;
	}
	return MStatus::kSuccess;
}

/*!
	Computes a unit vector aligned to the vector.
	\return 
	The unit vector.
*/
inline MFloatVector MFloatVector::normal() const
{
	MFloatVector tmp(*this);
	tmp.normalize();
	return tmp;
}

/*!
	\return The length of the vector.
*/
inline float MFloatVector::length() const
{
	return sqrtf(x*x+y*y+z*z);
}

/*!
	The vector equality operator.  This returns true if all three
	of the x, y, and z components are identical.

	\param[in] other Vector to compare against.

	\return
	Bool true if the vectors are identical and false otherwise.
*/
inline bool MFloatVector::operator== (const MFloatVector& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

/*!
	The vector inequality operator.  This returns false if all three
	of the x, y, and z components are identical.

	\param[in] other Vector to compare against.

	\return
	Bool false if the vectors are identical and true otherwise.
*/
inline bool MFloatVector::operator!= (const MFloatVector& other) const
{
	return !(*this == other);
}

#if ! defined(SWIG)

/*!
	The multiplication operator that allows the scalar value
	to precede the vector.

	\param[in] scalar multiplication factor.
	\param[in] other Vector to scale.

	\return
	Scaled vector.
*/
inline MFloatVector operator * (int scalar, const MFloatVector& other)
{
	return float(scalar) * other;
}

/*!
	The multiplication operator that allows the scalar value
	to precede the vector.

	\param[in] scalar multiplication factor.
	\param[in] other Vector to scale.

	\return
	Scaled vector.
*/
inline MFloatVector operator * (short scalar, const MFloatVector& other)
{
	return float(scalar) * other;
}

/*!
	The multiplication operator that allows the scalar value
	to precede the vector.

	\param[in] scalar multiplication factor.
	\param[in] other Vector to scale.

	\return
	Scaled vector.
*/
inline MFloatVector operator * (unsigned int scalar, const MFloatVector& other)
{
	return float(scalar) * other;
}

/*!
	The multiplication operator that allows the scalar value
	to precede the vector.

	\param[in] scalar multiplication factor.
	\param[in] other Vector to scale.

	\return
	Scaled vector.
*/
inline MFloatVector operator * (unsigned short scalar, const MFloatVector& other)
{
	return float(scalar) * other;
}

/*!
	The multiplication operator that allows the scalar value
	to precede the vector.

	\param[in] scalar multiplication factor.
	\param[in] other Vector to scale.

	\return
	Scaled vector.
*/
inline MFloatVector operator * (float scalar, const MFloatVector& other)
{
	return MFloatVector(scalar*other.x, scalar*other.y, scalar*other.z);
}

/*!
	The multiplication operator that allows the scalar value
	to precede the vector.

	\param[in] scalar multiplication factor.
	\param[in] other Vector to scale.

	\return
	Scaled vector.
*/
inline MFloatVector operator * (double scalar, const MFloatVector& other)
{
	return float(scalar) * other;
}

#endif // ! defined(SWIG)

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFloatVector */
