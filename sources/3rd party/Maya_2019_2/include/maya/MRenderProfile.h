#ifndef _MRenderProfile
#define _MRenderProfile
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
// CLASS:    MRenderProfile
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MString.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRenderProfile)

//! \ingroup OpenMayaRender
//! \brief Render profile.
/*!
The MRenderProfile class describes the rendering APIs and algorithms supported
by a given rendering entity (e.g. a shading node, a renderer). A single profile
can contain multiple entries allowing, for example, a shading node to specify
that it supports both OpenGL and Direct3D rendering. The profile entries refer
to renderers rather than rendering APIs as the rendering elements may depend on
specific services, information or algorithms implemented by the renderer (e.g.
a global light table, or render state cache).
*/
class OPENMAYARENDER_EXPORT MRenderProfile
{
public:

	MRenderProfile();
	~MRenderProfile();

	//! Maya's internal renderers.
	typedef enum
	{
		kMayaSoftware,	//!< \nop
		kMayaOpenGL,	//!< \nop
		kMayaD3D	//!< \nop
	} MStandardRenderer;

	unsigned int	numberOfRenderers() const;

	void			addRenderer( MStandardRenderer renderer);

	void			addRenderer( const MString& name, float version);

	bool			hasRenderer( MStandardRenderer renderer ) const;

	bool			hasRenderer( const MString &name, float version ) const;

protected:

	void*			_ptr;

private:
// No private members

};
OPENMAYA_NAMESPACE_CLOSE

#endif
#endif // _MRenderProfile
