#ifndef _MHWGeometry
#define _MHWGeometry
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


#include <maya/MTypes.h>
#include <maya/MString.h>
#include <maya/MObject.h>
#include <maya/MColor.h>
#include <maya/MDagPath.h>
#include <maya/MBoundingBox.h>
#include <maya/MDeprecate.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{

// ****************************************************************************
// CLASS DECLARATION (MGeometry)

//! \ingroup OpenMayaRender
//! \brief Class for working with geometric structures used to draw objects
/*!
This class provides a means to specify and access geometric data used to draw
objects in Viewport 2.0.

Objects of type MGeometry cannot be created or destroyed. They are only
accessed through methods on draw override classes such as MPxShaderOverride
or MPxGeometryOverride.

Each MGeometry object contains zero or more vertex buffers and zero or more
index buffers. In read-only scenarios, these buffers describe geometry that
is about to be drawn and can be used to bind custom shaders to the actual
geometry streams on the GPU (see MPxShaderOverride for more details). In the
non-read only scenario, MGeometry can be used to provide vertex and index data
to Maya to use to draw Maya DAG objects with arbitrary shaders (see
MPxGeometryOverride). In all cases the MGeometry instance owns the vertex and
index buffers and manages their lifetimes.
*/
class OPENMAYARENDER_EXPORT MGeometry
{
public:

	//! Specifies data types to use with buffers
	enum DataType {
		//! Invalid element type (default value)
		kInvalidType,
		//! IEEE single precision floating point
		kFloat,
		//! IEEE double precision floating point
		kDouble,
		//! Signed char
		kChar,
		//! Unsigned char
		kUnsignedChar,
		//! Signed 16-bit integer
		kInt16,
		//! Unsigned 16-bit integer
		kUnsignedInt16,
		//! Signed 32-bit integer
		kInt32,
		//! Unsigned 32-bit integer
		kUnsignedInt32
	};
	static const MString& dataTypeString(MGeometry::DataType d);

	//! Specifies the semantic of a data buffer
	enum Semantic {
		//! Invalid data type (default value)
		kInvalidSemantic,
		//! Position vector
		kPosition,
		//! Normal vector
		kNormal,
		//! Texture coordinate tuple
		kTexture,
		//! Color tuple
		kColor,
		//! Tangent vector
		kTangent,
		//! Bi-normal vector
		kBitangent,
		//! Tangent vector with winding order sign
		kTangentWithSign,
	};
	static const MString& semanticString(MGeometry::Semantic s);

	//! Specifies the data primitive type constructed by the indexing array
	enum Primitive
	{
		//! Default value is not valid
		kInvalidPrimitive,
		//! List of points
		kPoints,
		//! List of lines
		kLines,
		//! A line strip
		kLineStrip,
		//! List of triangles
		kTriangles,
		//! A triangle strip
		kTriangleStrip,
        //! A list of triangle with adjacency
        kAdjacentTriangles,
        //! A triangle strip with adjacency
        kAdjacentTriangleStrip,
        //! A list of lines with adjacency
        kAdjacentLines,
        //! A line strip with adjacency
        kAdjacentLineStrip,
        //! A patch
        kPatch,
	};
	static const MString& primitiveString(MGeometry::Primitive p);

	//! Specifies draw mode for render items
	enum DrawMode
	{
		//! Draw in wireframe mode
		kWireframe = (0x1 << 0),
		//! Draw in shaded mode
		kShaded = (0x1 << 1),
		//! Draw in textured mode
		kTextured = (0x1 << 2),
		//! Draw in bounding box mode
		kBoundingBox = (0x1 << 3),
		//! Draw only for selection - not visible in viewport - can be combined with wire/shaded to further restrict draw modes
		kSelectionOnly = (0x1 << 4),
		//! Draw in all visible modes
		kAll = (kWireframe | kShaded | kTextured | kBoundingBox)
	};
	static const MString drawModeString(MGeometry::DrawMode d);

	int vertexBufferCount() const;
	const MVertexBuffer* vertexBuffer(int index) const;
	MVertexBuffer* vertexBuffer(int index);
	MVertexBuffer* createVertexBuffer(const MVertexBufferDescriptor& desc);
	bool addVertexBuffer(MVertexBuffer* buffer);
	bool deleteVertexBuffer(int index);

	int indexBufferCount() const;
	const MIndexBuffer* indexBuffer(int index) const;
	MIndexBuffer* indexBuffer(int index);
	MIndexBuffer* createIndexBuffer(MGeometry::DataType type);
	bool addIndexBuffer(MIndexBuffer* buffer);
	bool deleteIndexBuffer(int index);

	static const char* className();

private:
	MGeometry();
	MGeometry(const MGeometry&) {}
	MGeometry& operator=(const MGeometry&) { return *this; }
	~MGeometry();

	void* fVertexData;
	void* fIndexData;

	friend class MRenderItem;
	friend class MGeometryUtilities;
};


// ****************************************************************************
// CLASS DECLARATION (MVertexBufferDescriptor)
//! \ingroup OpenMayaRender
//! \brief Describes properties of a vertex buffer
/*!
This class gives a complete description of a vertex buffer and is used by
MGeometry, MVertexBuffer and MGeometryRequirements to simplify management of
vertex buffer attributes.

Note that not all combinations of semantics, data types and dimensions are
currently supported. Attempts to create vertex buffers with unsupported
attributes will fail.
*/
class OPENMAYARENDER_EXPORT MVertexBufferDescriptor
{
public:
	MVertexBufferDescriptor();
	MVertexBufferDescriptor(
		const MString& name,
		MGeometry::Semantic semantic,
		MGeometry::DataType dataType,
		int dimension);
	MVertexBufferDescriptor(
		const MString& name,
		MGeometry::Semantic semantic,
		const MString& semanticName,
		MGeometry::DataType dataType,
		int dimension);
	~MVertexBufferDescriptor();

BEGIN_NO_SCRIPT_SUPPORT:
	//! NO SCRIPT SUPPORT
	MVertexBufferDescriptor(const MVertexBufferDescriptor& other);
	//! NO SCRIPT SUPPORT
	MVertexBufferDescriptor& operator=(const MVertexBufferDescriptor& other);
END_NO_SCRIPT_SUPPORT:

	MString name() const;
	void setName(const MString& n);

	MGeometry::Semantic semantic() const;
	void setSemantic(MGeometry::Semantic s);

	MString semanticName() const;
	void setSemanticName(const MString& n);

	MGeometry::DataType dataType() const;
	void setDataType(MGeometry::DataType d);
	unsigned int dataTypeSize() const;

	int dimension() const;
	void setDimension(int d);

	int offset() const;
    void setOffset(int o);

	int stride() const;

	static const char* className();

private:

	MString fName;
	MGeometry::Semantic fSemantic;
	MGeometry::DataType fDataType;
	int fDimension;
	int fOffset;
	int fStride;
	void* fData;

	MString fSemanticName;

	friend class MRenderItem;
	friend class MGeometryRequirements;
	friend class MShaderInstance;
};

// ****************************************************************************
// CLASS DECLARATION (MVertexBufferDescriptorList)
//! \ingroup OpenMayaRender
//! \brief A list of MVertexBufferDescriptor objects
/*!
A list of MVertexBufferDescriptor objects.
*/
class OPENMAYARENDER_EXPORT MVertexBufferDescriptorList
{
public:
	MVertexBufferDescriptorList();
	~MVertexBufferDescriptorList();

	int	length() const;
	bool getDescriptor(int index, MVertexBufferDescriptor& desc) const;

	bool append(const MVertexBufferDescriptor& desc);
	bool removeAt(int index);
	void clear();

	static const char* className();

private:
	MVertexBufferDescriptorList(const MVertexBufferDescriptorList&) {}
	MVertexBufferDescriptorList& operator=(const MVertexBufferDescriptorList&) { return *this; }

	void* fData;
};


// ****************************************************************************
// CLASS DECLARATION (MVertexBuffer)
//! \ingroup OpenMayaRender
//! \brief Vertex buffer for use with MGeometry
/*!
This class represents a vertex buffer with attributes described by the
descriptor member.

When retrieving a vertex buffer for binding to custom shaders
(MPxShaderOverride), resourceHandle() may be called to get the device dependent
handle to the vertex buffer on the GPU.

When creating a vertex buffer to supply geometric data to Maya
(MPxGeometryOverride), acquire() may be called to get a pointer to a block of
memory to fill with said data. Once filled, commit() must be called to apply
the data to the buffer.

When exploring the contents of an already filled vertex buffer, map() may
be called to get a pointer to the read-only block of memory containing the
data. Once done exploring, unmap() must be called to allow the buffer to
be reused.
*/
class OPENMAYARENDER_EXPORT MVertexBuffer
{
public:
	MVertexBuffer(const MVertexBufferDescriptor& descriptor, bool softwareStaged=true);
	MVertexBuffer(const MVertexBuffer&);
	~MVertexBuffer();

	const MVertexBufferDescriptor& descriptor() const;

	void* resourceHandle() const;
    void resourceHandle(void * handle, unsigned int size);

    bool hasCustomResourceHandle() const;

	void lockResourceHandle();
	void unlockResourceHandle();

	void* acquire(unsigned int size, bool writeOnly);
	void commit(void* buffer);

    void* map();
 	void unmap();

    MStatus update(const void* buffer, unsigned int destOffset, unsigned int numVerts, bool truncateIfSmaller);

	void unload();

	unsigned int vertexCount() const;

	static const char* className();

	// Deprecated methods.
	OPENMAYA_DEPRECATED(2019, "Use void* acquire(unsigned int size, bool writeOnly) instead")
	void* acquire(unsigned int size);	// Obsolete use void* acquire(unsigned int size, bool writeOnly) instead

private:

	MVertexBuffer& operator=(const MVertexBuffer&) { return *this; }

    void* getVertexBuffer() const;

	MVertexBufferDescriptor fDescriptor;
	MGeometry* fOwnerMGeometry;

    void* fMayaBuffer;
	void* fHwResource;      // fMayaBuffer->vertexBuffer()->DeviceObject()

    bool fUsingCustomBuffer;
    bool fUsingSharedBuffer;
    int fSharedBufferIdx;
    unsigned int fCustomBufferSize;

	bool fBufferNeedRegistration;
	bool fSoftwareStaged;

	friend class MGeometry;
	friend class MGeometryUtilities;
	friend class MRenderUtilities;
	friend class MRenderItem;
	friend class MPxSubSceneOverride;
	friend class MPxDrawOverride;
};

// ****************************************************************************
// CLASS DECLARATION (MVertexBufferArray)
//! \ingroup OpenMayaRender
//! \brief Array of Vertex buffers
/*!
This class represents an array of vertex buffers.
*/
class OPENMAYARENDER_EXPORT MVertexBufferArray
{
public:
	MVertexBufferArray();
	~MVertexBufferArray();

	unsigned int count() const;

	MVertexBuffer* getBuffer(const MString& name) const;
	MVertexBuffer* getBuffer(unsigned int index) const;
	MString getName(unsigned int index) const;

	MStatus addBuffer(const MString& name, MVertexBuffer* buffer);
	void clear();

	static const char* className();

private:
	void* arr;

};

// ****************************************************************************
// CLASS DECLARATION (MIndexBufferDescriptor)
//! \ingroup OpenMayaRender
//! \brief MIndexBufferDescriptor describes an indexing scheme.
/*!
This class represents a description of an indexing scheme.  It provides the
 indexing type (including custom named types), primitive type,
 primitive stride (for use with patch primitives), and component information.

*/
class OPENMAYARENDER_EXPORT MIndexBufferDescriptor
{
public:

    enum IndexType{
        kVertexPoint = 0,
        kEdgeLine,
        kTriangleEdge,
        kTriangle,
        kFaceCenter,
        kEditPoint,
        kControlVertex,
        kHullEdgeLine,
        kHullTriangle,
        kHullFaceCenter,
        kHullEdgeCenter,
        kHullUV,
        kSubDivEdge,
        kTangent,
        kCustom, // use the name if usage is Custom
    };

    MIndexBufferDescriptor();
    MIndexBufferDescriptor(MIndexBufferDescriptor::IndexType type, const MString& name,
        MGeometry::Primitive primitive, unsigned int primitiveStride = 0,
        MObject component = MObject::kNullObj,
        MGeometry::DataType dataType = MGeometry::kUnsignedInt32);

    ~MIndexBufferDescriptor();

    MString name() const;
    void setName(MString& val);

    MIndexBufferDescriptor::IndexType indexType() const;
    void setIndexType(MIndexBufferDescriptor::IndexType val);

    MGeometry::Primitive primitive() const;
    void setPrimitive(MGeometry::Primitive val);

    unsigned int primitiveStride() const;
    void setPrimitiveStride(unsigned int id);

    MObject component() const;
    void setComponent(MObject component);

    MGeometry::DataType dataType() const;
    void dataType(MGeometry::DataType dataType);

private:

    MIndexBufferDescriptor::IndexType fType;
    MString fName;
    MGeometry::Primitive fPrimitive;
    unsigned int fPatchStride;
    MObject fComponent;
    MGeometry::DataType fDataType;
    void* fData;
};

// ****************************************************************************
// CLASS DECLARATION (MIndexBufferDescriptorList)
//! \ingroup OpenMayaRender
//! \brief A list of MIndexBufferDescriptor objects
/*!
A list of MIndexBufferDescriptor objects.
*/
class OPENMAYARENDER_EXPORT MIndexBufferDescriptorList
{
public:
    MIndexBufferDescriptorList();
    ~MIndexBufferDescriptorList();

    int	length() const;
    bool getDescriptor(int index, MIndexBufferDescriptor& desc) const;

    bool append(const MIndexBufferDescriptor& desc);
    bool removeAt(int index);
    void clear();

    static const char* className();

private:
    MIndexBufferDescriptorList(const MIndexBufferDescriptorList&) {}
    MIndexBufferDescriptorList& operator=(const MIndexBufferDescriptorList&) { return *this; }

    void* fData;
};

// ****************************************************************************
// CLASS DECLARATION (MIndexBuffer)
//! \ingroup OpenMayaRender
//! \brief Index buffer for use with MGeometry
/*!
This class represents an index buffer with a specific data type.

When retrieving an index buffer for binding to custom shaders
(MPxShaderOverride), resourceHandle() may be called to get the device dependent
handle to the index buffer on the GPU.

When creating an index buffer to supply geometric data to Maya
(MPxGeometryOverride), acquire() may be called to get a pointer to a block of
memory to fill with said data. Once filled, commit() must be called to apply
the data to the buffer.
*/
class OPENMAYARENDER_EXPORT MIndexBuffer
{
public:
	MIndexBuffer(MGeometry::DataType type, bool softwareStaged = true);
	MIndexBuffer(const MIndexBuffer&);
	~MIndexBuffer();

	MGeometry::DataType dataType() const;
	unsigned int size() const;

	void* resourceHandle() const;
    void resourceHandle(void * handle, unsigned int size);

    bool hasCustomResourceHandle() const;

	void lockResourceHandle();
	void unlockResourceHandle();

	void* acquire(unsigned int size, bool writeOnly);
	void commit(void* buffer);

    void* map();
 	void unmap();

    MStatus update(const void* buffer, unsigned int destOffset, unsigned int numIndices, bool truncateIfSmaller);

	void unload();

	static const char* className();

	// Deprecated methods.
	void* acquire(unsigned int size);	// Obsolete use void* acquire(unsigned int size, bool writeOnly) instead

private:
	MIndexBuffer& operator=(const MIndexBuffer&) { return *this; }

    void* getIndexBuffer() const;
    void setIndexBuffer(void* ib);

	MGeometry::DataType fDataType;
	MGeometry* fOwnerMGeometry;
	void* fHwResource;
    void* fMayaBuffer;

    bool fUsingCustomBuffer;
    unsigned int fCustomBufferSize;

	bool fBufferNeedRegistration;
	bool fSoftwareStaged;

	friend class MGeometry;
	friend class MGeometryUtilities;
	friend class MRenderUtilities;
	friend class MRenderItem;
	friend class MPxSubSceneOverride;
	friend class MPxDrawOverride;
};

// ****************************************************************************
// CLASS DECLARATION (MGeometryIndexMapping)
//! \ingroup OpenMayaRender
//! \brief A mapping of geometry index
/*!
This class represents the index mapping of the multiple objects concatenated
into a single render item.

When multiple objects are compatible, their geometry can be consolidated into a single
render item, to provide better performance by concatening their index and vertex buffers.

The Geometry Index Mapping can be used to get the indexing details of the
concatenated geometries to be able to render them separately.
*/
class OPENMAYARENDER_EXPORT MGeometryIndexMapping
{
public:
	MGeometryIndexMapping();
	~MGeometryIndexMapping();

	int geometryCount() const;

	MDagPath dagPath(int geometryIdx) const;
	MObject component(int geometryIdx) const;
	int indexStart(int geometryIdx) const;
	int indexLength(int geometryIdx) const;

private:
	void* fData;

	void appendGeometry(void *geometryData);
	friend class MRenderItem;
};

// ****************************************************************************
// CLASS DECLARATION (MRenderItem)
//! \ingroup OpenMayaRender
//! \brief A single renderable entity
/*!
MRenderItem represents a single renderable entity. Each such entity is defined
by a name, a render item type and a primitive type. The render item type affects
how Maya treats the render item under various contexts.

A list of MRenderItem objects is passed to the draw method of implementations
of MPxShaderOverride to indicate the objects that should be drawn immediately
by the override. If the implementation wishes to manually bind data streams it
can call MRenderItem::geometry() to access the vertex buffers and index buffers
that have already been generate for the render item.

MRenderItem objects are used by implementations of MPxGeometryOverride for two
purposes. First, MPxGeometryOverride::updateRenderItems() can create and add new
render items to the list of items that will be drawn for a specific instance
of a DAG object. This method can also enable/disable render items in the list
and set a custom solid color shader on any user defined items. Later, a list
of render items is provided to MPxGeometryOverride::populateGeometry(), to
indicate index buffers that need to be generated. This list contains both the
automatically generated render items Maya creates for shader assignments as
well as the render items created in MPxGeometryOverride::updateRenderItems().

MRenderItem objects are also used by MPxSubSceneOverride for specifying all
information about all drawable objects produced by the override.
*/
class OPENMAYARENDER_EXPORT MRenderItem
{
public:
	//! Dormant filled primitive depth priority.
	static unsigned int sDormantFilledDepthPriority;
	//! Dormant wireframe primitive depth priority
	static unsigned int sDormantWireDepthPriority;
	//! Hilite wireframe primitive depth priority
	static unsigned int sHiliteWireDepthPriority;
	//! Active wireframe primitive depth priority
	static unsigned int sActiveWireDepthPriority;
	//! Active line / edge depth priority
	static unsigned int sActiveLineDepthPriority;
	//! Dormant point depth priority
	static unsigned int sDormantPointDepthPriority;
	//! Active point depth priority
	static unsigned int sActivePointDepthPriority;
	//! Selection depth priority
	static unsigned int sSelectionDepthPriority;

	//! Type descriptor for render items which drives how Maya treats them in the render pipeline.
	enum RenderItemType
	{
		/*!
		A render item which represents an object in the scene that should
		interact with the rest of the scene and viewport settings (e.g. a shaded
		piece of geometry which should be considered in processes like shadow
		computation, viewport effects, etc.). Inclusion in such processes can
		also still be controlled through the appropriate methods provided by
		this class.
		*/
		MaterialSceneItem,
		/*!
		A render item which represents an object in the scene that should not
		interact with the rest of the scene and viewport settings, but that is
		also not part of the viewport UI (e.g. a curve or a bounding box which
		should not be hidden when 'Viewport UI' is hidden but which should also
		not participate in processes like shadow computation, viewport effects,
		etc.).
		*/
		NonMaterialSceneItem,
		/*!
		A render item which should be considered to be part of the viewport UI
		(e.g. selection wireframe, components, etc.).
		*/
		DecorationItem,
		/*!
		A render item which was created by Maya for internal purposes (e.g. A
		render item created as the result of a shader being assigned to a DAG
		node).
		*/
		InternalItem,
		/*!
		An internally created MaterialSceneItem for non-textured mode display.
		*/
		InternalMaterialItem,
		/*!
		An internally created MaterialSceneItem for textured mode display.
		*/
		InternalTexturedMaterialItem,
		/*!
		An internally created MaterialSceneItem for showing an unsupported 
		material.
		*/
		InternalUnsupportedMaterialItem,
		/*!
		A render item which was created by Maya for internal purposes and has been
		assigned a user defined effect (see MPxShaderOverride::overridesNonMaterialItems)
		*/
		OverrideNonMaterialItem
	};

	//! Definition for callback function triggered when shader instance assigned by setShaderFromNode() is no longer valid.
	typedef void (*LinkLostCallback)(MUserData* userData);

	static MRenderItem* Create(
		const MString& name,
		RenderItemType type,
		MGeometry::Primitive primitive);
	static MRenderItem* Create(
		const MString& name,
		RenderItemType type,
		MGeometry::Primitive primitive,
		int stride);
	static MRenderItem* Create(const MRenderItem& item);
	static void Destroy(MRenderItem*& item);

	void setDrawMode(MGeometry::DrawMode mode);
	bool setMatrix(const MMatrix* mat);
	bool setShader(
		const MShaderInstance* shader,
		const MString* customStreamName=NULL);
	bool setShaderFromNode(
		const MObject& shaderNode,
		const MDagPath& shapePath,
		LinkLostCallback linkLostCb = NULL,
		MUserData* linkLostUserData = NULL,
		bool nonTextured = false);
	void setExcludedFromPostEffects(bool exclude);
	void setSupportsAdvancedTransparency(bool support);
	void setCustomData(MUserData* userData);
    void enable(bool on);
    void castsShadows(bool on);
    void receivesShadows(bool on);
	void depthPriority( unsigned int val );

	bool isShaderFromNode() const;
	MShaderInstance* getShader();
	const MShaderInstance* getShader() const;
	bool associateWithIndexBuffer(const MIndexBuffer* buffer) const;
	bool excludedFromPostEffects() const;
	bool supportsAdvancedTransparency() const;
	MUserData* customData() const;
	bool isEnabled() const;
    bool castsShadows() const;
    bool receivesShadows() const;
	unsigned int depthPriority() const;
	MString name() const;
	RenderItemType type() const;
	MGeometry::Primitive primitive() const;
	MGeometry::Primitive primitive(int &stride) const;
	MGeometry::DrawMode drawMode() const;
	MDagPath sourceDagPath() const;
	MObject component() const;
	MObject shadingComponent() const;

	void setAllowIsolateSelectCopy(bool state);
	bool allowIsolateSelectCopy() const;
	bool isIsolateSelectCopy() const;

	const MVertexBufferDescriptorList& requiredVertexBuffers() const;
	const MGeometry* geometry() const;
	MBoundingBox boundingBox(MSpace::Space space = MSpace::kObject, MStatus* ReturnStatus = NULL) const;
	bool isConsolidated() const;
    bool isMultiDraw() const;
	void sourceIndexMapping(MGeometryIndexMapping& geometryIndexMapping) const;

	bool wantConsolidation() const;
	void setWantConsolidation(bool state);

	bool isCompatibleWithMayaInstancer() const;
	void setCompatibleWithMayaInstancer(bool state);

	void setTreatAsTransparent(bool val);

	MSelectionMask selectionMask() const;
	void setSelectionMask(const MSelectionMask& mask);

	unsigned int	availableShaderParameters(MStringArray& parameterNames, MStatus* ReturnStatus = NULL) const;
	bool 			getShaderBoolParameter(const MString& parameterName, MStatus* ReturnStatus = NULL) const;
	int 			getShaderIntParameter(const MString& parameterName, MStatus* ReturnStatus = NULL) const;
	float 			getShaderFloatParameter(const MString& parameterName, MStatus* ReturnStatus = NULL) const;
	const float* 	getShaderFloatArrayParameter(const MString& parameterName, unsigned int &size, MStatus* ReturnStatus = NULL) const;

	static const char* className();

	// Deprecated
    OPENMAYA_DEPRECATED(2019, "Please use the other MRenderItem::Create() method now.")
	static MRenderItem* Create(
		const MString& name,
		MGeometry::Primitive primitive,
		MGeometry::DrawMode mode,
		bool raiseAboveShaded);

	OPENMAYA_DEPRECATED(2019, "Please use MRenderItem::wantConsolidation().")
	bool wantSubSceneConsolidation() const;

	OPENMAYA_DEPRECATED(2019, "Please use MRenderItem::setWantConsolidation().")
	void setWantSubSceneConsolidation(bool state);

private:
	MRenderItem(
		const MString& name,
		RenderItemType type,
		MGeometry::Primitive primitive,
		int stride);
	~MRenderItem();
	MRenderItem();
	MRenderItem(const MRenderItem&);
	MRenderItem& operator=(const MRenderItem&) { return *this; }
	void* getOwnedData();
	void clearOwnedData();

	const void* fData;
	void* fOwnedData;
	const void* fIndexBuffer;
    void* fDrawContext;
    void* fShaderData;
	MVertexBufferDescriptorList fReqDescList;

	friend class MGeometryRequirements;
	friend class MDrawContext;
	friend class MSubSceneContainer;
	friend class MPxSubSceneOverride;
	friend class MPxDrawOverride;
};


// ****************************************************************************
// CLASS DECLARATION (MRenderItemList)
//! \ingroup OpenMayaRender
//! \brief A list of MRenderItem objects
/*!
A list of MRenderItem objects. All items in the list are owned by the list.
This class cannot be created or destroyed, it is only passed to the user
through various interfaces.
*/
class OPENMAYARENDER_EXPORT MRenderItemList
{
public:
	int	length() const;
	int indexOf(const MString& name) const;
	int indexOf(const MString& name, MRenderItem::RenderItemType type) const;
	int indexOf(
		const MString& name,
		MGeometry::Primitive primitive,
		MGeometry::DrawMode mode) const;
	const MRenderItem* itemAt(int index) const;

	MRenderItem* itemAt(int index);
	bool append(MRenderItem* item);
	bool removeAt(int index);
	void clear();

	static const char* className();

private:
	MRenderItemList();
	MRenderItemList(const MRenderItemList&) {}
	MRenderItemList& operator=(const MRenderItemList&) { return *this; }
	~MRenderItemList();

	void* fData;

	friend class MGeometryRequirements;
};


// ****************************************************************************
// CLASS DECLARATION (MGeometryRequirements)
//! \ingroup OpenMayaRender
//! \brief Geometry requirements
/*!
This class is used to describe the geometry requirements to be fulfilled by an
implementation of MPxGeometryOverride. It represents the union of all
vertex requirements from all shaders assigned to the object being evaluated by
the MPxGeometryOverride instance.

There are two parts to the requirements.
First, the vertex requirements are accessed through the vertexRequirements()
method. For each such requirement, implementations of MPxGeometryOverride
should create a vertex buffer on the MGeometry instance which matches the
vertex buffer descriptor from the requirement and fill it with data.
Second, the indexing requirements are accessed through the indexingRequirements()
method.  This list is populated when making geometry requests using classes like
MGeometryExtractor.
*/
class OPENMAYARENDER_EXPORT MGeometryRequirements
{
public:
    MGeometryRequirements();
    ~MGeometryRequirements();

    void addVertexRequirement(const MVertexBufferDescriptor& descriptor);
    void addIndexingRequirement(const MIndexBufferDescriptor& descriptor);

	const MVertexBufferDescriptorList& vertexRequirements() const;
	const MIndexBufferDescriptorList& indexingRequirements() const;

	static const char* className();

private:

	void setVertexRequirements(void* requirements);

    MVertexBufferDescriptorList fVertexRequirements;
    MIndexBufferDescriptorList fIndexRequirements;

	friend class MRenderItem;
	friend class MShaderInstance;
};

} // namespace MHWRender

// ****************************************************************************
OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MHWGeometry */

