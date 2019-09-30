#ifndef _MNodeClass
#define _MNodeClass
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
// CLASS:    MNodeClass
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MTypeId.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MNodeClass)

//! \ingroup OpenMaya
//! \brief A class for performing node class-level operations in the dependency graph.
/*!
   MNodeClass allows the manipulation of dependency graph node classes. Operations
   specific to a particular node should instead use MFnDependencyNode.
*/

class OPENMAYA_EXPORT MNodeClass
{
public:
    MNodeClass( const MString &nodeClassName );
    MNodeClass( const MTypeId &nodeTypeId );
    MTypeId typeId					()										const;
    MString typeName				()										const;
    MString classification			()										const;
	MStatus	addToClassification		(const MString & str);
	MStatus	removeFromClassification(const MString & str);
    MString pluginName				(MStatus *ReturnStatus=NULL)			const;
    MStatus addExtensionAttribute	(const MObject & attr)					const;
    MStatus removeExtensionAttribute(const MObject & attr)					const;
    MStatus removeExtensionAttributeIfUnset(const MObject & attr)			const;
    MStatus getAttributes			(MObjectArray &attr)					const;
    unsigned int attributeCount		(MStatus *ReturnStatus=NULL)			const;
    MObject attribute(unsigned int index, MStatus *ReturnStatus=NULL)		const;
    MObject attribute(const MString &attrName, MStatus *ReturnStatus=NULL)	const;
    bool hasAttribute(const MString &attrName, MStatus *ReturnStatus=NULL)	const;

    static const char *className();

protected:
    // No protected members
private:
    MTypeId fNodeTypeId;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MNodeClass */
