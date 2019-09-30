#ifndef _MPxHwShaderNode
#define _MPxHwShaderNode
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
// CLASS:    MPxHwShaderNode
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

// ****************************************************************************
// DECLARATIONS

//! \brief Pointer to an array of floats.
typedef const float** floatArrayPtr;

//! \brief Coordinates of the upper-left and lower-right corners of a rectangular region.
typedef const float floatRegion[2][2];


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxHwShaderNode)

//! \ingroup OpenMayaUI MPx
//! \brief Base class for user defined hardware shaders 
/*!
 MPxHwShaderNode allows the creation of user-defined hardware shaders.
 A hardware shader allows the plug-in writer to control the on-screen
 display of an object in Maya.

 You must derive a hardware shader node from MPxHwShaderNode for it to
 have any affect on the on-screen display at all.  In addition to
 their affect on the on-screen display of objects, hardware shaders
 function as surface shader nodes.  This allows you to connect any
 shading network up to the hardware shader's outColor attribute to
 handle the shading during a software render.

 To create a working hardware shader, derive from this class and
 override the bind(), unbind(), and geometry() methods.  If your
 hardware shader uses texture coordinates from Maya, you also need to
 override either texCoordsPerVertex or getTexCoordSetNames().  The
 other methods of the parent class MPxNode may also be overridden to
 perform dependency node capabilities.

 NOTE: Plug-in hardware shaders are fully supported for polygonal
 mesh shapes.  NURBS surfaces are only supported in the High Quality
 Interactive viewport and Hardware Renderer if the
 glBind/glGeometry/glUnbind methods of this class are implemented.
*/
class OPENMAYAUI_EXPORT MPxHwShaderNode : public MPxNode
{
public:
	//! Bit masks used in conjuction with the 'writeable' parameter
	//! passed to the geometry() method to determine which arrays
	//! the shader is allowed to write to.
	enum Writeable {
		kWriteNone				= 0x0000,	//!< \nop
		kWriteVertexArray		= 0x0001,	//!< \nop
		kWriteNormalArray		= 0x0002,	//!< \nop
		kWriteColorArrays		= 0x0004,	//!< \nop
		kWriteTexCoordArrays	= 0x0008,	//!< \nop
		kWriteAll				= 0x000f	//!< \nop
	};

	//! Bit masks used in combination with the return value of the
	//! dirtyMask() method to determine which portions of the geometry
	//! are dirty.
	enum DirtyMask {
		kDirtyNone				= 0x0000,	//!< \nop
		kDirtyVertexArray		= 0x0001,	//!< \nop
		kDirtyNormalArray		= 0x0002,	//!< \nop
		kDirtyColorArrays		= 0x0004,	//!< \nop
		kDirtyTexCoordArrays	= 0x0008,	//!< \nop
		kDirtyAll				= 0x000f	//!< \nop
	};

	//! Bit masks to be returned by the shader's transparencyOptions() method.
	enum TransparencyOptions
	{
		//! Draw as a transparent object. If this bit is not set then
		//! the others are ignored.
		kIsTransparent = 0x0001,

		//! Do not use the two-pass front-and-back culling algorithm.
		kNoTransparencyFrontBackCull = 0x0002,

		//! Do not use two-pass drawing of back-to-front sorted polygons.
		kNoTransparencyPolygonSort = 0x0004
	};


	MPxHwShaderNode();

	virtual ~MPxHwShaderNode();

	virtual MPxNode::Type type() const;

	// Methods to overload


	// Override this method to set up the OpenGL state
	//
	virtual MStatus		bind( const MDrawRequest& request,
							  M3dView& view );

	// Override this method to return OpenGL to a sane state
	//
	virtual MStatus		unbind( const MDrawRequest& request,
								M3dView& view );

	// Override this method to actually draw primitives on the display
	//
	virtual MStatus		geometry( const MDrawRequest& request,
								  M3dView& view,
								  int prim,
								  unsigned int writable,
								  int indexCount,
								  const unsigned int * indexArray,
								  int vertexCount,
								  const int * vertexIDs,
								  const float * vertexArray,
								  int normalCount,
								  floatArrayPtr normalArrays,
								  int colorCount,
								  floatArrayPtr colorArrays,
								  int texCoordCount,
								  floatArrayPtr texCoordArrays);
	virtual MStatus		geometry( const MDrawRequest& request,
								  M3dView& view,
								  int prim,
								  unsigned int writable,
								  int indexCount,
								  const unsigned int * indexArray,
								  int vertexCount,
								  const int * vertexIDs,
								  const float * vertexArray,
								  int normalCount,
								  floatArrayPtr normalArrays,
								  int colorCount,
								  floatArrayPtr colorArrays,
								  int texCoordCount,
								  floatArrayPtr texCoordArrays,
								  const int * faceIDs,
								  const float * localUVCoord);

	// These methods should only be overridden if support
	// for the hardware renderer is required.
    // By default the interface above will invoke the methods below.
	//
	virtual MStatus     glBind  ( const MDagPath& shapePath );
	virtual MStatus	    glUnbind( const MDagPath& shapePath );
	virtual MStatus	    glGeometry( const MDagPath& shapePath,
                                  int glPrim,
								  unsigned int writeMask,
								  int indexCount,
								  const unsigned int* indexArray,
								  int vertexCount,
								  const int * vertexIDs,
								  const float * vertexArray,
								  int normalCount,
								  floatArrayPtr normalArrays,
								  int colorCount,
								  floatArrayPtr colorArrays,
								  int texCoordCount,
								  floatArrayPtr texCoordArrays);
	virtual MStatus	    glGeometry( const MDagPath& shapePath,
                                  int glPrim,
								  unsigned int writeMask,
								  int indexCount,
								  const unsigned int* indexArray,
								  int vertexCount,
								  const int * vertexIDs,
								  const float * vertexArray,
								  int normalCount,
								  floatArrayPtr normalArrays,
								  int colorCount,
								  floatArrayPtr colorArrays,
								  int texCoordCount,
								  floatArrayPtr texCoordArrays,
								  const int * faceIDs,
								  const float * localUVCoord);

	// Method to override to let Maya know this shader is batchable.
	//
	virtual bool	supportsBatching() const;

	// Method to override to tell Maya to invert texture coordinates
	//
	virtual bool	invertTexCoords() const;

	// Method which returns the path for the current object
	// being drawn using the shader.
	//
	const MDagPath & currentPath() const;

	// Method which returns which particular geometry items
	// have changed for the current object being drawn,
	// from the last time that geometry was
	// requested to draw (either via geometry() or glGeometry()).
	//
	// It is only guaranteed to contain valid information
	// within either of geometry calls.
	//
	unsigned int	dirtyMask() const;

	// Override this method to specify how many "normals" per vertex
	// the hardware shader would like.  Maya can provide from 0 to 3
	// normals per vertex.  The second and third "normal" will be
	// tangents.  If you do not override this method, Maya will
	// provide 1 normal per vertex.
	//
	virtual	int		normalsPerVertex();

	// Override this method to specify how many colors per vertex the
	// hardware shader would like Maya to provide.  Maya may not provide
	// this many if they are not available.  If you do not override
	// this method, Maya will provide 0 colors per vertex.
	//
	virtual int		colorsPerVertex();

BEGIN_NO_SCRIPT_SUPPORT:
	// Override this method to specify an array of names of color per
	// vertex sets that should be provided.  When Maya calls the
	// geometry method, the color values from the nth name in the list
	// will be passed in the nth colorArray.
	//
	//!	NO SCRIPT SUPPORT
	virtual int		getColorSetNames(MStringArray& names);
END_NO_SCRIPT_SUPPORT:

	// Override this method to specify how many texture coordinates
	// per vertex the hardware shader would like Maya to provide.
	// Maya may not provide this many if they are not available.  If
	// you do not override this method, Maya will provide 0 texture
	// coordinates per vertex.
	//
	virtual int		texCoordsPerVertex();

BEGIN_NO_SCRIPT_SUPPORT:
	// Override this method to specify an array of names of uvSets
	// that should be provided.  When Maya calls the geometry method,
	// the uv values from the nth name in the list will be passed in
	// the nth texCoordArray.
	//
	//!	NO SCRIPT SUPPORT
	virtual int		getTexCoordSetNames(MStringArray& names);
END_NO_SCRIPT_SUPPORT:

	// Specifies whether or not the hw shader uses transparency.  If
	// so, the objects that use this shader must be drawn after all
	// the opaque objects.
	//
	virtual bool	hasTransparency();

	// Specifies whether or not the hw shader wants a map of the
	// vertex IDs in the vertexArray provided to the geomery method.
	//
	virtual bool	provideVertexIDs();

	// Specifies whether or not the hw shader wants a map of the
	// vertex IDs in the vertexArray provided to the geomery method.
	//
	virtual bool	provideFaceIDs();

	// Specifies whether or not the hw shader wants the local uv coordinates 
	// of the subdivided face vertices.
	virtual bool	provideLocalUVCoord();

	// Specifies transparency parameters for the shader.
	// Returning "true" from hasTransparency() superscedes these options.
	//
	virtual unsigned int	transparencyOptions();

BEGIN_NO_SCRIPT_SUPPORT:
	// Override this method to specify the list of images
	// that are associated with the given uv set in this shader.
	// This method is used to determine which texture images
	// are available in the UV editor.
	//
	//!	NO SCRIPT SUPPORT
	virtual MStatus	getAvailableImages( const MString& uvSetName,
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
	//!	NO SCRIPT SUPPORT
	virtual MStatus	renderImage( const MString& imageName,
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
	//!	NO SCRIPT SUPPORT
	virtual MStatus	renderImage( const MString& imageName,
								 floatRegion region,
								 const MPxHwShaderNode::RenderParameters& parameters,
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
	//!	NO SCRIPT SUPPORT
	virtual MStatus	renderImage( MHWRender::MUIDrawManager& uiDrawManager,
								 const MString& imageName,
								 floatRegion region,
								 const MPxHwShaderNode::RenderParameters& parameters,
								 int& imageWidth,
								 int& imageHeight);

END_NO_SCRIPT_SUPPORT:

	// Override this method to draw a image for swatch rendering.
	virtual MStatus renderSwatchImage( MImage & image );

	static  MPxHwShaderNode* getHwShaderNodePtr( MObject& object );

	MObject	currentShadingEngine() const;

	// Attributes inherited from surfaceShader
	//! output color value
	static MObject outColor;
	//! output color red
	static MObject outColorR;
	//! output color green
	static MObject outColorG;
	//! output color blue
	static MObject outColorB;

	//! output transparency value
	static MObject outTransparency;
	//! output transparency red
	static MObject outTransparencyR;
	//! output transparency green
	static MObject outTransparencyG;
	//! output transparency blue
	static MObject outTransparencyB;

	//! output matte opacity value
	static MObject outMatteOpacity;
	//! output matte opacity red
	static MObject outMatteOpacityR;
	//! output matte opacity green
	static MObject outMatteOpacityG;
	//! output matte opacity blue
	static MObject outMatteOpacityB;

	//! output glow color value
	static MObject outGlowColor;
	//! output glow color red
	static MObject outGlowColorR;
	//! output glow color green
	static MObject outGlowColorG;
	//! output glow color blue
	static MObject outGlowColorB;

	static const char*	    className();

protected:
	// Current path.
	void				setCurrentPath( const void *pathPtr );
	MDagPath			fCurrentPath;

	// Dirty flags for geometry for current path.
	void				setDirtyMask( unsigned int mask );
	unsigned int		fDirtyMask;

private:
	static void				initialSetup();
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxNode */
