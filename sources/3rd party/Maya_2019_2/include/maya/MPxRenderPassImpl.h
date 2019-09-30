#ifndef _MPxRenderPassImpl
#define _MPxRenderPassImpl

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
// CLASS:    MPxRenderPassImpl
//
// ****************************************************************************

#if defined __cplusplus


#include <maya/MString.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MRenderPassRegistry)

//! \ingroup OpenMayaRender
//! \brief Render pass implementation
/*!
	The MPxRenderPassImpl class is an abstract base class which provides a
	common interface for pass implementations. Extend this class to create
	custom pass implementations.
*/

class OPENMAYA_EXPORT MPxRenderPassImpl
{
public:

	//! Data types that implementation may support
	enum PassTypeBit
	{
		//! Unsigned 8-bit integer
		kUInt8 = 0x1,
		//! Unsigned 16-bit integer
		kUInt16 = 0x2,
		//! Unsigned 32-bit integer
		kUInt32 = 0x4,
		//! Unsigned 64-bit integer
		kUInt64 = 0x8,
		//! Signed 8-bit integer
		kInt8 = 0x10,
		//! Signed 16-bit integer
		kInt16 = 0x20,
		//! Signed 32-bit integer
		kInt32 = 0x40,
		//! Signed 64-bit integer
		kInt64 = 0x80,
		//! half precision floating point
		kFloat16 = 0x100,
		//! single precision floating point
		kFloat32 = 0x200,
		//! double precision floating point
		kFloat64 = 0x400,
		//! single bit
		kBit = 0x800,
		//! undefined type
		kOther = 0x1000
	};

    //! Description of the nature of the data stored in the frame buffer
	enum PassSemantic {
		//! Invalid
		kInvalidSemantic = 0,
		//! RGB, RGBA, alpha, luminance, etc.
		kColorSemantic,
		//! General vector data (e.g. motion vectors, surface displacements)
		kVectorSemantic,
		//! Vectors that represent a direction (e.g. surface normals, light directions)
		kDirectionVectorSemantic,
		//! Depth or range data
		kDepthSemantic,
		//! Labels, such as material tags or object tags.
		kLabelSemantic,
		//! Pixel mask (e.g. a stencil buffer)
		kMaskSemantic,
		//! Undefined, unknown or custom semantic
		kOtherSemantic
	};

	//! Use to create a mask of PassTypeBit types
	typedef unsigned int PassTypeMask;

	MPxRenderPassImpl();
	virtual ~MPxRenderPassImpl();

	// Virtual overrides
	virtual bool isCompatible(const MString &renderer) const = 0;
	virtual PassTypeMask typesSupported() const = 0;
	virtual PassTypeBit getDefaultType() const = 0;
	virtual unsigned int getNumChannels() const = 0;
	virtual PassSemantic frameBufferSemantic() const = 0;
	virtual bool perLightPassContributionSupported() const = 0;

protected:
// No protected members

private:
	friend class MRenderPassDef;

	void* fInstance;
};

// ****************************************************************************

OPENMAYA_NAMESPACE_CLOSE

#endif/* __cplusplus */
#endif/* _MPxRenderPassImpl */
