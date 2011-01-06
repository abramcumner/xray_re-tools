/**********************************************************************
 *<
	FILE:			ObjectWrapper.h
	DESCRIPTION:	Geometry object abstraction layer
	CREATED BY:		Michaelson Britt
	HISTORY:		Created Friday, April 18, 2003

 *>	Copyright (c) 2003, All Rights Reserved.
 **********************************************************************/


#ifndef __OBJECTWRAPPER__H
#define __OBJECTWRAPPER__H

#include "maxheap.h"
#include "triobj.h"
#include "polyobj.h"
#include "meshadj.h"
#include "patchobj.h"

class HitListWrapper;

typedef Tab<float> FloatTab;

//-----------------------------------------------------------------------------
// struct GenFace
// Represents a "face" for any type of object

struct GenFace: public MaxHeapOperators {
	unsigned short numVerts; 
	DWORD* verts;
};

struct GenEdge: public MaxHeapOperators {
	DWORD f[2];
	DWORD v[2];
};

struct GenSoftSelData: public MaxHeapOperators {
	BOOL useSoftSel; //whether soft selection is active
	BOOL useEdgeDist;
	int edgeIts;
	BOOL ignoreBack;
	float falloff, pinch, bubble;
	//float* weights; //the soft selection values
	CoreExport GenSoftSelData(); //a struct with a constructor :)
};
BOOL operator==(GenSoftSelData& a, GenSoftSelData& b);


//-----------------------------------------------------------------------------
// class ObjectWrapper

class ObjectWrapper: public MaxHeapOperators {
	protected:
		TimeValue t;

		//FIXME: change the objectWrapper to store both the mesh and the object
		union {
			Mesh* mesh;
			MNMesh* poly;
			PatchMesh* patch;
		};
		union {
			MeshTempData* meshTempData;
			MNTempData* polyTempData;
		};
		BOOL isCopy;

		Point3* verts;
		GenFace* faces;
		GenEdge* edges;
		GeomObject* collapsedObj;

		BitArray faceSel, vertSel;
		GenSoftSelData softSelData;

		int invalidMask;

	public:
		enum { empty=0, triObject, polyObject, patchObject } type;
		enum { noneEnable=0, triEnable=1, polyEnable=2, patchEnable=4, allEnable=0x7 };

		CoreExport ObjectWrapper();
		CoreExport ~ObjectWrapper() {Release();}
		CoreExport void		Zero();

		// The 'enable' parameter will determine the valid object types for ObjectWrapper.
		// If the object is not valid (or not supported), it will be converted to the 'nativeType'
		CoreExport BOOL		Init( TimeValue t, ObjectState& os, BOOL copy=FALSE, int enable=allEnable, int nativeType=polyObject );
		CoreExport void		Release(); //frees the collapsed object, if any

		CoreExport BOOL	IsEmpty() {return type==empty;}

		CoreExport int		Type() {return type;}
		CoreExport Mesh*	GetTriMesh() {return type==triObject? mesh:NULL;}
		CoreExport MNMesh*	GetPolyMesh() {return type==polyObject? poly:NULL;}
		CoreExport PatchMesh* GetPatchMesh() {return type==patchObject? patch:NULL;}

		enum	{ iv_none=0x000, iv_verts=0x001, iv_faces=0x002, iv_edges=0x004 };
		CoreExport void		Invalidate( int iInvalid );

		CoreExport void		InvalidateGeomCache();

	//-- Geom vert support
		CoreExport int		NumVerts(); //geom verts
		CoreExport int		NumFaces(); //geom faces
		CoreExport int		NumEdges(); //geom edges

		CoreExport Point3*	Verts(); //geom verts
		CoreExport GenFace* Faces(); //geom faces
		CoreExport GenEdge* Edges(); //geom edges

		CoreExport Point3*	GetVert( int index );
		CoreExport void		SetVert( int index, Point3 &p );
		CoreExport void		SetNumVerts( int num );

		CoreExport GenFace	GetFace( int index );
		CoreExport void		SetFace( int index, GenFace &face );
		CoreExport void		SetNumFaces( int num );

		CoreExport GenEdge	GetEdge( int index );
		CoreExport int		NumFaceEdges( int faceIndex );
		CoreExport int		GetFaceEdgeIndex( int faceIndex, int edgeIndex );

		CoreExport void		GetVertexSpace( int index, Matrix3 & tm );
		CoreExport Point3*	GetVertexNormal( int index );

		CoreExport BOOL		GetFaceHidden( int faceIndex );
		CoreExport void		SetFaceHidden( int faceIndex, BOOL hidden );

	//-- Map channel support
		CoreExport BOOL		GetChannelSupport( int mapChannel );
		CoreExport void		SetChannelSupport( int mapChannel, BOOL b );

		CoreExport int		NumMapVerts( int channel );
		CoreExport void		SetNumMapVerts( int channel, int num );

		CoreExport Point3*	GetMapVert( int channel, int index );
		CoreExport void		SetMapVert( int channel, int index, Point3 &p );

		CoreExport GenFace	GetMapFace( int channel, int index );
		CoreExport void		SetMapFace( int channel, int index, GenFace &face );

	//-- Material ID support
		CoreExport MtlID	GetMtlID( int index ); // face index
		CoreExport void		SetMtlID( int index, MtlID mtlID ); // face index

	//-- Smoothing Group support
		CoreExport DWORD	GetSmoothingGroup( int index ); // face index
		CoreExport void		SetSmoothingGroup( int index, DWORD smGrp ); // face index

	//-- Sub-object selection support
		enum {SEL_OBJECT, SEL_VERT, SEL_FACE, SEL_ELEM}; //selection enum values
		CoreExport int		GetSelLevel(); //returns a selection enum value
		CoreExport void		SetSelLevel(int selLevel); //selLevel is a selection enum value

		CoreExport BitArray& GetVertSel();
		CoreExport void		SetVertSel( BitArray& );

		CoreExport BitArray& GetFaceSel();
		CoreExport void		SetFaceSel( BitArray& sel );

		CoreExport BitArray& GetElemSel();
		CoreExport void		SetElemSel( BitArray& sel );

		CoreExport BitArray* GetSel( int selLevel ); //gets the selection of the given type
		CoreExport void		SetSel( int selLevel, BitArray& sel ); //sets the selection of the given type

		CoreExport void		FaceToElemSel( BitArray& elemSel, BitArray& faceSel ); //convert face to elem selection

	//-- Selection methods
		CoreExport void		GrowSelection( int selLevel, BitArray& newSel );
		CoreExport void		ShrinkSelection( int selLevel, BitArray& newSel );

	//-- Soft selection support
		CoreExport void		GetSoftSel( GenSoftSelData& softSelData );
		CoreExport void		SetSoftSel( GenSoftSelData& softSelData );
		CoreExport void		UpdateSoftSel();

		CoreExport float*	SoftSelWeights();
		CoreExport Point3	SoftSelColor( float f );

	//-- Hit testing and ray intersection
		enum { SUBHIT_IGNOREBACKFACING=(1<<5) }; //other SUBHIT_... values defined in Mesh.h are also supported

		CoreExport BOOL		SubObjectHitTest(
								int selLevel, GraphicsWindow* gw, Material* ma, HitRegion *hr, 
								DWORD flags, HitListWrapper& hitList, int numMat=1, Matrix3 *mat = NULL );

		CoreExport int		IntersectRay(  Ray& ray, float& intersectAt, Point3& norm, int& fi, FloatTab& bary  );

		CoreExport void		Render(GraphicsWindow *gw, Material *ma, RECT *rp, int compFlags, int numMat = 1);

	//-- Enum conversion
		CoreExport int		SelLevel( int selLevel ); //generic enum to object-specific enum //FIXME: should be private?
		CoreExport int		HitLevel( int selLevel ); //generic enum to object-specific hit level enum //FIXME: should be private?

};


//-----------------------------------------------------------------------------
// class HitListWrapper

class HitListWrapper: public MaxHeapOperators {
	protected:
		friend class ObjectWrapper;
		union {
			SubObjHitList* meshHitList;
			SubObjHitList* polyHitList;
			SubPatchHitList* patchHitList;
		};
		union {
			MeshSubHitRec* curMeshHitRec;
			MeshSubHitRec* curPolyHitRec;
			PatchSubHitRec* curPatchHitRec;
		};

	public:
		enum { empty=0, triObject, polyObject, patchObject } type;

		CoreExport HitListWrapper() {type=empty;}
		CoreExport ~HitListWrapper();

		CoreExport bool GoToFirst();
		CoreExport bool GoToNext();
		CoreExport DWORD GetDist();
		CoreExport int GetIndex();

		CoreExport void Init( int type );
		CoreExport void Release();
};


#endif //__OBJECTWRAPPER__H
