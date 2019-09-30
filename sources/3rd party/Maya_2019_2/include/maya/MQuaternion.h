#ifndef _MQuaternion
#define _MQuaternion
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
// CLASS:    MQuaternion
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MTransformationMatrix.h>


#define kQuaternionEpsilon 1.0e-10

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MQuaternion)

//! \ingroup OpenMaya
//! \brief Quaternion math.
/*!
	This class provides methods for working with Quaternions.
	Quaternions can be used to specify orientations and rotations of 3-D
	objects relative to a starting reference, similar to the way that cartesian
	vectors can be used to specify positions and translations of 3-D objects
	relative to an origin.  Quaternions represent orientations as a single
	rotation, just as rectangular co-ordinates represent position as a single
	vector.
*/
class OPENMAYA_EXPORT MQuaternion
{
public:
					MQuaternion();
					MQuaternion(const MQuaternion &src);
					MQuaternion(double xx, double yy, double zz, double ww);
					MQuaternion(const double q[4]);
					MQuaternion(const MVector &a, const MVector &b);
					MQuaternion(const MVector &a, const MVector &b,
								double angleFactor);
					MQuaternion(double angle, const MVector &axisb);
					~MQuaternion();
 	MQuaternion &	operator=(const MQuaternion& src);
	MQuaternion &	operator=(const MMatrix &matrix);
	MQuaternion &	operator=(const MEulerRotation &matrix);
	MMatrix			asMatrix() const;
	MEulerRotation	asEulerRotation() const;
	MQuaternion &	setAxisAngle(const MVector &axis, double theta);
	bool			getAxisAngle(MVector &axis, double &theta) const;
	MQuaternion &	setToXAxis(double theta);
	MQuaternion &	setToYAxis(double theta);
	MQuaternion &	setToZAxis(double theta);
	MStatus			get(double dest[4]) const;
	double			operator[](unsigned int i) const;
 	MQuaternion   	operator+(const MQuaternion &other) const;
 	MQuaternion   	operator*(const MQuaternion &other) const;
 	MQuaternion & 	operator*=(const MQuaternion &rhs);
 	MQuaternion   	operator-(const MQuaternion &other) const;
 	MQuaternion   	operator-() const;
	MQuaternion &	negateIt();
 	bool           	operator==(const MQuaternion &other) const;
 	bool          	operator!=(const MQuaternion &other) const;
 	bool          	isEquivalent(const MQuaternion &other,
								 double tolerance = kQuaternionEpsilon) const;
	MQuaternion &	scaleIt(double scale);
	MQuaternion 	normal() const;
	MQuaternion &	normalizeIt();
	MQuaternion 	conjugate() const;
	MQuaternion &	conjugateIt();
	MQuaternion 	inverse() const;
	MQuaternion &	invertIt();
	MQuaternion		log() const;
	MQuaternion		exp() const;

	//! The multiplicative identity.
	static const MQuaternion identity;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	double &		operator[](unsigned int i);

	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MQuaternion operator*(double scale,
												 const MQuaternion &other);
	friend OPENMAYA_EXPORT MQuaternion slerp(const MQuaternion &p,
											 const MQuaternion &q,
											 double t);
	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MQuaternion slerp(const MQuaternion &p,
											 const MQuaternion &q,
											 double t,
											 short spin);
    //!	NO SCRIPT SUPPORT
    friend OPENMAYA_EXPORT MQuaternion dslerp(const MQuaternion &p,
                                             const MQuaternion &q,
                                             double t,
                                             int dir);
    //!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MQuaternion squad(const MQuaternion &p,
											 const MQuaternion &a,
											 const MQuaternion &b,
											 const MQuaternion &q,
											 double t);
	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MQuaternion squad(const MQuaternion &p,
											 const MQuaternion &a,
											 const MQuaternion &b,
											 const MQuaternion &q,
											 double t, short spin);
	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT MQuaternion squadPt(const MQuaternion &q0,
											   const MQuaternion &q1,
											   const MQuaternion &q2);
	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MQuaternion &q);

	//!	NO SCRIPT SUPPORT
	operator		MMatrix() const;

END_NO_SCRIPT_SUPPORT:

	//! The quaternion's imaginary x component.
	double x;

	//! The quaternion's imaginary y component.
	double y;

	//! The quaternion's imaginary z component.
	double z;

	//! The quaternion's real component.
	double w;

protected:
	// No protected members

private:
	// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MQuaternion */
