#ifndef _MTransformationMatrix
#define _MTransformationMatrix
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
// CLASS:    MTransformationMatrix
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MDeprecate.h>


#define MTransformationMatrix_kTol 1.0e-10

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MTransformationMatrix)

//! \ingroup OpenMaya
//! \brief Transformation matrix. 
/*!
  An MTransformationMatrix allows the manipulation of the individual
  transformation components (eg scale, rotation, shear, etc) of a four by four
  transformation matrix.

  The transformation in the node is represented as a 4x4 transformation matrix.
  This class allows access to the whole matrix, or the individual components
  (eg scale, rotation, shear, etc) of the transformation.  This breakdown provides
  animators fine control over the animation of these parameters. Therefore, it is
  necessary to describe the order in which these attributes are applied to build
  the final matrix attribute.

  A transformation matrix is composed of the following components:


  \li <b>Scale pivot point</b>        point around which scales are performed [Sp]
  \li <b>Scale</b>                    scaling about x, y, z axes [S]
  \li <b>Shear</b>                    shearing in xy, xz, yx [Sh]
  \li <b>Scale pivot translation</b>  translation introduced to preserve existing
                                       scale transformations when moving pivot.  This
									   is used to prevent the object from moving when
									   the objects pivot point is not at the origin
									   and a non-unit scale is applied to the object [St].
  \li <b>Rotate pivot point</b>       point about which rotations are performed [Rp]
  \li <b>Rotation orientation</b>     rotation to orient local rotation space [Ro]
  \li <b>Rotation</b>                 rotation [R]
  \li <b>Rotate pivot translation</b> translation introduced to preserve exisitng
                                       rotate transformations when moving pivot. This
									   is used to prevent the object from moving when
									   the objects pivot point is not at the origin and
									   the pivot is moved. [Rt]
  \li <b>Translate</b>                translation in x, y, z axes [T]


  Note that the default RotationOrder is kXYZ.

  The matrices are post-multiplied in Maya. For example, to transform
  a point P from object-space to world-space (P') you would need to
  post-multiply by the worldMatrix. (P' = P x WM)

  The transformation matrix is then constructed as follows:

\code
     -1                       -1
  [Sp]x[S]x[Sh]x[Sp]x[St]x[Rp]x[Ro]x[R]x[Rp]x[Rt]x[T]
\endcode

  where 'x' denotes matrix multiplication and '-1' denotes matrix inversion

\code
     Sp = |  1    0    0    0 |     St = |  1    0    0    0 |
          |  0    1    0    0 |          |  0    1    0    0 |
          |  0    0    1    0 |          |  0    0    1    0 |
          | spx  spy  spz   1 |          | sptx spty sptz  1 |

     S  = |  sx   0    0    0 |     Sh = |  1    0    0    0 |
          |  0    sy   0    0 |          | shxy  1    0    0 |
          |  0    0    sz   0 |          | shxz shyz  1    0 |
          |  0    0    0    1 |          |  0    0    0    1 |

     Rp = |  1    0    0    0 |     Rt = |  1    0    0    0 |
          |  0    1    0    0 |          |  0    1    0    0 |
          |  0    0    1    0 |          |  0    0    1    0 |
          | rpx  rpy  rpz   1 |          | rptx rpty rptz  1 |

     Ro = AX * AY * AZ

     AX = |  1    0    0    0 |     AY = |  cy   0   -sy   0 |
          |  0    cx   sx   0 |          |  0    1    0    0 |
          |  0   -sx   cx   0 |          |  sy   0    cy   0 |
          |  0    0    0    1 |          |  0    0    0    1 |

     AZ = |  cz   sz   0    0 |     sx = sin(rax), cx = cos(rax)
          | -sz   cz   0    0 |     sy = sin(ray), cx = cos(ray)
          |  0    0    1    0 |     sz = sin(raz), cz = cos(raz)
          |  0    0    0    1 |

  	 R  = RX * RY * RZ  (Note: order is determined by rotateOrder)

     RX = |  1    0    0    0 |     RY = |  cy   0   -sy   0 |
          |  0    cx   sx   0 |          |  0    1    0    0 |
          |  0   -sx   cx   0 |          |  sy   0    cy   0 |
          |  0    0    0    1 |          |  0    0    0    1 |

     RZ = |  cz   sz   0    0 |     sx = sin(rx), cx = cos(rx)
          | -sz   cz   0    0 |     sy = sin(ry), cx = cos(ry)
          |  0    0    1    0 |     sz = sin(rz), cz = cos(rz)
          |  0    0    0    1 |

     T  = |  1    0    0    0 |
          |  0    1    0    0 |
          |  0    0    1    0 |
          |  tx   ty   tz   1 |
\endcode
*/
class OPENMAYA_EXPORT MTransformationMatrix
{
public:
	//! Order of rotation.
	enum RotationOrder {
		kInvalid,	//!< \nop
		kXYZ,		//!< \nop
		kYZX,		//!< \nop
		kZXY,		//!< \nop
		kXZY,		//!< \nop
		kYXZ,		//!< \nop
		kZYX,		//!< \nop
		kLast		//!< \nop
	};
	~MTransformationMatrix();
	MTransformationMatrix();
	MTransformationMatrix( const MTransformationMatrix& src );
	MTransformationMatrix( const MMatrix& src );
	MTransformationMatrix&   	operator= ( const MTransformationMatrix &src );
	MTransformationMatrix&   	operator= ( const MMatrix &src );
	MMatrix		asMatrix()					const;
 	MMatrix		asMatrix(double percent)	const;
 	MMatrix		asMatrixInverse()			const;
	MMatrix		asScaleMatrix()				const;
 	MMatrix		asRotateMatrix()			const;
 	MStatus		getScale( double scale[3], MSpace::Space space) const;
 	MStatus		setScale( const double scale[3], MSpace::Space space);
 	MStatus		addScale( const double scale[3], MSpace::Space space);
	MQuaternion	rotation() const;
	MTransformationMatrix &rotateTo( const MQuaternion &q );
	MTransformationMatrix &rotateBy( const MQuaternion &q,
									 MSpace::Space space,
									 MStatus *ReturnStatus = NULL);
	MEulerRotation	eulerRotation() const;
	MTransformationMatrix &rotateTo( const MEulerRotation &e );
	MTransformationMatrix &rotateBy( const MEulerRotation &e,
									 MSpace::Space space,
									 MStatus *ReturnStatus = NULL);
 	MStatus		getRotationQuaternion( double &x, double &y, double &z,
									   double &w ) const;
 	MStatus		setRotationQuaternion( double x, double y, double z,
									   double w );
 	MStatus		addRotationQuaternion( double x, double y, double z,
									   double w, MSpace::Space );
 	MStatus		getRotation( double[3],
							 RotationOrder& order ) const;
 	MStatus		setRotation( const double rotation[3],
							 RotationOrder order );
	MStatus		addRotation( const double rotation[3],
							 RotationOrder order, MSpace::Space );
	RotationOrder	rotationOrder( MStatus* ReturnStatus = NULL ) const;
	MStatus	    reorderRotation( const RotationOrder& order );
	MStatus     setToRotationAxis( const MVector &axis, double rotation );
	MQuaternion rotationOrientation() const;
	MTransformationMatrix &setRotationOrientation( const MQuaternion &q );
	MVector		getTranslation( MSpace::Space, MStatus* ReturnStatus=NULL ) const;
 	MStatus		setTranslation( const MVector& vector, MSpace::Space );
	MStatus		addTranslation( const MVector& vector, MSpace::Space );
	MStatus		setShear( const double shear[3], MSpace::Space space);
	MStatus		getShear( double shear[3], MSpace::Space space) const;
	MStatus		addShear( const double shear[3], MSpace::Space space);
	MPoint		scalePivot( MSpace::Space, MStatus* ReturnStatus = NULL ) const;
	MStatus		setScalePivot( const MPoint&, MSpace::Space, bool balance );
	MVector		scalePivotTranslation(MSpace::Space,
									  MStatus* ReturnStatus=NULL )const;
	MStatus		setScalePivotTranslation( const MVector& vector,
										  MSpace::Space );
	MPoint		rotatePivot( MSpace::Space, MStatus* ReturnStatus=NULL ) const;
	MStatus		setRotatePivot( const MPoint& point,
								MSpace::Space, bool balance );
	MVector		rotatePivotTranslation( MSpace::Space space,
					MStatus* ReturnStatus=NULL ) const;
	MStatus		setRotatePivotTranslation( const MVector& vector,
										   MSpace::Space space );
 	bool		isEquivalent ( const MTransformationMatrix &other,
					double tolerance = MTransformationMatrix_kTol ) const;
 	bool		operator == (const MTransformationMatrix & other ) const;
	bool		operator != ( const MTransformationMatrix & other ) const;

    // Obsolete
	OPENMAYA_DEPRECATED(2019, "Use the other MTransformationMatrix::getRotation method instead.")
 	MStatus		getRotation( double[3],
							 RotationOrder& order, MSpace::Space ) const;
    // Obsolete
	OPENMAYA_DEPRECATED(2019, "Use the other MTransformationMatrix::setRotation method instead.")
 	MStatus		setRotation( const double rotation[3],
							 RotationOrder order, MSpace::Space );
    // Obsolete
	OPENMAYA_DEPRECATED(2019, "Use the other MTransformationMatrix::getRotationQuaternion method instead.")
 	MStatus		getRotationQuaternion( double &x, double &y, double &z,
									   double &w, MSpace::Space ) const;
    // Obsolete
	OPENMAYA_DEPRECATED(2019, "Use the other MTransformationMatrix::setRotationQuaternion method instead.")
 	MStatus		setRotationQuaternion( double x, double y, double z,
									   double w, MSpace::Space );
    // Obsolete
	OPENMAYA_DEPRECATED(2019, "Use MTransformationMatrix::getTranslation method instead.")
	MVector		translation( MSpace::Space, MStatus* ReturnStatus=NULL ) const;

	static const char*	className();

	//! The identity transformation matrix.
	static const MTransformationMatrix identity;


protected:
// No protected members

private:
	const void*		data;
	MTransformationMatrix( const void* );
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MTransformationMatrix */
