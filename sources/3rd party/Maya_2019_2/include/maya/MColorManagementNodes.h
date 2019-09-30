#ifndef _MColorManagementNodes
#define _MColorManagementNodes
//
//-
// ===========================================================================
// Copyright 2017 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MTypes.h>


// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

class MStatus;
class MObject;
class MDGModifier;


// ****************************************************************************
// CLASS DECLARATION (MColorManagementNodes)
//! \ingroup OpenMayaRender
//! \brief Utilities for manipulating color manageable nodes.
class OPENMAYARENDER_EXPORT MColorManagementNodes
{
public:
    static const char* className();

    static bool isColorManageable(const MObject& object);
    static bool isColorManaged(const MObject& object);

    static MStatus colorManage(MObject& object, const MString& filename, MDGModifier& dgModifier);

    static MStatus colorManageAllNodes(MDGModifier& dgModifier);

private:
    ~MColorManagementNodes();
#ifdef __GNUC__
    friend class shutUpAboutPrivateDestructors;
#endif
};

// ****************************************************************************

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MColorManagementNodes */
