#ifndef _M3dView
#define _M3dView
//-
// ===========================================================================
// Copyright 2017 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    M3dView
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MColor.h>
#include <maya/MNativeWindowHdl.h>
#include <maya/MDeprecate.h>

#include <maya/MGL.h>
// ****************************************************************************
// DECLARATIONS

class QWidget;


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (M3dView)

//! \ingroup OpenMayaUI
//! \brief A 3-D view
/*!
  M3dView provides methods for working with 3D model views.  3D views
  are based on OpenGL drawing areas.

  Maya can operate in two different color modes, RGBA and color index.
  Color index mode is used to increase performance when shading is not
  required.  Drawing in color index mode is more complicated, but this
  class provides methods to simplify color selection.

  Maya has four color tables that can be used in RGBA, and that must
  be used in color index mode.  These four color tables represent four
  sets of bit planes that are independent of each other.  So, for
  example, it is possible to clear all active objects from the display
  and redraw them without redrawing the dormant and templated objects.
  The active and dormant color tables contain the same colors, but use
  different bitplanes.

  The extra performance of color index mode comes at the cost of a
  limited number of colors.  If this restriction causes difficulty,
  then it is possible for the user to force all displays into RGBA
  mode where any color may be used.

  When an object is affected by another in the scene, it is drawn in a
  magenta colour by default.  This is denoted in the DisplayStatus
  enum by kActiveAffected.  These objects are drawn in the active
  planes even though they are dormant for performance reasons.
*/
class OPENMAYAUI_EXPORT M3dView {

public:

	//! Display styles for a 3D view.
    enum DisplayStyle {
        kBoundingBox,			//!< Bounding box display.
        kFlatShaded,			//!< Flat shaded display.
        kGouraudShaded,			//!< Gouraud shaded display.
        kWireFrame,				//!< Wire frame display.
        kPoints					//!< Points only display.
    };

	//! Drawing modes for individual objects
    enum DisplayStatus {
        kActive,				//!< Object is active (selected).
        kLive,					//!< Object is live (construction surface).
        kDormant,				//!< Object is dormant (not selected, no other drawing mode enabled).
        kInvisible,				//!< Object is invisible (not drawn).
        kHilite,				//!< Object is hilited (has selectable components).
        kTemplate,				//!< Object is templated (Not renderable).
        kActiveTemplate,		//!< Object is active and templated.
        kActiveComponent,		//!< Object has active components.
		kLead,					//!< Last selected object.
		kIntermediateObject,	//!< Construction object (not drawn).
		kActiveAffected,		//!< Affected by active object(s).
        kNoStatus				//!< Object does not have a valid display status.
    };

	//! Reference to color palettes.
    enum ColorTable {
        kActiveColors = kActive,			//!< Colors for active objects
        kDormantColors = kDormant,			//!< Colors for dormant objects
        kTemplateColor = kTemplate,			//!< Colors for templated objects
        kBackgroundColor					//!< Colors for background color
    };

	//! Alignment values when drawing text.
    enum TextPosition {
        kLeft,					//!< Draw text to the left of the point
        kCenter,				//!< Draw text centered around the point
        kRight					//!< Draw text to the right of the point
    };

	//! Display modes
	//! Bit masks used in combination with the return value of the
	//! dirtyMask() method to determine which portions of the geometry
	//! are dirty.
	enum DisplayObjects {
		kDisplayEverything			= ~0,			//!< Show everything
		kDisplayNurbsCurves			= 1 << 0,		//!< Show nurbs curves
		kDisplayNurbsSurfaces		= 1 << 1, 		//!< Show nurbs surfaces
		kDisplayMeshes				= 1 << 2,		//!< Show meshes
		kDisplayPlanes				= 1 << 3,		//!< Show planes
		kDisplayLights				= 1 << 4,		//!< Show lights
		kDisplayCameras				= 1 << 5,		//!< Show camera
		kDisplayJoints				= 1 << 6,		//!< Show joints
		kDisplayIkHandles			= 1 << 7,		//!< Show IK handles
		kDisplayDeformers			= 1 << 8,		//!< Show deformers
		kDisplayDynamics			= 1 << 9,		//!< Show dynamics
		kDisplayParticleInstancers  = 1 << 10,		//!< Show particle instancers
		kDisplayLocators			= 1 << 11,		//!< Show locators
		kDisplayDimensions			= 1 << 12,		//!< Show dimensions
		kDisplaySelectHandles		= 1 << 13,		//!< Show selection handles
		kDisplayPivots				= 1 << 14,		//!< Show pivots
		kDisplayTextures			= 1 << 15,		//!< Show textures
		kDisplayGrid				= 1 << 16,		//!< Show the grid
		kDisplayCVs					= 1 << 17,		//!< Show NURBS CVs
		kDisplayHulls				= 1 << 18,		//!< Show NURBS hulls
		kDisplayStrokes				= 1 << 19,		//!< Show strokes
		kDisplaySubdivSurfaces		= 1 << 20,		//!< Show subdivision surfaces
		kDisplayFluids				= 1 << 21,		//!< Show fluids
		kDisplayFollicles			= 1 << 22,		//!< Show follicles
		kDisplayHairSystems			= 1 << 23,		//!< Show hair systems
		kDisplayImagePlane			= 1 << 24,		//!< Show image plane
		kDisplayNCloths				= 1 << 25,		//!< Show nCloths
		kDisplayNRigids				= 1 << 26,		//!< Show nRigids
		kDisplayDynamicConstraints	= 1 << 27,		//!< Show nDynamicConstraints
		kDisplayManipulators		= 1 << 28,		//!< Show Manipulators
		kDisplayNParticles			= 1 << 29,		//!< Show nParticles
		kExcludeMotionTrails		= 1 << 30,		//!< Show motion trails
		kExcludePluginShapes		= 1 << 31		//!< Show plugin shapes
	};

	//! Lighting mode used in this 3D view.
	enum LightingMode {
		kLightAll,							//!< All lights
		kLightSelected,						//!< Selected lights
		kLightActive,						//!< Active lights
		kLightDefault,						//!< Default light
		kUnused1,							//!< Not currently used in Maya
		kLightNone							//!< No lights / lighting disabled
	};

	//! Current hardware rendering engine used in this view.
	enum RendererName {
		//! Equivalent to when the renderer name is "base_OpenGL_Renderer" when queried from the "modelEditor" command
		kDefaultQualityRenderer,
		//! Equivalent to when the renderer name is "hwRender_OpenGL_Renderer" when queried from the "modelEditor" command
		kHighQualityRenderer,
		//! Equivalent to the viewport 2.0 renderer
		kViewport2Renderer,
		//! An externally defined renderer name has been set.
		kExternalRenderer
	};

	//! Possible depth buffer formats to read into.
	enum DepthBufferFormat
	{
		kDepth_8 = 0,						//!< 8 bits.
		kDepth_Float						//!< Floating point.
	};

	//! Line stipple pattern
	enum LineStipplePattern
	{
		kStippleNone,		//!< No stipple. Solid line
		kStippleDashed		//!< Dashed line stipple
	};

    M3dView();
    M3dView(const M3dView& other);
	M3dView &	operator=(const M3dView & other);
    virtual ~M3dView();

	// global methods
    static M3dView		active3dView( MStatus * ReturnStatus = NULL );
	static unsigned int     numberOf3dViews();
	static MStatus      get3dView( const unsigned int index,
							                M3dView & view );
	static DisplayStatus	displayStatus( const MDagPath& path, MStatus * ReturnStatus = NULL );

	MStatus makeSharedContextCurrent();
#if defined (LINUX)
	//! Linux
    Display *			display( MStatus * ReturnStatus = NULL );
	//! Linux
	MGLContext	        glxContext( MStatus * ReturnStatus = NULL );
#else
    //! Mac OS X and Windows
	MGLContext			display ( MStatus * ReturnStatus = NULL );

#	if defined (_WIN32)
	//! Windows
	HDC     			deviceContext( MStatus * ReturnStatus = NULL );
#	endif
#endif

	static MNativeWindowHdl applicationShell( MStatus * ReturnStatus = NULL );
	void		getScreenPosition( int &x, int &y, MStatus *ReturnStatus = NULL ) const;
	QWidget*	widget( MStatus * ReturnStatus = NULL ) const;
	MNativeWindowHdl	window( MStatus * ReturnStatus = NULL );
    int         portWidth( MStatus * ReturnStatus = NULL );
    int         portHeight( MStatus * ReturnStatus = NULL );
    bool        isVisible( MStatus * ReturnStatus = NULL );

	int         playblastPortWidth( MStatus * ReturnStatus = NULL );
	int         playblastPortHeight( MStatus * ReturnStatus = NULL );

	// Viewport sizing methods
    MStatus		pushViewport (unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    MStatus		popViewport  ();
    MStatus		viewport (unsigned int &x, unsigned int &y, unsigned int &width, unsigned int &height) const;


	// OpenGL wrapper methods

  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use Viewport 2.0 APIs instead. See 'Viewport 2.0' chapter of the 'API Guide' for alternatives.")
    MStatus     beginGL();

  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use Viewport 2.0 APIs instead. See 'Viewport 2.0' chapter of the 'API Guide' for alternatives.")
    MStatus     endGL();

    //  OpenGL Selection
    //
    void            beginSelect (GLuint *buffer = NULL, GLsizei size = 0);
    GLint           endSelect   ();
    bool            selectMode  () const;
	bool 			textureMode () const;
    void            loadName    (GLuint name);
    void            pushName    (GLuint name);
    void            popName     ();
    void            initNames   ();

	// XOR drawing methods
	MStatus		beginXorDrawing(bool drawOrthographic = true,
								bool disableDepthTesting = true,
								float lineWidth = 1.0f,
								LineStipplePattern stipplePattern = kStippleNone,
								const MColor& lineColor = MColor(1, 1, 1));
	MStatus		endXorDrawing();

    // Color methods
  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MUIDrawManager::setColorIndex method instead.")
    MStatus     setDrawColor( unsigned int index,
							  ColorTable table = kActiveColors );

  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MUIDrawManager::setColor method instead.")
    MStatus     setDrawColor( const MColor & color );

    OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MUIDrawManager::setColor method instead.")
    MStatus     setDrawColorAndAlpha( const MColor & color );

    unsigned int    numDormantColors( MStatus * ReturnStatus = NULL );
    unsigned int    numActiveColors( MStatus * ReturnStatus = NULL );
    unsigned int    numUserDefinedColors( MStatus * ReturnStatus = NULL );

    MStatus     setUserDefinedColor( unsigned int index, const MColor & color );
    unsigned int    userDefinedColorIndex( unsigned int index,
                                       MStatus * ReturnStatus = NULL );

    MColor      colorAtIndex( unsigned int index, ColorTable table = kActiveColors,
                              MStatus * ReturnStatus = NULL );
	MStatus		getColorIndexAndTable( unsigned int glindex, unsigned int &index,
									   ColorTable &table ) const;

    OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MUIDrawManager instead.")
	MStatus		colorMask(bool &r, bool &g, bool &b, bool &a);

	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MUIDrawManager instead.")
	MStatus		setColorMask(bool r, bool g, bool b, bool a);

	static bool         isBackgroundGradient( MStatus * ReturnStatus = NULL );

	static MColor       templateColor( MStatus * ReturnStatus = NULL );
	static MColor       backgroundColor( MStatus * ReturnStatus = NULL );
	static MColor       backgroundColorTop( MStatus * ReturnStatus = NULL );
	static MColor       backgroundColorBottom( MStatus * ReturnStatus = NULL );
	static MColor       liveColor( MStatus * ReturnStatus = NULL );
	static MColor       referenceLayerColor( MStatus * ReturnStatus = NULL );
	static MColor       activeTemplateColor( MStatus * ReturnStatus = NULL );
	static MColor       leadColor( MStatus * ReturnStatus = NULL );
	static MColor       hiliteColor( MStatus * ReturnStatus = NULL );
	static MColor       activeAffectedColor( MStatus * ReturnStatus = NULL );


    // Text drawing methods
  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MUIDrawManager in a MHWRender::MHUDRender operation instead.")
    MStatus     drawText( const MString & text, const MPoint position,
                          TextPosition textPosition = kLeft );

    // Camera methods

    MStatus     getCamera( MDagPath & camera );
    MStatus     setCamera( MDagPath & camera );

    static MStatus scheduleRefreshAllViews();
	MStatus		scheduleRefresh();

    MStatus     refresh( bool all = false, bool force = false );
    MStatus     refresh( bool all, bool force, bool offscreen );
    MStatus     refresh( MPxGlBuffer &buffer );
    MStatus     refresh( MPxGlBuffer &buffer, bool offscreen );
    MStatus     refresh( MPxGlBuffer &buffer, bool offscreen, const MMatrix& projectionMatrix );

	// Light methods
	MStatus		getLightCount( unsigned int &count, bool visible = true );
	MStatus		getLightingMode(LightingMode &mode );
	MStatus		getLightPath( unsigned int lightNumber, MDagPath &light );
	MStatus		isLightVisible( unsigned int lightNumber, bool &visible );
	MStatus		getLightIndex( unsigned int lightNumber, unsigned int &lightIndex );

    // Transformation methods

    MStatus     viewToWorld( short x_pos, short y_pos,
                             MPoint & worldPt, MVector & worldVector ) const;
    MStatus     viewToWorld( short x_pos, short y_pos,
                             MPoint & nearClipPt, MPoint & farClipPt ) const;
    MStatus     viewToObjectSpace( short x_pos, short y_pos,
                                   const MMatrix & localMatrixInverse,
                                   MPoint & oPt, MVector & oVector ) const;
    bool        worldToView( const MPoint& worldPt,
                             short& x_pos, short& y_pos,
                             MStatus * ReturnStatus = NULL ) const;
	MStatus     projectionMatrix( MMatrix& projectionMatrix ) const;

	MStatus     modelViewMatrix( MMatrix& modelViewMatrix ) const;

	//	View selected prefix
	//
	MString			viewSelectedPrefix(MStatus *ReturnStatus) const;
	MStatus			setViewSelectedPrefix( const MString &prefix);

	//	isolateSelect behavior
	//
	bool			showViewSelectedChildren(MStatus *ReturnStatus) const;
	MStatus			setShowViewSelectedChildren(bool);

	//	Method to get a M3dView from a modelPanel
	//
	static MStatus 	getM3dViewFromModelPanel(const MString &modelPaneName,
											 M3dView &view);

	//	Method to get a M3dView from a modelEditor
	//
	static MStatus 	getM3dViewFromModelEditor(const MString &modelPaneName,
											  M3dView &view);

    // Display style methods
	//
    DisplayStyle    displayStyle( MStatus * ReturnStatus = NULL ) const;
    bool        isShadeActiveOnly( MStatus * ReturnStatus = NULL ) const;
    MStatus     setDisplayStyle ( DisplayStyle style, bool activeOnly = false);


	//	Exclude/display flags
	//
	MStatus 		setObjectDisplay( unsigned int displayMask );
	unsigned int 	objectDisplay( MStatus *ReturnStatus = NULL );
	MStatus			setPluginObjectDisplay(const MString& pluginDisplayFilter, bool on);
	bool			pluginObjectDisplay( const MString& pluginDisplayFilter, MStatus *ReturnStatus = NULL );
	// Renderer methods
	RendererName	getRendererName(MStatus *ReturnStatus /* = NULL */) const;
	MString			rendererString( MStatus * ReturnStatus = NULL ) const;
	bool			wireframeOnlyInShadedMode(MStatus *ReturnStatus /* = NULL */) const;
    bool            wireframeOnShaded( MStatus *ReturnStatus = NULL ) const;
	bool            xray( MStatus *ReturnStatus = NULL ) const;
	bool            xrayJoints( MStatus *ReturnStatus = NULL ) const;
	bool            twoSidedLighting( MStatus *ReturnStatus = NULL ) const;

	// Viewport buffer methods
  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MRenderTargetManager::acquireRenderTarget() instead.")
	MStatus			readColorBuffer( MImage &image, bool readRGBA = false );

  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MQuadRender operation inside MHWRender::MRenderOverride instead.")
  	MStatus			writeColorBuffer( const MImage &image, signed short x = 0, signed short y = 0) const;

  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MRenderTargetManager::acquireRenderTarget() instead.")
	MStatus			readDepthMap(unsigned short x, unsigned short y,
								unsigned int width, unsigned int height,
								unsigned char* bufferPtr,
								DepthBufferFormat depthMapPrecision);

  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MRenderTargetManager instead.")
	MStatus			readBufferTo2dTexture(unsigned short x, unsigned short y,
								unsigned int width, unsigned int height);

	// Texture mipmap methods
	bool			usingMipmappedTextures() const;

	// Material settings
	bool 			usingDefaultMaterial() const;

	MStatus			setDisallowPolygonOffset( bool v );
	bool			disallowPolygonOffset() const;
	// Multiple draw pass methods
	MStatus			updateViewingParameters() ;
	bool			multipleDrawEnabled() const;

  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MRenderOverride instead.")
	void			setMultipleDrawEnable( bool enable );

  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MRenderOverride instead.")	
	unsigned		multipleDrawPassCount();

  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MRenderOverride instead.")
	void			setMultipleDrawPassCount( unsigned count );

	// Projection matrix override
  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MRenderOverride instead.")
	MStatus			beginProjMatrixOverride( MMatrix& projectionMatrix );

  	OPENMAYA_DEPRECATED(2019, "This function is deprecated. Please use MHWRender::MRenderOverride instead.")
	MStatus			endProjMatrixOverride();

	// Render override
	MStatus			setRenderOverrideName( const MString & name );
	MString			renderOverrideName(MStatus *ReturnStatus = NULL) const;

	// Object list filter override.
	// Note: These methods are not implemented yet.
	MStatus			setObjectListFilterName( const MString & name );
	MString			objectListFilterName(MStatus *ReturnStatus = NULL) const;
	MStatus			setShowObjectFilterNameInHUD(bool show);
	bool			showObjectFilterNameInHUD(MStatus *ReturnStatus) const;

	// View filtering
	bool			viewIsFiltered(MStatus *ReturnStatus = NULL) const;
	MStatus			filteredObjectList(MSelectionList &list) const;

BEGIN_NO_SCRIPT_SUPPORT:
	//!     NO SCRIPT SUPPORT
	MStatus			getRendererString( MString &stringName ) const;

END_NO_SCRIPT_SUPPORT:
    static const char* className();

protected:
// No protected members

private:
    M3dView( const void * );
    const void * fPtr;
	void *fXorDrawPtr;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _M3dView */
