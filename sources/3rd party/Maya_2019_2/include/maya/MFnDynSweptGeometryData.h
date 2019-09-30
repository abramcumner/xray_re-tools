#ifndef _MFnDynSweptGeometryData
#define _MFnDynSweptGeometryData
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
// CLASS:    MFnDynSweptGeometryData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// DECLARATIONS


// ****************************************************************************
// CLASS DECLARATION (MFnDynSweptGeometryData)

//! \ingroup OpenMayaFX MFn
//! \brief Swept Geometry function set for dependency node data 
/*!
  MFnDynSweptGeometryData provides access to the MDynSweptLine and
  MDynSweptTriangle data for use in a user defined dependency graph node.
  The data is provided as an output from the geoConnector node and
  is primarily used to determine positional information over time.

  If a user written dependency node accepts MFnDynSweptGeometryData, then
  this class is used to extract data that comes from the geoConnector node.
  The MDataHandle::type method will return kDynSweptGeometry when data of
  this type is present. To access it, the MDataHandle::data() method is
  used to get an MObject for the data and this should then be used
  to initialize an instance of MFnDynSweptGeometryData.

  Important note: Users can create the data for connections but
  cannot produce the contents of the data as this is reserved
  for the Maya Dynamics internals.
*/
class OPENMAYAFX_EXPORT MFnDynSweptGeometryData : public MFnData
{

	declareMFn(MFnDynSweptGeometryData, MFnData);

public:
	int					lineCount( MStatus* ReturnStatus = NULL ) const;
	int					triangleCount( MStatus* ReturnStatus = NULL ) const;
	MDynSweptLine		sweptLine( int index,
							MStatus* ReturnStatus = NULL ) const;
	MDynSweptTriangle	sweptTriangle( int index,
							MStatus* ReturnStatus = NULL ) const;

	MObject				create( MStatus* ReturnStatus=NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnDynSweptGeometryData, MFnData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnDynSweptGeometryData */
