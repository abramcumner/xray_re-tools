#ifndef _MFnSphereData
#define _MFnSphereData
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
// CLASS:    MFnSphereData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnSphereData)

//! \ingroup OpenMaya MFn
//! \brief Sphere function set for dependency node data .
/*!
  MFnSphereData allows the creation and manipulation of sphere
  (represented as MObjects) data objects for use in the dependency
  graph.

  If a user written dependency node either accepts or produces
  MObjects containing spheres, then this class is used to extract or
  create the data that comes from or goes to other dependency graph
  nodes. The MDataHandle::type method will return kSphere when data of
  this type is present. To access it, the MDataHandle::data method is
  used to get an MObject for the data and this should then be used to
  initialize an instance of MFnNurbsSurface.
*/
class OPENMAYA_EXPORT MFnSphereData : public MFnData
{
	declareMFn(MFnSphereData, MFnData);

public:
	MObject			create( double rad=1, MStatus* ReturnStatus = NULL );
	double			radius( MStatus* ReturnStatus = NULL ) const;
	MStatus			setRadius( double rad );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnSphereData, MFnData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnSphereData */
