#ifndef _MFnCamera
#define _MFnCamera
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
// CLASS:    MFnCamera
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MPoint.h>
#include <maya/MDeprecate.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnCamera)

//! \ingroup OpenMaya MFn
//! \brief Function set for cameras. 
/*!
Function set for creation, edit, and query of cameras.

Many of the camera settings only affect the resulting rendered image.
E.g. the F/Stop, shutter speed, the film related options, etc.
*/
class OPENMAYA_EXPORT MFnCamera : public MFnDagNode
{
	declareDagMFn(MFnCamera,MFnDagNode);
public:
	MObject 	create( MStatus * ReturnStatus = NULL );
	MObject 	create( MObject & parent, MStatus * ReturnStatus = NULL );
	MPoint		eyePoint( MSpace::Space space = MSpace::kObject,
					MStatus * ReturnStatus = NULL ) const;
	MVector		viewDirection( MSpace::Space space = MSpace::kObject,
					MStatus * ReturnStatus = NULL ) const;
	MVector		upDirection( MSpace::Space space = MSpace::kObject,
					MStatus * ReturnStatus = NULL ) const;
	MVector		rightDirection( MSpace::Space space = MSpace::kObject,
					MStatus * ReturnStatus = NULL ) const;
	MPoint		centerOfInterestPoint(
					MSpace::Space space = MSpace::kObject,
					MStatus * ReturnStatus = NULL ) const;
    MStatus     set ( const MPoint &	wsEyeLocation,
					  const MVector & 	wsViewDirection,
					  const MVector & 	wsUpDirection,
					  double         	horizFieldOfView,
					  double        	aspectRatio );
    MStatus     setEyePoint( const MPoint &eyeLocation,
					MSpace::Space space = MSpace::kObject );
    MStatus     setCenterOfInterestPoint( const MPoint &centerOfInterest,
					MSpace::Space space = MSpace::kObject );
    bool        hasSamePerspective( const MDagPath &otherCamera,
								MStatus * ReturnStatus = NULL );
    MStatus     copyViewFrom( const MDagPath &otherCamera );
	MStatus		getFilmFrustum( double distance, MPointArray clipPlanes,
                                bool applyPanZoom = false) const;
	MStatus		getFilmFrustum( double distance,
								double& hSize,
								double& vSize,
								double& hOffset,
								double& vOffset,
                                bool applyPanZoom = false) const;
	MStatus		getPortFieldOfView( int width, int height,
									double& horizontalFOV,
									double& verticalFOV ) const;
	MStatus		getViewParameters( double windowAspect,
                                   double& apertureX, double& apertureY,
                                   double& offsetX, double& offsetY,
                                   bool applyOverscan = false,
                                   bool applySqueeze = false,
                                   bool applyPanZoom = false) const;
    MStatus		getViewingFrustum( double windowAspect,
								   double& left, double& right,
								   double& bottom, double& top,
								   bool applyOverscan = false,
								   bool applySqueeze = false,
                                   bool applyPanZoom = false) const;
    MStatus		getRenderingFrustum( double windowAspect,
									 double& left, double& right,
									 double& bottom, double& top ) const;
	MFloatMatrix	projectionMatrix( MStatus * ReturnStatus = NULL ) const;
    MStatus		setHorizontalFilmAperture( double hFilmAperture );
    double		horizontalFilmAperture( MStatus * ReturnStatus = NULL ) const;
    MStatus		setVerticalFilmAperture( double vFilmAperture );
    double      verticalFilmAperture( MStatus * ReturnStatus = NULL ) const;
    MStatus		getFilmApertureLimits( double & min, double & max ) const;
    MStatus		setAspectRatio( double aspectRatio );
    double      aspectRatio( MStatus * ReturnStatus = NULL ) const;
    MStatus		getAspectRatioLimits(double & min, double & max) const;
    MStatus		setVerticalLock( bool lockFlag );
    bool        isVerticalLock( MStatus * ReturnStatus = NULL ) const;
	double		horizontalFilmOffset( MStatus * ReturnStatus = NULL ) const;
	MStatus		setHorizontalFilmOffset( double hOffset );
	double		verticalFilmOffset( MStatus * ReturnStatus = NULL ) const;
	MStatus		setVerticalFilmOffset( double vOffset );
	bool		shakeEnabled( MStatus * ReturnStatus = NULL) const;
	MStatus		setShakeEnabled( bool enabled );
	double		horizontalShake( MStatus * ReturnStatus = NULL ) const;
	MStatus		setHorizontalShake( double hOffset );
	double		verticalShake( MStatus * ReturnStatus = NULL ) const;
	MStatus		setVerticalShake( double vOffset );
	bool		shakeOverscanEnabled( MStatus * ReturnStatus = NULL) const;
	MStatus		setShakeOverscanEnabled( bool enabled );
	double		shakeOverscan( MStatus * ReturnStatus = NULL ) const;
	MStatus		setShakeOverscan( double vOffset );
    bool        panZoomEnabled( MStatus * ReturnStatus = NULL ) const;
    MStatus     setPanZoomEnabled( bool enabled );
    bool        renderPanZoom( MStatus * ReturnStatus = NULL ) const;
    MStatus     setRenderPanZoom( bool render );
    double      horizontalPan( MStatus * ReturnStatus = NULL ) const;
    MStatus     setHorizontalPan( double hPan );
    double      verticalPan( MStatus * ReturnStatus = NULL ) const;
    MStatus     setVerticalPan( double vPan );
    double      zoom( MStatus * ReturnStatus = NULL ) const;
    MStatus     setZoom( double zoom );
	bool		stereoHITEnabled( MStatus * ReturnStatus = NULL) const;
	MStatus		setStereoHITEnabled( bool enabled );
	double		stereoHIT( MStatus * ReturnStatus = NULL ) const;
	MStatus		setStereoHIT( double vOffset );


	//! Specifies how to fit the digital image to the film back.
	/*!
	This describes how the digital image (in pixels) relates to
	the film back. Since the film back is defined in terms of real
	numbers with some arbitrary film aspect, and the digital image
	is defined in integer pixels with an equally arbitrary (and
	different) resolution, relating the two can get complicated.
	*/
	enum FilmFit {
		/*!
		The system calculates both
		horizontal and vertical fits and then applies the one that
		makes the digital image larger than the film back.
		*/
		kFillFilmFit,

		/*!
		The digital image is made to fit the film
		back exactly in the horizontal direction. This then gives each
		pixel a horizontal size = (film back width) / (horizontal
		resolution). The pixel height is then = (pixel width) / (pixel
		aspect ratio). Now that the pixel has a size, resolution gives
		us a complete image. That image will match the film back
		exactly in width. It will almost never match in height, either
		being too tall or too short. By playing with the numbers you
		can get it pretty close though.
		*/
		kHorizontalFilmFit,

		/*!
		The same idea as horizontal fit, only applied
		vertically. Thus the digital image will match the film back
		exactly in height, but miss in width.
		*/
		kVerticalFilmFit,

		/*!
		Over-scanning the film gate in the camera view allows us
		to choreograph action outside of the frustum from within the
		camera view without having to resort to a dolly or zoom. This
		feature is also essential for animating image planes.
		*/
		kOverscanFilmFit,

		kInvalid	//!< \nop
    };
	MStatus		setFilmFit( FilmFit filmFit );
	FilmFit		filmFit( MStatus * ReturnStatus = NULL ) const;
	MStatus		setFilmFitOffset( double filmFitOffset );
	double		filmFitOffset( MStatus * ReturnStatus = NULL ) const;
	MStatus		setOverscan( double overscan );
	double		overscan( MStatus * ReturnStatus = NULL ) const;
	MStatus		setHorizontalRollPivot( double horizontalRollPivot );
	double		horizontalRollPivot( MStatus * ReturnStatus = NULL ) const;
	MStatus 	setVerticalRollPivot( double verticalRollPivot );
	double		verticalRollPivot( MStatus * ReturnStatus = NULL ) const;
	MStatus 	setFilmRollValue( double filmRollValue );
	double 		filmRollValue( MStatus * ReturnStatus = NULL ) const;

	//! \brief Order in which film back rotation and its transformation by the
	//!	pivot point are applied.
	enum RollOrder {
		kRotateTranslate,	//!< Rotate the film back before translating it by the pivot point.
		kTranslateRotate	//!< Translate the film back by the pivot point before rotating it.
	};
	MStatus		setFilmRollOrder( RollOrder filmRollOrder );
	RollOrder	filmRollOrder( MStatus * ReturnStatus = NULL ) const;

	MStatus		setPreScale( double sf );
	double		preScale( MStatus * ReturnStatus = NULL ) const;
	MStatus		setPostScale( double sf );
	double		postScale( MStatus * ReturnStatus = NULL ) const;
	MStatus		setFilmTranslateH( double translate );
	double		filmTranslateH( MStatus * ReturnStatus = NULL ) const;
	MStatus		setFilmTranslateV( double translate );
	double		filmTranslateV( MStatus * ReturnStatus = NULL ) const;
	MFloatMatrix postProjectionMatrix( MStatus * ReturnStatus = NULL ) const;
	MStatus		setDisplayGateMask( bool displayGateMask );
	bool		isDisplayGateMask( MStatus * ReturnStatus = NULL ) const;
	MStatus		setDisplayFilmGate( bool displayFilmGate );
	bool		isDisplayFilmGate( MStatus * ReturnStatus = NULL ) const;
    MStatus		setHorizontalFieldOfView( double fov );
    double      horizontalFieldOfView( MStatus * ReturnStatus = NULL ) const;
    MStatus		setVerticalFieldOfView( double fov );
    double      verticalFieldOfView( MStatus * ReturnStatus = NULL ) const;
    MStatus		setFocalLength( double focalLength );
    double      focalLength( MStatus * ReturnStatus = NULL ) const;
    MStatus		getFocalLengthLimits( double & min, double & max ) const;
    MStatus		setLensSqueezeRatio( double lensSqueezeRatio );
    double      lensSqueezeRatio( MStatus * ReturnStatus = NULL ) const;
    MStatus		setClippingPlanes( bool ClippingPlanes );
    bool        isClippingPlanes( MStatus * ReturnStatus = NULL ) const;
    MStatus		setNearClippingPlane( double dNear );
    double      nearClippingPlane( MStatus * ReturnStatus = NULL ) const;
    MStatus		setFarClippingPlane( double dFar );
    double      farClippingPlane( MStatus * ReturnStatus = NULL ) const;
	MStatus		setNearFarClippingPlanes( double dNear, double dFar );
    MStatus		setDepthOfField( bool depthOfField );
    bool        isDepthOfField( MStatus * ReturnStatus = NULL ) const;
    MStatus		setFStop( double fStop );
    double      fStop( MStatus * ReturnStatus = NULL ) const;
    MStatus		setFocusDistance( double distance );
    double		focusDistance( MStatus * ReturnStatus = NULL ) const;
    MStatus		setNearFocusDistance( double nearFocusDistance );
    double      nearFocusDistance( MStatus * ReturnStatus = NULL ) const;
    MStatus		setFarFocusDistance( double farFocusDistance );
    double      farFocusDistance( MStatus * ReturnStatus = NULL ) const;
    MStatus		computeDepthOfField();
    MStatus		computeDepthOfField( double nearLimit );
    MStatus		setMotionBlur( bool motionBlur );
    bool        isMotionBlur( MStatus * ReturnStatus = NULL ) const;
    MStatus		setShutterAngle( double shutterAngle );
    double      shutterAngle( MStatus * ReturnStatus = NULL ) const;
    MStatus		setCenterOfInterest( double dist );
    double      centerOfInterest( MStatus * ReturnStatus = NULL ) const;
    MStatus		setIsOrtho( bool orthoState );
    MStatus		setIsOrtho( bool orthoState, double useDist );
    bool        isOrtho( MStatus * ReturnStatus = NULL ) const;
    MStatus		setOrthoWidth( double orthoWidth );
    double      orthoWidth( MStatus * ReturnStatus = NULL ) const;
    MStatus		setCameraScale( double scale );
    double      cameraScale( MStatus * ReturnStatus = NULL ) const;
	MStatus		setTumblePivot( const MPoint& point );
	MPoint		tumblePivot( MStatus * ReturnStatus = NULL ) const;
    double      unnormalizedNearClippingPlane( MStatus * ReturnStatus = NULL ) const;
    double      unnormalizedFarClippingPlane( MStatus * ReturnStatus = NULL ) const;

BEGIN_NO_SCRIPT_SUPPORT:

 	declareDagMFnConstConstructor( MFnCamera,MFnDagNode );

	//! Obsolete & no script support
    OPENMAYA_DEPRECATED(2019, "This is an obsolete method, only kept for backward compatibility reasons. It does not do anything.")
	MStatus		setStereo( bool stereo );
	//! Obsolete & no script support
    OPENMAYA_DEPRECATED(2019, "This is an obsolete method, only kept for backward compatibility reasons. It does not do anything.")
	bool    	isStereo( MStatus * ReturnStatus = NULL ) const;
	//! obsolete & no script support
	MStatus		setParallelView( bool parallelView );
	//! obsolete & no script support
    OPENMAYA_DEPRECATED(2019, "This is an obsolete method, only kept for backward compatibility reasons. It does not do anything.")
	bool    	isParallelView( MStatus * ReturnStatus = NULL ) const;
	//! Obsolete & no script support
    OPENMAYA_DEPRECATED(2019, "This is an obsolete method, only kept for backward compatibility reasons. It does not do anything.")
	MStatus		setEyeOffset( double eyeOffset );
	//! Obsolete & no script support
    OPENMAYA_DEPRECATED(2019, "This is an obsolete method, only kept for backward compatibility reasons. It does not do anything.")
	double		eyeOffset( MStatus * ReturnStatus = NULL ) const;

	//!	No script support
	MStatus		getFilmFrustum( double distance, MPoint clipPlane[4],
                                bool applyPanZoom = false) const;

END_NO_SCRIPT_SUPPORT:
	MStatus		setUsePivotAsLocalSpace( const bool how );
	bool		usePivotAsLocalSpace( MStatus * ReturnStatus = NULL ) const;

public:
	// Special methods using an alternate context for evaluation
	MFloatMatrix	postProjectionMatrix( const MDGContext& context, MStatus * ReturnStatus = NULL ) const;
	MFloatMatrix	projectionMatrix	( const MDGContext& context, MStatus * ReturnStatus = NULL ) const;

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnCamera */
