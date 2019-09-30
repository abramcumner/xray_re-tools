#ifndef _MWeight
#define _MWeight
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
// CLASS:    MWeight
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MWeight)

//! \ingroup OpenMaya
//! \brief Component weight data. 
/*!
  Methods for accessing component weight data. This class is currently
  only used to access soft select and symmetry selection weights. Other
  weight data (e.g. deformer weights) does not use this class and can
  be accessed through the corresponding MFn class or directly from the
  node's attributes.
*/
class OPENMAYA_EXPORT MWeight
{
public:
	MWeight();
	MWeight( const MWeight & src );
 	~MWeight();

 	MWeight &     	operator=( const MWeight & other );

 	float  			influence() const;
 	float  			seam() const;
	void			setInfluence( float influence );
	void			setSeam( float seam );

	static const char* className();

protected:
	MWeight( float, float );
 	float 			i;
	float			s;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MWeight */
