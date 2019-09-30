#ifndef _MPxShadingNodeOverride
#define _MPxShadingNodeOverride
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MObject.h>
#include <maya/MString.h>
#include <maya/MPlug.h>
#include <maya/MViewport2Renderer.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// CLASS DECLARATION (MAttributeParameterMapping)
//! \ingroup OpenMayaRender
//! \brief Class for defining relationship between Maya attributes and fragment
//!        parameters
/*!
	MAttributeParameterMapping allows implementations of MPxShadingNodeOverride
	to describe which attribute on a Maya node drives which parameter on the
	corresponding shader fragment or fragment graph. The plugin can provide a
	list of mappings in getCustomMappings(). Additional mappings will be
	automatically created by Maya after getCustomMappings() is called for any
	attributes that have a parameter on the fragment or fragment graph with the
	same name and type.
*/
class OPENMAYARENDER_EXPORT MAttributeParameterMapping
{
public:
	MAttributeParameterMapping(
		const MString& paramName,
		const MString& attrName,
		bool allowConnection,
		bool allowRename);
	~MAttributeParameterMapping();

	const MString& parameterName() const;
	const MString& resolvedParameterName() const;
	const MString& attributeName() const;
	bool allowConnection() const;
	bool allowRename() const;

	static const char* className();

private:
	MAttributeParameterMapping(void* data);
	MAttributeParameterMapping(const MAttributeParameterMapping& other);
	MAttributeParameterMapping& operator=(const MAttributeParameterMapping& other);

	friend class MAttributeParameterMappingList;

	void* fData;
	MString fParamName;
	MString fResolvedParamName;
	MString fAttrName;
	bool fAllowConnection;
	bool fAllowRename;
};

// ****************************************************************************
// CLASS DECLARATION (MAttributeParameterMappingList)
//! \ingroup OpenMayaRender
//! \brief A list of MAttributeParameterMapping objects
/*!
A list of MAttributeParameterMapping objects. Ownership of mapping objects
added to the list remains with the caller; the list makes a copy.
*/
class OPENMAYARENDER_EXPORT MAttributeParameterMappingList
{
public:
	MAttributeParameterMappingList();
	~MAttributeParameterMappingList();

	unsigned int length() const;
	const MAttributeParameterMapping* getMapping(unsigned int index) const;
	const MAttributeParameterMapping* findByAttributeName(
		const MString& attributeName) const;
	const MAttributeParameterMapping* findByParameterName(
		const MString& parameterName) const;

	void append(const MAttributeParameterMapping& mapping);
	void clear();

	static const char* className();

private:
	MAttributeParameterMappingList(const MAttributeParameterMappingList&) {}
	MAttributeParameterMappingList& operator=(const MAttributeParameterMappingList&) { return *this; }
	void append(void* data);


	void* fData;
};

// ****************************************************************************
// CLASS DECLARATION (MPxShadingNodeOverride)
//! \ingroup OpenMayaRender MPx
//! \brief Base class for user defined shading node overrides.
/*!
MPxShadingNodeOverride allows the user to specify how a plugin shading node in
Maya should interact with other shading nodes in Viewport 2.0. Specifically
this class lets the user inform Maya which shading fragment (or fragment graph)
to use to represent the node in the Viewport 2.0 shading graph. Using this
system, plugin nodes may function seamlessly with internal nodes as well as
other plugin nodes from unrelated plugins (both geometry and shaders).

MPxShadingNodeOverride differs from MPxShaderOverride in that implementations
of MPxShaderOverride are required to produce the whole shading effect for a
shader graph (including lighting) while MPxShadingNodeOverride is only required
to produce a small fragment for an individual node.

Shade fragments and graphs are managed by MFragmentManager. New fragments
and/or graphs may be defined using XML and registered with Maya through
MFragmentManager. These fragments and graphs may then be referenced by
implementations of MPxShadingNodeOverride. For the purposes of
MPxShadingNodeOverride, fragments and fragment graphs can be used
interchangeably. A fragment graph is merely a special case of a fragment. It
still has input and output parameters it is just composed of other fragments
instead of directly defining shading code itself. In the following descriptions
assume that any reference to a fragment can also refer to a fragment graph.

Implementations of MPxShadingNodeOverride have two main responsibilities. One
is to define the fragment to use and how the parameters of that fragment are
related to the attributes on the Maya node. The second (optional)
responsibility is to manually set the values of the parameters for the fragment
on the final shader when the associated Maya shading node changes.

When Maya needs to create a shading effect for a particular shading node graph,
it will create a shading fragment for each node and connect them together to
form the full effect. Any node in the node graph whose node type is associated
with an implementation of MPxShadingNodeOverride will use the shade fragment
named by the fragmentName() method. This will happen any time the Maya shading
node graph changes in such a way as to require a rebuild of the full effect (for
example, if connections are changed).

The parameters on the shading fragment are automatically driven by attributes
on the Maya node which have matching names and types. The plugin may also
specify associations between attributes and parameters of the same type but
with different names by overriding the getCustomMappings() function. This
method is called immediately after the fragment is created and before the
automatic mappings are done. No automatic mapping will be created for any
parameter on the fragment that already has a custom mapping.

All functionality is driven through these attribute parameter mappings. When
Maya is traversing the shading node graph building and connecting fragments it
will only traverse connections where the input attribute on the node has a
defined mapping (custom or automatic). Also, as fragments are combined for all
the nodes in the Maya shading graph, their parameters are renamed in order to
avoid collisions (allowing the same fragment type to be used multiple times in
a graph). Only parameters with mappings will be renamed, all others may suffer
name collisions which will produce unpredictable results.

In addition to informing Maya about the relationship between attributes and
parameters, custom mappings can be used to prevent Maya from trying to connect
other fragments to a particular parameter or to prevent a parameter from being
renamed (name collisions become the responsibility of the user). Custom
mappings may also be used to tell Maya to rename a parameter to avoid name
collisions but not to associate it with any attribute (when the attribute name
of the mapping is the empty string). The values for such a parameter must be
set manually in updateShader(). This can be useful when a parameter is not
directly driven by an attribute but must still be set with a computed value.

An implementation of MPxShadingNodeOverride which specifies a shade fragment
to use along with optional custom mappings will function well in the Viewport
2.0 shading system. The parameter values on the final effect will automatically
be set to the values of the attributes on the Maya node whenever the Maya node
changes. However, if additional control is required, the implementation can also
override the updateDG() and updateShader() methods. These two methods will be
called when Maya needs to update the parameter values on the final shader.

In updateDG() the implementation should query and cache any information it
needs from the Maya dependency graph. It is an error to attempt to access the DG
in updateShader() and doing so may result in instability.

In updateShader() the implementation is given the MShaderInstance of which the
fragment it specified is a part. It is also given the full list of attribute
parameter mappings known to Maya for the node (both automatic and custom).
Since most parameters are renamed from the original names on the fragment, the
implementation must use the "resolved" name from the mappings to set values on
the MShaderInstance. The implementation may set the value of any parameter on
the shader instance but any parameter with a mapping that defines an attribute
will be set automatically. Only parameters that need special computation to
set their values need to be handled. Although it is possible to set values on
the MShaderInstance for parameters from other fragments in the Maya shading
node graph, this behaviour is not recommended or supported. Such values may get
overwritten and behaviour will be unpredictable.

Implementations of MPxShadingNodeOverride must be registered with Maya through
MDrawRegistry.
*/
class OPENMAYARENDER_EXPORT MPxShadingNodeOverride
{
public:
	MPxShadingNodeOverride(const MObject& obj);
	virtual ~MPxShadingNodeOverride();

	virtual ::MHWRender::DrawAPI supportedDrawAPIs() const;
	virtual bool allowConnections() const;

	virtual MString fragmentName() const = 0;
	virtual void getCustomMappings(
		MAttributeParameterMappingList& mappings);
	virtual MString outputForConnection(
		const MPlug& sourcePlug,
		const MPlug& destinationPlug);

	virtual bool valueChangeRequiresFragmentRebuild(const MPlug* plug) const;

	virtual void updateDG();
	virtual void updateShader(
		MShaderInstance& shader,
		const MAttributeParameterMappingList& mappings);

	static const char* className();

private:
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxShadingNodeOverride */
