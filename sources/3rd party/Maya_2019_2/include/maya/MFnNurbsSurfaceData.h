#ifndef _MFnNurbsSurfaceData
#define _MFnNurbsSurfaceData
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
// CLASS:    MFnNurbsSurfaceData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnGeometryData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnNurbsSurfaceData)

//! \ingroup OpenMaya MFn
//! \brief Nurbs Surface function set for dependency node data.
/*!

  MFnNurbsSurfaceData allows the creation and manipulation of
  NurbsSurface data objects for use in the dependency graph.

  If a user written dependency node either accepts or produces Nurbs
  Surfaces, then this class is used to extract or create the data that
  comes from or goes to other dependency graph nodes. The
  MDataHandle::type method will return kNurbsSurface when data of this
  type is present.

  If a node is receiving a Nurbs Surface via an input attribute, the
  asNurbsSurface method of MDataHandle can be used to access that
  input Nurbs Surface.

  If a node is to create a Nurbs Surface and send it via an output
  attribute, a new MFnNurbsSurfaceData must be instantiated and then
  the create method called to build the actual data block as an
  MObject.  This MObject should be passed to the
  MFnNurbsSurface::create method as the parentOrOwner parameter so
  that the Nurbs Surface will be constructed inside the data block.
  The MDataHandle::set routine is then used to pass the data block
  down the connection.
*/
class OPENMAYA_EXPORT MFnNurbsSurfaceData : public MFnGeometryData
{
	declareMFn(MFnNurbsSurfaceData, MFnGeometryData);

public:
	MObject		create( MStatus* ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnNurbsSurfaceData, MFnGeometryData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnNurbsSurfaceData */
