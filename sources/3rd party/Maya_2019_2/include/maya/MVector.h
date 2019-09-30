#ifndef _MVector
#define _MVector
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
// CLASS:    MVector
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MTransformationMatrix.h>
#include <math.h> // for sqrt

#define MVector_kTol 1.0e-10

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MVector)

//! \ingroup OpenMaya
//! \brief A vector math class for vectors of doubles.
/*!
This class provides access to Maya's internal vector math library allowing
vectors to be handled easily, and in a manner compatible with internal
Maya data structures.

All methods that query the vector are threadsafe, all methods that
modify the vector are not threadsafe.

*/
class OPENMAYA_EXPORT MVector
{
public:
	//! Axes.
	enum Axis {
		kXaxis,	//!< \nop
		kYaxis,	//!< \nop
		kZaxis,	//!< \nop
		kWaxis	//!< \nop
	};

					MVector();
					MVector( const MVector&);
					MVector( const MFloatVector&);
					MVector( const MPoint&);
					MVector( const MFloatPoint&);
					MVector( double xx, double yy, double zz = 0.0);
					MVector( const double d[3] );
					MVector( const float f[3] );
					~MVector();
 	MVector&		operator= ( const MVector& src );
 	double   		operator()( unsigned int i ) const;
	double			operator[]( unsigned int i )const;
 	MVector			operator^( const MVector& right) const;
 	double          operator*( const MVector& right ) const;
 	MVector&   		operator/=( double scalar );
 	MVector     	operator/( double scalar ) const;
	MVector&		operator*=( int scalar );
	MVector&		operator*=( short scalar );
	MVector&		operator*=( unsigned int scalar );
	MVector&		operator*=( unsigned short scalar );
	MVector&		operator*=( float scalar );
 	MVector& 		operator*=( double scalar );
	MVector			operator*( int scalar ) const;
	MVector			operator*( short scalar ) const;
	MVector			operator*( unsigned int scalar ) const;
	MVector			operator*( unsigned short scalar ) const;
	MVector			operator*( float scalar ) const;
 	MVector   		operator*( double scalar ) const;
 	MVector   		operator+( const MVector& other) const;
	MVector&		operator+=( const MVector& other );
 	MVector   		operator-() const;
	MVector&		operator-=( const MVector& other );
 	MVector   		operator-( const MVector& other ) const;
 	MVector  		operator*( const MMatrix&) const;
 	MVector&		operator*=( const MMatrix&);
 	bool          	operator!=( const MVector& other ) const;
 	bool           	operator==( const MVector& other ) const;
	MVector			rotateBy( double x, double y, double z, double w) const;
	MVector			rotateBy( const double rotXYZ[3],
							  MTransformationMatrix::RotationOrder order )
                              const;
	MVector			rotateBy( MVector::Axis axis, const double angle ) const;
	MVector			rotateBy( const MQuaternion & ) const;
	MVector			rotateBy( const MEulerRotation & ) const;
	MQuaternion		rotateTo( const MVector & ) const;
	MStatus			get( double[3] ) const;
 	double         	length() const;
 	MVector  		normal() const;
	MStatus			normalize();
 	double       	angle( const MVector& other ) const;
	bool			isEquivalent( const MVector& other,
						double tolerance = MVector_kTol ) const;
 	bool          	isParallel( const MVector& other,
						double tolerance = MVector_kTol ) const;
	MVector			transformAsNormal( const MMatrix & matrix ) const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	double&      	operator()( unsigned int i );

	//!	NO SCRIPT SUPPORT
 	double&      	operator[]( unsigned int i );

	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MVector operator*( const MMatrix&, const MVector&);
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MVector operator*( int, const MVector&);
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MVector operator*( short, const MVector&);
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MVector operator*( unsigned int, const MVector&);
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MVector operator*( unsigned short, const MVector&);
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MVector operator*( float, const MVector&);
	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MVector operator*( double, const MVector&);

	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream&	operator<<(std::ostream& os, const MVector& v);

END_NO_SCRIPT_SUPPORT:

	//! The null vector
	static const MVector zero;
	//! The vector <1,1,1>
	static const MVector one;
	//! Unit vector in the positive x direction
	static const MVector xAxis;
	//! Unit vector in the positive y direction
	static const MVector yAxis;
	//! Unit vector in the positive z direction
	static const MVector zAxis;
	//! Unit vector in the negative z direction
	static const MVector xNegAxis;
	//! Unit vector in the negative z direction
	static const MVector yNegAxis;
	//! Unit vector in the negative z direction
	static const MVector zNegAxis;
	//! The x component of the vector
	double x;
	//! The y component of the vector
	double y;
	//! The z component of the vector
	double z;

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

//	These need to be included *after* the MVector class definition,
//	otherwise we end up with a chicken-and-egg situation due to mutual
//	header inclusions.
//
#ifndef _MFloatPoint
#include <maya/MFloatPoint.h>
#endif
#ifndef _MFloatVector
#include <maya/MFloatVector.h>
#endif
#ifndef _MPoint
#include <maya/MPoint.h>
#endif

OPENMAYA_MAJOR_NAMESPACE_OPEN

/*!
	The default class constructor. Creates a null vector.
*/
inline MVector::MVector()
 	: x(0.0)
	, y(0.0)
	, z(0.0)
{
}

/*!
	The copy constructor.  Create a new vector and initialize it
	to the same values as the given vector.

	\param[in] src the vector object to copy
*/
inline MVector::MVector(const MVector& src)
 	: x(src.x)
	, y(src.y)
	, z(src.z)
{
}

/*!
	Class constructor.  Create a new vector and initialize it
	to the same values as the given MFloatVector.

	\param[in] src the vector object to copy
*/
inline MVector::MVector(const MFloatVector& src)
 	: x(src.x)
	, y(src.y)
	, z(src.z)
{
}

/*!
	Class constructor.  Initializes the vector with the
	explicit x, y and z values provided as arguments.

	\param[in] xx the x component of the vector
	\param[in] yy the y component of the vector
	\param[in] zz the z component of the vector.  Defaults to 0.0.
*/
inline MVector::MVector(double xx, double yy, double zz)
 	: x(xx)
	, y(yy)
	, z(zz)
{
}

/*!
	Class constructor.  Initializes the vector with the
	explicit x, y and z values provided in the given double array.

	\param[in] d the 3 element array containing the initial x, y, and z values
*/
inline MVector::MVector( const double d[3] )
 	: x(d[0])
	, y(d[1])
	, z(d[2])
{
}

/*!
	Class constructor.  Initializes the vector with the
	explicit x, y and z values provided in the given float array.

	\param[in] f the 3 element array containing the initial x, y, and z values
*/
inline MVector::MVector( const float f[3] )
 	: x(f[0])
	, y(f[1])
	, z(f[2])
{
}

/*!
	Class constructor.  Create a new vector and initialize it
	to the same x, y, z values as the given MPoint.

	\param[in] src the point object to copy
*/
inline MVector::MVector(const MPoint& src)
 	: x(src.x)
	, y(src.y)
	, z(src.z)
{
}

/*!
	Class constructor.  Create a new vector and initialize it
	to the same x, y, z values as the given point.

	\param[in] src the point object to copy
*/
inline MVector::MVector(const MFloatPoint& src)
 	: x(src.x)
	, y(src.y)
	, z(src.z)
{
}

/*!
	Class destructor.
*/
inline MVector::~MVector()
{
}

/*!
	The assignment operator.  Allows assignment between MVectors.

	\param[in] src Vector to copy from.
*/
inline MVector& MVector::operator= (const MVector& src)
{
	x=src.x;
	y=src.y;
	z=src.z;
	return (*this);
}

/*!
	The index operator.  If its argument is 0 it will return the
	x component of the vector.  If its argument is 1 it will return
	the y component of the vector.  Otherwise it will return the
	z component of the vector.

	\param[in] i value indicating which component to return
*/
inline double& MVector::operator()( unsigned int i )
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

	\param[in] i value indicating which component to return
*/
inline double MVector::operator()( unsigned int i ) const
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

	\param[in] i value indicating which component to return
*/
inline double& MVector::operator[]( unsigned int i )
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

	\param[in] i value indicating which component to return
*/
inline double MVector::operator[]( unsigned int i ) const
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
*/
inline MVector MVector::operator^ (const MVector& right) const
{
	return MVector(y*right.z - z*right.y,
				   z*right.x - x*right.z,
				   x*right.y - y*right.x);
}

/*!
	The dot product operator.

	\param[in] right Vector to take the dot product with.
*/
inline double MVector::operator* (const MVector& right) const
{
	return (x*right.x + y*right.y + z*right.z);
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector& MVector::operator*= (int scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}


/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector& MVector::operator*= (short scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector& MVector::operator*= (unsigned int scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector& MVector::operator*= (unsigned short scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector& MVector::operator*= (float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

/*!
	The in place multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector& MVector::operator*= (double scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector MVector::operator* ( int scalar ) const
{
	MVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector MVector::operator* ( short scalar ) const
{
	MVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector MVector::operator* ( unsigned int scalar ) const
{
	MVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector MVector::operator* ( unsigned short scalar ) const
{
	MVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector MVector::operator* ( float scalar ) const
{
	MVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The multiplication operator.

	\param[in] scalar Scale factor.
*/
inline MVector MVector::operator* ( double scalar ) const
{
	MVector tmp(*this);
	tmp *= scalar;
	return tmp;
}

/*!
	The in place division operator.

	\param[in] scalar Division factor.
*/
inline MVector& MVector::operator/= (double scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

/*!
	The division operator.

	\param[in] scalar Division factor.
*/
inline MVector MVector::operator/ ( double scalar ) const
{
	MVector tmp(*this);
	tmp /= scalar;
	return tmp;
}

/*!
	The vector subtraction operator.

	\param[in] other Vector to substract.
*/
inline MVector MVector::operator- (const MVector& other) const
{
	return MVector(x-other.x, y-other.y, z-other.z);
}

/*!
	The vector addition operator.

	\param[in] other Vector to add.
*/
inline MVector MVector::operator+ (const MVector& other) const
{
	return MVector(x+other.x, y+other.y, z+other.z);
}

/*!
	The in place vector addition operator.

	\param[in] other Vector to add.
*/
inline MVector& MVector::operator+= (const MVector& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

/*!
	The unary minus operator.  Negates the value of each of the
	x, y, and z components of the vector.
*/
inline MVector MVector::operator- () const
{
	return MVector(-x,-y,-z);
}

/*!
	The in place vector subtraction operator.

	\param[in] other Vector to subtract.
*/
inline MVector& MVector::operator-= (const MVector& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

/*!
	Performs an in place normalization of the vector

	\return
	Always returns MS::kSuccess.
*/
inline MStatus MVector::normalize()
{
	double lensq = x*x + y*y + z*z;
	if(lensq>1e-20) {
		double factor = 1.0 / sqrt(lensq);
		x *= factor;
		y *= factor;
		z *= factor;
	}
	return MStatus::kSuccess;
}

/*!
	Return a normalized copy of this vector.
*/
inline MVector MVector::normal() const
{
	MVector tmp(*this);
	tmp.normalize();
	return tmp;
}

/*!
	Return the length of the vector.
*/
inline double MVector::length() const
{
	return sqrt(x*x+y*y+z*z);
}

/*!
	Returns true if the vector and the one passed as an
    argument are equal to each other within the specified tolerance.

	\param[in] other The vector to compare to.
	\param[in] tol The tolerance to use during the comparison.

	\return
	Bool true if the vectors are equivalent and false otherwise.
*/
inline bool MVector::isEquivalent(const MVector& other, double tol) const
{
	MVector diff = *this - other;
	return (diff.x*diff.x + diff.y*diff.y + diff.z*diff.z) < tol*tol;
}

/*!
	The vector equality operator.  This returns true if all three
	of the x, y, and z components are identical.

	\param[in] other The vector to compare to.

	\return
	Bool true if the vectors are identical and false otherwise.
*/
inline bool MVector::operator== (const MVector& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

/*!
	The vector inequality operator.  This returns false if all three
	of the x, y, and z components are identical.

	\param[in] other The vector to compare to.

	\return
	Bool false if the vectors are identical and true otherwise.
*/
inline bool MVector::operator!= (const MVector& other) const
{
	return !(*this == other);
}

/*!
	Extracts the x, y, and z components of the vector and places
	them in elements 0, 1, and 2 of the double array passed.

	\param[out] dest the array of 3 doubles into which the results are placed.

	\return
	MS::kSuccess if dest is a non-zero pointer and MS::kFailure otherwise.
*/
inline MStatus MVector::get( double dest[3] ) const
{
	if(dest != NULL)
	{
		dest[0] = x;
		dest[1] = y;
		dest[2] = z;
		return MStatus::kSuccess;
	}
	return MStatus::kFailure;
}

#if ! defined(SWIG)

/*!
	The multiplication operator that allows the scalar value
	to preceed the vector.

	\param[in] scalar Scale factor.
	\param[in] other Vector to scale.

	\return
	The scales vector.
*/
inline MVector operator * (double scalar, const MVector& other)
{
	return MVector(scalar*other.x, scalar*other.y, scalar*other.z);
}

/*!
	The multiplication operator that allows the scalar value
	to preceed the vector.

	\param[in] scalar Scale factor.
	\param[in] other Vector to scale.

	\return
	The scales vector.
*/
inline MVector operator * (float scalar, const MVector& other)
{
	return double(scalar) * other;
}

/*!
	The multiplication operator that allows the scalar value
	to preceed the vector.

	\param[in] scalar Scale factor.
	\param[in] other Vector to scale.

	\return
	The scales vector.
*/
inline MVector operator * (unsigned short scalar, const MVector& other)
{
	return double(scalar) * other;
}

/*!
	The multiplication operator that allows the scalar value
	to preceed the vector.

	\param[in] scalar Scale factor.
	\param[in] other Vector to scale.

	\return
	The scales vector.
*/
inline MVector operator * (unsigned int scalar, const MVector& other)
{
	return double(scalar) * other;
}

/*!
	The multiplication operator that allows the scalar value
	to preceed the vector.

	\param[in] scalar Scale factor.
	\param[in] other Vector to scale.

	\return
	The scales vector.
*/
inline MVector operator * (short scalar, const MVector& other)
{
	return double(scalar) * other;
}

/*!
	The multiplication operator that allows the scalar value
	to preceed the vector.

	\param[in] scalar Scale factor.
	\param[in] other Vector to scale.

	\return
	The scales vector.
*/
inline MVector operator * (int scalar, const MVector& other)
{
	return double(scalar) * other;
}
#endif // ! defined(SWIG)

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MVector */
