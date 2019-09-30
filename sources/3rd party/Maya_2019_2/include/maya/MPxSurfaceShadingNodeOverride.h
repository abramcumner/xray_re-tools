#ifndef _MPxSurfaceShadingNodeOverride
#define _MPxSurfaceShadingNodeOverride
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

#include <maya/MPxShadingNodeOverride.h>

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// CLASS DECLARATION (MPxSurfaceShadingNodeOverride)

//! \ingroup OpenMayaRender MPx
//! \brief Base class for user defined surface shading node overrides.
/*!
MPxSurfaceShadingNodeOverride is an extension of MPxShadingNodeOverride which
is specialized for surface shaders. Plugin surface shader nodes that may be
connected directly to a Maya shading engine should define an override which
derives from this class instead of MPxShadingNodeOverride when providing
support for Viewport 2.0.

In addition to providing all the functionality of MPxShadingNodeOverride,
MPxSurfaceShadingNodeOverride also lets the override flag certain parameters
and attributes for special processing.

Maya also treats MPxSurfaceShadingNodeOverride objects differently than
plain MPxShadingNodeOverride objects. In particular, Maya will attempt to
connect the shade fragments for lights to the fragment provided by an
implementation of MPxSurfaceShadingNodeOverride. The following named input
parameters on the fragment will be recognized and have the specified Maya
lighting parameters automatically connected to them.

<p><table>
	<tr>
		<td><b>Parameter name</b></td>
		<td><b>Parameter type</b></td>
		<td><b>Automatically connected value from lights</b></td>
	</tr>
	<tr>
		<td>Lw</td>
		<td>3-float</td>
		<td>World space light direction vector for diffuse lighting</td>
	</tr>
	<tr>
		<td>HLw and SLw</td>
		<td>3-float</td>
		<td>World space light direction vector for specular lighting</td>
	</tr>
	<tr>
		<td>diffuseI</td>
		<td>3-float</td>
		<td>Diffuse irradiance for light</td>
	</tr>
	<tr>
		<td>specularI</td>
		<td>3-float</td>
		<td>Specular irradiance for light</td>
	</tr>
	<tr>
		<td>ambientIn</td>
		<td>3-float</td>
		<td>Total contribution from all ambient lights</td>
	</tr>
</table></p>

When there are multiple lights in the scene, the contribution from the
fragments that make up the final shading effect is computed once for each
light and then the results are accumulated. So the parameters above are all
for the whichever light is currently being computed.

Please examine the fragments and fragment graphs for the Maya phong shader
(mayaPhongSurface) for examples of how light information can be used.

Implementations of MPxSurfaceShadingNodeOverride must be registered with Maya
through MDrawRegistry.
*/
class OPENMAYARENDER_EXPORT MPxSurfaceShadingNodeOverride : public MPxShadingNodeOverride
{
public:
	MPxSurfaceShadingNodeOverride(const MObject& obj);
	virtual ~MPxSurfaceShadingNodeOverride();

	virtual MString primaryColorParameter() const;
	virtual MString transparencyParameter() const;
	virtual MString bumpAttribute() const;

	static const char* className();

private:
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxSurfaceShadingNodeOverride */
