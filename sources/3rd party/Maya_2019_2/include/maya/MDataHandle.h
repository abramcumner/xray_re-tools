#ifndef _MDataHandle
#define _MDataHandle
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
// CLASS:    MDataHandle
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <string.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnData.h>
#include <maya/MTypeId.h>
#include <maya/MObject.h>
#include <maya/MDeprecate.h>

// ****************************************************************************
// FORWARD DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MDataHandle)

//! \ingroup OpenMaya
//! \brief Data handle for information contained in a data block. 
/*!

  An MDataHandle is a smart pointer into a data block (MDataBlock).  A
  data handle corresponds to the data for a particular attribute or
  plug.  For array data (eg CVs of a curve) use an MArrayDataHandle.
  To get a data handle, request it from the data block.

  Some simple numeric data is handled directly by the dependency
  graph.  If the data is more complicated (eg surface geometry), then
  the data handle can supply a pointer that should be given to the
  appropriate data function set.

  MDataHandle allows the manipulation and setting of generic
  attributes as created by the MFnGenericAttribute class.  It is
  possible to create numeric generic attributes using two scenarios.
  A plug-in may request that the generic attribute support singleton
  types such as MFnNumericData::kFloat or kChar. Or a plug-in may
  request array types such as MFnNumericData::k2Float k2Double.  In
  the singleton case, getting and setting the attribute using
  MDataHandle is done with the asGeneric*() or setGeneric*() methods.
  In the case of the array types, the MDataHandle.data() method can be
  used to retrieve the MObject for the attribute and to initialize the
  MFnNumericData function set.  A call to MDataHandle::setData() on
  the updated attribute must be made to pass the information back to
  Maya.  NOTE: It is not possible to distinguish the singleton generic
  types from one another.  There is no method that can be called to
  return if the singleton generic attribute is a float, char etc.

  MDataHandle methods cannot be assumed to be threadsafe.

*/
class OPENMAYA_EXPORT MDataHandle {

public:
	MDataHandle();
	bool                 isNumeric() const;
	bool                 isGeneric( bool& isNumeric, bool& isNull ) const;
    MFnNumericData::Type numericType() const;
    MFnData::Type        type() const;
    MTypeId              typeId() const;
	MStatus              acceptedTypeIds( MUintArray &) const;

	MObject              data();
	MStatus				 copy( const MDataHandle& src );
	MStatus				 copyWritable( const MDataHandle& src );

	void                 setClean();

	bool&			     asBool()	const;
	char&			     asChar()	const;
	unsigned char&       asUChar()	const;
	short&			     asShort()	const;
    // Obsolete
    OPENMAYA_DEPRECATED(2019, "Use MDataHandle::asInt instead.")
	int&			     asLong()	const;
	int&			     asInt()	const;
	MInt64&			     asInt64()	const;
    void*&               asAddr()   const;
	float&			     asFloat()	const;
	double&		         asDouble()	const;
    MDistance            asDistance()	const;
    MAngle               asAngle()	const;
    MTime                asTime()	const;

	short2&              asShort2()	const;
    // Obsolete
    OPENMAYA_DEPRECATED(2019, "Use MDataHandle::asInt2 instead.")
	long2&               asLong2()	const;
	int2&				 asInt2()	const;
	float2&              asFloat2()	const;
	double2&             asDouble2()const;
	short3&              asShort3()	const;
    // Obsolete
    OPENMAYA_DEPRECATED(2019, "Use MDataHandle::asInt3 instead.")
	long3&               asLong3()	const;
	int3&                asInt3()	const;
	float3&              asFloat3()  const;
	double3&             asDouble3() const;
	double4&             asDouble4() const;
	MVector&             asVector()  const;
	MFloatVector&        asFloatVector() const;
    MMatrix&             asMatrix()  const;
    MFloatMatrix&        asFloatMatrix()  const;
	MString&             asString() const;

    MObject              asNurbsCurve() const;
    MObject              asNurbsSurface() const;
    MObject              asMesh() const;
    MObject              asSubdSurface() const;
    MObject              asNurbsCurveTransformed() const;
    MObject              asNurbsSurfaceTransformed() const;
    MObject              asMeshTransformed() const;
    MObject              asSubdSurfaceTransformed() const;
    const MMatrix &      geometryTransformMatrix() const;
	MPxData *            asPluginData() const;

BEGIN_NO_SCRIPT_SUPPORT:
    //!	No script support
    void                 set( bool );
    //!	No script support
    void                 set( char );
    //!	No script support
    void                 set( short );
    //!	No script support
    void                 set( int );
    //!	No script support
    void                 set( MInt64 );
    //!	No script support
    void                 set( float );
    //!	No script support
    void                 set( double );
    //!	No script support
    void                 set( const MMatrix& );
    //!	No script support
    void                 set( const MFloatMatrix& );
    //!	No script support
    void                 set( const MVector& );
    //!	No script support
	void                 set( const MFloatVector& );
    //!	No script support
    void                 set( const MDistance& );
    //!	No script support
    void                 set( const MAngle& );
    //!	No script support
    void                 set( const MTime& );
    //!	No script support
	void                 set( short, short );
    //!	No script support
	void                 set( int, int );
    //!	No script support
	void                 set( float, float );
    //!	No script support
	void                 set( double, double );
    //!	No script support
	void                 set( short, short, short );
    //!	No script support
	void                 set( int, int, int );
    //!	No script support
	void                 set( float, float, float );
    //!	No script support
	void                 set( double, double, double );
    //!	No script support
	void                 set( const MString  &);
	//!	No script support
	MStatus              set( const MObject &data );
	//!	No script support
	MStatus              set( MPxData * data );

END_NO_SCRIPT_SUPPORT:

    void                 setBool( bool );
    void                 setChar( char );
    void                 setShort( short );
    void                 setInt( int );
    void                 setInt64( MInt64 );
    void                 setFloat( float );
    void                 setDouble( double );
    void                 setMMatrix( const MMatrix& );
    void                 setMFloatMatrix( const MFloatMatrix& );
    void                 setMVector( const MVector& );
	void                 setMFloatVector( const MFloatVector& );
    void                 setMDistance( const MDistance& );
    void                 setMAngle( const MAngle& );
    void                 setMTime( const MTime& );
	void                 set2Short( short, short );
	void                 set2Int( int, int );
	void                 set2Float( float, float );
	void                 set2Double( double, double );
	void                 set3Short( short, short, short );
	void                 set3Int( int, int, int );
	void                 set3Float( float, float, float );
	void                 set3Double( double, double, double );
	void                 set4Double(double, double, double, double);
	void                 setString( const MString  &);

	MStatus              setMObject( const MObject &data );
	MStatus              setMPxData( MPxData * data );

	bool                 asGenericBool() const;
	unsigned char        asGenericChar() const;
	double               asGenericDouble() const;
	float                asGenericFloat() const;
	short                asGenericShort() const;
	int                  asGenericInt() const;
	MInt64               asGenericInt64() const;

	void                 setGenericBool( bool value, bool force );
	void                 setGenericChar( unsigned char value, bool force );
	void                 setGenericDouble( double value, bool force );
	void                 setGenericFloat( float value, bool force );
	void                 setGenericShort( short value, bool force );
	void                 setGenericInt( int value, bool force );
	void                 setGenericInt64( MInt64 value, bool force );

    MDataHandle          child( const MPlug & plug );
    MDataHandle          child( const MObject & attribute );

	MDataBlock           datablock( MStatus * ReturnStatus = NULL ) const;

	MDataHandle& operator=( const MDataHandle& other );
    MDataHandle( const MDataHandle & );

	static const char*	className();

protected:
// No protected members

private:
	friend class MDataBlock;
	friend class MArrayDataBuilder;
	friend class MArrayDataHandle;
	friend class MItGeometry;
	MDataHandle( void* );

	char	f_data[32];
};

inline MDataHandle::MDataHandle( const MDataHandle &other )
{
	memcpy( this, &other, sizeof(MDataHandle) );
}

inline MDataHandle& MDataHandle::operator=( const MDataHandle& other )
{
	memcpy( this, &other, sizeof(MDataHandle) );
	return *this;
}

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MDataHandle */
