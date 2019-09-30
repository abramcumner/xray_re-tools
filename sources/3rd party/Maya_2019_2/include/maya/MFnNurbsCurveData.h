#ifndef _MFnNurbsCurveData
#define _MFnNurbsCurveData
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
// CLASS:    MFnNurbsCurveData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnGeometryData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnNurbsCurveData)

//! \ingroup OpenMaya MFn
//! \brief Nurbs Curve function set for dependency node data.
/*!
  MFnNurbsCurveData allows the creation and manipulation of NurbsCurve
  data objects for use in the dependency graph.

  If a user written dependency node either accepts or produces Nurbs
  Curves, then this class is used to create data blocks into which
  Nurbs Curves can be constructed before being passed to other
  dependency graph nodes.  The MDataHandle::type method will return
  kNurbsCurve when data of this type is present.

  If a node is receiving a Nurbs Curve via an input attribute, the
  asNurbsCurve method of MDataHandle can be used to access that input
  Nurbs Curve.

  If a node is to create a Nurbs Curve and send it via an output
  attribute, a new MFnNurbsCurveData must be instantiated and then the
  create method called to build the actual data block as an MObject.
  This MObject should be passed to the MFnNurbsCurve::create method as
  the parentOrOwner parameter so that the Nurbs Curve will be
  constructed inside the data block.  The MDataHandle::set routine is
  then used to pass the data block down the connection.
*/
class OPENMAYA_EXPORT MFnNurbsCurveData : public MFnGeometryData
{
	declareMFn(MFnNurbsCurveData, MFnGeometryData);

public:
	MObject		create( MStatus* ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnNurbsCurveData, MFnGeometryData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnNurbsCurveData */
