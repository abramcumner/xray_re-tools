#ifndef _MGeometryPrimitive
#define _MGeometryPrimitive
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
// CLASS:    MGeometryPrimitive
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MGeometryData.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MGeometryPrimitive)

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
  \deprecated
  Use MHWRender::MRenderItem and MHWRender::MIndexBuffer instead.

  MGeometryPrimitive is a class describes the topology used for
  accessing MGeometryData.

  Topology is specified as a set of index values which references into
  data elements in an MGeometryData. Index values can be assumed to be
  stored in contiguous memory.

  A "draw primitive type" indicates the format of the indexing as follows:

	\li kPoint : individual unconnected points.
	\li kLine : individual unconnected line segments.
	\li kLineStrip : connected line strip.
	\li kLineLoop : closed line loop.
	\li kTriangle : filled triangle.
	\li kTriangleStrip : filled triangle strip.
	\li kTriangleFan : filled triangle fan
	\li kQuad : filled quadrilateral (quad).
	\li kQuadStrip : filled quad strip.
	\li kPolygon : filled N-sided polygon


  Internal Maya data which is passed to the user via this class is
  always assumed to be non-modifiable. <b>If modified, stability
  cannot be ensured.</b>
*/
class OPENMAYARENDER_EXPORT MGeometryPrimitive
{
public:
	//! Copy constructor
	MGeometryPrimitive( const MGeometryPrimitive& other );

	//! Specifies the data primitive type constructed by the indexing array
	enum DrawPrimitiveType {
		//! Default value is not valid
		kInvalidIndexType = 0,
		//! Corresponds to GL_POINTS in OpenGL
		kPoints,
		//! Corresponds to GL_LINES in OpenGL (individual unconnected line segments)
		kLines,
		//! Corresponds to GL_LINE_STRIP in OpenGL
		kLineStrip,
		//! Corresponds to GL_LINE_LOOP	in OpenGL (non-filled, connected line segments)
		kLineLoop,
		//! Corresponds to GL_TRIANGLES In OpenGL
		kTriangles,
		//! Corresponds to GL_TRIANGLE_STRIP in OpenGL
		kTriangleStrip,
		//! Corresponds to GL_TRIANGLE_FAN in OpenGL
		kTriangleFan,
		//! Corresponds to GL_QUADS in OpenGL
		kQuads,
		//! Corresponds to GL_QUAD_STRIP in OpenGL
		kQuadStrip,
		//! Corresponds to GL_POLYGON in OpenGL
		kPolygon,
		//! Number of primitive types.
		kMaxDrawPrimitiveTypeIndex	// Valid entries are < kMaxDrawModeIndex
	};

	//! Destructor
	~MGeometryPrimitive();

	int				uniqueID() const;
	DrawPrimitiveType drawPrimitiveType() const;
    unsigned int	elementCount() const;
	MGeometryData::ElementType dataType() const;
    const void *	data() const;

protected:
	// Default constructor is protected
	MGeometryPrimitive();
	MGeometryPrimitive(void *);
	//
	// Data set and release is protected
	void set( void* );
	void release();

	friend class MGeometryLegacy;
	friend class MVaryingParameter;
	friend class MHardwareRenderer;

	void *_pGeometryIndex;

private:
// No private members

};
OPENMAYA_NAMESPACE_CLOSE

#endif
#endif // _MGeometryPrimitive
