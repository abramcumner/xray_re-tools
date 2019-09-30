#ifndef _MFragmentManager
#define _MFragmentManager
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
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MString.h>
#include <maya/MObject.h>
#include <maya/MDagPath.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// DECLARATIONS

// ****************************************************************************
// CLASS DECLARATION (MFragmentManager)
//! \ingroup OpenMayaRender
//! \brief Provides facilities for managing fragments for use with Viewport 2.0
/*!
MFragmentManager provides functionality for dealing with Viewport 2.0 shade
fragments and fragment graphs. Both are defined using an XML format and can be
specified using either a file or a string buffer.

The XML format describes input and output parameters for the fragment as well
as default values for parameters. It also allows the specification of multiple
implementations of the fragment (e.g. a Cg implementation and an HLSL
implementation) so that fragments can be created that work in both the OpenGL
and DirectX versions of Viewport 2.0. The format can also be used to define
how several fragments should be connected together into a single fragment graph.

Please see the Viewport 2.0 section of the Maya API guide for a more detailed
description of the XML along with a complete XML Schema (XSD) for both fragments
and fragment graphs.

In addition to the description in the API guide MFragmentManager provides
several methods for examining how Maya works with fragments and graphs.

The XML for many of the fragments and fragment graphs used for Maya's internal
nodes are available for inspection. fragmentList() will return a list of the
names of all non-hidden fragments and fragment graphs. For any of these names,
getFragmentXML() may be called to retrieve the XML for that fragment.

While combining fragments, Maya often creates intermediate fragment graphs on
the fly. These are not stored in the manager so they may not be queried through
getFragmentXML(). To see the XML for these intermediate graphs, call
setIntermediateGraphOutputDirectory() with a directory and Maya will dump the
XML to files in that directory every time it creates such a graph.

When all fragments have been connected and joined together, Maya will create
a final effect. This effect is compiled and used to drive geometry requirements
and final shading. The text for this effect can be dumped to disk every time it
is created by calling setEffectOutputDirectory() with a valid directory. This
text can be used to see how final effects are created out of fragment graphs.

Note that for the intermediate graph and effect output directories, Maya will
use whatever value was set last. So multiple plugins that set these values may
conflict with each other. These methods are intended to be used as debug tools
only during development of a plugin. The usage of them incurs a performance
penalty and so it is strongly discouraged to use them in a production setting.
*/
class OPENMAYARENDER_EXPORT MFragmentManager
{
public:
	bool hasFragment(const MString& fragmentName) const;

	void fragmentList(MStringArray& fragmentNames) const;

	bool getFragmentXML(
		const MString& fragmentName,
		MString& buffer) const;
	bool getFragmentXML(
		const MObject& shadingNode,
		MString& buffer,
		bool includeUpstreamNodes=false,
		const MDagPath* objectContext=NULL) const;

	MString addShadeFragmentFromBuffer(
		const char* buffer,
		bool hidden);
	MString addShadeFragmentFromFile(
		const MString& fileName,
		bool hidden);

	MString addFragmentGraphFromBuffer(const char* buffer);
	MString addFragmentGraphFromFile(const MString& fileName);

	bool removeFragment(const MString& fragmentName);

	bool addFragmentPath(const MString& path);

	void setEffectOutputDirectory(const MString& dir);
	MString getEffectOutputDirectory() const;
	void setIntermediateGraphOutputDirectory(const MString& dir);
	MString getIntermediateGraphOutputDirectory() const;

	static const char* className();

private:
	MFragmentManager();
	~MFragmentManager();

	friend class MRenderer;
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFragmentManager */
