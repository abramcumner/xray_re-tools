#ifndef _MPx3dModelView
#define _MPx3dModelView
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

// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/M3dView.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// CLASS DECLARATION (MPx3dModelView)

//! \ingroup OpenMayaUI MPx
//! \brief \obsolete
/*!
  \deprecated
  Use MHWRender::MRenderOverride instead.

  MPx3dModelView is the class for user defined model views. The
  MPx3dModelView class works with the MPxModelEditorCommand class to
  create a user defined model editor that may be used in a window or
  in a scripted panel. When registering the model editor with the
  MFnPlugin::registerModelEditorCommand() method, an appropriate
  MPx3dModelView::creator() method is required. This class works for
  interactive Maya views and is not designed for rendering.

  One of the interesting uses of a MPx3dModelView is that it allows
  multiple cameras to be drawn into the same window. Like a normal
  model view, this view has a main camera associated with it. This
  camera is obtained with the getCamera() method and is the camera
  used for manipulations and selection.

  To setup a multiple camera draw, first the number of passes must be
  set by an overloaded multipleDrawPassCount() method. The
  preMultipleDraw() method allows any setup to be performed. The
  preMultipleDrawPass(unsigned int) is called for each pass, with the
  argument indicating which pass is currently being used. The camera
  for the specific pass may be set with the setCameraInDraw()
  method. The postMultipleDrawPass(unsigned int) method is called
  after the drawing for the indicated pass is complete. Finally any
  cleanup may be done with the postMultipleDraw() method.

  During the drawing, a filter exists to determine which items should
  be drawn.  The okForMultipleDraw(MdagPath &) allows filtering of
  what should be drawn.  Another approach which is faster is to tuen
  on the view selected mode (setViewSelected()) and use the
  setObjectsToView() or viewSelectedSet() to specify which items
  should be drawn. These values may be set per pass so that each
  camera has control over what gets drawn.
*/
class OPENMAYAUI_EXPORT MPx3dModelView {
public:

	//! Lighting mode used in this view.
	enum LightingMode {
		kLightAll,							//!< Use all lights.
		kLightSelected,						//!< Selected lights only.
		kLightActive,						//!< Active lights only.
		kLightDefault,						//!< Default light only.
		kLightNone,							//!< Use no lights.
		kLightQuality						//!< Use per pixel lighting
	};

	//! Fog computation modes.
	enum FogSource {
		kFogFragment,	//!< Computed per pixel (default).
		kFogCoordinate	//!< Computed by specified vertex fog coordinates.
	};

	//! Drop-off modes for fog.
	enum FogMode {
		kFogLinear,				//!< Linear drop off.
		kFogExponential,		//!< Exponential drop-off.
		kFogExponentialSquared	//!< Squared exponential drop-off.
	};

						MPx3dModelView();
	virtual				~MPx3dModelView();

			MString		name(MStatus *ReturnStatus = NULL) const;
	virtual	MString		viewType() const;
	virtual void		preMultipleDraw();
	virtual void		postMultipleDraw();
	virtual void		preMultipleDrawPass(unsigned int index);
	virtual void		postMultipleDrawPass(unsigned int index);
	virtual bool		okForMultipleDraw(const MDagPath &);
	virtual unsigned int 	multipleDrawPassCount();
	bool				multipleDrawEnabled() const;
	void				setMultipleDrawEnable(bool enable);
	bool				destroyOnPanelDestruction() const;
	void				setDestroyOnPanelDestruction(bool how);
	MStatus				updateViewingParameters();

	virtual void		removingCamera(MDagPath &cameraPath);

	MStatus				setDoUpdateOnMove( bool value );
	bool				doUpdateOnMove( MStatus *ReturnStatus = NULL ) const;

	MStatus				refresh(bool all = false, bool force = false);

	//	Text methods
	//
	MStatus				drawText(const MString &text, const MPoint position,
								 M3dView::TextPosition textPosition
								 = M3dView::kLeft);

	//
	//	OpenGL wrapper methods
	//
	MStatus				beginGL();
	MStatus				endGL();

	//	Camera methods
	//
	MStatus 			setCameraInDraw(MDagPath & camera);
	MStatus				setDrawCameraOverride( const MMatrix &worldMatrix, 
											   const MMatrix &projectionMatrix,
											   double left,  double right,
											   double bottom,double top,
											   double nearpt,double farpt,
											   bool isOrtho = false );
	MStatus				setDrawCameraOverride( const MMatrix &worldMatrix, 
											   const MMatrix &projectionMatrix, 
											   const MPointArray &frustum ); 
	MStatus 			setCamera(MDagPath & camera);
	MStatus				getCamera(MDagPath & camera);
	MStatus				setCameraSet(MObject & cameraSet);
	MStatus				getCameraSet(MObject & cameraSet);
	MStatus				setCurrentCameraSetCamera(const MString & cameraName);
	MStatus				getCurrentCameraSetCamera(MString & cameraName);

	//	Heads Up Display Methods
	//
	virtual MString		getCameraHUDName();
	MStatus 			setDisplayHUD(bool display);
	bool				displayHUD(MStatus *ReturnStatus = NULL) const;
	MStatus				drawHUDNow();

	MStatus		    setDrawAdornments(bool display);
	bool			drawAdornments( MStatus *ReturnStatus = NULL );
	MStatus			drawAdornmentsNow();

	MStatus			setDisplayAxis(bool display);
	bool			displayAxisOn(MStatus *ReturnStatus = NULL) const;
	MStatus			setDisplayAxisAtOrigin(bool display);
	bool			displayAxisAtOriginOn(MStatus *ReturnStatus = NULL) const;
	MStatus			setDisplayCameraAnnotation(bool display);
	bool		displayCameraAnnotationOn(MStatus *ReturnStatus = NULL) const;

	bool			isVisible(MStatus *ReturnStatus = NULL) const;

	//	Display style methods
	//
	M3dView::DisplayStyle	displayStyle(MStatus *ReturnStatus = NULL) const;
	bool			isShadeActiveOnly(MStatus *ReturnStatus = NULL) const;
	MStatus			setDisplayStyle(M3dView::DisplayStyle style,
									bool activeOnly = false);

	int				portWidth( MStatus * ReturnStatus = NULL );
	int				portHeight( MStatus * ReturnStatus = NULL );

	// XOR drawing methods
	MStatus			beginXorDrawing(
						bool drawOrthographic = true,
						bool disableDepthTesting = true,
						float lineWidth = 1.0f,
						M3dView::LineStipplePattern stipplePattern = M3dView::kStippleNone,
						const MColor& lineColor = MColor(1, 1, 1)
					);
	MStatus			endXorDrawing();

	// Color methods
	//

	MStatus			setDrawColor(unsigned int index,
						M3dView::ColorTable table = M3dView::kActiveColors );
	MStatus			setDrawColor(const MColor & color);

	unsigned int		numDormantColors(MStatus * ReturnStatus = NULL );
	unsigned int 		numActiveColors(MStatus * ReturnStatus = NULL );
	unsigned int 		numUserDefinedColors(MStatus * ReturnStatus = NULL);

	MStatus			setUserDefinedColor(unsigned int index, const MColor & color);
	unsigned int		userDefinedColorIndex(unsigned int index,
										  MStatus * ReturnStatus = NULL );

	bool			isBackgroundGradient( MStatus * ReturnStatus = NULL );

	MColor			templateColor(MStatus * ReturnStatus = NULL);
	MColor			backgroundColor(MStatus * ReturnStatus = NULL);
	MColor			backgroundColorTop(MStatus * ReturnStatus = NULL);
	MColor			backgroundColorBottom(MStatus * ReturnStatus = NULL);

	MColor 			colorAtIndex(unsigned int index, M3dView::ColorTable table
								 = M3dView::kActiveColors,
								 MStatus * ReturnStatus = NULL);
	MStatus			getColorIndexAndTable(unsigned int glindex, unsigned int &index,
										  M3dView::ColorTable &table ) const;


	// Transformation methods
	//

	MStatus			viewToWorld(short x_pos, short y_pos,
								MPoint & worldPt, MVector & worldVector ) const;
	MStatus			viewToWorld(short x_pos, short y_pos,
								MPoint & nearClipPt, MPoint & farClipPt ) const;
	MStatus			viewToObjectSpace(short x_pos, short y_pos,
									  const MMatrix & localMatrixInverse,
									  MPoint & oPt, MVector & oVector ) const;
	bool			worldToView(const MPoint& worldPt,
								short& x_pos, short& y_pos,
								MStatus * ReturnStatus = NULL ) const;

	//	Exclude/display flags
	//
	MStatus 		setObjectDisplay(M3dView::DisplayObjects, bool);
	bool			objectDisplay(M3dView::DisplayObjects,
								  MStatus *ReturnStatus = NULL);
	//	Culling flags
	//
	MStatus			setBackfaceCulling(bool cull);
	bool			isBackfaceCulling(MStatus *ReturnStatus = NULL) const;

	//	texture/display flags
	//
	MStatus			setWireframeOnShaded(bool on);
	bool			isWireframeOnShaded(MStatus *ReturnStatus = NULL) const;
	MStatus			setXrayEnabled(bool xray);
	bool			isXrayEnabled(MStatus *ReturnStatus = NULL) const;

	MStatus			setTextureDisplayEnabled(bool texture);
	bool			isTextureDisplayEnabled(MStatus *ReturnStatus = NULL) const;

	//	Lighting flags
	MStatus			setTwoSidedLighting(bool twoSided);
	bool			isTwoSidedLighting(MStatus *ReturnStatus = NULL) const;
	MStatus			setLightingMode(MPx3dModelView::LightingMode);
	MPx3dModelView::LightingMode   lightingMode(MStatus *ReturnStatus = NULL) const;

	// 	Fog
	MStatus			setFogEnabled(bool state);
	bool			isFogEnabled(MStatus *ReturnStatus = NULL) const;
	MPx3dModelView::FogSource	fogSource(MStatus *ReturnStatus = NULL) const;
	MStatus			setFogSource(MPx3dModelView::FogSource);
	MPx3dModelView::FogMode		fogMode(MStatus *ReturnStatus = NULL) const;
	MStatus 		setFogMode(MPx3dModelView::FogMode);
	double			fogDensity(MStatus *ReturnStatus = NULL) const;
	MStatus			setFogDensity(double);
	double			fogStart(MStatus *ReturnStatus = NULL) const;
	MStatus			setFogStart(double);
	double 			fogEnd(MStatus *ReturnStatus = NULL) const;
	MStatus			setFogEnd(double);
	MColor			fogColor(MStatus *ReturnStatus = NULL) const;
	MStatus			setFogColor(const MColor &);
	bool			isBackgroundFogEnabled(MStatus *ReturnStatus = NULL) const;
	MStatus			setBackgroundFogEnabled(bool enable);


	//	View Selected
	MString			viewSelectedPrefix(MStatus *ReturnStatus) const;
	MStatus			setViewSelectedPrefix(const MString &prefix);
	bool			viewSelected(MStatus *ReturnStatus = NULL) const;
	MStatus			setViewSelected(bool viewSelected);

	MObject			viewSelectedSet(MStatus *ReturnStatus = NULL) const;
	MStatus			setViewSelectedSet(const MObject &set);

	MStatus			getObjectsToView(MSelectionList &list) const;
	MStatus			setObjectsToView(const MSelectionList &list);

	bool			viewIsFiltered(MStatus *ReturnStatus = NULL) const;
	MStatus			filteredObjectList(MSelectionList &list) const;

	bool		    hasStereoBufferSupport() const;

	MStatus			getAsM3dView(M3dView &view);
	static MPx3dModelView* getModelView(const MString &name,
										MStatus *ReturnStatus = NULL);

	// Stereo methods
	virtual bool    wantStereoGLBuffer() const;
	MStatus			setInStereoDrawMode( bool flag);

	//-------------------------------------------------------------------------
	// THESE METHODS ARE FOR INTERNAL AUTODESK USE.  THEY WILL BE REMOVED IN A
	// FUTURE VERSION OF MAYA
	//-------------------------------------------------------------------------
	bool				customDrawEnabled() const;
	void				setCustomDrawEnable(bool);
	virtual void	 	customDraw( void *, void * );
	void 				drawOnePass( void *, void * );
	//-------------------------------------------------------------------------

	virtual bool		requestOkForDraw( const MDrawRequest & );
	virtual void		processDraw( MDrawRequest & );
	void				handleDraw( void *, MDrawRequest & );
	void				setDrawInterrupt( bool v );
	bool				drawInterrupt() const; 
	void				setIncludeInvisible( void *, bool v );
	bool				includeInvisible( void * ) const; 
	//-------------------------------------------------------------------------
											
	static const char*  className();

protected:
private:
	void setData(void *ptr);
	void *				instance;
	bool				fDestroyOnPanelDestruction;
	void *				fXorDrawPtr;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPx3dModelView */
