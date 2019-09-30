#ifndef _MEulerRotation
#define _MEulerRotation
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
// CLASS:    MEulerRotation
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MIOStreamFwd.h>


#define kEulerRotationEpsilon 1.0e-10

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MEulerRotation)

//! \ingroup OpenMaya
//! \brief Euler Rotation Math. 
/*!
This class provides methods for working with euler angle rotations.
Euler angles are described by rotations in radians around the x, y, and z axes,
and the order in which those rotations occur.
*/
class OPENMAYA_EXPORT MEulerRotation
{
public:
	//! Valid rotaton orders.
	enum RotationOrder {
		kXYZ,	//!< \nop
		kYZX,	//!< \nop
		kZXY,	//!< \nop
		kXZY,	//!< \nop
		kYXZ,	//!< \nop
		kZYX	//!< \nop
	};

						MEulerRotation();
						MEulerRotation(const MEulerRotation &src);
						MEulerRotation(const MVector &v,
									   RotationOrder ord = kXYZ);
						MEulerRotation(double xx, double yy, double zz,
									   RotationOrder ord = kXYZ);
						~MEulerRotation();
	MEulerRotation &	operator=(const MEulerRotation &src);
 	MEulerRotation &	operator=(const MQuaternion &q);
 	MEulerRotation &	operator=(const MMatrix &m);
 	MEulerRotation &	operator=(const MVector &v);
	MEulerRotation &	setValue(const MVector &v,
								 RotationOrder ord = kXYZ);
	MEulerRotation &	setValue(double xx, double yy, double zz,
								 RotationOrder ord = kXYZ);
	MQuaternion			asQuaternion() const;
	MMatrix				asMatrix() const;
 	MVector				asVector() const;
	double				operator[](unsigned int i) const;
 	MEulerRotation   	operator+(const MEulerRotation &other) const;
	MEulerRotation &	operator+=(const MEulerRotation &other);
 	MEulerRotation   	operator-(const MEulerRotation &other) const;
	MEulerRotation &	operator-=(const MEulerRotation &other);
 	MEulerRotation   	operator-() const;
	MEulerRotation		operator*(const MEulerRotation &other) const;
	MEulerRotation		operator*(const MQuaternion &other) const;
	MEulerRotation		operator*(double other) const;
	MEulerRotation &	operator*=(const MEulerRotation &other);
	MEulerRotation &	operator*=(const MQuaternion &other);
	MEulerRotation &	operator*=(double other);
 	bool           		operator==(const MEulerRotation &other) const;
 	bool          		operator!=(const MEulerRotation &other) const;
 	bool          		isEquivalent(const MEulerRotation &other,
									 double tolerance = kEulerRotationEpsilon)
		                             const;
 	bool          		isZero(double tolerance = kEulerRotationEpsilon) const;
	MEulerRotation &	incrementalRotateBy(const MVector &axis,
											double angle);
	MEulerRotation		inverse() const;
	MEulerRotation &	invertIt();
	MEulerRotation		reorder(RotationOrder ord) const;
	MEulerRotation &	reorderIt(RotationOrder ord);
	MEulerRotation		bound() const;
	MEulerRotation &	boundIt(const MEulerRotation &src);
	MEulerRotation &	boundIt();
	MEulerRotation		alternateSolution() const;
	MEulerRotation &	setToAlternateSolution(const MEulerRotation &src);
	MEulerRotation &	setToAlternateSolution();
	MEulerRotation		closestSolution(const MEulerRotation &dst) const;
	MEulerRotation &	setToClosestSolution(const MEulerRotation &src,
											 const MEulerRotation &dst);
	MEulerRotation &	setToClosestSolution(const MEulerRotation &dst);
	MEulerRotation		closestCut(const MEulerRotation &dst) const;
	MEulerRotation &	setToClosestCut(const MEulerRotation &src,
										const MEulerRotation &dst);
	MEulerRotation &	setToClosestCut(const MEulerRotation &dst);
	static MEulerRotation decompose(const MMatrix &matrix, RotationOrder ord);

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
	double &			operator[](unsigned int i);

	//!	NO SCRIPT SUPPORT
	static MEulerRotation bound(const MEulerRotation &src);
	//!	NO SCRIPT SUPPORT
	static MEulerRotation alternateSolution(const MEulerRotation &src);
	//!	NO SCRIPT SUPPORT
	static MEulerRotation closestSolution(const MEulerRotation &src,
										  const MEulerRotation &dst);
	//!	NO SCRIPT SUPPORT
	static MEulerRotation closestCut(const MEulerRotation &src,
									 const MEulerRotation &dst);

	//!	NO SCRIPT SUPPORT
 	friend OPENMAYA_EXPORT MEulerRotation operator*(double scale,
													const MEulerRotation
													    &other);
	//!	NO SCRIPT SUPPORT
	friend OPENMAYA_EXPORT std::ostream &operator<<(std::ostream &os,
											   const MEulerRotation &rotation);


END_NO_SCRIPT_SUPPORT:

	//! The euler rotation identity.  The rotation order is XYZ.
	static const MEulerRotation identity;

	//! The x component of the euler rotation in radians
	double x;
	//! The y component of the euler rotation in radians
	double y;
	//! The z component of the euler rotation in radians
	double z;
	//! The rotation order of the euler rotation
	RotationOrder order;
#if defined(LINUX)
	char			_padding[4];		// Pad out sizeof class
#endif

protected:
	// No protected members

private:
	// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MEulerRotation */
