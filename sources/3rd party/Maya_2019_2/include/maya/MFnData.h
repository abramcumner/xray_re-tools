#ifndef _MFnData
#define _MFnData
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
// CLASS:    MFnData
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnBase.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnData)

//! \ingroup MFn OpenMaya
//! Parent class for dependency graph data function sets.
/*!
   MFnData is the parent class for all dependency graph data function
   sets.  Conceptually, data objects are what flow through the connections
   in the dependency graph.

   Each node in the dependency graph has a data block associated with it.
   The data block holds the data objects for all of the node's attributes
   (see MDataBlock).  The data block is only available during the compute
   method of a node.  A data handle (MDataHandle) can be created to access
   a particular attribute's data inside of the data block.

   Once a data handle is obtained, it is possible to get the data object
   from it.  This is only necessary for heavier data structures such as
   geometry or strings.  Simple numeric data can be manipulated straight
   through the data handle.

   Note: using kDynSweptGeometry data requires that a plug-in or stand
   alone API application be linked with the OpenMayaFX library.
*/
class OPENMAYA_EXPORT MFnData : public MFnBase
{
	declareMFnNoVoidConstructor( MFnData, MFnBase );

public:
	MFnData();

	//! Types of dependency graph data
	enum Type {
        //! Invalid value
		kInvalid,
        //! Numeric, use MFnNumericData extract the node data.
		kNumeric,
		//! Plugin Blind Data, use MFnPluginData to extract the node data.
		kPlugin,
        //! Plugin Geometry, use MFnGeometryData to extract the node data.
		kPluginGeometry,
        //! String, use MFnStringData to extract the node data.
		kString,
        //! Matrix, use MFnMatrixData to extract the node data.
		kMatrix,
        //! String Array, use MFnStringArrayData to extract the node data.
		kStringArray,
        //! Double Array, use MFnDoubleArrayData to extract the node data.
		kDoubleArray,
        //! Float Array, use MFnFloatArrayData to extract the node data.
		kFloatArray,
        //! Int Array, use MFnIntArrayData to extract the node data.
		kIntArray,
        //! Point Array, use MFnPointArrayData to extract the node data.
		kPointArray,
        //! Vector Array, use MFnVectorArrayData to extract the node data.
		kVectorArray,
        //! Matrix Array, use MFnMatrixArrayData to extract the node data.
		kMatrixArray,
        //! Component List, use MFnComponentListData to extract the node data.
		kComponentList,
        //! Mesh, use MFnMeshData to extract the node data.
		kMesh,
        //! Lattice, use MFnLatticeData to extract the node data.
		kLattice,
        //! Nurbs Curve, use MFnNurbsCurveData to extract the node data.
		kNurbsCurve,
        //! Nurbs Surface, use MFnNurbsSurfaceData to extract the node data.
		kNurbsSurface,
        //! Sphere, use MFnSphereData to extract the node data.
		kSphere,
        //! ArrayAttrs, use MFnArrayAttrsData to extract the node data.
		kDynArrayAttrs,
        /*! SweptGeometry, use MFnDynSweptGeometryData to extract the
		  node data. This data node is in OpenMayaFX which must be
		  linked to.
		*/
		kDynSweptGeometry,
        //! Subdivision Surface, use MFnSubdData to extract the node data.
		kSubdSurface,
        //! nObject data, use MFnNObjectData to extract node data
		kNObject,
        //! nId data, use MFnNIdData to extract node data
		kNId,
        //! Typically used when the data can be one of several types.
        kAny,
		/*! Last value. It does not represent real data, but can be
		  used to loop on all possible types
		*/
		kLast
	};

BEGIN_NO_SCRIPT_SUPPORT:

	declareMFnNoVoidConstructorConstConstructor( MFnData, MFnBase );

END_NO_SCRIPT_SUPPORT:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnData */
