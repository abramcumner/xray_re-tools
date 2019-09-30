#ifndef _MFnGeometryData
#define _MFnGeometryData
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
// CLASS:    MFnGeometryData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnData.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnGeometryData)

//! \ingroup OpenMaya MFn
//! \brief  Geometry data function set. 
/*!
This class is the function set for geometry data.

Geometry data adds matrix and grouping (set) information to regular data
and is used to pass geometry types such as mesh, lattice, and NURBS shape
data through DG connections.

User defined geometry data types (MPxGeometryData) are also supported
by this class.

The matrix within geometry data is the matrix used to convert the object
into local space.
*/
class OPENMAYA_EXPORT MFnGeometryData : public MFnData
{
    declareMFn( MFnGeometryData, MFnData );

public:

    MStatus        setMatrix( const MMatrix & );
    MStatus        getMatrix ( MMatrix & ) const;
    bool           matrixIsIdentity( MStatus * ReturnStatus = NULL ) const;
    bool           matrixIsNotIdentity( MStatus * ReturnStatus = NULL ) const;

    // methods to add/remove object groups
    //
    bool           hasObjectGroup( unsigned int id,
                                   MStatus* ReturnStatus = NULL  ) const;
    MStatus        addObjectGroup( unsigned int );
    MStatus        removeObjectGroup( unsigned int );
    MStatus        changeObjectGroupId( unsigned int , unsigned int );

    // methods to get the i'th object group
    //
    unsigned int       objectGroupCount( MStatus * ReturnStatus = NULL ) const;
    unsigned int       objectGroup( unsigned int index,
                                MStatus* ReturnStatus = NULL ) const;

    // method to get the type of component stored by an object group
    //
    MFn::Type      objectGroupType( unsigned int,
                                    MStatus* ReturnStatus = NULL ) const;

    // methods to access and edit members of object group
    //
    MObject        objectGroupComponent( unsigned int,
                                         MStatus* ReturnStatus = NULL ) const;
    MStatus        setObjectGroupComponent( unsigned int, MObject & );
    MStatus        addObjectGroupComponent( unsigned int, MObject & );
    MStatus        removeObjectGroupComponent( unsigned int, MObject & );

    // Method to copy object groups from the geometry data sent in.
    //
    MStatus        copyObjectGroups( MObject & inGeom );

BEGIN_NO_SCRIPT_SUPPORT:

 	declareMFnConstConstructor( MFnGeometryData, MFnData );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No Private members

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnGeometryData */
