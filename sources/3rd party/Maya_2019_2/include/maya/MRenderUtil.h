#ifndef _MRenderUtil
#define _MRenderUtil
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
// CLASS:    MRenderUtil
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MDeprecate.h>

// ****************************************************************************
// DECLARATIONS

class MFloat;

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRenderUtil)

//! \ingroup OpenMayaRender
//! \brief Common API rendering functions.
/*!
	MRenderUtil is a class which provides static API methods to access
	Maya's rendering functionalities.
*/
class OPENMAYARENDER_EXPORT MRenderUtil
{
public:

	//! Defines the current rendering state of Maya
	enum MRenderState {
		kNotRendering,		//!< Not rendering.
		kBatchRender,		//!< Performing a batch render.
		kInteractiveRender,	//!< Performing an interactive render.
		kIprRender,			//!< Performing an IPR render.
		kHardwareRender		//!< Performing a hardware render.
	};

	//! Defines the current pass of Maya
	enum MRenderPass {
		kAll,			//!< Default case, compute everything.
		kColorOnly,		//!< Compute only color information, no shadows.
		kShadowOnly,	//!< Compute only shadow information, no color.
		kAmbientOnly,	//!< Compute only ambient information.
		kDiffuseOnly,	//!< Compute only diffuse information.
		kSpecularOnly	//!< Compute only specular information.
	};

	static MRenderState	mayaRenderState();

	static MStatus	raytrace(
						const MFloatVector& rayOrigin,  // in camera space
						const MFloatVector& rayDirection,
						const void* objectId,
						const void* raySampler,
						const short rayDepth,

						// storage for return value

						MFloatVector& resultColor,
						MFloatVector& resultTransparency,

						// true for reflected rays, false for refracted rays
						const bool isReflectedRays = true
					);

	// vectorized version

	static MStatus	raytrace(
						const MFloatVectorArray& rayOrigins,  // in camera space
						const MFloatVectorArray& rayDirections,
						const void* objectId,
						const void* raySampler,
						const short rayDepth,

						// storage for return value

						MFloatVectorArray& resultColors,
						MFloatVectorArray& resultTransparencies,

						// true for reflected rays, false for refracted rays
						const bool isReflectedRays = true
					);

	static MStatus	raytraceFirstGeometryIntersections(
						const MFloatVectorArray& rayOrigins,  // in camera space
						const MFloatVectorArray& rayDirections,
						const void* objectId,
						const void* raySampler,

						// storage for return value

						MFloatVectorArray& 	resultIntersections,
						MIntArray& 			resultIntersected
					);


	static MStatus sampleShadingNetwork(

		MString             shadingNodeName,
		int                numSamples,
		bool				useShadowMaps,
		bool				reuseMaps,

		MFloatMatrix		&cameraMatrix,	// eye to world

		MFloatPointArray    *points,	// in world space
		MFloatArray         *uCoords,
		MFloatArray         *vCoords,
		MFloatVectorArray   *normals,	// in world space
		MFloatPointArray    *refPoints,	// in world space
		MFloatVectorArray   *tangentUs,	// in world space
		MFloatVectorArray   *tangentVs,	// in world space
		MFloatArray         *filterSizes,

		MFloatVectorArray   &resultColors,
		MFloatVectorArray   &resultTransparencies
	);

	static bool 	   generatingIprFile();

	static MString relativeFileName(
						const MString& absFileName,
						MStatus *ReturnStatus = NULL
						);

BEGIN_NO_SCRIPT_SUPPORT:

	static bool exactFileTextureName(
						const MObject& fileNode,
						MString& texturePath,
						MStatus *ReturnStatus = NULL
					);
	//! obsolete, use the one with contextNodeFullName param
	OPENMAYA_DEPRECATED(2019, "Context (owner Node full name) must be provided to the Maya resolver. Use the one with the contextNodeFullName input param.")
	static bool exactFileTextureName(
						const MString& baseName,
						bool           useFrameExt,
						const MString& currentFrameExt,
						MString&       exactName,
						MStatus *ReturnStatus = NULL
					);
	static bool exactFileTextureName(
						const MString& baseName,
						bool           useFrameExt,
						const MString& currentFrameExt,
						const MString& contextNodeFullName,
						MString&       exactName,
						MStatus *ReturnStatus = NULL
					);

	static bool exactFileTextureUvTileData(
						const MObject& fileNode,
						MStringArray& tilePaths,
						MFloatArray& tilePositions,
						MStatus *ReturnStatus = NULL);

	//! Internal use only
	static bool convertPsdFile(
						const MObject& fileNode,
						MString& texturePath,
						const bool		&forExport = false,
						MStatus *ReturnStatus = NULL
					);

	static bool exactImagePlaneFileName(
						const MObject& imagePlaneNode,
						MString& texturePath,
						MStatus *ReturnStatus = NULL
					);

END_NO_SCRIPT_SUPPORT:

	static MString exactFileTextureName(
						const MObject& fileNode,
						MStatus *ReturnStatus = NULL
					);
	//! obsolete, use the one with contextNodeFullName param
	OPENMAYA_DEPRECATED(2019, "Context (owner Node full name) must be provided to the Maya resolver. Use the one with the contextNodeFullName input param.")
	static MString exactFileTextureName(
						const MString& baseName,
						bool           useFrameExt,
						const MString& currentFrameExt,
						MStatus *ReturnStatus = NULL
					);
	static MString exactFileTextureName(
						const MString& baseName,
						bool           useFrameExt,
						const MString& currentFrameExt,
						const MString& contextNodeFullName,
						MStatus *ReturnStatus = NULL
					);

	//! Internal use only
	static MString convertPsdFile(
						const MObject& fileNode,
						const bool		&forExport = false,
						MStatus *ReturnStatus = NULL
					);

	static MString exactImagePlaneFileName(
						const MObject& imagePlaneNode,
						MStatus *ReturnStatus = NULL
					);

    static bool inCurrentRenderLayer(
                        const MDagPath& objectPath,
		                MStatus *ReturnStatus = NULL
                    );

	static MRenderPass renderPass( void );

	static MString mainBeautyPassCustomTokenString();
	static MString mainBeautyPassName();

	static float	diffuseReflectance(
						const void* lightBlindData,
						const MFloatVector& lightDirection,
						const MFloatVector& pointCamera,
						const MFloatVector& normal,
						bool lightBackFace,
						MStatus *ReturnStatus = NULL );

	static MFloatVector		maximumSpecularReflection(
								const void* lightBlindData,
								const MFloatVector& lightDirection,
								const MFloatVector&  pointCamera,
								const MFloatVector& normal,
								const MFloatVector& rayDirection,
								MStatus *ReturnStatus = NULL );

	static float	lightAttenuation(
							const void* lightBlindData,
							const MFloatVector& pointCamera,
							const MFloatVector& normal,
							bool lightBackFace,
							MStatus *ReturnStatus = NULL );

	static float	hemisphereCoverage(
							const void* lightBlindData,
							const MFloatVector& lightDirection,
							const MFloatVector& pointCamera,
							const MFloatVector& rayDirection,
							bool lightBackFace,
							MStatus *ReturnStatus = NULL );

	static void		sendRenderProgressInfo (const MString &pixFile, int percentageDone);

	// Fill the set of common render settings, based on current renderer
	static void		getCommonRenderSettings(MCommonRenderSettingsData &rgData);

    static MStatus  renderObjectItem(
                            const void* objectId,
                            MSelectionList& item);

	static const char* className();

	static MStatus		eval2dTexture( const MObject& texNode,
							MDoubleArray& uCoords,
							MDoubleArray& vCoords,
							MVectorArray* resultColors,
							MDoubleArray* resultAlphas );

	// Methods for accessing Maya's noise table used by many procedural textures
	static unsigned int noiseTableSize();
	static unsigned int noiseTableCubeSide();
	static float valueInNoiseTable(unsigned int index);
	static float noise1(float x);
	static float noise2(float x, float y);
	static float noise3(float x, float y, float z);
	static float noise4(float x, float y, float z, float t, unsigned short numTimeSteps);

private:
    ~MRenderUtil();
#ifdef __GNUC__
	friend class shutUpAboutPrivateDestructors;
#endif
};

// ****************************************************************************

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderUtil */
