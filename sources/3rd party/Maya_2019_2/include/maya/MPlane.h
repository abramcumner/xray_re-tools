#ifndef _MPlane
#define _MPlane
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
// CLASS:    MPlane
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MVector.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS


// ****************************************************************************
// CLASS DECLARATION (MPlane)

//! \ingroup OpenMaya
//! \brief A plane equation.
/*!
This class describes a mathematical plane.
*/
class OPENMAYA_EXPORT MPlane
{
public:

					MPlane();
					MPlane( const MPlane&);

	MVector  		normal() const;
	double			distance() const;

	void			setPlane( double a, double b, double c, double d);
	void			setPlane( const MVector& n, double d);

	double			distance( const MVector& ) const;
	double			directedDistance( const MVector& ) const;

	static const char* className();

protected:
// No protected members

private:
	double			a, b, c, d;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPlane */
