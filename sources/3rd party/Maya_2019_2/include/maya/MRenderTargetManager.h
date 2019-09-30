#ifndef _MRenderTargetManager
#define _MRenderTargetManager
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
#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// DECLARATIONS

// ****************************************************************************
// CLASS DECLARATION (MRenderTargetDescription)
//! \ingroup OpenMayaRender
//!\brief Class which provides a description of a hardware render target
//! The name is the unique identifier for a render target.
//
class OPENMAYARENDER_EXPORT MRenderTargetDescription
{
public:
	MRenderTargetDescription();
	MRenderTargetDescription(const MString &name,
							unsigned int width, unsigned int height,
							unsigned int multiSampleCount,
							::MHWRender::MRasterFormat rasterFormat,
							unsigned int arraySliceCount,
							bool isCubeMap);
	virtual ~MRenderTargetDescription();

	const MString & name() const;
	unsigned int width() const;
	unsigned int height() const;
	unsigned int multiSampleCount() const;
	::MHWRender::MRasterFormat rasterFormat() const;
	unsigned int arraySliceCount() const;
	bool isCubeMap() const;
	bool allowsUnorderedAccess() const; 

	void setName(const MString & name);
	void setWidth(unsigned int val);
	void setHeight(unsigned int val);
	void setMultiSampleCount(unsigned int val);
	void setRasterFormat(::MHWRender::MRasterFormat val);
	void setArraySliceCount(unsigned int val);
	void setIsCubeMap(bool val);
    void setAllowsUnorderedAccess(bool val);
 
	bool compatibleWithDescription( const MRenderTargetDescription & desc );

private:
	MString			mName;
	unsigned int	mWidth;
	unsigned int	mHeight;
	unsigned int	mMultiSampleCount;
	::MHWRender::MRasterFormat mFormat;
	unsigned int	mArraySliceCount;
	bool			mIsCubeMap;
	bool			mAllowsUnorderedAccess;
};

// ****************************************************************************
// CLASS DECLARATION (MRenderTarget)
//! \ingroup OpenMayaRender
//! \brief An instance of a render target that may be used with Viewport 2.0
/*!
This class represents a render target that may be used with the MRenderOperation class
for rendering in Viewport 2.0.
*/
class OPENMAYARENDER_EXPORT MRenderTarget
{
public:
	MStatus updateDescription( const MRenderTargetDescription & targetDescription);
	void targetDescription(MRenderTargetDescription& desc) const;

	static const char* className();

	void * resourceHandle() const;

	void * rawData(int &rowPitch, size_t &slicePitch);

	static void freeRawData(void *data);

private:
	MRenderTarget(void* data, unsigned int *rasterMap, bool );
	~MRenderTarget();

	void* fData;
	unsigned int* fRasterMap;
	bool fIsInternalTarget;

	friend class MRenderer;
	friend class MRenderTargetManager;
	friend class MRenderUtilities;
	friend class MRenderItem;
	friend class MShaderInstance;
    friend class MRenderParameters;
};


// ****************************************************************************
// CLASS DECLARATION (MShaderManager)
//! \ingroup OpenMayaRender
//! \brief Provides access to MRenderTarget objects for use in Viewport 2.0.
/*!
This class generates MRenderTarget objects for use with
MRenderOperation objects. Any MRenderTarget objects created by this class are
owned by the caller.
*/
class OPENMAYARENDER_EXPORT MRenderTargetManager
{
public:
	MRenderTarget* acquireRenderTarget(const MRenderTargetDescription& targetDescription) const;

	MRenderTarget* acquireRenderTargetFromScreen(const MString &targetName) const;

	bool formatSupportsSRGBWrite( ::MHWRender::MRasterFormat format ) const;

	void releaseRenderTarget( const MRenderTarget* target) const;

	static const char* className();

private:
	MRenderTargetManager(unsigned int *rasterMap);
	~MRenderTargetManager();

	MRenderTarget* target( void*, bool ) const ;

	unsigned int* fRasterMap;

	friend class MFrameContext;
	friend class MDrawContext;
	friend class MRenderer;
};

} // namespace MHWRender
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MRenderTargetManager */
