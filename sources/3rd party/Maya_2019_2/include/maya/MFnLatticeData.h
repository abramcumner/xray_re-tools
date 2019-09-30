#ifndef _MFnLatticeData
#define _MFnLatticeData
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
// CLASS:    MFnLatticeData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnGeometryData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnLatticeData)

//! \ingroup OpenMaya MFn
//! \brief Lattice Data dependency graph type. 
/*!

  This simple function set is used to create lattice data objects that
  can be transmitted through the dependency graph.  It can also be
  used to extract the lattice geometry from the data object.

  See MDataHandle and MDataBlock for more information about getting
  and setting data in the dependency graph.
*/
class OPENMAYA_EXPORT MFnLatticeData : public MFnGeometryData
{
	declareMFn(MFnLatticeData, MFnGeometryData);

public:
	MObject create( MStatus* ReturnStatus = NULL );

	MObject lattice( MStatus* ReturnStatus = NULL );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnLatticeData, MFnGeometryData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnLatticeData */
