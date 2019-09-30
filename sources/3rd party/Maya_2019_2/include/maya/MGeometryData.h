#ifndef _MGeometryData
#define _MGeometryData
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
// CLASS:    MGeometryData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MGeometryData)

//! \ingroup OpenMayaRender
//! \brief \obsolete
/*!
  \deprecated
  Use MHWRender::MVertexBuffer and MHWRender::MIndexBuffer instead.

  This class allows storage of arbitrary data which is formated to be
  specifically suitable for usage using a 3D display interface such as
  OpenGL.

  Format options include:

  <ul>
  <li> Currently, each element can be of a 1, 2, 3, 4 tuple.
  <li> Element type can be IEEE single float or double float,
  signed or unsigned byte (character), 16 or 32 bit signed unsigned integers.
  <li> There are specific "type" identifiers to provide a semantic on
  for the data. These include:

  <ul>
	<li> position : vertex position vector
	<li> normal : vertex normal vector
	<li> texCoord : vertextexture coordinate vector
	<li> color : vertex color vector
	<li> weight : vertex skin weight vector
	<li> tangent : vertex tangent vector
	<li> binormal : vertex binormal vector
	<li> velocity : vertex velocity vector
	<li> primitiveCenter : center a primitive (e.g. a triangle)
	<li> colorMask : colour existance mask. 1:1 correspondance with color
	<li> useData : some user defined data. A semantic that Maya does
	not understand.
   </ul>
 </ul>


Currently Maya only interprets a fixed format subset for data with
recongnized semantics, This does not mean that the user cannot create
any arbitrary format for data storage. Support formats with semantics
includes:

  \li 3 float position interpreted as (x,y,z).
  \li 3 float normal interpreted as (x,y,z).
  \li 2 float texture coordinate. Coorindate is interpreted as a (U,V) tuple.
  \li 4 float color. Color is interpreted as (Red, Green, Blue, Alpha) tuple.
  \li 3 float tangent interpreted as (x,y,z).
  \li 3 float binormal interpreted as (x,y,z).
  \li 3 float velocity interpreted as (x,y,z).
  \li 3 float primitive center position interpreted as (x,y,z).
  \li 1 float color mask interpreted as 1=mapped, and 0 = unmapped.

Memory allocation of the correct size is left up to the user of this class.

Memory can be marked as "owned" by this class or the user of this
class. Ownership by this class is the default behaviour specified in
the constructor.  If the data is marked as being owned by the class,
it is assumed that the data is created using a "new" operation, as the
destructor of this class will use a "delete" operation to free memory.

Internal Maya data which is passed to the user via this class is
always assumed to be non-modifiable. <b>If modified, stability cannot
be ensured.</b>
*/

class OPENMAYARENDER_EXPORT MGeometryData
{
public:
	//! Specifies the size or dimension of each data element of the storage.
	enum ElementSize {
		//! Invalid element size
		kInvalidElementSize = 0,
		//! Single value
		kOne = 1,
		//! 2-tuple
		kTwo,
		//! 3-tuple
		kThree,
		//! 4-tuple
		kFour
	};

	//! Specifies the data type of each data element of the storage.
	enum ElementType {
		//! Invalid element type (default value)
		kInvalidElementType = -1,
		//! IEEE single precision floating point
		kFloat = 0,
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

	//! Specifies the data type of the storage array.
	enum DataType {
		//! Invalid data type (default value)
		kInvalidDataType = 0,
		//! Position vector
		kPosition,
		//! Normal vector
		kNormal,
		//! Texture coordinate tuple
		kTexCoord,
		//! Color tuple
		kColor,
		//! Vertex weighting data
		kWeight,
		//! Separator to indicate native draw API supported types
		kAPISupported,
		//! Tangent vector
		kTangent,
		//! Bi-normal vector
		kBiNormal,
		//! Velocity vector
		kVelocity,
		//! Center of primitive
		kPrimitiveCenter,
		//! Mapped, unmapped color mask
		kColorMask,
		//! Arbitrary "user data"
		kUserData,
		//! Valid entries are < kMaxDataTypeIndex
		kMaxDataTypeIndex
	};

	MGeometryData(
		const char *	dataName,
        DataType		dataType,
        ElementType		elementType,
        ElementSize		elementSize,
        unsigned int	elementCount,
        void*			dataPtr = NULL,
		bool			ownsData = true);

	MGeometryData(const MGeometryData&);

	~MGeometryData();

	const char *	objectName() const;
	int				uniqueID() const;
    DataType 		dataType() const;
    ElementType		elementType() const;
	unsigned int	elementTypeSize() const;
    ElementSize		elementSize() const;
    unsigned int	elementCount() const;
    void *	data() const;
    void			setCollectionNumber(int cn);
    int				collectionNumber() const;
	void			setObjectOwnsData(bool val);
	bool			objectOwnsData() const;

protected:
	// Default constructor is protected
	MGeometryData();
	MGeometryData(void *);
	//
	// Data destructor is protected
	void release();

	friend class MGeometryLegacy;
	friend class MHardwareRenderer;
	friend class MD3D9Renderer;

	void *_pGeometryData;

private:
// No private members

};
OPENMAYA_NAMESPACE_CLOSE

#endif
#endif // _MGeometryData
