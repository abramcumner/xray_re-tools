#ifndef _MShaderManager
#define _MShaderManager
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
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MStringArray.h>
#include <maya/MColor.h>
#include <maya/MMatrix.h>
#include <maya/MFloatMatrix.h>
#include <maya/MTextureManager.h>
#include <maya/MRenderTargetManager.h>




OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// DECLARATIONS

// ****************************************************************************
// CLASS DECLARATION (MTextureAssignment)
//! \ingroup OpenMayaRender
//! \brief Structure to hold the information required to set a texture parameter on a shader
//! using a texture as input.
/*!
This structure holds the information required to set a texture parameter on a shader
using a texture as input.
*/
struct MTextureAssignment
{
	MHWRender::MTexture *texture;
};

// ****************************************************************************
// CLASS DECLARATION (MRenderTargetAssignment)
//! \ingroup OpenMayaRender
//! \brief Structure to hold the information required to set a texture parameter on a shader
//! using a render target as input.
/*!
This structure holds the information required to set a texture parameter on a shader
using a render target as input
*/
struct MRenderTargetAssignment
{
	MHWRender::MRenderTarget *target;
};

// ****************************************************************************
// CLASS DECLARATION (MShaderCompileMacro)
//! \ingroup OpenMayaRender
//! \brief Structure to define a shader compiler macro.
/*!
This structure holds the information required to define
a macro to be used when acquiring an shader instance
from an effects file.
*/
struct MShaderCompileMacro
{
	MString mName;			//!< Name of the macro
	MString mDefinition;	//!< Macro definition
};

// ****************************************************************************
// CLASS DECLARATION (MShaderInstance)
//! \ingroup OpenMayaRender
//! \brief An instance of a shader that may be used with Viewport 2.0
/*!
This class represents a shader that may be used with the MRenderItem class
for rendering in Viewport 2.0.
*/
class OPENMAYARENDER_EXPORT MShaderInstance
{
public:
	//! Definition for pre/post draw callback functions
	typedef void (*DrawCallback)(MDrawContext& context, const MRenderItemList& renderItemList, MShaderInstance *shaderInstance);

	//! Specifies parameter types
	enum ParameterType {
		//! Invalid element type (default value)
		kInvalid,
		//! Boolean
		kBoolean,
		//! Signed 32-bit integer
		kInteger,
		//! IEEE single precision floating point
		kFloat,
		//! IEEE single precision floating point (x2)
		kFloat2,
		//! IEEE single precision floating point (x3)
		kFloat3,
		//! IEEE single precision floating point (x4)
		kFloat4,
		//! IEEE single precision floating point row-major matrix (4x4)
		kFloat4x4Row,
		//! IEEE single precision floating point column-major matrix (4x4)
		kFloat4x4Col,
		//! 1D texture
		kTexture1,
		//! 2D texture
		kTexture2,
		//! 3D texture
		kTexture3,
		//! Cube texture
		kTextureCube,
		//! Sampler
		kSampler
	};

	DrawCallback preDrawCallback() const;
	DrawCallback postDrawCallback() const;

	void parameterList(MStringArray& list) const;
	ParameterType parameterType(const MString& parameterName) const;
	bool isArrayParameter(const MString& parameterName) const;
	int getArraySize(const MString& parameterName) const;
	MString semantic(const MString& parameterName) const;
    bool isVaryingParameter(const MString& parameterName) const;

	MStatus setParameter(const MString& parameterName, bool value);
	MStatus setParameter(const MString& parameterName, int value);
	MStatus setParameter(const MString& parameterName, float value);
	MStatus setParameter(const MString& parameterName, const float* value);
	MStatus setParameter(const MString& parameterName, const MFloatVector& value);
	MStatus setParameter(const MString& parameterName, const MMatrix& value);
	MStatus setParameter(const MString& parameterName, const MFloatMatrix& value);
	MStatus setParameter(const MString& parameterName, MTextureAssignment& textureAssignment);
	MStatus setParameter(const MString& parameterName, MRenderTargetAssignment& targetAssignment);
	MStatus setParameter(const MString& parameterName, const MSamplerState & sampler);

	MStatus setArrayParameter(
		const MString& parameterName,
		const bool* values,
		unsigned int count);
	MStatus setArrayParameter(
		const MString& parameterName,
		const int* values,
		unsigned int count);
	MStatus setArrayParameter(
		const MString& parameterName,
		const float* values,
		unsigned int count);
	MStatus setArrayParameter(
		const MString& parameterName,
		const MMatrix* values,
		unsigned int count);

	MStatus addInputFragment(const MString& fragmentName, const MString& outputName, const MString& inputName, 
		const MString promotedInputName = MString());
	MStatus addOutputFragment(const MString& fragmentName, const MString& inputName);
    MStatus renameParameter(const MString& oldName, const MString& newName);
    MStatus setSemantic(const MString& paramName, const MString& semantic);
    MStatus setAsVarying(const MString& paramName, bool);

	MStatus bind(const MDrawContext& context) const;
	unsigned int getPassCount(const MDrawContext& context, MStatus* status=NULL) const;
	MStatus activatePass(const MDrawContext& context, unsigned int pass) const;
	MStatus unbind(const MDrawContext& context) const;
	MStatus updateParameters(const MDrawContext& context) const;

	bool isTransparent() const;
	MStatus setIsTransparent(bool value);

	MShaderInstance* clone() const;

	MShaderInstance* createShaderInstanceWithColorManagementFragment(const MString& inputColorSpace);

	MStatus requiredVertexBuffers(MVertexBufferDescriptorList& list) const;

	static const char* className();

	int annotationAsInt(const MString& parameterName, const MString& annotationName, MStatus& status);
	float annotationAsFloat(const MString& parameterName, const MString& annotationName, MStatus& status);
	MString annotationAsString(const MString& parameterName, const MString& annotationName, MStatus& status);

	MString parameterSemantic(const MString& parameterName, MStatus& status);

	void* parameterDefaultValue(const MString& parameterName, MStatus& status);

	MString resourceName(const MString& parameterName, MStatus& status);
	MString uiWidget(const MString& parameterName, MStatus& status);
	MString uiName(const MString& parameterName, MStatus& uiName);

	MStatus techniqueNames(MStringArray& techniqueNames) const;
	int techniqueAnnotationAsInt(const MString& annotationName, MStatus& status);
	float techniqueAnnotationAsFloat(const MString& annotationName, MStatus& status);
	MString techniqueAnnotationAsString(const MString& annotationName, MStatus& status);

	int passAnnotationAsInt(unsigned int pass, const MString& annotationName, MStatus& status);
	float passAnnotationAsFloat(unsigned int pass, const MString& annotationName, MStatus& status);
	MString passAnnotationAsString(unsigned int pass, const MString& annotationName, MStatus& status);

	MStatus writeEffectSourceToFile(const MString& filePath) const;

private:
	MShaderInstance(void* data, DrawCallback preCb, DrawCallback postCb);
	~MShaderInstance();

	static void requiredVertexBuffers( MVertexBufferDescriptorList &list, 
										void* format);

	void* fData;
	DrawCallback fPreDrawCallback;
	DrawCallback fPostDrawCallback;

	friend class MShaderManager;
	friend class MPassContext;
	friend class MRenderItem;
	friend class MPxShaderOverride;
};

// ****************************************************************************
// CLASS DECLARATION (MShaderManager)
//! \ingroup OpenMayaRender
//! \brief Provides access to MShaderInstance objects for use in Viewport 2.0.
/*!
This class generates MShaderInstance objects for use with user-created
MRenderItem objects. Any MShaderInstance objects created by this class are
owned by the caller.

The methods for adding shader and shader include paths will allow
for additional search paths to be used with the "getEffectsFileShader()"
method which searches for shader files on disk. The default
search path is in the "Cg" and "HLSL" folders found in the runtime
directory. It is recommended that user defined shaders not reside
at this location to avoid any potential shader conflicts.

When acquiring a shader instance the caller may also specify pre-draw and
post-draw callback functions. These functions will be invoked any time a render
item that uses the shader instance is about to be drawn. These callbacks are
passed an MDrawContext object as well as the render item being drawn and may
alter the draw state in order to get different rendering effects. Accessing
Maya nodes from within these callbacks is an error and may result in
instability.
*/
class OPENMAYARENDER_EXPORT MShaderManager
{
public:
	MStatus addShaderPath(const MString & path) const;
	MStatus shaderPaths(MStringArray & paths) const;
	MStatus addShaderIncludePath(const MString & path) const;
	MStatus shaderIncludePaths(MStringArray & paths) const;

	MStatus clearEffectCache() const;

	MStatus removeEffectFromCache(
		const MString& effectsFileName,
		const MString& techniqueName,
		MShaderCompileMacro* macros = 0,
		unsigned int numberOfMacros = 0) const;

	void getEffectsTechniques(
		const MString& effectsFileName,
		MStringArray& techniqueNames,
		MShaderCompileMacro* macros = 0,
		unsigned int numberOfMacros = 0,
		bool useEffectCache = true) const;

	MShaderInstance* getEffectsFileShader(
		const MString& effectsFileName,
		const MString& techniqueName,
		MShaderCompileMacro* macros = 0,
		unsigned int numberOfMacros = 0,
		bool useEffectCache = true,
		MShaderInstance::DrawCallback preCb = 0,
		MShaderInstance::DrawCallback postCb = 0) const;

	MShaderInstance* getEffectsBufferShader(
		const void* buffer,
		unsigned int size,
		const MString& techniqueName,
		MShaderCompileMacro* macros = 0,
		unsigned int numberOfMacros = 0,
		bool useEffectCache = true,
		MShaderInstance::DrawCallback preCb = 0,
		MShaderInstance::DrawCallback postCb = 0) const;

	MShaderInstance* getFragmentShader(
		const MString& fragmentName,
		const MString& structOutputName,
		bool decorateFragment,
		MShaderInstance::DrawCallback preCb = 0,
		MShaderInstance::DrawCallback postCb = 0) const;

	//! Definition for callback function triggered when a MShaderInstance acquired by calling getShaderFromNode() is no longer valid for that node.
	typedef void (*LinkLostCallback)(MShaderInstance *shaderInstance, MUserData* userData);

	MShaderInstance* getShaderFromNode(
		const MObject& shaderNode,
		const MDagPath& shapePath,
		LinkLostCallback linkLostCb = 0,
		MUserData* linkLostUserData = 0,
		MShaderInstance::DrawCallback preCb = 0,
		MShaderInstance::DrawCallback postCb = 0,
		bool nonTextured = false) const;

	//! Name of an available "stock" shader
	enum MStockShader
	{
		k3dSolidShader,	//!< An instance of a solid color shader for 3d rendering
		k3dBlinnShader,	//!< An instance of a Blinn shader for 3d rendering
		k3dDefaultMaterialShader, //!< An instance of a stock "default material" shader for 3d rendering
        k3dSolidTextureShader,	//!< An instance of a stock solid texture shader for 3d rendering
        k3dCPVFatPointShader,	//!< An instance of a stock color per vertex fat point shader for 3d rendering
		k3dColorLookupFatPointShader, //!< An instance of a stock fat point shader using a 1D color texture lookup. Output is (RGB, 1.0f)
		k3dOpacityLookupFatPointShader, //!< An instance of a stock fat point shader using a 1D color texture lookup. Output is (InsColor, A) where InsColor is a shader parameter.
		k3dColorOpacityLookupFatPointShader, //!< An instance of a stock fat point shader using two 1D color textures lookup. Output is (RGB, A)
		k3dShadowerShader, //!< An instance of a stock shader which can be used when rendering shadow maps
        k3dFatPointShader, //!< An instance of a stock fat point shader for 3d rendering
        k3dThickLineShader, //!< An instance of a stock thick line shader for 3d rendering
        k3dCPVThickLineShader, //!< An instance of a color per vertex stock thick line shader for 3d rendering
        k3dDashLineShader, 	//!< An instance of a stock dash line shader for 3d rendering
        k3dCPVDashLineShader, //!< An instance of a color per vertex stock dash line shader for 3d rendering
		k3dStippleShader, //!< An instance of a stipple shader for drawing 3d filled triangles
		k3dThickDashLineShader,	//!< An instance of a stock thick dash line shader for 3d rendering.
		k3dCPVThickDashLineShader, //!< An instance of a color per vertex stock thick dash line shader for 3d rendering.
		k3dDepthShader, //!< An instance of a stock shader that can be used for 3d rendering of depth
		k3dCPVSolidShader, //!< An instance of a stock solid color per vertex shader for 3d rendering
		k3dIntegerNumericShader, //!< An instance of a stock shader for drawing single integer values per vertex for 3d rendering.
		k3dFloatNumericShader, //!< An instance of a stock shader for drawing single float values values per vertex for 3d rendering.
		k3dFloat2NumericShader, //!< An instance of a stock shader for drawing 2 float values per vertex for 3d rendering.
		k3dFloat3NumericShader, //!< An instance of a stock shader for drawing 3 float values per vertex for 3d rendering.
		k3dPointVectorShader, //!< An instance of a stock shader that can be used for 3d rendering of lines based on a point and a vector stream
		k3dPointLightShadowerShader, //!< An instance of a stock shader which can be used when rendering point light shadow maps

		k3dCPVShader = k3dCPVFatPointShader, // !< Deprecated.
	};

	MShaderInstance* getStockShader(
		MStockShader name,
		MShaderInstance::DrawCallback preCb = 0,
		MShaderInstance::DrawCallback postCb = 0) const;

	void releaseShader(MShaderInstance* shader) const;

	static bool isSupportedShaderSemantic( const MString &value );

	static MString getLastError();
	static MString getLastErrorSource(bool displayLineNumber=false, bool filterSource=false, unsigned int numSurroundingLines=2);

	static const char* className();

private:
	MShaderManager();
	~MShaderManager();

	friend class MRenderer;
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MShaderManager */
