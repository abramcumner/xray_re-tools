#ifndef _MPxPolyTrg
#define _MPxPolyTrg
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
// CLASS:    MPxPolyTrg
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MPxNode.h>
#include <maya/MStatus.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxPolyTrg)

//! \ingroup OpenMaya MPx
//! \brief User defined poly triangulation support.
/*!
<b>MPxPolyTrg</b> is the the parent class for nodes which define
a custom  face triangulation for meshes. In order to override
default maya triangulation, the user has to do the following
things:

<ul>
<li> Derive a new node from <b>MPxPolyTrg</b> class.

<li> Define a static function on the new node to
triangulate a single face in the mesh. The signature
of this function is given by <b>polyTrgFnct</b> type.
The input to the triangulation function is:
    <ul>
	<li> *vert  - pointer to an array of vertex positions
                   for the face. It size is 3*nbVertices.
	<li> *norm  - pointer to an array of per vertex normals
                   for the face. It size is 3*nbVertices.
	<li> *loopSizes - pointer to an array of loop sizes.
                       The size of this array is <b>nbLoops</b>
	<li> nbLoops - number of loops in the face.
	<li> nbTrg   - expected number of triangles to be
                    generated.
	</ul>

    Note: The number of vertices can be calculated by adding
    all the loop sizes.

	The output of this function is an array of triangle
    description:

	<ul>
	<li> *trg    - pointer to an integer array. This array is
                    already allocated with the size of 3*nbTrg.
                    It has to be filled with the triangle
                    descriptions given by the face-relative position
                    of vertices i.e.

                    nbTrg = 2
                    trg: 0 1 2 2 3 0


    Tip: Refer to the example plugin: <b>polyTrgNode</b> for
    more details on how to implement it.
	</ul>

<li> Register that function when the node is created
for the first time (in the postConstructor() method)
by calling the <b>registerTrgFunction()</b>.

<li> Unregister the function in the destructor by calling
the <b>unregisterTrgFunction</b>.
</ul>

Once the node is defined, the user has to inform the mesh
about it. For each mesh the user wants to override the
default triangulation, he has to set the <b>usertTrg</b>
attribute on the mesh to the name under which the function
has been registered.

Example:
/code
        setAttr mesh.userTrg -type "string" "triangulate";
/endcode

Once that attribute is set, the default maya triangulation
is turned off and the one provided by the user is used to
draw the mesh.
*/
class OPENMAYA_EXPORT MPxPolyTrg : public MPxNode
{
public:

	MPxPolyTrg();
	virtual ~MPxPolyTrg();

	virtual void		postConstructor();
	virtual MStatus		compute( const MPlug& plug,
								 MDataBlock& dataBlock );
	virtual bool		isAbstractClass() const;


	// Type for the signature of the triangulate function
	//! \brief Pointer to a function which triangulates a poly face.
	/*
	\param[in] vert Array of the face's vertex positions.
	\param[in] norm Array of the face's vertex normals.
	\param[in] loopSizes Array of loop sizes.
	\param[in] nbLoop Number of loops in the face.
	\param[in] nbTrg Expected number of triangles to be generated.
	\param[out] trg Array of face-relative vertex indices describing the generated triangles.
	*/
	typedef void (*polyTrgFnct)( const float *vert,
								 const float *norm,
								 const int   *loopSizes,
								 const int nbLoop,
								 const int nbTrg, unsigned short *trg );

	MStatus				registerTrgFunction( char *name, polyTrgFnct f );
	MStatus				unregisterTrgFunction( char *name );

private:

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxPolyTrg */
