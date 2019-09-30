#ifndef _MGeometryExtractor__
#define _MGeometryExtractor__
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
// CLASS: MGeometryExtractor
//
// ****************************************************************************


#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MHWGeometry.h>
#include <maya/MDagPath.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MDeprecate.h>

// ****************************************************************************
// DECLARATIONS


namespace MHWRender
{
	typedef int MPolyGeomOptions;

	//! Indicates the geometry options for polygon.
	enum {
		kPolyGeom_Normal = 0,				//!< Normal Indicates the polygon performs the default geometry.
		kPolyGeom_NotSharing = 1 << 0,  /*!< NotSharing Indicates if you don't want vertex sharing to be computed by the extractor.
										Vertex buffer size will not be reduced if sharing can be performed.
										*/
		kPolyGeom_BaseMesh = 1 << 1,	/*!< BaseMesh Indicates if you want the base geometry in smoothCage mode.
										The geometry in extractor is always the base geometry in normal mode.
										*/
	};
}

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{
    // ****************************************************************************
    // CLASS DECLARATION (MGeometryExtractor)

    //! \ingroup OpenMayaRender
    //! \brief class for extracting renderable geometry
    /*!
        Construct an instance of this class to populate buffers with vertex and indexing 
        data.  Buffers are filled based on the supplied requirements.  Use the 
        MIndexBufferDescriptor and MVertexBufferDescriptor classes to request a broad range
        of data types, formats, and content.

        Buffers are filled in-place so you should request the primitive count and vertex count 
        before filling the buffers.  Use the count multiplied by the element stride to construct
        data buffers large enough to fill properly prior to calling one of the populate methods.
    */
    class OPENMAYARENDER_EXPORT MGeometryExtractor
    {
     public:

       // initialization
        MGeometryExtractor(const MHWRender::MGeometryRequirements& requirements, 
           const MDagPath& path, ::MHWRender::MPolyGeomOptions options, MStatus* status = NULL);
        ~MGeometryExtractor();

        // Indices
        unsigned int primitiveCount(const MHWRender::MIndexBufferDescriptor& indexDesc) const;
        MStatus populateIndexBuffer( void* data, const unsigned int primitiveCount,
            const MHWRender::MIndexBufferDescriptor& indexDesc) const;

        // Streams
        unsigned int vertexCount( ) const;
        MStatus populateVertexBuffer( void* data, const unsigned int vertexCount,
            const MHWRender::MVertexBufferDescriptor& bufferDesc ) const;

        static unsigned int minimumBufferSize( unsigned int primitiveCount, 
            MGeometry::Primitive primitive, unsigned int primitiveStride=0 );

		 // Deprecated methods
		OPENMAYA_DEPRECATED(2019, "Use MGeometryExtractor(const MGeometryRequirements&, const MDagPath&, const bool, const bool, MStatus*) instead.")
		MGeometryExtractor(const MHWRender::MGeometryRequirements& requirements, 
			const MDagPath& path, const bool sharing = true, MStatus* status = NULL);
    private:
		void MGeometryExtractorConstructHelper(const MHWRender::MGeometryRequirements& requirements, const MDagPath& path,
			::MHWRender::MPolyGeomOptions options, MStatus* status);

        bool isValid() const;

        void* shape;
        void* extractor;
        void* dataPtr; // for future use.
    };

} // namespace MHWRender

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MGeometryExtractor__ */
