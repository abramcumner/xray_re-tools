#ifndef _MPxHardwareShader
#define _MPxHardwareShader
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
// CLASS:    MPxHardwareShader
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MDrawRequest.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MDeprecate.h>

// ****************************************************************************
// DECLARATIONS

//! \brief Coordinates of the upper-left and lower-right corners of a rectangular region.
typedef const float floatRegion[2][2];


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxHardwareShader)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for user defined hardware shaders 
/*!
 MPxHardwareShader allows the creation of user-defined hardware shaders.
 A hardware shader allows the plug-in writer to control the on-screen
 display of an object in Maya.

 You must derive a hardware shader node from MPxHardwareShader for it to
 have any affect on the on-screen display at all.  In addition to
 their affect on the on-screen display of objects, hardware shaders
 function as surface shader nodes.  This allows you to connect any
 shading network up to the hardware shader's outColor attribute to
 handle the shading during a software render.

 To create a working hardware shader, derive from this class and
 override the render() and optionally the populateRequirements() methods. The
 other methods of the parent class MPxNode may also be overridden to
 perform dependency node capabilities.

 NOTE: Plug-in hardware shaders are fully supported for polygonal
 mesh shapes.  NURBS surfaces are only supported in the High Quality
 Interactive viewport and Hardware Renderer.
*/
class OPENMAYAUI_EXPORT MPxHardwareShader : public MPxNode
{
public:
	//! Transparency option bitmasks.
	enum TransparencyOptions
	{
		//! When set means draw transparent
		kIsTransparent = 0x0001,

		//! When set means ignore front back cull
		kNoTransparencyFrontBackCull = 0x0002,

		//! When set means ignore polygon sorting
		kNoTransparencyPolygonSort = 0x0004
	};

BEGIN_NO_SCRIPT_SUPPORT:
	//! Provides contextual information about the current invocation
	//! of the shader.
	struct ShaderContext
	{
		//! DAG path for the given invocation of the shader
		MDagPath path;

		//! Shading engine node for the given invocation of the shader
		MObject shadingEngine;
	};
END_NO_SCRIPT_SUPPORT:

	MPxHardwareShader();

	virtual ~MPxHardwareShader();

	virtual MPxNode::Type type() const;

	// Set the varying parameters used for this shader
	MStatus setVaryingParameters( const MVaryingParameterList& parameters, bool remapCurrentValues = true, MDagModifier* dagModifier = NULL);

	// Set the uniform parameters used for this shader
	MStatus setUniformParameters( const MUniformParameterList& parameters, bool remapCurrentValues = true, MDagModifier* dagModifier = NULL);

	// Methods to overload

	// Override this method to render geometry
	virtual MStatus render( MGeometryList& iterator);

	// Specifies transparency parameters for the shader.
	// Returning "true" from hasTransparency() superscedes these options.
	//
	virtual unsigned int	transparencyOptions();

	// Query the renderers supported by this shader
	//
	virtual const MRenderProfile& profile();

BEGIN_NO_SCRIPT_SUPPORT:

	// Override this method to specify this shader's geometry requirements
	//
	//! \noscript
	virtual MStatus populateRequirements( const MPxHardwareShader::ShaderContext& context, MGeometryRequirementsLegacy& requirements);

	// Override this method to specify the list of images
	// that are associated with the given uv set in this shader.
	// This method is used to determine which texture images
	// are available in the UV editor.
	//
	//! \noscript
	virtual MStatus	getAvailableImages( const MPxHardwareShader::ShaderContext& context,
										const MString& uvSetName,
								        MStringArray& imageNames);

	// Override this method to draw an image of this material.
	// This method allows a shader to override the rendering of
	// the background image used in the UV editor.
	// Maya will only use this method if getAvailableImages
	// returns at least one image name. The imageWidth and
	// imageHeight parameters should be populated with the
	// native resolution of the input image to allow pixel
	// snapping or other resolution dependent operations.
	//
	//! \noscript
    OPENMAYA_DEPRECATED(2019, "Use renderImage() that provides RenderParameters instead.")
	virtual MStatus	renderImage( const MPxHardwareShader::ShaderContext& context,
								 const MString& imageName,
								 floatRegion region,
								 int& imageWidth,
								 int& imageHeight);

	//! Provides information on how to render the image.
	struct RenderParameters
	{
		MColor baseColor;
		bool unfiltered;
		bool showAlphaMask;
	};

	// Override this method to draw an image of this material.
	// This method allows a shader to override the rendering of
	// the background image used in the UV editor.
	// Maya will only use this method if getAvailableImages
	// returns at least one image name. The imageWidth and
	// imageHeight parameters should be populated with the
	// native resolution of the input image to allow pixel
	// snapping or other resolution dependent operations.
	//
	//! \noscript
	virtual MStatus	renderImage( const MPxHardwareShader::ShaderContext& context,
								 const MString& imageName,
								 floatRegion region,
								 const MPxHardwareShader::RenderParameters& parameters,
								 int& imageWidth,
								 int& imageHeight);

	// Override this method to draw an image of this material.
	// This method allows a shader to override the rendering of
	// the background image used in the UV editor in viewport 2.0.
	// Maya will only use this method if getAvailableImages
	// returns at least one image name. The imageWidth and
	// imageHeight parameters should be populated with the
	// native resolution of the input image to allow pixel
	// snapping or other resolution dependent operations.
	//
	//! \noscript
	virtual MStatus	renderImage( const MPxHardwareShader::ShaderContext& context,
								 MHWRender::MUIDrawManager& uiDrawManager,
								 const MString& imageName,
								 floatRegion region,
								 const MPxHardwareShader::RenderParameters& parameters,
								 int& imageWidth,
								 int& imageHeight);

END_NO_SCRIPT_SUPPORT:

	// Override this method to draw a image for swatch rendering.
	virtual MStatus renderSwatchImage( MImage & image );

	static  MPxHardwareShader* getHardwareShaderPtr( MObject& object );

	static MString findResource( const MString& name, const MString& shaderPath, MStatus* status = NULL);

	// Attributes inherited from surfaceShader
	//! output color value
	static MObject outColor;
	//! output color red
	static MObject outColorR;
	//! output color green
	static MObject outColorG;
	//! output color blue
	static MObject outColorB;

	static const char*	    className();

private:
	static void				initialSetup();
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxNode */
