/**********************************************************************
 *<
	FILE: VNormal.h

	DESCRIPTION:

	CREATED BY:

	HISTORY:

 *>	Copyright (c) 2004, All Rights Reserved.
 **********************************************************************/

#include "Max.h"
#include "MeshNormalSpec.h"
#include "NormalRender.h"

#ifndef __VNORMAL__H
#define __VNORMAL__H

typedef unsigned short WORD;
typedef struct Basis{
	Point3 uBasis, vBasis;
} TangentBasis;

TangentBasis ConstructTangentBasis( Point3 uBasis, Point3 vBasis );
extern Point3 zeroNormal; // (0,0,0)
extern Point3 stubNormal; // (0,0,1)
extern TangentBasis zeroTangentBasis; // (0,0,0), (0,0,0)  = ConstructTangentBasis( Point3(0,0,0), Point3(0,0,0) );
extern TangentBasis stubTangentBasis; // (1,0,0), (0,1,0)  = ConstructTangentBasis( Point3(1,0,0), Point3(0,1,0) );


// Struct VertFace
// Indicates a face bordering a given vertex, with the subindex of the vertex
typedef struct {
	int						faceIndex;
	short					faceVertIndex;
} VertFace;

//===========================================================================
//
// Class VertFaceLookup
//
// Indicates the faces bordering each vertex in a mesh
// Helper for VertFaceGrouping
//
//===========================================================================

class VertFaceLookup {
	public:
		VertFaceLookup();
		~VertFaceLookup();
		void				Init( Mesh& mesh );
		int					GetCount( int vertIndex );
		VertFace			GetVertFace( int vertIndex, int i );
	protected:
		VertFace*			vertFaces;
		int*				vertFaceIndex;
};


//===========================================================================
//
// Class VertFaceGrouping
//
// Indicates the faces bordering each vertex in a mesh
// Grouping is by smoothing groups, specified normals and map topology
//
//===========================================================================

class VertFaceGrouping {
	public:
		void				Init( Mesh& mesh, int mapChannel );
		void				Free();
		int					GetGroup( int faceIndex, int faceVertIndex );
		void				SetGroup( int faceIndex, int faceVertIndex, int group );
		WORD				GetBits( int faceIndex ) {return faceBits[faceIndex];}
		void				SetBits( int faceIndex, WORD bits ) {faceBits[faceIndex]=bits;}
		enum {BIT_ISFACETED=0x8000}; // Bit flags

		//-- Accessory data
		WORD				GetGroupCount( int vertIndex );
		void				Compact(); // Delete all accessory data

	protected:

		class MeshData {
			public:
			void			Init( Mesh& mesh, int mapChannel );
			Face*			geomFaces;
			MeshNormalSpec*	normalSpec;
			TVFace*			mapFaces;
			BitArray		mapBackfacing;
		};

		class GroupData {
			public:
			void			Init( MeshData& meshData, int faceIndex, int faceVertIndex );
			BOOL			IsMatch( GroupData& that );
			void			Merge( GroupData& that );
			DWORD			smGroup;
			Point3			normalSpec;
			int				mapVert; //backfacing flag held in upper bit
		};

		class FaceData {
			public:
			void			Init( MeshData& meshData, int faceIndex );
			BOOL			IsFaceted();
			GroupData&		operator[]( int i ) {return groupData[i];}
			GroupData		groupData[3];
		};

		void				InitVert( Tab<FaceData>& faceData, VertFaceLookup& lookup, int vertIndex );
		Tab<WORD>			faceBits;
		Tab<WORD>			groupCounts;
};


//===========================================================================
//
// Class VNormalChannel
//
// Holds a normal vector and related data at each vertex & group;
// that is, one piece of data per vertex per group at the vertex.
//
//===========================================================================

class VNormalChannel {
	public:
		VNormalChannel();
		~VNormalChannel() {Free();}
		void				Init( Mesh& mesh, int mapChannel );
		void				Free();
		BOOL				Valid(); //whether Init() has been called

		void				InitTangentBasis( Mesh& mesh, Matrix3& tm, int mapChannel );
		BOOL				ValidTangentBasis();
		TangentBasis&		GetTangentBasis( int faceIndex, int vertIndex, int faceVertIndex );

		void				InitNormals( Mesh& mesh, Matrix3& tm );
		BOOL				ValidNormals();
		Point3&				GetNormal( int faceIndex, int vertIndex, int faceVertIndex );

		int					GetMapChannel() {return mapChannel;}

	protected:
		BOOL				IsFaceted( int faceIndex );
		int					FindIndex( int faceIndex, int vertIndex, int faceVertIndex );

		BOOL				valid, validTangentBasis, validNormals;
		int					mapChannel;

		// Data arrays:
		// - One entry per vertex per group, plus one entry per faceted face.
		// - Indices into these arrays must be calculated using FindIndex()
		int					numItems; // Number of array items allocated
		Tab<Point3>			normalsSet;
		Tab<TangentBasis>	tangentBasisSet;

		// Lookup data:
		// - For each face, the group number of each of its verts
		// - For facted faces, one array entry is allocated for the face;
		//   A base index and offset is used instead of the group numbers
		// - For each vert, a base index and offset into the data array(s);
		//   Vert array indices are calculated as (base+offset+group)
		// - The base + offset scheme reduces memory,
		//   because the offset values have a low bit depth compared to the base values  
		VertFaceGrouping	grouping; // for each face, the group numbers of its verts
		Tab<int>			faceBase; // one base value per 256 faces
		Tab<int>			vertBase; // one base value per 256 verts
		Tab<WORD>			vertOffset; // one offset value per vert, low bit depth
};


//===========================================================================
//
// Class VNormalBank
//
// Holds one VNormalChannel for each mapping channel of an object
//
//===========================================================================

class VNormalBank {
	public:
		VNormalBank();
		~VNormalBank() {Free();}
		void				Init() {}
		void				Free();
		void				InitChannel( Mesh& mesh, int mapChannel );
		BOOL				ValidChannel( int mapChannel );
		VNormalChannel*		GetChannel( int mapChannel );
	protected:
		Tab<VNormalChannel*> channels; // one per channel
};


//===========================================================================
//
// Class IVNormalMgr
//
//===========================================================================

#define IVNORMALMGR_INTERFACE_ID Interface_ID(0x28af453a, 0x1eb87a50)
class IVNormalMgr : public FPStaticInterface {
	public:
		DECLARE_DESCRIPTOR( IVNormalMgr );
		BaseInterface*		GetInterface(Interface_ID id);
};

//===========================================================================
//
// Class VNormalMgr
//
//===========================================================================

// FIXME !! The ShadeContext should support this itself
class VNormalMgr : public IVNormalMgr
{
	public:
		VNormalMgr();
		~VNormalMgr();
		void				Free(); //free all data

		void				InitChannel( Mesh& mesh, int nodeID, int mapChannel );
		BOOL				ValidChannel( int nodeID, int mapChannel );
		VNormalChannel*		GetChannel( int nodeID, int mapChannel );

		//-- Convenience methods
		void				InitTangentBasis( ShadeContext& sc, int mapChannel );
		void				GetTangentBasis( ShadeContext& sc, int mapChannel, TangentBasis& tangentBasis );

	protected:
		//-- Internal methods use no locking; appropriate when caller code already has a lock
		void				InitChannel_Internal( Mesh& mesh, int nodeID, int mapChannel );
		BOOL				ValidChannel_Internal( int nodeID, int mapChannel );
		VNormalChannel*		GetChannel_Internal( int nodeID, int mapChannel );

		ReaderWriterLock	lock;
		Tab<VNormalBank*>	banks;
};

VNormalMgr* GetVNormalMgr();


#endif //__VNormal__H