#ifndef _MFnAssembly
#define _MFnAssembly
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
// CLASS:    MFnAssembly
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// FORWARD DECLARATIONS


// ****************************************************************************
// CLASS DECLARATION (MFnAssembly)

//! \ingroup OpenMaya MFn
//! \brief Function set for scene assembly.
/*!
An assembly is a node that is used to maintain a
list of representations, and to keep one as the active
representation.  Correspondingly, representations can be activated and
inactivated.  In general, a representation can be a full hierarchy of
animated geometry, but can also be as simple as a locator or a
bounding box.  The assembly can be used to activate the
representation that best corresponds to a given task: a simple,
fast-loading, fast-drawing representation for overall planning, or a
more detailed representations for fine tuning.  

The assembly API does not specify the implementation of
representations.  For example, representations can be actual DAG
nodes, or data structures internal to the assembly that
can be used to generate DAG nodes in the Maya scene graph on
representation activation.  In this function set as 
well as in MPxAssembly, representations are identified only by a string
identifier.  Representations in a single MPxAssembly must have unique
string names.  Representation name strings must not contain spaces and
quotes, as these characters would cause argument parsing to fail when
names are used in commands.

Representations are created from a set of representation types, which each
assembly type must define.  A representation of a given
type will share properties (such as viewport appearance, or memory
size) with other representations of the same type.  Examples could
include a "BoundingBox" representation type, or a "BakedGeometry"
representation type.  As with representation names, representation type
name strings must not contain spaces and quotes.  Representation type
names are not used in the UI, and are not localized.  Rather, each
representation type has a corresponding type label that is used for
this purpose.  The representation type label can contain spaces and
quotes, and should be localized.

An assembly object is also a DAG container.  As such, operations on
containers can be done on assemblies.  For use of container
functionality on assemblies, see function set MFnContainerNode.

<h2>Scene Assembly Services</h2>

Scene assembly nodes are DAG container nodes that provide the
following services:
<dl>
<dt>Fidelity / performance tradeoff</dt>
<dd>Assembly nodes have a single active representation, chosen from a
list of available representations, to best suit the task at hand, and
/ or the size of the data set.</dd>

<dt>Scene segmentation</dt>
<dd>Assembly nodes represent a part of a scene that can be loaded and 
unloaded efficiently as a block, and that can be worked upon separately
and cooperatively.</dd>

<dt>Non-destructive edits</dt>
<dd>Assembly nodes can track certain edits done on their
members. These edits are non-destructive and recorded separately from
the representation data onto which they are applied.</dd>

<dt>Plugin extensibility</dt>
<dd>Scene assembly is fully exposed through the OpenMaya API, and
its Autodesk reference implementation is done through this API, to
ensure parity of capability between third-party scene assembly node
implementations, and Autodesk implementations.</dd>

<dt>DAG containment</dt>
<dd>Scene assembly nodes inherit the same capabilities as their DAG
container class parents, namely an embodiment in the 3D scene hierarchy as
transforms, as well as encapsulating containment of their member nodes,
which are created by activating a representation.</dd>
</dl>
*/

class OPENMAYA_EXPORT MFnAssembly : public MFnDagNode
{
    declareDagMFn(MFnAssembly, MFnDagNode);

public:

    static MObjectArray getTopLevelAssemblies();

    MString createRepresentation(
       const MString& input,
       const MString& type,
       MDagModifier*  undoRedo = NULL,
       MStatus*       status = NULL
    );
    MString createRepresentation(
       const MString& input,
       const MString& type,
       const MString& representation,
       MDagModifier*  undoRedo = NULL,
       MStatus*       status = NULL
    );
   
    MStatus  postLoad();

    MStatus activate(const MString& representation);
    MString getActive(MStatus* status = NULL) const;
    MStatus activateNonRecursive(const MString& representation);
    bool    canActivate(MStatus* status = NULL) const;
 
	bool    isActive(const MString& representation, MStatus* status = NULL) const;
 
	// Deprecated
    OPENMAYA_DEPRECATED(2019, "Use the method MFnAssembly::getInitialRep(bool&, MStatus*) instead")
    MString getInitialRep(MStatus* status=NULL) const;

    MString getInitialRep(bool& hasInitialRep, MStatus* status=NULL) const;
  
    MStringArray getRepresentations(MStatus* status = NULL) const;
    MString getRepType(
       const MString& representation, MStatus* status = NULL) const;
    MString getRepLabel(
       const MString& representation, MStatus* status = NULL) const;
   
    MStringArray repTypes(MStatus* status = NULL) const;
   
    MStatus deleteRepresentation(const MString& representation);
    MStatus deleteAllRepresentations();
   
    MString getRepNamespace(MStatus* status = NULL) const;
    MString setRepName(const MString& representation, const MString& newName, MStatus* status = NULL);
    MStatus setRepLabel(const MString& representation, const MString& label);

    MStatus importFile(
       const MString& fileName,
       const char*    type = NULL,
       bool           preserveReferences = false,
       const char*    nameSpace = NULL,
       bool           ignoreVersion = false
    );

   MString getAbsoluteRepNamespace(MStatus* status = NULL) const;

   bool isTopLevel(MStatus* status = NULL) const;
   bool supportsEdits(MStatus* status = NULL) const;
   bool supportsMemberChanges(MStatus* status = NULL) const;

   bool canRepApplyEdits(
   const MString& representation, MStatus* status = NULL) const;

   bool handlesAddEdits(MStatus* status = NULL) const;

   MObject getParentAssembly(MStatus* status = NULL) const;
   MObjectArray	getSubAssemblies(MStatus* status = NULL) const;

BEGIN_NO_SCRIPT_SUPPORT:
    declareDagMFnConstConstructor(MFnAssembly, MFnDagNode);
END_NO_SCRIPT_SUPPORT:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnAssembly */
