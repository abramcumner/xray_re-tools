#ifndef _MLightLinks
#define _MLightLinks
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
// CLASS:    MLightLinks
//
// ***************************************************************************

#if defined __cplusplus

// ***************************************************************************
// INCLUDED HEADER FILES

#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>
#include <maya/MIOStream.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ***************************************************************************
// CLASS DECLARATION (MLightLinks)

//! \ingroup OpenMayaRender
//! \brief Class providing read-only Light Linking API functions
/*!
This class provides read-only access to the light linking information
expressed in Maya's light linker nodes.

<b>Usage</b>

To extract Maya's light linking information, use the following sequence of
calls:

  <ol>
	<li> Call parseLinks() once.  MLightLinks will examine the
				   connections to the scene's light linker nodes, and compile
				   the information into a table of light-object links.
				   parseLinks() needs to be recalled whenever the scene's
				   light linking changes (objects/lights are added or deleted,
				   or light links are made or broken).
	<li> Call getLinkedLights() or getLinkedObjects() to query the
				   lights linked to a particular object, or the objects (and
				   components) linked to a particular light.
  </ol>

Lights can be linked either to DAG hierarchies or to shading
groups. Additionnally, light linking information can store a list of
illuminating lights and a list excluded lights. The algorithm used to
determine which light should contribute can be described by the pseudo
code below.



	\li If the entire object is all in one shading group, then only
	consider object-level light links. The code to compute lightList
	will look like:

	\code
    MObject nullObj;                                     // A void component object
    getLinkedLights(objectPath, nullObj, lightList);
	\endcode

	\li If different parts of the object belong to different shading
	groups, then the list of lights is the union of object and shading
	group linked lights, where the ignored lights have been
	removed. The code to compute lightList will look like:


	for each face group on the object, corresponding to each shading
	engine, do the following:

	\code
    MObject nullObj;                                     // A void component object
    getLinkedLights(objectPath, nullObj, ObjectLink);    // Lights linked to the whole object
    getIgnoredLights(objectPath, nullObj, ObjectIgnore); // Lights ignored on the whole object
    getLinkedLights(objectPath, component, FaceLink);    // Lights linked to the face component
    getIgnoredLights(objectPath, component, FaceIgnore); // Lights ignored for the face component
	\endcode

    For the given face group, the set of lights illuminating it are
    then (ObjectLink - FaceIgnore) + (FaceLink - ObjectIgnore).

    It is important to note that component linking works at the
    granularity of a shading group on a surface. In order to get
    results from the MLightLinks API, it is necessary to pass in
    components that correspond exactly to all the faces on that object
    in a particular shading group. For example, if
    pPlaneShape1.f[0:10] belongs to a shading group which is linked to
    a given light, then there will be no result from
    MLightLinks::getLinkedLights() if the argument is set to
    pPlaneShape1.f[0], or pPlaneShape1.f[0:5], or pPlaneShape1.f[10],
    etc - it must be exactly pPlaneShape1.f[0:10].
*/
class OPENMAYARENDER_EXPORT MLightLinks
{
public:

	MLightLinks();

	~MLightLinks();

	MStatus parseLinks( const MObject& linkNode = MObject::kNullObj,
						bool verbose = false,
						std::ostream *stream = NULL,
						bool useIgnore = false,
						bool componentSupport = true );

	MStatus getLinkedLights( const MDagPath& path,
							 const MObject& component,
							 MDagPathArray& lights );

	MStatus getLinkedObjects( const MDagPath& light,
							  MSelectionList& objects );

	MStatus getIgnoredLights( const MDagPath& path,
							  const MObject& component,
							  MDagPathArray& lights );

	MStatus getIgnoredObjects( const MDagPath& light,
							   MSelectionList& objects );

	MStatus getShadowLinkedLights( const MDagPath& path,
							 const MObject& component,
							 MDagPathArray& lights );

	MStatus getShadowLinkedObjects( const MDagPath& light,
							  MSelectionList& objects );

	MStatus getShadowIgnoredLights( const MDagPath& path,
							  const MObject& component,
							  MDagPathArray& lights );

	MStatus getShadowIgnoredObjects( const MDagPath& light,
							   MSelectionList& objects );

private:
	void *f_linkUtil;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MLightLinks */
