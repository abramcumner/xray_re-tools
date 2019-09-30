#ifndef _MFnSubdData
#define _MFnSubdData
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
// CLASS:    MFnSubdData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnGeometryData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnSubdData)

//! \ingroup OpenMaya MFn
//! \brief Subdiv Surface function set for dependency node data. 
/*!
MFnSubdData allows the creation and manipulation of Subdivision Surface
data objects for use in the dependency graph.

If a user written dependency node either accepts or produces Subdivision
Surfaces, then this class is used to extract or create the data that
comes from or goes to other dependency graph nodes. The
MDataHandle::type method will return kSubdiv when data of this type
is present.

If a node is receiving a Subdivision Surface via an input attribute, the
asSubdSurface method of MDataHandle can be used to access that input
Subdivision Surface.

If a node is to create a Subdivision Surface and send it via an output
attribute, a new MFnSubdData must be instantiated and then the create method
called to build the actual data block as an MObject.  This MObject should
be passed to the MFnSubd::create method as the parentOrOwner
parameter so that the Subdivision Surface will be constructed inside the data
block.  The MDataHandle::set routine is then used to pass the data block
down the connection.
*/
class OPENMAYA_EXPORT MFnSubdData : public MFnGeometryData
{
	declareMFn(MFnSubdData, MFnGeometryData);

public:
	MObject		create( MStatus* ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnSubdData, MFnGeometryData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnSubdData */
