#ifndef _MMaterial
#define _MMaterial
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
// CLASS:    MMaterial
//
// ****************************************************************************
//
#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>
#include <maya/M3dView.h>
#include <maya/MDeprecate.h>

// ****************************************************************************
// DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MMaterial)

//! \ingroup OpenMayaUI
//! \brief \obsolete
/*!
\deprecated
Use MHWRender::MShaderInstance or MHWRender::MFragmentManager instead.

This class is used in the draw functions of user defined shapes
(see MPxSurfaceShapeUI) for setting up and querying materials
used in shaded mode drawing.
*/
class OPENMAYAUI_EXPORT MMaterial
{
public:
	MMaterial();
	MMaterial( const MMaterial& in );
	MMaterial( const MObject&, MStatus* ReturnStatus=NULL );
	~MMaterial();

public:
	// Evaluation methods.
	MStatus		evaluateMaterial(M3dView&, const MDagPath& );
	MStatus		evaluateShininess();
	MStatus		evaluateDiffuse();
	MStatus		evaluateEmission();
	MStatus		evaluateSpecular();
	MStatus		evaluateTexture( MDrawData & data );

	bool		materialIsTextured() const;

	// Draw methods.
    // Can call these after calling correct evaluate method above.
	MStatus		setMaterial(const MDagPath&, bool hasTransparency);
	MStatus		getShininess( float & );
	MStatus		getDiffuse( MColor & );
	MStatus		getEmission( MColor & );
	MStatus		getSpecular( MColor & );
    MStatus		getHasTransparency( bool & );
	MStatus		getTextureTransformation(const MDrawData& data,
                                         MMatrix& texXform);
	MStatus		getTextureTransformation(const MDrawData& data,
                                         float& rotateUV,
                                         float& scaleU,
										 float& scaleV,
										 float& translateU,
										 float& translateV,
										 float& rotateFrame);

	// This method sets the texture so that it can be used in
	// the specified view.
	void		applyTexture( M3dView&, const MDrawData& );

	//! Channels which can be queried. Only Lambert, Phong,
	//! PhongE, Blinn, and Anisotropic are supported
	typedef enum
	{
		kColor = 0,			//!< nop
		kTransparency,		//!< \nop
		kAmbientColor,		//!< \nop
		kIncandescence,		//!< \nop
		kBumpMap,			//!< \nop
		kDiffuse,			//!< \nop
		kTransluscence,		//!< \nop
		kRoughness,			//!< PhongE only
		kHighlightSize,		//!< PhongE only
		kWhiteness,			//!< PhongE only
		kCosinePower,		//!< Phong only
		kEccentricity,		//!< Blinn only
		kSpecularRollOff,	//!< Blinn only
		kSpecularColor,		//!< Blinn and Phong(E) only
		kReflectivity,		//!< Blinn and Phong(E) only
		kReflectedColor		//!< Blinn and Phong(E) only

	} MtextureChannel;
	MStatus textureImage( MImage &image, MColor &color,
						MtextureChannel chan, bool &mapped,
						MDagPath &dagPath,
						int xRes = -1, int yRes = -1);

	MPxHwShaderNode *	getHwShaderNode( MStatus * ReturnStatus = NULL );

	static MMaterial	defaultMaterial();

	MObject				shadingEngine(); 

	static const char*	className();

	//	Obsolete: This method was retained to provide backwards
	//	compatability.
    OPENMAYA_DEPRECATED(2019, "Use evaluateTexture() instead.")
	MStatus		evaluateTextureTransformation();

	//	Obsolete: This method was retained to provide backwards
	//	compatability.
    OPENMAYA_DEPRECATED(2019, "Use the non-deprecated overloads of the getTextureTransformation() member function instead.")
	MStatus		getTextureTransformation(float& scaleU,
										 float& scaleV,
										 float& translateU,
										 float& translateV,
										 float& rotate);
protected:
// No protected members

private:
    MMaterial( void* in );

	void*	 fMaterial;
};
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MMaterial */
