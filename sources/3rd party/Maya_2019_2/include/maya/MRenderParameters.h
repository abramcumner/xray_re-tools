#ifndef _MRenderParameters_h
#define _MRenderParameters_h
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
// CLASS:    MHWRender::MRenderer
//
// ****************************************************************************
//
// CLASS DESCRIPTION (MHWRender::MRenderer)
//
//  MHWRender::MRenderer is the main interface class to the renderer
//	which is used for rendering interactive viewports in "Viewport 2.0" mode
//	as well as for rendering with the "Maya Hardware 2.0" batch renderer.
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MDagPath.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>
#include <maya/MFloatPoint.h>
#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>
#include <maya/MIntArray.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE
namespace MHWRender
{

// ****************************************************************************
// CLASS DECLARATION (MRenderParameters)
//! \ingroup OpenMayaRender
//! \brief Class which defines a set of render parameters
//
class OPENMAYARENDER_EXPORT MRenderParameters
{
public:
    MRenderParameters();
    MRenderParameters(void*);
    MRenderParameters(const MRenderParameters& copy);
    virtual ~MRenderParameters();

    MRenderParameters& operator=(const MRenderParameters& other);

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
        kSampler,
        //! Render target
        kRenderTarget,
        //! String
        kString
    };

    void parameterList(MStringArray& list) const;
    ParameterType parameterType(const MString& parameterName) const;
    bool isArrayParameter(const MString& parameterName) const;
    MString semantic(const MString& parameterName) const;

    // set parameter values by type
    MStatus setParameter(const MString& parameterName, bool value);
    MStatus setParameter(const MString& parameterName, int value);
    MStatus setParameter(const MString& parameterName, float value);
    MStatus setParameter(const MString& parameterName, const float* value);
    MStatus setParameter(const MString& parameterName, const MFloatVector& value);
    MStatus setParameter(const MString& parameterName, const MMatrix& value);
    MStatus setParameter(const MString& parameterName, const MFloatMatrix& value);
    MStatus setParameter(const MString& parameterName, MTextureAssignment& textureAssignment);
    MStatus setParameter(const MString& parameterName, MRenderTargetAssignment& targetAssignment);
    MStatus setParameter(const MString& parameterName, const MSamplerState& sampler);
    MStatus setParameter(const MString& parameterName, const MString& str);

    // set array parameter values by type
    MStatus setArrayParameter(const MString& parameterName, const bool* values, unsigned int count);
    MStatus setArrayParameter(const MString& parameterName, const int* values, unsigned int count);
    MStatus setArrayParameter(const MString& parameterName, const float* values, unsigned int count);
    MStatus setArrayParameter(const MString& parameterName, const MMatrix* values, unsigned int count);
    MStatus setArrayParameter(const MString& parameterName, const MString* values, unsigned int count);

    // get parameter values by type
    MStatus getParameter(const MString& parameterName, bool& value);
    MStatus getParameter(const MString& parameterName, int& value);
    MStatus getParameter(const MString& parameterName, float& value);
    MStatus getParameter(const MString& parameterName, float*& value);
    MStatus getParameter(const MString& parameterName, MFloatVector& value);
    MStatus getParameter(const MString& parameterName, MMatrix& value);
    MStatus getParameter(const MString& parameterName, MFloatMatrix& value);
    MStatus getParameter(const MString& parameterName, MTextureAssignment& textureAssignment);
    MStatus getParameter(const MString& parameterName, MRenderTargetAssignment& targetAssignment);
    MStatus getParameter(const MString& parameterName, MSamplerStateDesc& samplerDesc);
    MStatus getParameter(const MString& parameterName, MString& str);

    // get array parameter values by type
    MStatus getParameter(const MString& parameterName, MIntArray& values);
    MStatus getParameter(const MString& parameterName, MFloatArray& values);
    MStatus getParameter(const MString& parameterName, MMatrixArray& values);
    MStatus getParameter(const MString& parameterName, MStringArray& values);

private:

    void* mData;
};

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderParameters_h */
