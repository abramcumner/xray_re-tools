#ifndef _MFloatPoint
#define _MFloatPoint
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
// CLASS:    MFloatPoint
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MIOStreamFwd.h>
#include <maya/MStatus.h>


#define MFloatPoint_kTol	1.0e-5F

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFloatPoint)

//! \ingroup OpenMaya
//! \brief Implementation of a point.
/*!
  This class provides an implementation of a point in float. Numerous
  convienence operators are provided to help with the manipulation of
  points. This includes operators that work with the MFloatVector and
  MFloatMatrix classes.

  All methods that query the point are threadsafe, all methods that
  modify the point are not threadsafe.

*/
class OPENMAYA_EXPORT MFloatPoint
{
public:
						MFloatPoint();	// defaults to 0,0,0,1
						MFloatPoint( const MFloatPoint & srcpt );
						MFloatPoint( const MPoint & srcpt );
						MFloatPoint( const MFloatVector & src );
						MFloatPoint( const MVector & src );
						MFloatPoint( float xx, float yy,
									 float zz = 0.0, float ww = 1.0 );
						MFloatPoint( const float f[4] );
						MFloatPoint( const double d[4] );
						~MFloatPoint();
	MStatus				get( double dest[4] ) const;
	MStatus				get( float dest[4] ) const;
	MStatus				setCast( const MPoint & srcpt );
	MStatus				setCast( const MVector & src );
	MStatus				setCast( const double d[4] );
	MFloatPoint &		operator=( const MFloatPoint & src );
	float				operator()(unsigned int i) const;
	float				operator[](unsigned int i) const;
	MFloatVector		operator-( const MFloatPoint & other ) const;
	MFloatPoint			operator+( const MFloatVector & other ) const;
	MFloatPoint			operator-( const MFloatVector & other ) const;
	MFloatPoint &		operator+=( const MFloatVector & vector );
	MFloatPoint &		operator-=( const MFloatVector & vector );
	MFloatPoint			operator*(const int scale) const;
	MFloatPoint			operator*(const short scale) const;
	MFloatPoint			operator*(const unsigned int scale) const;
	MFloatPoint			operator*(const unsigned short scale) const;
	MFloatPoint			operator*(const float scale) const;
	MFloatPoint			operator*(const double scale) const;
	MFloatPoint			operator/(const float scale) const;
	MFloatPoint			operator*(const MFloatMatrix &) const;
	MFloatPoint &		operator*=(const MFloatMatrix &);
	MFloatPoint & 		operator*=(const int);
	MFloatPoint & 		operator*=(const short);
	MFloatPoint & 		operator*=(const unsigned int);
	MFloatPoint & 		operator*=(const unsigned short);
	MFloatPoint & 		operator*=(const float);
	MFloatPoint & 		operator*=(const double);
	bool				operator==( const MFloatPoint & other ) const;
	bool				operator!=( const MFloatPoint & other ) const;
	MFloatPoint &		cartesianize();
	MFloatPoint &		rationalize();
	MFloatPoint &		homogenize();
	float				distanceTo( const MFloatPoint & other ) const;
	bool				isEquivalent( const MFloatPoint & other,
									  float tolerance = MFloatPoint_kTol)
									const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	float &         	operator()(unsigned int i);

	//!	NO SCRIPT SUPPORT
	float &         	operator[](unsigned int i);

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MFloatPoint operator*( int, const MFloatPoint &);

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MFloatPoint operator*( short, const MFloatPoint &);

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MFloatPoint operator*( unsigned int, const MFloatPoint &);

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MFloatPoint operator*( unsigned short, const MFloatPoint &);

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MFloatPoint operator*( float, const MFloatPoint &);

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MFloatPoint operator*( double, const MFloatPoint &);

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MFloatPoint operator*( const MFloatMatrix &,
												  const MFloatPoint & );
	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream& operator<< ( std::ostream& os,
												 const MFloatPoint& p );

END_NO_SCRIPT_SUPPORT:

	static const MFloatPoint origin;
	//! the x component of the point
	float				x;
	//! the y component of the point
	float				y;
	//! the z component of the point
	float				z;
	//! the w component of the point
	float				w;

	static const char* className();

protected:
// No protected members
};

OPENMAYA_NAMESPACE_CLOSE

//	These need to be included *after* the MFloatPoint class definition,
//	otherwise we end up with a chicken-and-egg situation due to mutual
//	header inclusions.
//
#ifndef _MFloatVector
#include <maya/MFloatVector.h>
#endif
#ifndef _MPoint
#include <maya/MPoint.h>
#endif
#ifndef _MVector
#include <maya/MVector.h>
#endif

OPENMAYA_MAJOR_NAMESPACE_OPEN

/*!
	Default constructor. The instance is initialized to the origin.
*/
inline MFloatPoint::MFloatPoint()
 	: x(0.0f)
	, y(0.0f)
	, z(0.0f)
	, w(1.0f)
{
}

/*!
	Copy constructor.  Creates an new instance and initializes it to
	the same point as the given point.

	\param[in] srcpt the point object to copy
*/
inline MFloatPoint::MFloatPoint(const MFloatPoint& srcpt)
 	: x(srcpt.x)
	, y(srcpt.y)
	, z(srcpt.z)
	, w(srcpt.w)
{
}

/*!
	Copy constructor.  Creates an new instance and initializes it to
	the same point as the given point.

	\warning This method converts double precision floating point values to
	         single precision. This will result in a loss of precision and,
			 if the double precision value exceeds the valid range for single
			 precision, the result will be undefined and unusable.

	\param[in] srcpt the point object to copy
*/
inline MFloatPoint::MFloatPoint(const MPoint& srcpt)
 	: x(static_cast<float>(srcpt.x))
	, y(static_cast<float>(srcpt.y))
	, z(static_cast<float>(srcpt.z))
	, w(static_cast<float>(srcpt.w))
{
}

/*!
	Create a new instance and initialize it to the given position.

	\param[in] f an array of 4 floats used to initialize x, y, z, and
	                w respectively
*/
inline MFloatPoint::MFloatPoint(const float f[4] )
 	: x(f[0])
	, y(f[1])
	, z(f[2])
	, w(f[3])
{
}

/*!
	Create a new instance and initialize it to the given position.

	\warning This method converts double precision floating point values to
	         single precision. This will result in a loss of precision and,
			 if the double precision value exceeds the valid range for single
			 precision, the result will be undefined and unusable.

	\param[in] d an array of 4 doubles used to initialize x, y, z, and
	                w respectively
*/
inline MFloatPoint::MFloatPoint(const double d[4] )
 	: x(static_cast<float>(d[0]))
	, y(static_cast<float>(d[1]))
	, z(static_cast<float>(d[2]))
	, w(static_cast<float>(d[3]))
{
}

/*!
	Create a new instance and initialize it to the given position.

	\param[in] xx the initial value of x
	\param[in] yy the initial value of y
	\param[in] zz the initial value of z
	\param[in] ww the initial value of w
*/
inline MFloatPoint::MFloatPoint(float xx, float yy, float zz, float ww)
 	: x(xx)
	, y(yy)
	, z(zz)
	, w(ww)
{
}

/*!
	Create a new point and initialize it
	to the same x, y, z values as the given MFloatVector.

	\param[in] srcpt the vector object to copy
*/
inline MFloatPoint::MFloatPoint(const MFloatVector& srcpt)
 	: x(srcpt.x)
	, y(srcpt.y)
	, z(srcpt.z)
	, w(1.0f)
{
}

/*!
	Create a new point and initialize it to the same x, y, z values as the
	given MVector.

	\warning This method converts double precision floating point values to
	         single precision. This will result in a loss of precision and,
			 if the double precision value exceeds the valid range for single
			 precision, the result will be undefined and unusable.

	\param[in] srcpt the vector object to copy
*/
inline MFloatPoint::MFloatPoint(const MVector& srcpt)
 	: x(static_cast<float>(srcpt.x))
	, y(static_cast<float>(srcpt.y))
	, z(static_cast<float>(srcpt.z))
	, w(1.0f)
{
}

/*!
	Class destructor.
*/
inline MFloatPoint::~MFloatPoint()
{
}

/*!
	Copy the values of x, y, z, and w from srcpt to the instance.

	\warning This method converts double precision floating point values to
	         single precision. This will result in a loss of precision and,
			 if the double precision value exceeds the valid range for single
			 precision, the result will be undefined and unusable.

	\param[in] srcpt the point to copy the x, y, z and w values from.

	\return
	MS::kSuccess always returned.
*/
inline MStatus	MFloatPoint::setCast(const MPoint& srcpt)
{
	x = static_cast<float>(srcpt.x);
	y = static_cast<float>(srcpt.y);
	z = static_cast<float>(srcpt.z);
	w = static_cast<float>(srcpt.w);

	return( MS::kSuccess );
}

/*!
	Copy the values of x, y, z, and w from src to the instance.

	\warning This method converts double precision floating point values to
	         single precision. This will result in a loss of precision and,
			 if the double precision value exceeds the valid range for single
			 precision, the result will be undefined and unusable.

	\param[in] src the vector to copy the x, y, z and w values from.

	\return
	MS::kSuccess always returned.
*/
inline MStatus MFloatPoint::setCast( const MVector& src )
{
	x = static_cast<float>(src.x);
	y = static_cast<float>(src.y);
	z = static_cast<float>(src.z);
	w = 1.0;
	return( MS::kSuccess );
}

/*!
	Copy the values of x, y, z, and w to the instance from the four
	elements of the given array of doubles.

	\warning This method converts double precision floating point values to
	         single precision. This will result in a loss of precision and,
			 if the double precision value exceeds the valid range for single
			 precision, the result will be undefined and unusable.

	\param[in] d the four element array of doubles

	\return
	MS::kSuccess if dest is a non-zero pointer and MS::kFailure otherwise
*/
inline MStatus MFloatPoint::setCast( const double d[4] )
{
	MStatus res;
	if( d != NULL )
	{
		x = static_cast<float>(d[0]);
		y = static_cast<float>(d[1]);
		z = static_cast<float>(d[2]);
		w = static_cast<float>(d[3]);
		res=MS::kSuccess;
	}
	else
		res=MS::kFailure;

	return res;
}

/*!
	The assignment operator.

	\param[in] src The point to copy from.
*/
inline MFloatPoint& MFloatPoint::operator=(const MFloatPoint& src)
{
	x = src.x;
	y = src.y;
	z = src.z;
	w = src.w;
	return (*this);
}

/*!
	The index operator.
	\li If the argument is 0 it will return the x component of the instance.
	\li If the argument is 1 it will return the y component of the instance.
	\li If the argument is 2 it will return the z component of the instance.
	\li If the argument is 3 it will return the w component of the instance.
	Otherwise it will return the x component of the instance.

	\param[in] i value indicating which component to return

	\return
	The value of the indicated component of the instance.
*/
inline float& MFloatPoint::operator() (unsigned int i)
{
	switch( i )
	{
		default:
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}
}
/*!
	The index operator.
	\li If the argument is 0 it will return the x component of the constant
	instance.
	\li If the argument is 1 it will return the y component of the constant
	instance.
	\li If the argument is 2 it will return the z component of the constant
	instance.
	\li If the argument is 3 it will return the w component of the constant
	instance.
	Otherwise it will return the x component of the point.

	\param[in] i value indicating which component to return

	\return
	The value of the indicated component of the instance.
*/
inline float MFloatPoint::operator() (unsigned int i) const
{
	switch( i )
	{
		default:
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}
}

/*!
	The index operator.
	\li If the argument is 0 it will return the x component of the instance.
	\li If the argument is 1 it will return the y component of the instance.
	\li If the argument is 2 it will return the z component of the instance.
	\li If the argument is 3 it will return the w component of the instance.
	Otherwise it will return the x component of the instance.

	\param[in] i value indicating which component to return

	\return
	The value of the indicated component of the instance.
*/
inline float& MFloatPoint::operator[]( unsigned int i )
{
	switch( i )
	{
		default:
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}
}

/*!
	The index operator.
	\li If the argument is 0 it will return the x component of the constant
	instance.
	\li If the argument is 1 it will return the y component of the constant
	instance.
	\li If the argument is 2 it will return the z component of the constant
	instance.
	\li If the argument is 3 it will return the w component of the constant
	instance.
	Otherwise it will return the x component of the point.

	\param[in] i value indicating which component to return

	\return
	The value of the indicated component of the instance.
*/
inline float MFloatPoint::operator[]( unsigned int i ) const
{
	switch( i )
	{
		default:
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}
}

/*!
	Copy the values of x, y, z, and w from the instance to the four
	elements of the given array of doubles.

	\param[out] dest the four element array of doubles

	\return
	MS::kSuccess if dest is a non-zero pointer and MS::kFailure otherwise.
*/
inline MStatus MFloatPoint::get( double dest[4] ) const
{
	if( dest != NULL )
	{
		dest[0] = x;
		dest[1] = y;
		dest[2] = z;
		dest[3] = w;
		return MStatus::kSuccess;
	}
	return MStatus::kFailure;
}

/*!
	Copy the values of x, y, z, and w from the instance to the four
	elements of the given array of floats.

	\param[out] dest the four element array of floats

	\return
	MS::kSuccess if dest is a non-zero pointer and MS::kFailure otherwise.
*/
inline MStatus MFloatPoint::get( float dest[4] ) const
{
	if( dest != NULL )
	{
		dest[0] = x;
		dest[1] = y;
		dest[2] = z;
		dest[3] = w;
		return MStatus::kSuccess;
	}
	return MStatus::kFailure;
}

/*!
	The in-place addition operator for adding an MFloatVector to an
	MFloatPoint. The current instance is translated from its original position
	by the vector.

	\param[in] vector The vector to add.
*/
inline MFloatPoint& MFloatPoint::operator+= (const MFloatVector& vector )
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
	return *this;
}

/*!
	The in-place subtraction operator for subtracting an MFloatVector from an
	MFloatPoint. The current instance is translated from its original position
	by the inverse of the vector.

	\param[in] vector The vector to substract.
*/
inline MFloatPoint& MFloatPoint::operator-= (const MFloatVector& vector )
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	return *this;
}

/*!
	The operator for adding an MFloatVector to an MFloatPoint.
	A new point is returned whose position is that of the original
	point translated by the vector.

	\param[in] other The vector to add.
*/
inline MFloatPoint MFloatPoint::operator+ (const MFloatVector& other) const
{
	if(w==1.0f) {
		return MFloatPoint(x+other.x, y+other.y, z+other.z);
	} else {
		MFloatPoint p1(*this); p1.cartesianize();
		return MFloatPoint(p1.x+other.x, p1.y+other.y, p1.z+other.z);
	}
}

/*!
	The operator for subtracting an MFloatVector from an MFloatPoint.
	A new point is returned whose position is that of the original
	point translated by the inverse of the vector.

	\param[in] other The vector to substract.
*/
inline MFloatPoint MFloatPoint::operator- (const MFloatVector& other) const
{
	if(w==1.0f) {
		return MFloatPoint(x-other.x, y-other.y, z-other.z);
	} else {
		MFloatPoint p1(*this); p1.cartesianize();
		return MFloatPoint(p1.x-other.x, p1.y-other.y, p1.z-other.z);
	}
}

/*!
	The subtraction operator for two MFloatPoints.  The result is the
	MFloatVector from the other point to this instance.

	\param[in] other The point to substract.

	\return
	MFloatVector from the other point to this point.
*/
inline MFloatVector MFloatPoint::operator- (const MFloatPoint& other) const
{
	if(w==1.0f && other.w==1.0f) {
		return MFloatVector(x-other.x, y-other.y, z-other.z);
	} else {
		MFloatPoint p1(*this); p1.cartesianize();
		MFloatPoint p2(other); p2.cartesianize();
		return MFloatVector(p1.x-p2.x, p1.y-p2.y, p1.z-p2.z);
	}
}

/*!
	The multipication operator that allows the vector to by scaled
	by the given parameter.  The x, y, and z components are
	each multiplied by the parameter.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
*/
inline MFloatPoint MFloatPoint::operator* (const int scale) const
{
	return MFloatPoint( x*(float)scale, y*(float)scale, z*(float)scale, w );
}

/*!
	The multipication operator that allows the vector to by scaled
	by the given parameter.  The x, y, and z components are
	each multiplied by the parameter.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
*/
inline MFloatPoint MFloatPoint::operator* (const short scale) const
{
	return MFloatPoint( x*(float)scale, y*(float)scale, z*(float)scale, w );
}

/*!
	The multipication operator that allows the vector to by scaled
	by the given parameter.  The x, y, and z components are
	each multiplied by the parameter.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
*/
inline MFloatPoint MFloatPoint::operator* (const unsigned int scale) const
{
	return MFloatPoint( x*(float)scale, y*(float)scale, z*(float)scale, w );
}

/*!
	The multipication operator that allows the vector to by scaled
	by the given parameter.  The x, y, and z components are
	each multiplied by the parameter.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
*/
inline MFloatPoint MFloatPoint::operator* (const unsigned short scale) const
{
	return MFloatPoint( x*(float)scale, y*(float)scale, z*(float)scale, w );
}

/*!
	The multipication operator that allows the vector to by scaled
	by the given float parameter.  The x, y, and z components are
	each multiplied by the parameter.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
*/
inline MFloatPoint MFloatPoint::operator* (const float scale) const
{
	return MFloatPoint( x*scale, y*scale, z*scale, w );
}

/*!
	The multipication operator that allows the vector to by scaled
	by the given parameter.  The x, y, and z components are
	each multiplied by the parameter.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
*/
inline MFloatPoint MFloatPoint::operator* (const double scale) const
{
	return MFloatPoint( x*(float)scale, y*(float)scale, z*(float)scale, w );
}

/*!
	The division operator that allows the vector to by scaled
	by the given float parameter.  The x, y, and z components are
	each divided by the parameter.
	The w component remains unchanged.

	\param[in] scale The scale parameter.

	\return
	Scaled point.
*/
inline MFloatPoint MFloatPoint::operator/ (const float scale) const
{
	return MFloatPoint( x/scale, y/scale, z/scale, w );
}

/*!
	The equality operator.  Returns true if all of the x, y, z, and w
	components of the two points are identical.

	\param[in] other The point to compare with.
*/
inline bool MFloatPoint::operator== (const MFloatPoint& other) const
{
	return (x == other.x && y == other.y && z == other.z && w == other.w);
}

/*!
	The inequality operator.  Returns true if any of the x, y, z, and w
	components of the two points are not identical.

	\param[in] other The point to compare with.
*/
inline bool MFloatPoint::operator!= (const MFloatPoint& other) const
{
	return !(*this == other);
}

/*!
	Returns true if this instance the the point passed as an argument
	represent the same position within the specified tolerance.

	\param[in] other the other point to compare to
	\param[in] tol the tolerance to use during the comparison

	\return
	True if the points are equal within the given tolerance and false
	otherwise.
*/
inline bool MFloatPoint::isEquivalent(const MFloatPoint& other, float tol) const
{
	MFloatPoint diff = *this - other;
	return (diff.x*diff.x + diff.y*diff.y + diff.z*diff.z) < tol*tol;
}

/*!
	Return the distance between this instance and the point passed as an
	argument.

	\param[in] other the point to compute the distance to

	\return
	The distance between the two points.
*/
inline float MFloatPoint::distanceTo(const MFloatPoint& other) const
{
	MFloatVector diff = *this - other;
	return diff.length();
}

#if ! defined(SWIG)


/*!
	The multiplication operator that allows the scalar value to
	precede the point.  The point's x, y, and z components are
	each multiplied by the scalar.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
	\param[in] p The point to be scaled.

	\return
	A new point containing the value of 'p' scaled by 'scale'.
*/
inline MFloatPoint operator* (int scale, const MFloatPoint& p)
{
	return float(scale) * p;
}

/*!
	The multiplication operator that allows the scalar value to
	precede the point.  The point's x, y, and z components are
	each multiplied by the scalar.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
	\param[in] p The point to be scaled.

	\return
	A new point containing the value of 'p' scaled by 'scale'.
*/
inline MFloatPoint operator* (short scale, const MFloatPoint& p)
{
	return float(scale) * p;
}

/*!
	The multiplication operator that allows the scalar value to
	precede the point.  The point's x, y, and z components are
	each multiplied by the scalar.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
	\param[in] p The point to be scaled.

	\return
	A new point containing the value of 'p' scaled by 'scale'.
*/
inline MFloatPoint operator* (unsigned int scale, const MFloatPoint& p)
{
	return float(scale) * p;
}

/*!
	The multiplication operator that allows the scalar value to
	precede the point.  The point's x, y, and z components are
	each multiplied by the scalar.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
	\param[in] p The point to be scaled.

	\return
	A new point containing the value of 'p' scaled by 'scale'.
*/
inline MFloatPoint operator* (unsigned short scale, const MFloatPoint& p)
{
	return float(scale) * p;
}

/*!
	The multiplication operator that allows the scalar value to
	precede the point.  The point's x, y, and z components are
	each multiplied by the scalar.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
	\param[in] p The point to be scaled.

	\return
	A new point containing the value of 'p' scaled by 'scale'.
*/
inline MFloatPoint operator* (float scale, const MFloatPoint& p)
{
	return MFloatPoint( p.x*scale, p.y*scale, p.z*scale, p.w );
}

/*!
	The multiplication operator that allows the scalar value to
	precede the point.  The point's x, y, and z components are
	each multiplied by the scalar.
	The w component remains unchanged.

	\param[in] scale The scale parameter.
	\param[in] p The point to be scaled.

	\return
	A new point containing the value of 'p' scaled by 'scale'.
*/
inline MFloatPoint operator* (double scale, const MFloatPoint& p)
{
	return float(scale) * p;
}

/*!
	The in-place multiplication operator for computing the scalar product
	of this point instance with the given scalar.

	\param[in] factor Scalar to multiply this point by.

	\return
	This point.
*/
inline MFloatPoint& MFloatPoint::operator*= (const int factor)
{
	x *= (float)factor; y *= (float)factor; z *= (float)factor;
	return *this;
}

/*!
	The in-place multiplication operator for computing the scalar product
	of this point instance with the given scalar.

	\param[in] factor Scalar to multiply this point by.

	\return
	This point.
*/
inline MFloatPoint& MFloatPoint::operator*= (const short factor)
{
	x *= (float)factor; y *= (float)factor; z *= (float)factor;
	return *this;
}

/*!
	The in-place multiplication operator for computing the scalar product
	of this point instance with the given scalar.

	\param[in] factor Scalar to multiply this point by.

	\return
	This point.
*/
inline MFloatPoint& MFloatPoint::operator*= (const unsigned int factor)
{
	x *= (float)factor; y *= (float)factor; z *= (float)factor;
	return *this;
}

/*!
	The in-place multiplication operator for computing the scalar product
	of this point instance with the given scalar.

	\param[in] factor Scalar to multiply this point by.

	\return
	This point.
*/
inline MFloatPoint& MFloatPoint::operator*= (const unsigned short factor)
{
	x *= (float)factor; y *= (float)factor; z *= (float)factor;
	return *this;
}

/*!
	The in-place multiplication operator for computing the scalar product
	of this point instance with the given scalar.

	\param[in] factor Scalar to multiply this point by.

	\return
	This point.
*/
inline MFloatPoint& MFloatPoint::operator*= (const float factor)
{
	x *= factor; y *= factor; z *= factor;
	return *this;
}

/*!
	The in-place multiplication operator for computing the scalar product
	of this point instance with the given scalar.

	\param[in] factor Scalar to multiply this point by.

	\return
	This point.
*/
inline MFloatPoint& MFloatPoint::operator*= (const double factor)
{
	x *= (float)factor; y *= (float)factor; z *= (float)factor;
	return *this;
}

#endif /* ! defined(SWIG) */

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFloatPoint */
