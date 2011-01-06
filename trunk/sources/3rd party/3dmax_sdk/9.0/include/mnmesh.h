/**********************************************************************
 *<
	FILE: MNMesh.h

	DESCRIPTION:  Special mesh structures useful for face and edge based mesh operations.

	CREATED BY: Steve Anderson, working for Kinetix!

	HISTORY: created March 1997 from old SDMesh and WMesh.

 *>	Copyright (c) 1996 Autodesk, Inc., All Rights Reserved.
 **********************************************************************/

// Necessary prior inclusions: max.h

// Classes:
// MNMesh
// MNVert
// MNEdge
// MNFace
// MNMeshBorder

#ifndef __MN_MESH_H_
#define __MN_MESH_H_

// Turn on the following to enable Face Data in PolyObjects:
#define MNMESH_FACEDATA_IMPL

#include "maxheap.h"
#include "export.h"
#include "baseinterface.h"
#include <hitdata.h>

#define REALLOC_SIZE 10

//#define SEPARATE_GEOM_TOPO_INVALIDATION

// Boolean types: we use the same ones defined in mesh.h
//#define MESHBOOL_UNION 1
//#define MESHBOOL_INTERSECTION 2
//#define MESHBOOL_DIFFERENCE 3

// General flags for all components
// For MNVerts, MNEdges, and MNFaces, bits 0-7 are used for common characteristics
// of all components.  Bits 8-15 are used for component-specific flags.  Bits 16-23 are reserved
// for temporary use in MNMesh algorithms.  Bits 24-31 are reserved for MNMath.lib users.
#define MN_SEL (1<<0)
#define MN_DEAD (1<<1)
#define MN_TARG (1<<2)
#define MN_BACKFACING (1<<3)
#define MN_HIDDEN (1<<4)
#define MN_CACHEINVALID (1<<5)
#define MN_INFOREGROUND (1<<7) //this tags a face as not renderable but hittestable BTW 6 is skipped because the tri strip uses that as face flag signifing a face to be stripped

#define MN_WHATEVER (1<<16) // Temporary flag used internally for whatever.
#define MN_LOCAL_SEL (1<<17) // Alternate selections (not passed up the pipe).
#define MN_HITTEST_CULLED (1<<18)	// used to indicate "culled" components (neither selected nor not selected) in window-region hit testing
#define MN_USER (1<<24)	// Anything above this can be used by applications.

// Vertex flags
#define MN_VERT_DONE (1<<8)
#define MN_VERT_WELDED (1<<9)
// CAL-04/07/03: ANDY: New flag here, to track which vertices are direct "descendants" of
// the original cage vertices in subdivision.
#define MN_VERT_SUBDIVISION_CORNER (1<<10)

class IHardwareShader;
class TriStrip;
class MNMesh;

class MNVert : public FlagUser {
public:
	Point3 p;
	int orig;	// Original point this vert comes from

	MNVert () { orig = -1; }
	DllExport MNVert & operator= (const MNVert & from);
	bool operator==(const MNVert & from) { return (from.p==p)&&(from.ExportFlags()==ExportFlags()); }
};

// Edge flags
#define MN_EDGE_INVIS (1<<8)
#define MN_EDGE_NOCROSS (1<<9)
#define MN_EDGE_MAP_SEAM (1<<10)
// CAL-03/14/03: ANDY: New flag here, to track which edges are direct "descendants" of
// the original cage edges in subdivision.
#define MN_EDGE_SUBDIVISION_BOUNDARY (1<<11)
// SCA - 6/20/04 - new flag to track unwanted edges created by Cut, so they might
// be removed later.
#define MN_EDGE_CUT_EXTRA (1<<12)

// Edge goes from v1 to v2
// f1 is forward-indexing face (face on "left" if surface normal above, v2 in front)
// f2 is backward-indexing face, or -1 if no such face exists.  (Face on "right")
class MNEdge : public FlagUser {
public:
	int v1, v2;
	int f1, f2;
	int track;	// Keep track of whatever.

	MNEdge() { Init(); }
	MNEdge (int vv1, int vv2, int fc) { f1=fc; f2=-1; v1=vv1; v2=vv2; track=-1; }
	void Init() { v1=v2=f1=0; f2=-1; track=-1; }
	int OtherFace (int ff) { return (ff==f1) ? f2 : f1; }
	int OtherVert (int vv) { return (vv==v1) ? v2 : v1; }
	void Invert () { int hold=v1; v1=v2; v2=hold; hold=f1; f1=f2; f2=hold; }
	DllExport void ReplaceFace (int of, int nf, int vv1=-1);
	void ReplaceVert (int ov, int nv) { if (v1 == ov) v1 = nv; else { MaxAssert (v2==ov); v2 = nv; } }
	DllExport bool Uncrossable ();
	DllExport MNEdge & operator= (const MNEdge & from);
	bool operator==(MNEdge & f) { return (f.v1==v1)&&(f.v2==v2)&&(f.f1==f1)&&(f.f2==f2)&&(f.ExportFlags()==ExportFlags()); }
	int& operator[](int i) { return i ? v2 : v1; }  
	const int& operator[](int i) const { return i ? v2 : v1; }  
	DllExport void MNDebugPrint ();
};

class MNFace;

class MNMapFace: public MaxHeapOperators {
	friend class MNMesh;
	int dalloc;
public:
	int deg;
	int *tv;

	MNMapFace() { Init(); }
	DllExport MNMapFace (int d);
	~MNMapFace () { Clear(); }
	DllExport void Init();
	DllExport void Clear();
	DllExport void SetAlloc (int d);
	void SetSize (int d) { SetAlloc(d); deg=d; }
	DllExport void MakePoly (int fdeg, int *tt);
	DllExport void Insert (int pos, int num=1);
	DllExport void Delete (int pos, int num=1);
	DllExport void RotateStart (int newstart);
	DllExport void Flip ();	// Reverses order of verts.  0 remains start.

	DllExport int VertIndex (int vv);
	DllExport void ReplaceVert (int ov, int nv);

	DllExport MNMapFace & operator= (const MNMapFace & from);
	DllExport MNMapFace & operator= (const MNFace & from);
	DllExport bool operator== (const MNMapFace & from);
	DllExport void MNDebugPrint ();
};

// MNFace flags:
#define MN_FACE_OPEN_REGION (1<<8)	// Face is not part of closed submesh.
#define MN_FACE_CHECKED (1<<9)	// for recursive face-and-neighbor-checking
#define MN_FACE_CHANGED (1<<10)
// The following macro has been added
// in 3ds max 4.2.  If your plugin utilizes this new
// mechanism, be sure that your clients are aware that they
// must run your plugin with 3ds max version 4.2 or higher.
#define MN_FACE_CULLED (1<<11)	// Used during hit-testing.
// End of 3ds max 4.2 Extension

// Diagonal sorting algorithm:
// Puts diagonals in increase-by-last-index, decrease-by-first order:
DllExport void DiagSort (int dnum, int *diag);

class MNFace : public FlagUser {
	friend class MNMesh;
	int dalloc;
	void SwapContents (MNFace & from);
public:
	int deg;	// Degree: number of vtx's and edg's that are relevant.
	int *vtx;	// Defining verts of this face.
	int *edg;	// edges on this face.
	int *diag;	// Diagonals
	DWORD smGroup;
	MtlID material;
	int track;	// Keep track of whatever -- MNMesh internal use only.
	BitArray visedg, edgsel;
	BitArray bndedg;	// CAL-03/18/03: boundary edges

	MNFace() { Init(); }
	MNFace (int d) { Init(); SetDeg (d); }
	DllExport MNFace (const MNFace *from);
	~MNFace () { Clear(); }

	DllExport void Init();
	DllExport void SetDeg (int d);
	DllExport void Clear();
	int TriNum() { return deg-2; }
	DllExport int FindTriPoint (int edge);	// finds point that makes tri with this edge.
	DllExport int FindTriPoint (int a, int b);	// does the same for nonsequential verts - result is between a,b.
	DllExport void GetTriangles (Tab<int> &tri);
	DllExport void SetAlloc (int d);
	DllExport void MakePoly (int fdeg, int *vv, bool *vis=NULL, bool *sel=NULL);
	DllExport void Insert (int pos, int num=1);
	DllExport bool Delete (int pos, int num=1, int edir=1, bool fixtri=TRUE);
	DllExport void RotateStart (int newstart);
	DllExport void Flip ();	// Reverses order of verts.  0 remains start.

	DllExport int VertIndex (int vv, int ee=-1);
	DllExport int EdgeIndex (int ee, int vv=-1);
	DllExport void ReplaceVert (int ov, int nv, int ee=-1);
	DllExport void ReplaceEdge (int oe, int ne, int vv=-1);

	DllExport MNFace & operator= (const MNFace & from);
	DllExport bool operator== (const MNFace &from);
	int& operator[](int i) { return vtx[i]; }
	const int& operator[](int i) const { return vtx[i]; }  
	DllExport void MNDebugPrint (bool triprint=FALSE);

	DllExport IOResult Save (ISave *isave);
	DllExport IOResult Load (ILoad *iload);
};

class MNMap : public FlagUser {
	friend class MNMesh;
	int nv_alloc, nf_alloc;
public:
	MNMapFace *f;
	UVVert *v;

	int numv, numf;

	MNMap () { Init(); }
	~MNMap () { ClearAndFree (); }

	// Initialization, allocation:
	DllExport void Init ();
	DllExport void VAlloc (int num, bool keep=TRUE);
	DllExport void FAlloc (int num, bool keep=TRUE);

	// Data access:
	int VNum () const { return numv; }
	UVVert V(int i) const { return v[i]; }
	int FNum () const { return numf; }
	MNMapFace *F(int i) const { return &(f[i]); }

	// Adding new components -- all allocation should go through here!
	DllExport int NewTri (int a, int b, int c);
	DllExport int NewTri (int *vv);
	DllExport int NewQuad (int a, int b, int c, int d);
	DllExport int NewQuad (int *vv);
	DllExport int NewFace (int degg=0, int *vv=NULL);
	DllExport void setNumFaces (int nfnum);
	DllExport int NewVert (UVVert p, int uoff=0, int voff=0);
	DllExport void setNumVerts (int nvnum);

	DllExport void CollapseDeadVerts (MNFace *faces);	// Figures out which are dead.
	DllExport void CollapseDeadFaces (MNFace *faces);
	DllExport void Clear ();	// Deletes everything.
	DllExport void ClearAndFree ();	// Deletes everything, frees all memory

	DllExport void Transform (Matrix3 & xfm);	// o(n) -- transforms verts

	// operators and debug printing
	DllExport MNMap & operator= (const MNMap & from);
	DllExport MNMap & operator+= (const MNMap & from);
	DllExport MNMap & operator+= (const MNMesh & from);
	DllExport void ShallowCopy (const MNMap & from);
	DllExport void NewAndCopy ();
	DllExport void MNDebugPrint (MNFace *faces);
	DllExport bool CheckAllData (int mapChannel, int nf, MNFace *faces);

	DllExport IOResult Save (ISave *isave, MNFace *faces=NULL);
	DllExport IOResult Load (ILoad *iload, MNFace *faces=NULL);
};

// Per-edge data
#define MAX_EDGEDATA 10
#define EDATA_KNOT 0
#define EDATA_CREASE 1

DllExport int EdgeDataType (int edID);
DllExport void *EdgeDataDefault (int edID);

// MNMesh::flags definitions
/*! \defgroup Mesh Flags
The following represent flags describing various flags that can be set on the mnmesh.
*/
//@{
#define MN_MESH_NONTRI (1<<0)				//!< At least 2 triangles have been joined
#define MN_MESH_FILLED_IN (1<<1)			//!< All topological links complete
#define MN_MESH_RATSNEST (1<<2)				//!< Set if we've replicated points to avoid rats' nest meshes.
#define MN_MESH_NO_BAD_VERTS (1<<3)			//!< Set if we've established that each vert has exactly one connected component of faces & edges.
#define MN_MESH_VERTS_ORDERED (1<<4)		//!< Set if we've ordered the fac, edg tables in each vert.
#define MN_MESH_FACE_NORMALS_INVALID (1<<6)	//!< Set if any of the face normals (fnorm) have become invalid - making this flag instead of deleting fnorm all the time
#define MN_MESH_HAS_VOLUME (1<<7)			//!< Some subset of mesh describes closed surface of solid
#define MN_MESH_HITTEST_REQUIRE_ALL (1<<8)	//!< Force faces to be hit only if all triangles are hit.  (Internal use.)
#define MN_MESH_PARTIALCACHEINVALID (1<<16) //!< MNMesh flag that means that the mnmesh has only set some of the vertices as invalid and not to reprocess the entire mesh just the vertices that changed
//@}

#define MN_MESH_CACHE_FLAGS (MN_MESH_FILLED_IN|MN_MESH_NO_BAD_VERTS|MN_MESH_VERTS_ORDERED)

// These flags are for internal use only.
#define MN_MESH_TEMP_1 (1<<13)
#define MN_MESH_TEMP_2 (1<<14)
//watje internal use flag
#define MN_MESH_DONTTRISTRIP (1<<15)


class MNMeshBorder;
class MNChamferData;
class MNFaceClusters;

// MNMesh selection levels:
// MNM_SL_CURRENT is an acceptable argument to methods that take a selection level,
// indicating "use the current mesh selection level".
enum PMeshSelLevel { MNM_SL_OBJECT, MNM_SL_VERTEX, MNM_SL_EDGE, MNM_SL_FACE, MNM_SL_CURRENT };

// MNMesh display flags
#define MNDISP_VERTTICKS 0x01
#define MNDISP_SELVERTS	0x02
#define MNDISP_SELFACES	0x04
#define MNDISP_SELEDGES	0x08
#define MNDISP_NORMALS 0x10
#define MNDISP_SMOOTH_SUBSEL 0x20
#define MNDISP_BEEN_DISP 0x40
#define MNDISP_DIAGONALS 0x80
// CAL-03/14/03: ANDY: New display flag here for the behavior we want.
#define MNDISP_HIDE_SUBDIVISION_INTERIORS 0x100

// Flags for sub object hit test
// NOTE: these are the same bits used for object level.
//#define SUBHIT_SELONLY		(1<<0)
//#define SUBHIT_UNSELONLY	(1<<2)
//#define SUBHIT_ABORTONHIT	(1<<3)
//#define SUBHIT_SELSOLID		(1<<4)

#define SUBHIT_MNUSECURRENTSEL (1<<22)		// When this bit is set, the sel only and unsel only tests will use the current level (edge or face) selection when doing a vertex level hit test
#define SUBHIT_OPENONLY		(1<<23)
#define SUBHIT_MNVERTS		(1<<24)
#define SUBHIT_MNFACES		(1<<25)
#define SUBHIT_MNEDGES		(1<<26)
#define SUBHIT_MNDIAGONALS  (1<<27)		// Hit-test on "diagonals" of polygons - MNDiagonalHitData is the type of HitData returned.
#define SUBHIT_MNTYPEMASK	(SUBHIT_MNVERTS|SUBHIT_MNFACES|SUBHIT_MNEDGES|SUBHIT_MNDIAGONALS)

///  \brief 
/// This class is a new form of HitData used for hit-testing diagonals of polygons in Editable Poly and Edit Poly.
/// 
///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly,
/// as well as in the Edit Poly modifier, in maxsdk\samples\mesh\EditPoly 
class MNDiagonalHitData : public HitData {
public:
	///  \brief 
	/// mFace and mDiag are the data members describing the face and diagonal hit.
	/// (For each face with degree "deg", there are deg-3 diagonals.)
	/// 
	int mFace, mDiag;

	///  \brief 
	/// Constructor.  Sets mFace and mDiagonal to the face and diagonal given.
	/// 
	MNDiagonalHitData (int face, int diagonal) : mFace(face), mDiag(diagonal) { }

	///  \brief 
	/// Destructor.  Doesn't do anything.
	/// 
	~MNDiagonalHitData () { }
};

// Subdivision flags:
#define MN_SUBDIV_NEWMAP 0x01
#define MN_SUBDIV_HIDE_INTERNAL_EDGES 0x10

class MNMesh : public FlagUser, public BaseInterfaceServer {
	friend class HardwareMNMesh;
	friend void gfxCleanup(void *data);
	friend class MNNormalSpec;
	friend class MNMeshUtilites8Impl;
	friend class MNMeshFGBGObject;

private:
	static int		refCount;
	static HANDLE	workMutex;
	static HANDLE	workEndEvent;

	int nv_alloc, ne_alloc, nf_alloc, nm_alloc;

	// Cache geometric data for quick rendering
	Box3 bdgBox;
	Point3 *fnorm;
	RVertex *rVerts;		// <<< instance specific.
	GraphicsWindow *cacheGW;  		// identifies rVerts cache

	//these are counter of various derived data so we can track when to delete them when they become unused
	UINT		mDXMeshCacheDrawCount;  //this the number of times that the direct mesh cache has been used to draw the mesh exclusively
	UINT		mRedrawCountFromLoad;  //this is the number of times the mesh has been redrawn from load.  This is used to prevent an initial memory spike on load
										//this will be set to -1 if the mesh was created and not loaded
	UINT		mBoxDisplayCount;  //this the number of times that the direct mesh cache has been drawn as a box exclusively


	// SCA revision for Max 5.0 - value of 0 for unbuilt, 1 for built according to legacy 4.0 scheme,
	// and 2 for built according to new scheme - this is so that we refresh normals if the user changes
	// the "Use Legacy 4.0 Vertex Normals" checkbox.
	int normalsBuilt;
	float norScale;

	// Hidden map channels
	// There are always NUM_HIDDENMAPS of these, but we dynamically allocate them
	// so that M(mapChannel) can be a const method.  (This wouldn't work if we had hmap[NUM_HIDDENMAPS].)
	MNMap *hmap;
	// Formerly public normal map channels:
	MNMap *m;

	// Vertex color arrays - for display use.
	int curVCChan;	// current map channel to use for colors (default = 0)
	UVVert *curVCVerts;	// possible external color array (default = NULL)
	MNMapFace *curVCFaces;	// possible external face array (default = NULL)

	DWTab		norInd;			// indirection array for fast normal lookup
	int			normalCount;	// total number of normals
	Point3 *	gfxNormals;		// flattened list of normals

	// Derived arrays to contain generated texture coordinates
	int			numTexCoords[GFX_MAX_TEXTURES];
	Point3 *	texCoords[GFX_MAX_TEXTURES];

	// Derived table of TriStrips, depends on topology
//	Tab<TriStrip *>	*tstab; // OLP Doesn't seem to be used anymore (HardwareMNMesh redeclares it). Should remove at next SDK breakage

	// Internal part of SabinDoo method:
	void SDVEdgeFace (int id, int vid, int *fv, Tab<Tab<int> *> & fmv, int selLevel);

	// Internal part of recursive smoothing-group search.
	DWORD FindReplacementSmGroup (int ff, DWORD os, int call);

	// Internal parts of Boolean. (MNBool.cpp)
	int BooleanZip (DWORD sortFlag, float weldThresh);
	bool BoolZipSeam (Tab<int> *lpi, Tab<int> *lpj, int & starth, int & startk, float weldThresh);
	void BoolPaintClassification (int ff, DWORD classification);

	// Internal data cache stuff (MNPipe.cpp)
	void buildBoundingBox ();
	void buildFaceNormals ();

	GraphicsWindow *getCacheGW() { return cacheGW; }
	void setCacheGW (GraphicsWindow *gw) { cacheGW = gw; }

	//Just some temporary structs to hold a list of only changed faces and verts.  They are used when building normals.
	BitArray        mInvalidVerts;
	BitArray        mInvalidFaces;
	Tab<int>	mInvalidVertList;
	Tab<int>	mInvalidFaceList;

	//! \brief This only draws the selected edges used to draw them over the directX cache 
	/*!	This draws the selected edges of the mnmesh.  Backface culling is not supported
	\param[in] gw The graphics window associated with the draw.
	*/
	void	DrawOnlySelectedEdges(GraphicsWindow *gw);

	//! \brief This only draws the selected faces used to draw them over the directX cache 
	/*!	This draws the selected faces of the mnmesh.  Backface culling is not supported
	\param[in] gw The graphics window associated with the draw.
	*/
	void	DrawOnlySelectedFaces(GraphicsWindow *gw);


	//! \brief This draws the normals vectors of the mnmesh 
	/*!	This draws the normals vectors of the mnmesh.  	
	\param[in] gw The graphics window associated with the draw.
	*/
	void	DrawNormals(GraphicsWindow *gw);
	//! \brief This draws the subobject vertex ticks of the mnmesh 
	/*!	This draws the subobject vertex ticks of the mnmesh.  This includes selected vertice and soft selection of the vertices
		In the end this just ends up calling DrawVertexticksOGL_SZB or DrawVertexTicksDX
	\param[in] gw The graphics window associated with the draw.
	*/
	void	DrawVertexTicks(GraphicsWindow *gw);

	// These next 2 functions need to be kept in sync if you make a chnage to the vertex display
	//! \brief This draws the subobject vertex ticks of the mnmesh when in OGL, SZB or using non 3d markers such as the +,0 etc markers
	/*!	This draws the subobject vertex ticks of the mnmesh when in OGL, SZB or using non 3d markers such as the +,0 etc markers
	\param[in] gw The graphics window associated with the draw.
	*/
	void	DrawVertexTicksOGL_SZB(GraphicsWindow *gw);
	//! \brief This draws the subobject vertex ticks of the mnmesh when in DirectX and only using the regular point marker
	/*!	This draws the subobject vertex ticks of the mnmesh when in DirectX and only using the regular point marker
		this is becuase it uses the DirectX vertex buffer list directly and ONLY SUPPORTS THE DOT MARKER TYPE
	\param[in] gw The graphics window associated with the draw.
	*/
	void	DrawVertexTicksDX(GraphicsWindow *gw);

	/// We have 3 draw mesh functions the drawpoly in the hardware mesh when the DX mesh cache is used
	/// and a regular non cached dx draw, ogl and szb draw
	/// These all need to be kept in sync 
	//! \brief This draws the mesh edges and or shaded triangles in DX when not using the cache
	/*!	This draws the mesh edges and or shaded triangles in DX when not using the cache.
		This typically happens when the mesh has an fx shader on it or is being edited
	\param[in] gw The graphics window associated with the draw.
	\param[in] compFlags One or more of the following flags:
		COMP_TRANSFORM
			Forces recalculation of the model to screen transformation; otherwise attempt to use the cache.
		COMP_IGN_RECT
			Forces all polygons to be rendered; otherwise only those intersecting the box will be rendered.
		COMP_LIGHTING
			Forces re-lighting of all vertices (as when a light moves); otherwise only re-light moved vertices
		COMP_ALL
			All of the above flags.
		COMP_OBJSELECTED
			If this bit is set then the node being displayed by this mesh is selected. Certain display flags only activate when this bit is set.
	\param[in] ma The The list of materials to use to render the mesh. See Class Material.
	\param[in] numMat The number of materials for the MNMesh.
	*/
	void	DrawMeshDX_NoCache(GraphicsWindow *gw, int compFlags, Material *ma, int numMat=-1);

	//! \brief This draws the mesh edges and or shaded triangles in OGL
	/*!	This draws the mesh edges and or shaded triangles in OGL.
	\param[in] gw The graphics window associated with the draw.
	\param[in] compFlags One or more of the following flags:
		COMP_TRANSFORM
			Forces recalculation of the model to screen transformation; otherwise attempt to use the cache.
		COMP_IGN_RECT
			Forces all polygons to be rendered; otherwise only those intersecting the box will be rendered.
		COMP_LIGHTING
			Forces re-lighting of all vertices (as when a light moves); otherwise only re-light moved vertices
		COMP_ALL
			All of the above flags.
		COMP_OBJSELECTED
			If this bit is set then the node being displayed by this mesh is selected. Certain display flags only activate when this bit is set.
	\param[in] ma The The list of materials to use to render the mesh. See Class Material.
	\param[in] numMat The number of materials for the MNMesh.
	*/
	void	DrawMeshOGL(GraphicsWindow *gw, int compFlags, Material *ma, int numMat=-1);
	
	//! \brief This draws the mesh edges and or shaded triangles in SZB
	/*!	This draws the mesh edges and or shaded triangles in SZB.
	\param[in] gw The graphics window associated with the draw.
	\param[in] compFlags One or more of the following flags:
		COMP_TRANSFORM
			Forces recalculation of the model to screen transformation; otherwise attempt to use the cache.
		COMP_IGN_RECT
			Forces all polygons to be rendered; otherwise only those intersecting the box will be rendered.
		COMP_LIGHTING
			Forces re-lighting of all vertices (as when a light moves); otherwise only re-light moved vertices
		COMP_ALL
			All of the above flags.
		COMP_OBJSELECTED
			If this bit is set then the node being displayed by this mesh is selected. Certain display flags only activate when this bit is set.
	\param[in] ma The The list of materials to use to render the mesh. See Class Material.
	\param[in] numMat The number of materials for the MNMesh.
	*/
	void	DrawMeshSZB(GraphicsWindow *gw, int compFlags, Material *ma, int numMat=-1);


	// New MNMesh routines to drive HardwareShaders
	// NOTE alot of these were never implemented
	bool	CanDrawTriStrips(DWORD rndMode, int numMtls, Material *mtl);
	bool	BuildTriStrips(DWORD rndMode, int numMtls, Material *mtl);
	void	TriStripify(DWORD rndMode, int numTex, TVFace *tvf[], TriStrip *s, StripData *sd, int vtx);
	void	Draw3DTriStrips(IHardwareShader *phs, int numMat, Material *ma);
	void	Draw3DWireTriStrips(IHardwareShader *phs, int numMat, Material *ma);
	void	Draw3DVisEdgeList(IHardwareShader *phs, DWORD flags);
	int		render3DTriStrips(IHardwareShader *phs, int kmat, int kstrips);
	int		render3DWireTriStrips(IHardwareShader *phs, int kmat, int kstrips);
	int		render3DFaces(IHardwareShader *phs, DWORD index, int *custVis=NULL);

	// Vertex Removal approaches - Luna task 748O
	bool RemoveInternalVertex (int vertex);
	bool RemoveBorderVertex (int vertex);

	// Renders without any color handling - used only in hit-testing at this point.
	void renderSingleDiagonal (GraphicsWindow *gw, int ff, int d);

	// new methods for edge connection with slide and pinch values max8 
	bool ConnectEdges (DWORD edgeFlag, int segments=2, int pinch = 0, int slide = 0 );
	void MultiDivideEdge (int edge, int segments, int pinch, int slide, bool in_flipEdge );

	// new split method with extra distance parameters 
	bool SplitFlaggedVertices (DWORD in_vertexFlag,	float in_distance );
	bool SplitFlaggedEdges ( DWORD in_edgeFlag , float in_distance );

	// new chamfer method with extra open flag 
	bool ChamferVertices (DWORD in_flag, MNChamferData *in_mcd, bool in_open);
	bool ChamferEdges (DWORD in_flag, MNChamferData *in_mcd, bool in_open );

public:
	MNVert *v;
	MNEdge *e;
	MNFace *f;
	int numv, nume, numf, numm;

	// Vertex Data -- handled as in Meshes:
	PerData *vd;
	BitArray vdSupport;

	// Edge Data
	PerData *ed;
	BitArray edSupport;

	int selLevel;
	DWORD dispFlags;

	// Derived data:
	Tab<int> *vedg;
	Tab<int> *vfac;

	// Basic class ops
	MNMesh () { Init(); DefaultFlags (); }
	MNMesh (const Mesh & from) { Init(); SetFromTri (from); FillInMesh (); }
	MNMesh (const MNMesh & from) { Init(); DefaultFlags(); *this = from; }
	DllExport ~MNMesh();

	// Initialization:
	void DefaultFlags () { ClearAllFlags (); }
	DllExport void Init ();

	// Array allocation: these functions (& Init) have sole control over nvalloc, etc.
	DllExport void VAlloc (int num, bool keep=TRUE);
	DllExport void VShrink (int num=-1);	// default means "Shrink array allocation to numv"
	DllExport void freeVEdge();
	DllExport void VEdgeAlloc();
	DllExport void freeVFace();
	DllExport void VFaceAlloc();
	DllExport void EAlloc (int num, bool keep=TRUE);
	DllExport void EShrink (int num=-1);
	DllExport void FAlloc (int num, bool keep=TRUE);
	DllExport void FShrink (int num=-1);
	DllExport void MAlloc (int num, bool keep=TRUE);
	DllExport void MShrink (int num=-1);

	// Access to components
	int VNum () const { return numv; }
	MNVert *V(int i) const { return &(v[i]); }
	Point3 & P(int i) const { return v[i].p; }
	int ENum () const { return nume; }
	MNEdge *E(int i) const { return &(e[i]); }
	int FNum () const { return numf; }
	MNFace *F(int i) const { return &(f[i]); }
	int MNum () const { return numm; }
	DllExport MNMap *M(int mapChannel) const;
	DllExport void SetMapNum (int mpnum);
	DllExport void InitMap (int mapChannel);	// Inits to current MNMesh topology.
	DllExport void ClearMap (int mapChannel);
	DllExport UVVert MV (int mapChannel, int i) const;
	DllExport MNMapFace *MF (int mapChannel, int i) const;
	DllExport int TriNum () const;

	// Per Vertex Data:
	DllExport void setNumVData (int ct, BOOL keep=FALSE);
	int VDNum () const { return vdSupport.GetSize(); }

	DllExport BOOL vDataSupport (int vdChannel) const;
	DllExport void setVDataSupport (int vdChannel, BOOL support=TRUE);
	void *vertexData (int vdChannel) const { return vDataSupport(vdChannel) ? vd[vdChannel].data : NULL; }
	float *vertexFloat (int vdChannel) const { return (float *) vertexData (vdChannel); }
	DllExport void freeVData (int vdChannel);
	DllExport void freeAllVData ();

	// Two specific vertex data: these VDATA constants are defined in mesh.h
	float *getVertexWeights () { return vertexFloat(VDATA_WEIGHT); }
	void SupportVertexWeights () { setVDataSupport (VDATA_WEIGHT); }
	void freeVertexWeights () { freeVData (VDATA_WEIGHT); }
	float *getVSelectionWeights () { return vertexFloat(VDATA_SELECT); }
	void SupportVSelectionWeights () { setVDataSupport (VDATA_SELECT); }
	void freeVSelectionWeights () { freeVData (VDATA_SELECT); }

	// Per Edge Data:
	DllExport void setNumEData (int ct, BOOL keep=FALSE);
	int EDNum () const { return edSupport.GetSize(); }

	DllExport BOOL eDataSupport (int edChannel) const;
	DllExport void setEDataSupport (int edChannel, BOOL support=TRUE);
	void *edgeData (int edChannel) const { return eDataSupport(edChannel) ? ed[edChannel].data : NULL; }
	float *edgeFloat (int edChannel) const { return (float *) edgeData (edChannel); }
	DllExport void freeEData (int edChannel);
	DllExport void freeAllEData ();

	// One specific edge data: this EDATA constant is defined above
	float *getEdgeKnots () { return edgeFloat(EDATA_KNOT); }
	void SupportEdgeKnots () { setEDataSupport (EDATA_KNOT); }
	void freeEdgeKnots () { freeEData (EDATA_KNOT); }

	// Vertex face/edge list methods:
	DllExport void VClear (int vv);
	DllExport void VInit (int vv);
	DllExport int VFaceIndex (int vv, int ff, int ee=-1);
	DllExport int VEdgeIndex (int vv, int ee);
	void VDeleteEdge (int vv, int ee) { if (vedg) vedg[vv].Delete (VEdgeIndex(vv, ee), 1); }
	DllExport void VDeleteFace (int vv, int ff);
	DllExport void VReplaceEdge (int vv, int oe, int ne);
	DllExport void VReplaceFace (int vv, int of, int nf);
	DllExport void CopyVert (int nv, int ov);	// copies face & edge too if appropriate
	DllExport void MNVDebugPrint (int vv);

	// Adding new components -- all allocation should go through here!
	DllExport int NewTri (int a, int b, int c, DWORD smG=0, MtlID mt=0);
	DllExport int NewTri (int *vv, DWORD smG=0, MtlID mt=0);
	DllExport int NewQuad (int a, int b, int c, int d, DWORD smG=0, MtlID mt=0);
	DllExport int NewQuad (int *vv, DWORD smG=0, MtlID mt=0);
	DllExport int NewFace (int initFace, int degg=0, int *vv=NULL, bool *vis=NULL, bool *sel=NULL);
	DllExport int AppendNewFaces (int nfnum);
	DllExport void setNumFaces (int nfnum);
	DllExport int RegisterEdge (int v1, int v2, int f, int fpos);
	DllExport int SimpleNewEdge (int v1, int v2);
	DllExport int NewEdge (int v1, int v2, int f, int fpos);
	DllExport int AppendNewEdges (int nenum);
	DllExport void setNumEdges (int nenum);
	DllExport int NewVert (Point3 & p);
	DllExport int NewVert (Point3 & p, int vid);
	DllExport int NewVert (int vid);
	DllExport int NewVert (int v1, int v2, float prop);
	DllExport int AppendNewVerts (int nvnum);
	DllExport void setNumVerts (int nvnum);

	// To delete, set MN_*_DEAD flag and use following routines, which are all o(n).
	DllExport void CollapseDeadVerts ();
	DllExport void CollapseDeadEdges ();
	DllExport void CollapseDeadFaces ();
	DllExport void CollapseDeadStructs();
	DllExport void Clear ();	// Deletes everything.
	DllExport void ClearAndFree ();	// Deletes everything, frees all memory
	DllExport void freeVerts();
	DllExport void freeEdges();
	DllExport void freeFaces();
	DllExport void freeMap(int mapChannel);
	DllExport void freeMaps();

	// En Masse flag-clearing and setting:
	void ClearVFlags (DWORD fl) { for (int i=0; i<numv; i++) v[i].ClearFlag (fl); }
	void ClearEFlags (DWORD fl) { for (int i=0; i<nume; i++) e[i].ClearFlag (fl); }
	void ClearFFlags (DWORD fl) { for (int i=0; i<numf; i++) f[i].ClearFlag (fl); }
	DllExport void PaintFaceFlag (int ff, DWORD fl, DWORD fenceflags=0x0);
	DllExport void VertexSelect (const BitArray & vsel);
	DllExport void EdgeSelect (const BitArray & esel);
	DllExport void FaceSelect (const BitArray & fsel);
	bool getVertexSel (BitArray & vsel) { return getVerticesByFlag (vsel, MN_SEL); }
	bool getEdgeSel (BitArray & esel) { return getEdgesByFlag (esel, MN_SEL); }
	bool getFaceSel (BitArray & fsel) { return getFacesByFlag (fsel, MN_SEL); }
	// In following 3, if fmask is 0 it's set to flags, so only those flags are compared.
	DllExport bool getVerticesByFlag (BitArray & vset, DWORD flags, DWORD fmask=0x0);
	DllExport bool getEdgesByFlag (BitArray & eset, DWORD flags, DWORD fmask=0x0);
	DllExport bool getFacesByFlag (BitArray & fset, DWORD flags, DWORD fmask=0x0);
	DllExport void ElementFromFace (int ff, BitArray & fset);
	DllExport void BorderFromEdge (int ee, BitArray & eset);

	// Following also set visedg, edgsel bits on faces:
	DllExport void SetEdgeVis (int ee, BOOL vis=TRUE);
	DllExport void SetEdgeSel (int ee, BOOL sel=TRUE);

	// I/O with regular Meshes.
	void SetFromTri (const Mesh & from) { Clear (); AddTri (from); }
	DllExport void AddTri (const Mesh & from);	// o(n) -- Add another mesh -- simple union
	DllExport void OutToTri (Mesh & tmesh);	// o(n)

	// Internal computation routines
	// These three build on each other and should go in order:
	// FillInMesh, EliminateBadVerts, OrderVerts.
	DllExport void FillInMesh ();	// o(n*5) or so
	DllExport void FillInFaceEdges (); // o(n).
	DllExport void FillInVertEdgesFaces ();	// o(n)
	DllExport bool EliminateBadVerts (DWORD flag=0);	// o(n*8) or so
	DllExport void OrderVerts ();	// o(n*3) or so
	DllExport void OrderVert (int vid);
	DllExport void Triangulate ();	// o(n)
	DllExport void TriangulateFace (int ff);	// o(triangles)

	// Random useful stuff.
	DllExport void Transform (Matrix3 & xfm);	// o(n) -- transforms verts
	bool IsClosed() { for (int i=0; i<nume; i++) if (e[i].f2<0) return FALSE; return nume?TRUE:FALSE; } // o(n)
	DllExport void FaceBBox (int ff, Box3 & bbox);
	DllExport void BBox (Box3 & bbox, bool targonly=FALSE);
	DllExport Box3 getBoundingBox (Matrix3 *tm=NULL, bool targonly=FALSE);

	// Methods for handling MN_TARG flags.
	DllExport int TargetVertsBySelection (int selLevel);	// o(n)
	DllExport int TargetEdgesBySelection (int selLevel);	// o(n)
	DllExport int TargetFacesBySelection (int selLevel);	// o(n)
	DllExport int PropegateComponentFlags (int slTo, DWORD flTo,
		int slFrom, DWORD flFrom, bool ampersand=FALSE, bool set=TRUE);
	DllExport void DetargetVertsBySharpness (float sharpval);	// o(n*deg)

	// Face-center methods
	DllExport void ComputeCenters (Point3 *ctr, bool targonly=FALSE);	// o(n)
	DllExport void ComputeCenter (int ff, Point3 & ctr);
	DllExport void ComputeNormal (int ff, Point3 & normal, Point3 *ctr=NULL);
	DllExport void ComputeSafeCenters (Point3 *ctr, bool targonly=FALSE, bool detarg=FALSE);	// o(n)
	DllExport bool ComputeSafeCenter (int ff, Point3 & ctr);	// o(deg^2)

	// Triangulation-of-polygon methods:
	DllExport void RetriangulateFace (int ff);	// o(deg^2)
	DllExport void FindDiagonals (int ff, int *diag);
	DllExport void FindDiagonals (int deg, int *vv, int *diag);
	DllExport void BestConvexDiagonals (int ff, int *diag=NULL);
	DllExport void BestConvexDiagonals (int deg, int *vv, int *diag);
	DllExport bool SetDiagonal (int ff, int d1, int d2);

	// Normal methods
	// NOTE that these do not make use of user-specified normals or smoothing groups at all.
	DllExport int FindEdgeFromVertToVert (int vrt1, int vrt2);	// o(deg)
	DllExport void GetVertexSpace (int vrt, Matrix3 & tm);	// o(deg)
	DllExport Point3 GetVertexNormal (int vrt);	// o(deg)
	DllExport Point3 GetEdgeNormal (int ed);	// o(deg)
	DllExport Point3 GetFaceNormal (int fc, bool nrmlz=FALSE);	//o(deg)
	Point3 GetEdgeNormal (int vrt1, int vrt2) { return GetEdgeNormal (FindEdgeFromVertToVert(vrt1, vrt2)); }
	DllExport float EdgeAngle (int ed);
	DllExport void FlipNormal(int faceIndex);

	// Smoothing-group handling
	DllExport void Resmooth (bool smooth=TRUE, bool targonly=FALSE, DWORD targmask=~0x0);	// o(n)
	DllExport DWORD CommonSmoothing (bool targonly=FALSE);	// o(n)
	DllExport DWORD GetNewSmGroup (bool targonly=FALSE);	// o(n)
	DllExport MtlID GetNewMtlID (bool targonly = FALSE); // o(n)
	DllExport DWORD GetOldSmGroup (bool targonly=FALSE);	// up to o(n).
	DllExport DWORD GetAllSmGroups (bool targonly=FALSE);	// up to o(n)
	DllExport DWORD FindReplacementSmGroup (int ff, DWORD os);
	DllExport void PaintNewSmGroup (int ff, DWORD os, DWORD ns);
	DllExport bool SeparateSmGroups (int v1, int v2);
	DllExport void AutoSmooth(float angle,BOOL useSel,BOOL preventIndirectSmoothing);

	// Use following to unify triangles into polygons across invisible edges.
	DllExport void MakePolyMesh (int maxdeg=0, BOOL elimCollin=TRUE);
	// NOTE: MakeConvexPolyMesh result not guaranteed for now.  Still requires MakeConvex() afterwards to be sure.
	DllExport void MakeConvexPolyMesh (int maxdeg=0);
	DllExport void RemoveEdge (int edge);
	DllExport void MakeConvex ();
	DllExport void MakeFaceConvex (int ff);
	DllExport void RestrictPolySize (int maxdeg);
	DllExport void MakePlanar (float planarThresh);
	DllExport void MakeFacePlanar (int ff, float planarThresh);
	DllExport void EliminateCollinearVerts ();
	DllExport void EliminateCoincidentVerts (float thresh=MNEPS);

	// Following set NOCROSS flags, delete INVIS flags to make "fences" for Sabin-Doo
	DllExport void FenceMaterials ();
	DllExport void FenceSmGroups ();
	DllExport void FenceFaceSel ();
	DllExport void FenceOneSidedEdges();
	DllExport void FenceNonPlanarEdges (float thresh=.9999f, bool makevis=FALSE);
	DllExport void SetMapSeamFlags ();
	DllExport void SetMapSeamFlags (int mapChannel);

	// Find get detail about a point on a face.
	DllExport int FindFacePointTri (int ff, Point3 & pt, double *bary, int *tri);
	DllExport UVVert FindFacePointMapValue (int ff, Point3 & pt, int mapChannel);

	// Extrapolate map information about a point near a face.
	DllExport UVVert ExtrapolateMapValue (int face, int edge, Point3 & pt, int mapChannel);

	// Useful for tessellation algorithms
	DllExport void Relax (float relaxval, bool targonly=TRUE);

	// Returns map verts for both ends of each edge (from f1's perspective)
	// (Very useful for creating new faces at borders.) mv[j*2] corresponds to edge j's v1.
	DllExport void FindEdgeListMapVerts (const Tab<int> & lp, Tab<int> & mv, int mapChannel);

	// Following functions can be used to find & fix holes in a mesh, if any.
	DllExport void GetBorder (MNMeshBorder & brd, int selLevel=MNM_SL_OBJECT, DWORD targetFlag=MN_SEL);
	DllExport void FillInBorders (MNMeshBorder *b=NULL);
	DllExport void FindOpenRegions ();

	// Doubled mapping verts are individual map vertices that are used to correspond
	// to different regular vertices.  For instance, a box could have a single (1,1,0)
	// vertex that it uses in the upper-right corner of all quads.  This design is
	// harmful to some of our algorithms, such as the various Tessellators.  So the
	// following two methods detect and fix such vertices.

	// This method is not really appropriate for release, it's more of a debugging
	// tool.  All doubled mapping verts will be DebugPrinted.
	DllExport BOOL CheckForDoubledMappingVerts();

	// This one's ok and encouraged for release.  (Linear-time algorithm.)
	DllExport void EliminateDoubledMappingVerts();
	DllExport void EliminateIsoMapVerts();
	DllExport void EliminateIsoMapVerts(int mapChannel);

	// operators and debug printing (MNFace.cpp)
	DllExport MNMesh & operator= (const MNMesh & from);
	DllExport MNMesh & operator+= (const MNMesh & from);
	DllExport void MNDebugPrint (bool triprint=FALSE);
	DllExport void MNDebugPrintVertexNeighborhood (int vv, bool triprint=FALSE);
	DllExport bool CheckAllData ();

	// Split functions maintain topological info.  (MNSplit.cpp)
	// Luna task 747: Someday these should all support specified normals - but not today.
	DllExport int SplitTriEdge (int ee, float prop=.5f, float thresh=MNEPS,
		bool neVis=TRUE, bool neSel=FALSE);
	DllExport int SplitTriFace (int ff, double *bary=NULL, float thresh=MNEPS,
		bool neVis=TRUE, bool neSel=FALSE);
	DllExport void SplitTri6 (int ff, double *bary=NULL, int *nv=NULL);
	DllExport int SplitEdge (int ee, float prop=.5f);
	DllExport int SplitEdge (int ee, float prop, Tab<int> *newTVerts);
	DllExport int SplitEdge (int ff, int ed, float prop, bool right, int *nf=NULL,
		int *ne=NULL, bool neVis=FALSE, bool neSel=FALSE, bool allconvex=FALSE);
	DllExport int IndentFace (int ff, int ei, int nv, int *ne=NULL, bool nevis=TRUE, bool nesel=FALSE);
	DllExport void SeparateFace (int ff, int a, int b, int & nf, int & ne, bool neVis=FALSE, bool neSel=FALSE);
	DllExport bool Slice (Point3 & N, float off, float thresh, bool split, bool remove, bool flaggedFacesOnly=false, DWORD faceFlags=MN_SEL);
	DllExport void DeleteFlaggedFaces (DWORD deathflags, DWORD nvCopyFlags=0x0);
	DllExport bool WeldVerts (int a, int b);
	DllExport bool WeldEdge (int ee);

	// Tessellation methods: (MNTess.cpp)
	// Luna task 747: Someday these should all support specified normals - but not today.
	DllExport void TessellateByEdges (float bulge, MeshOpProgress *mop=NULL);
	DllExport bool AndersonDo (float interp, int selLevel, MeshOpProgress *mop=NULL, DWORD subdivFlags=0);
	DllExport void TessellateByCenters (MeshOpProgress *mop=NULL);

	// Sabin-Doo tessellation: (MNSabDoo.cpp)
	// Luna task 747: Someday these should both support specified normals - but not today.
	DllExport void SabinDoo (float interp, int selLevel, MeshOpProgress *mop=NULL, Tab<Point3> *offsets=NULL);
	DllExport void SabinDooVert (int vid, float interp, int selLevel, Point3 *ctr, Tab<Point3> *offsets=NULL);

	// Non-uniform Rational Mesh Smooth (NURMS.cpp)
	// Luna task 747: Someday this should support specified normals - but not today.
	DllExport void CubicNURMS (MeshOpProgress *mop=NULL,
		Tab<Point3> *offsets=NULL, DWORD subdivFlags=0);

	//DllExport void CubicNURMSMesh (Mesh & out, MeshOpProgress *mop=NULL, DWORD subdivFlags=0);

	// Boolean functions: (MNBool.cpp)
	// Luna task 747: Someday these should all support specified normals - but not today.
	DllExport void PrepForBoolean ();
	DllExport bool BooleanCut (MNMesh & m2, int cutType, int fstart=0, MeshOpProgress *mop=NULL);
	DllExport bool MakeBoolean (MNMesh & m1, MNMesh & m2, int type, MeshOpProgress *mop=NULL);
	DllExport void Connect (MNMeshBorder & borderList, int segs, float tension,
		bool sm_bridge, bool sm_ends, Tab<int> *vsep=NULL);
	DllExport void ConnectLoops (Tab<int> & loop1, Tab<int> & loop2,
		int segs, float tension, DWORD smGroup, MtlID mat, bool sm_ends);

	// Small-scale Operations - in MNOps.cpp
	// Luna task 747: Someday these should all support specified normals - but not today.
	DllExport void FacePointBary (int ff, Point3 & p, Tab<float> & bary);
	DllExport void CloneVerts (DWORD cloneFlag = MN_SEL, bool clear_orig=TRUE);
	DllExport void CloneFaces (DWORD cloneFlag = MN_SEL, bool clear_orig=TRUE);
	DllExport int DivideFace (int ff, Tab<float> & bary);
	DllExport int CreateFace (int degg, int *vv);

	// Moves the flagged subobjects to their average plane.
	DllExport bool MakeFlaggedPlanar (int selLev, DWORD flag=MN_SEL, Point3 *delta=NULL);
	// Moves flagged vertices to the plane given, or produces an array of offsets which would do so.
	DllExport bool MoveVertsToPlane (Point3 & norm, float offset, DWORD flag=MN_SEL, Point3 *delta=NULL);
	DllExport bool SplitFlaggedVertices (DWORD flag=MN_SEL);
	DllExport bool SplitFlaggedEdges (DWORD flag=MN_SEL);
	DllExport bool DetachFaces (DWORD flag=MN_SEL);
	DllExport bool DetachElementToObject (MNMesh & nmesh, DWORD fflags=MN_SEL, bool delDetached=true);
	DllExport bool ExtrudeFaceClusters (MNFaceClusters & fclust);
	DllExport bool ExtrudeFaceCluster (MNFaceClusters & fclust, int cl);
	DllExport bool ExtrudeFaces (DWORD flag=MN_SEL);	// Does each face separately
	// Bug fix: second version of GetExtrudeDirection so that a faceFlag can be
	// passed in when there are no relevant face clusters.  -sca 2001.12.12
	// Version which accepts faceFlag is NEW FOR 5.0!
	// Do not use if you wish to maintain backward compatibility with 4.0!
	DllExport void GetExtrudeDirection (MNChamferData *mcd, DWORD faceFlag);
	DllExport void GetExtrudeDirection (MNChamferData *mcd,
		MNFaceClusters *fclust=NULL, Point3 *clustNormals=NULL);
	DllExport bool SetVertColor (UVVert clr, int mapChannel, DWORD flag=MN_SEL);
	DllExport bool SetFaceColor (UVVert clr, int mapChannel, DWORD flag=MN_SEL);
	DllExport bool ChamferVertices (DWORD flag=MN_SEL, MNChamferData *mcd=NULL);
	DllExport bool ChamferEdges (DWORD flag=MN_SEL, MNChamferData *mcd=NULL);
	DllExport bool FlipElementNormals (DWORD flag=MN_SEL);
	DllExport void SmoothByCreases (DWORD creaseFlag);
	DllExport int CutFace (int f1, Point3 & p1, Point3 & p2, Point3 & Z, bool split);
	DllExport int CutEdge (int e1, float prop1, int e2, float prop2, Point3 & Z, bool split);
	DllExport int Cut (int startv, Point3 & end, Point3 & Z, bool split);
	DllExport bool WeldBorderVerts (int v1, int v2, Point3 *destination);
	DllExport bool WeldBorderEdges (int e1, int e2);
	DllExport bool WeldBorderVerts (float thresh, DWORD flag=MN_SEL);

	// Pipeline object requirements.  (MNPipe.cpp)
	DllExport void ApplyMapper (UVWMapper & mapChannel, int channel=0, BOOL useSel=FALSE);
	//Invalidates the geometry cache of the MNMesh - Forces any geometry related data to be rebuilt
	DllExport void InvalidateGeomCache ();
	//Invalidates the topology cache of the MNMesh - Forces any topology related data to be rebuilt
	//in_cacheClearFlags specifies whether the "MN_MESH_CACHE_FLAGS" flags will be cleared
	//true means the flags will be cleared, false means the flags will not be touched
	//These flags dictate whether the mesh has valid data in the edge and vertex caches, which is
	//typically false after invalidating the topology cache.  (i.e. these flags should be cleared)
	//If unsure about what to pass to this method, use the default behaviour
	DllExport void InvalidateTopoCache ( bool in_clearCacheFlags = true );
	DllExport void UpdateDisplayVertexColors ();
	DllExport void SetDisplayVertexColors (int chan);
	DllExport void SetDisplayVertexColors (UVVert *mv, MNMapFace *mf);
	DllExport void PrepForPipeline ();
	DllExport void allocRVerts ();
	DllExport void updateRVerts (GraphicsWindow *gw);
	DllExport void freeRVerts ();

	DllExport void checkNormals (BOOL illum);
	DllExport void buildNormals ();
	DllExport void buildRenderNormals ();
	DllExport void UpdateBackfacing (GraphicsWindow *gw, bool force);

	// This method can be used to invalidate any display-technology-specific caches of the MNMesh.
	// In particular, it's currently used to invalidate the DirectX cache of the mesh.
	// This is important to call, for example, after changing Vertex Colors or Texture coordinates, if
	// you don't already call InvalidateGeomCache ().
	// "DWORD keepFlags" should be zero.  We may someday add flags indicating that some parts of
	// the hardware mesh cache should be kept, but for now, the whole cache is invalidated.
	// (An example usage is in maxsdk\samples\mesh\EditablePoly\PolyEdOps.cpp.)
	DllExport void InvalidateHardwareMesh (DWORD keepFlags=0);

	// Display flags
	void		SetDispFlag(DWORD f) { dispFlags |= f; }
	DWORD		GetDispFlag(DWORD f) { return dispFlags & f; }
	void		ClearDispFlag(DWORD f) { dispFlags &= ~f; }

	//ack what do all these do?
	DllExport void render(GraphicsWindow *gw, Material *ma, RECT *rp, int compFlags, int numMat=1, InterfaceServer *pi=NULL);
	DllExport void renderFace (GraphicsWindow *gw, int ff);
	DllExport void render3DFace (GraphicsWindow *gw, int ff);
	DllExport void render3DDiagonals (GraphicsWindow *gw, DWORD compFlags);
	DllExport void renderDiagonals (GraphicsWindow *gw, DWORD compFlags);
	DllExport void renderDiagonal (GraphicsWindow *gw, int ff, bool useSegments=false, bool *lastColorSubSel=NULL);
	DllExport void render3DEdges (GraphicsWindow *gw, DWORD compFlags);
	DllExport void renderEdges (GraphicsWindow *gw, DWORD compFlags);
	DllExport void renderEdge (GraphicsWindow *gw, int ee, bool useSegments=false, bool *lastColorSubSel=NULL);


	DllExport BOOL select (GraphicsWindow *gw, Material *ma, HitRegion *hr, int abortOnHit=FALSE, int numMat=1);
	DllExport BOOL SubObjectHitTest(GraphicsWindow *gw, Material *ma, HitRegion *hr,
							DWORD flags, SubObjHitList& hitList, int numMat=1 );
	DllExport int IntersectRay (Ray& ray, float& at, Point3& norm);
	DllExport int IntersectRay (Ray& ray, float& at, Point3& norm, int &fi, Tab<float> & bary);
	DllExport BitArray VertexTempSel (DWORD fmask=MN_DEAD|MN_SEL, DWORD fset=MN_SEL);

	DllExport void 		ShallowCopy(MNMesh *amesh, ChannelMask channels);
	DllExport void		NewAndCopyChannels(ChannelMask channels);
	DllExport void 		FreeChannels (ChannelMask channels, BOOL zeroOthers=1);

	DllExport IOResult Save (ISave *isave);
	DllExport IOResult Load (ILoad *iload);
	DllExport void ClearFlag (DWORD fl);

	// --- from InterfaceServer
	DllExport BaseInterface* GetInterface(Interface_ID id);

	// Luna task 748 - New operations for 5.0
	// Source in MNOps2.cpp
	// NOT AVAILABLE IN PREVIOUS VERSIONS - AVOID FOR BACKWARD COMPATIBILITY
	DllExport bool SeparateMapVerticesInFaceCluster (MNFaceClusters & fclust, int clusterID);
	DllExport bool ExtrudeFaceClusterAlongPath (Tab<Matrix3> tFrenets,
		MNFaceClusters & fclust, int clusterID, bool align);
	DllExport bool LiftFaceClusterFromEdge (int liftEdge, float liftAngle, int segments,
		MNFaceClusters & fclust, int clusterID);
	DllExport bool IsEdgeMapSeam (int mapChannel, int edge);
	DllExport void MultiDivideEdge (int edge, int segments);
	DllExport bool ConnectEdges (DWORD edgeFlag, int segments=2);
	DllExport bool ConnectVertices (DWORD vertexFlag);
	DllExport void SelectEdgeLoop (BitArray & edgeSel);
	DllExport void SelectEdgeRing (BitArray & edgeSel);
	DllExport bool ExtrudeVertices (DWORD vertexFlag, MNChamferData *pMCD, Tab<Point3> & tUpDir);
	DllExport bool ExtrudeEdges (DWORD edgeFlag, MNChamferData *pMCD, Tab<Point3> & tUpDir);
	DllExport bool WeldOpposingEdges (DWORD edgeFlag);
	DllExport bool FlipFaceNormals (DWORD faceFlag);
	DllExport bool RemoveVertex (int vertex);	// Luna task 748O
	DllExport bool RemoveSpur (int spur);	// Used both by WeldEdge and in Luna task 748O
	DllExport int InsertSpur (int face, int vertIndex, Tab<int> *ptMapVertArray=NULL);	// New for task 748F
	DllExport void FindFaceBaryVector (int face, int vertIndex, Point3 & vector, Tab<float> & tBary);
	DllExport bool SplitFacesUsingBothSidesOfEdge (DWORD edgeFlag=0x0);

	// Luna task 747 - New operations for 5.0
	// Source in MNNormals.cpp
	// NOT AVAILABLE IN PREVIOUS VERSIONS - AVOID FOR BACKWARD COMPATIBILITY
	DllExport MNNormalSpec *GetSpecifiedNormals();	// Returns NULL if SpecifyNormals() has not been called.
	DllExport MNNormalSpec *GetSpecifiedNormalsForDisplay();	// Returns NULL if no specified normals, or if specified normals not ready to use for display.
	DllExport void ClearSpecifiedNormals ();
	DllExport void SpecifyNormals ();
	// Copies bare geometry only - no per-vertex data, maps, normals, etc.
	DllExport void CopyBasics (const MNMesh & from, bool copyEdges = false);

	//! \brief This will set the mesh or as static or not.  As static mesh has no animated channels
	/*!	This will set the mesh or as static or not.  As static mesh has no animated channels.  This allows 
	use to certain display optimization since we know the mesh is not changing.
	\param[in] static Whether the mesh is to tagged static or not.
	*/
	DllExport void SetStaticMesh(bool staticMesh);

	//! \brief This will force all the derived data used to display the mesh to be deleted
	/*!	This will force all the derived data used to display the mesh to be deleted.  This data can be things 
		like the gfx normals, the direct x mesh cache, tri stripping data etc.
	*/
	DllExport void ReduceDisplayCaches();

#ifdef SEPARATE_GEOM_TOPO_INVALIDATION
	DllExport void InvalidateHardwareMeshTopo (DWORD keepFlags=0);
#endif
};

class MNMeshBorder: public MaxHeapOperators {
	friend class MNMesh;
	Tab<Tab<int> *> bdr;
	BitArray btarg;
public:
	~MNMeshBorder () { Clear(); }
	DllExport void Clear ();
	int Num () { return bdr.Count(); }
	Tab<int> *Loop (int i) { return bdr[i]; }
	bool LoopTarg (int i) { return ((i>=0) && (i<bdr.Count()) && (btarg[i])); }
	DllExport void MNDebugPrint (MNMesh *m);
};

class MNFaceElement: public MaxHeapOperators {
public:
	// For each face, which element is it in
	Tab<int> elem;
	int count;

	DllExport MNFaceElement (MNMesh &mesh);
	int operator[](int i) {return elem[i];}
};

class MNFaceClusters: public MaxHeapOperators {
public:
	// Cluster #, one for each face - non-selected faces have -1 for their id.
	Tab<int> clust;
	int count;

	// Makes clusters from distinct selected components.
	DllExport MNFaceClusters (MNMesh & mesh, DWORD clusterFlags);
	// This version separates cluster also using a minimum angle and optionally by flags.
	DllExport MNFaceClusters (MNMesh & mesh, float angle, DWORD clusterFlags);
	int operator[](int i) { return (i<clust.Count()) ? clust[i] : -1; }
	DllExport void MakeVertCluster(MNMesh &mesh, Tab<int> & vclust);
	DllExport void GetNormalsCenters (MNMesh &mesh, Tab<Point3> & norm, Tab<Point3> & ctr);
	DllExport void GetBorder (MNMesh &mesh, int clustID, Tab<int> & cbord);
	DllExport void GetOutlineVectors (MNMesh & m, Tab<Point3> & cnorms, Tab<Point3> & odir);
};

class MNEdgeClusters: public MaxHeapOperators {
public:
	Tab<int> clust;
	int count;

	DllExport MNEdgeClusters (MNMesh &mesh, DWORD clusterFlags);
	int operator[](int i) {return clust[i];}
	DllExport void MakeVertCluster (MNMesh &mesh, Tab<int> & vclust);
	DllExport void GetNormalsCenters (MNMesh &mesh, Tab<Point3> & norm, Tab<Point3> & ctr);
};

class MNChamferData: public MaxHeapOperators {
	Tab<UVVert> hmdir[NUM_HIDDENMAPS];
public:
	Tab<Point3> vdir;
	Tab<float> vmax;
	Tab<UVVert> *mdir;

	MNChamferData () { mdir=NULL; }
	MNChamferData (const MNMesh & m) { mdir=NULL; InitToMesh(m); }
	DllExport ~MNChamferData ();

	DllExport void InitToMesh (const MNMesh & m);
	DllExport void setNumVerts (int nv, bool keep=TRUE, int resizer=0);
	DllExport void ClearLimits ();
	DllExport void GetDelta (float amount, Tab<Point3> & delta);
	DllExport bool GetMapDelta (MNMesh & mm, int mapChannel, float amount, Tab<UVVert> & delta);
	Tab<UVVert> & MDir (int mapChannel) { return (mapChannel<0) ? hmdir[-1-mapChannel] : mdir[mapChannel]; }
};

// The following function has been added
// in 3ds max 4.2.  If your plugin utilizes this new
// mechanism, be sure that your clients are aware that they
// must run your plugin with 3ds max version 4.2 or higher.
DllExport void MNChamferDataDebugPrint (MNChamferData & mcd, int mapNum);
// End of 3ds max 4.2 Extension

class MNTempData : public BaseInterfaceServer {
private:
	MNEdgeClusters *edgeCluster;
	MNFaceClusters *faceCluster;
	Tab<int> *vertCluster;
	Tab<Point3> *normals;
	Tab<Point3> *centers;
	Tab<Point3> *vnormals;
	Tab<Tab<float> *> *clustDist;
	Tab<float> *selDist;
	Tab<float> *vsWeight;
	MNChamferData *chamData;

	Tab<Point3> *outlineDir;

	MNMesh *mesh;

	//this is used to keep track of the selDist data.  We used to compute all the seldist data which
	//was really slow.  We now only track the selDist that fall within  this limit which can make things
	//much faster.
	float mFalloffLimit;

public:
	DllExport MNTempData ();
	DllExport MNTempData (MNMesh *m);
	DllExport ~MNTempData ();

	void SetMesh (MNMesh *m) { mesh = m; }

	DllExport MNFaceClusters *FaceClusters (DWORD clusterFlags=MN_SEL);
	DllExport MNEdgeClusters *EdgeClusters (DWORD clusterFlags=MN_SEL);
	DllExport Tab<int> *VertexClusters (int sl, DWORD clusterFlags=MN_SEL);
	DllExport Tab<Point3> *ClusterNormals (int sl, DWORD clusterFlags=MN_SEL);
	DllExport Tab<Point3> *ClusterCenters (int sl, DWORD clusterFlags=MN_SEL);
	DllExport Matrix3 ClusterTM (int clust);
	DllExport Tab<Point3> *VertexNormals ();
	DllExport Tab<float> *VSWeight (BOOL useEdgeDist, int edgeIts,
									BOOL ignoreBack, float falloff, float pinch, float bubble,
									DWORD selFlags=MN_SEL);

	//! \brief This computes the distances of the unselected vertices from the selected vertices
	/*!	Computes the current distance of each vertex from the current selection wihtin the falloff. If cached, the 
	    cache is returned. Otherwise a cache is allocated and computed from the current mnmesh and 
		the parameters. The term "Selected verts" below refers to the vertices that are selected 
		in the mesh's current selection level. (See the MNMesh method GetTempSel for details.)
		\param[in] useEdgeDist If TRUE, the distance between vertices is computed along edges. If FALSE, it's computed directly through space. 
		\param[in] edgeIts This indicates the maximum number of edges the algorithm may travel 
					along in finding the distance between vertices. (Maximum path length.).
					WARNING: If useEdgeDist is FALSE, this is an n-squared algorithm: it compares 
					every nonselected vertex with every selected one within the falloffLimit. If useEdgeDist is TRUE, 
					the time it takes is proportional to the number of selected vertices times 
					edgeIts. 
		\param[in] falloffLimit = -1.0f this limits the number of unselected vertices to check against
		           Only vertices that fall within the selected vertice bounding box + this limit will be
				   computed.  If this value is -1.0 all vertcies will be computed.  Any vertex outside this limit
				   will be set to a distance of the limit.
		\return A table consisting of one float value per vertex. If this value is 0, the vertex is either 
					selected or on top of a selected vertex. Otherwise it represents the distance to the closest 
					selected vertex. If useEdgeDist is TRUE, values of -1.0 are returned for vertices with no edgeIts-length 
					path to a selected vertex.
	*/
	DllExport Tab<float> *SelectionDist (BOOL useEdgeDist, int edgeIts, DWORD selFlags=MN_SEL, float falloffLimit = -1.0f);
	DllExport Tab<float> *ClusterDist (int sl, DWORD clusterFlags, int clustId, BOOL useEdgeDist, int edgeIts);
	//DllExport Tab<Point3> *FaceExtDir (int extrusionType);
	DllExport Tab<Point3> *OutlineDir (int extrusionType, DWORD clusterFlags=MN_SEL);

	DllExport MNChamferData *ChamferData();

	DllExport void Invalidate (DWORD part);
	DllExport void InvalidateDistances ();
	DllExport void InvalidateSoftSelection ();
	DllExport void freeClusterDist ();
	DllExport void freeBevelInfo ();
	DllExport void freeChamferData();
	DllExport void freeAll ();
};

DllExport void SelectionDistance (MNMesh & mesh, float *selDist, DWORD selFlags);
DllExport void SelectionDistance (MNMesh & mesh, float *selDist, int iters, DWORD selFlags);
DllExport void ClustDistances (MNMesh & mesh, int numClusts, int *vclust,
							   Tab<float> **clustDist);
DllExport void ClustDistances (MNMesh & mesh, int numClusts, int *vclust,
							   Tab<float> **clustDist, int iters);

// Made into a class to match design rules.
class BasisFinder: public MaxHeapOperators {
public:
	DllExport static void BasisFromZDir (Point3 & zDir, Point3 & xDir, Point3 & yDir);
};

const int kMNNurmsHourglassLimit = 2500;	// between 4 and 5 iterations on a box.

//#define MNM_SELCONV_IGNORE_BACK 0x01	// Doesn't work - can't get good BACKFACING info.
#define MNM_SELCONV_REQUIRE_ALL 0x02

class MNMeshSelectionConverter : public FlagUser {
public:
	DllExport void FaceToElement (MNMesh & mesh, BitArray & faceSel, BitArray & elementSel);
	DllExport void EdgeToBorder (MNMesh & mesh, BitArray & edgeSel, BitArray & borderSel);
	DllExport void VertexToEdge (MNMesh & mesh, BitArray & vertexSel, BitArray & edgeSel);
	DllExport void VertexToFace (MNMesh & mesh, BitArray & vertexSel, BitArray & faceSel);
};

///  \brief 
/// This class is new with Max 7.0.
/// It contains new algorithms we would have put in MNMesh, but can't,
/// because we don't want to break the 6.0 SDK.
/// 
///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly,
/// as well as in the Edit Poly modifier, in maxsdk\samples\mesh\EditPoly 
class MNMeshUtilities: public MaxHeapOperators
{
private:
	MNMesh *mpMesh;

	// Private methods used by public ones:
	// (All Bridge stuff.)
	void GetBorderVertices (Tab<int> & border, Tab<int> & verts, bool reversedLoop=false);
	void GetBorderMapVertices (Tab<int> & border, Tab<int> & mapVerts, int mapChannel, bool reversedLoop=false);
	void InterpolateVertices (int v1, int v2, float f, int outputVert);
	void GetBridgeSmoothingGroups (Tab<int> & smVerts, Tab<int> & lgVerts, Tab<int> & match,
		float smoothThresh, Tab<DWORD> & smoothers);
	MtlID GetBridgeMaterial (Tab<int> & smaller, Tab<int> & larger);
	bool PolygonToBorder (int face, int start, Tab<int> & border);
	bool OrientBorders (Tab<int> & border1, Tab<int> & border2);
	bool SynchBorders (Tab<int> & border1, Tab<int> & border2);
	bool BridgeLoops (Tab<int> & smaller, Tab<int> & larger, float smoothThresh);
	bool BridgeLoops (Tab<int> & smaller, Tab<int> & larger, float smoothThresh,
		int segments, float taper, float bias);
	void MatchLoops (Tab<Tab<int> *> & loopList, Tab<int> & loopMatch,
		Tab<int> *loopClusterID=NULL, Tab<int> *clusterMatch=NULL);

public:
	/// \brief 
	/// Constructor.  Takes MNMesh pointer, which is the mesh that is operated upon
	/// in all MNMeshUtilities methods.
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	MNMeshUtilities (MNMesh *mesh) { mpMesh = mesh; }

	///  \brief 
	/// Given a one-sided edge in the MNMesh, retrieves the border loop which that edge forms with other
	/// one-sided edges.  The border loop produced will be in sequential order going CCW around the hole,
	/// starting with the edge given.
	/// 
	///  \param edge The edge to start from.  (If it is not a border edge, the "border" table will be
	/// left empty.
	///  \param border A reference to a table into which the border loop edge indices can be written.
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly
	DllExport bool GetBorderFromEdge (int edge, Tab<int> & border);

	///  \brief 
	/// Applies new smoothing groups to the MNMesh, smoothing between any faces that share an edge
	/// with an edge angle less than "threshold".
	/// 
	///  \param threshold The threshold angle for smoothing across edges, in radians.
	///  \param faceFlag If nonzero, this indicates that only faces with this flag set
	/// should have their smoothing groups affected.  (If faceFlag has multiple bits set, any flag
	/// matches are considered a match.)
	///  \remark  Similar to MNMesh::AutoSmooth, but allows the use of any flag or flags, not just MN_SEL 
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool AutoSmooth (float threshold, DWORD faceFlag);

	///  \brief 
	/// Indicates whether the hinge angle should be multiplied by -1, based on the local topology and
	/// geometry of the mesh.
	/// When users pick hinge edges, they may pick edges oriented in either direction, with the result
	/// that rotating around the edge by the hinge angle can produce inconsistent results.  This method
	/// analyses the hinge orientation and indicates whether a correction should occur.
	/// 
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool FlipHingeAngle (Point3 & origin, Point3 & axis, int hingeEdge, DWORD faceFlag);

	///  \brief 
	/// New version of old "MNMesh::LiftFaceClusterFromEdge" method which doesn't require a hinge edge.
	/// Does a "hinged extrusion", where selected face clusters are rotated around a hinge axis, and new
	/// faces are created connecting the selected faces to the unselected ones.
	/// 
	///  \param origin  The origin point for the hinge 
	///  \param axis  The axis direction for the hinge 
	///  \param angle  The amount to rotate, in radians 
	///  \param segments  The number of segments for the sides of the extrusion 
	///  \param fclust  The current set of Face Clusters 
	///  \param cluster  The ID of the cluster to rotate 
	///  \remark This extrusion has a slightly different topology from a regular extrusion, in that
	/// no new faces are created for edges between selected and unselected faces that lie exactly along
	/// the hinge.
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool HingeFromEdge (Point3 & origin, Point3 & axis, float angle, int segments,
									  MNFaceClusters & fclust, int clusterID);
	/// \brief 
	/// Determines an average plane of the flagged subobjects, and moves vertices toward it
	/// according to their soft selection value.
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool MakeFlaggedPlanar (int selLev, DWORD flag, float *softSel, Point3 *delta=NULL);

	/// \brief 
	/// Moves soft-selected vertices toward the plane given according to their soft selection value,
	/// or produces an array of offsets which would do so.  (Similar to MNMesh::MoveVertsToPlane,
	/// but with soft selection support.)
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool MoveVertsToPlane (Point3 & norm, float offset, float *softSel, Point3 *delta=NULL);

	///  \brief 
	/// Edge-based collapse function - all edges with the given flag will be collapsed,
	/// which means their vertex endpoints will be joined into a single vertex.
	/// 
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool CollapseEdges (DWORD edgeFlag);

	///  \brief 
	/// Constrains the vectors given for each vertex so that they are limited to travel
	/// along a neighboring edge.  It's acceptable for deltaIn to be equal to deltaOut.
	/// 
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport void ConstrainDeltaToEdges (Tab<Point3> *deltaIn, Tab<Point3> *deltaOut);

	///  \brief 
	/// Constrains the vectors given for each vertex so that they are limited to travel
	/// along a neighboring face.  It's acceptable for deltaIn to be equal to deltaOut.
	/// 
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport void ConstrainDeltaToFaces (Tab<Point3> *deltaIn, Tab<Point3> *deltaOut);

	///  \brief 
	/// Extrude method which creates new faces along open edges.
	/// 
	///  \param edgeFlag  The edges which should be extruded.
	/// (Non-open edges that have this flag set always have it cleared.)
	///  \param clearFlagOnOldEdges If true, the "edgeFlag" is cleared on the edges
	/// used as the base of the extrusion.
	///  \param useTracking If true, the new edges at the "top" of the extrusion have
	/// their "track" data member set to the edge at the base of their extrusion.
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool ExtrudeOpenEdges (DWORD edgeFlag, bool clearFlagOnOldEdges, bool useTracking);

	///  \brief 
	/// Creates a bridge between two border loops
	/// 
	///  \param edge1  An edge on the first border loop 
	///  \param twist1  The twist for the first border loop.  (The edge which is "twist1"
	/// edges around the loop from "edge1" will be bridged to the edge which is "twist2" edges
	/// around the loop from "edge2". 
	///  \param edge2  An edge on the second border loop 
	///  \param twist2  The twist for the second border loop.  (The edge which is "twist1"
	/// edges around the loop from "edge1" will be bridged to the edge which is "twist2" edges
	/// around the loop from "edge2". 
	///  \param smoothThresh  The threshold angle, in radians, for smoothing between successive
	/// columns of the bridge. 
	///  \param segments  The number of segments for the bridge 
	///  \param taper  The amount that the bridge should taper in
	///  \param bias  The bias for the location of the most tapered-in spot on the bridge.
	/// Range: -100 to 100, with 0 indicating that the taper should fall naturally in the middle of the 
	/// bridge.
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool BridgeBorders (int edge1, int twist1, int edge2, int twist2,
		float smoothThresh, int segments, float taper, float bias);

	///  \brief 
	/// Creates a bridge between two polygons.
	/// 
	///  \param face1  The first polygon 
	///  \param twist1  The twist for the first polygon.  (The twist1'th edge of polygon face1
	/// will be bridged to the twist2'th edge on polygon face2.) 
	///  \param face2  The second polygon 
	///  \param twist2  The twist for the second polygon.  (The twist1'th edge of polygon face1
	/// will be bridged to the twist2'th edge on polygon face2.) 
	///  \param smoothThresh  The threshold angle, in radians, for smoothing between successive
	/// columns of the bridge. 
	///  \param segments  The number of segments for the bridge 
	///  \param taper  The amount that the bridge should taper in 
	///  \param bias  The bias for the location of the most tapered-in spot on the bridge.
	/// Range: -100 to 100, with 0 indicating that the taper should fall naturally in the middle of the 
	/// bridge.
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool BridgePolygons (int face1, int twist1, int face2, int twist2,
		float smoothThresh, int segments, float taper, float bias);

	///  \brief 
	/// Creates bridges between polygon clusters.
	/// 
	///  \param polyFlag The flag indicating faces in the clusters.
	///  \param smoothThresh  The threshold angle, in radians, for smoothing between successive
	/// columns of the bridge. 
	///  \param segments  The number of segments for the bridge 
	///  \param taper  The amount that the bridge should taper in 
	///  \param bias  The bias for the location of the most tapered-in spot on the bridge.
	/// Range: -100 to 100, with 0 indicating that the taper should fall naturally in the middle of the 
	/// bridge.
	///  \param twist1  The twist for each "start" cluster. 
	///  \param twist2  The twist for each "end" cluster.
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool BridgePolygonClusters (DWORD polyFlag, float smoothThresh, int segments, float taper, float bias,
		int twist1, int twist2);

	///  \brief 
	/// Creates bridges between flagged borders.
	/// 
	///  \param edgeFlag Any borders with at least one edge that has this flag set will be considered
	/// for bridging. (Only borders that match up in pairs will be bridged, of course.)  
	///  \param smoothThresh  The threshold angle, in radians, for smoothing between successive
	/// columns of the bridge. 
	///  \param segments  The number of segments for the bridge 
	///  \param taper  The amount that the bridge should taper in 
	///  \param bias  The bias for the location of the most tapered-in spot on the bridge.
	/// Range: -100 to 100, with 0 indicating that the taper should fall naturally in the middle of the 
	/// bridge.
	///  \param twist1  The twist for each "start" cluster. 
	///  \param twist2  The twist for each "end" cluster. 
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool BridgeSelectedBorders (DWORD edgeFlag, float smoothThresh, int segments, float taper, float bias,
		int twist1, int twist2);

	///  \brief 
	/// Returns a good default twist for face2, when bridging between face1 and face2 with
	/// no twist for face1.  Default twist is based on an analysis of local geometry.
	/// 
	///  \param face1  The first polygon 
	///  \param face2  The second polygon
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport int FindDefaultBridgeTwist (int face1, int face2);

	///  \brief 
	/// "Turns" the diagonal by removing it and creating a new one going across it.
	/// This is like the traditional "Turn Edge" in Editable Mesh.
	/// (Any "diagonal" in a polygon separates two triangles.  If you remove that diagonal,
	/// you're left with a quad; this algorithm replaces the original diagonal with the
	/// diagonal you'd get by connecting the other two vertices of that quad.)
	/// 
	///  \param face The polygon face in which to turn the diagonal
	///  \param diagonal The index of the diagonal to turn.  A polygon of degree "deg"
	/// has (deg-3) diagonals. 
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool TurnDiagonal (int face, int diagonal);

	///  \brief 
	/// This is a new version of the MNMesh::Relax method, which is designed to move all vertices a little
	/// closer to their neighbors.  Neighbors are defined as vertices that share an edge.  This variation on
	/// the Relax method also accepts a soft selection.
	/// 
	///  \param vertexFlag  If "softSel" is NULL, only vertices with this flag set are relaxed. 
	///  \param softSel  This is an optional soft selection.  It may be NULL.  If not, it
	/// should point to an array of MNMesh::numv floats representing the selection value for each vertex.
	///  \param relaxAmount  This indicates the proportion of relaxing.  If it's zero, nothing happens;
	/// if it's one, each vertex is moved to the average of its neighbors.  Values of less than zero or more than one
	/// are accepted, but lead to unstable results.  The default in Editable Poly is .5f.
	///  \param relaxIters  This is the number of iterations of relax.  This is the number of times the
	/// relaxation is applied.  The more iterations, the more relaxed the result. 
	///  \param holdBoundaryPts  If true, "boundary" points should be held in place, unaffected by the Relax.
	/// "boundary" points are defined as those that are used by more edges than faces, which basically means
	/// they're on some sort of border of the mesh. This is turned on by default in Editable Poly, because
	/// if it's off, boundary points recede into the mesh very quickly.
	///  \param holdOuterPts  If true, "outer" points should be held in place.  "outer" points are defined
	/// as those where the sum of all the face angles is less than 2Pi (360 degrees).  This basically means the
	/// point is some sort of outer corner.  All the points on a 1-segment Box or on a Geosphere are "outer" points.
	/// On a multiple-segment plane, none of the interior points are "outer" points.  This parameter is most
	/// important in models like the Geosphere, which could relax down into nothing if outer points are not
	/// fixed in place.
	///  \param delta  If non-NULL, this should point to an array into which the effect of this relax
	/// should be placed, in the form of a Point3 offset per vertex. 
	///  \return  True if any relaxing occurred, false in cases where nothing happened, for instance because
	/// no vertices were selected.
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport bool Relax (DWORD vertexFlag, float *softSel, float relaxAmount,
		int relaxIters, bool holdBoundaryPts, bool holdOuterPts, Point3 *delta=NULL);

	///  \brief 
	/// Starting with the polygon given, this algorithm creates a BitArray selection of all polygons it can get to
	/// without crossing any edges whose angle is greater than the angle given.
	/// 
	///  \param startPolygon The polygon to start on.  If this polygon is already set in
	/// "polySel", nothing will happen.
	///  \param angle The maximum crossable edge angle, in radians
	///  \param polySel The BitArray where polygons should be selected 
	///  \return The number of newly-selected polygons
	///  \remark This method can be called multiple times with the same polySel parameter, for instance
	/// to turn a set of face hits from hit-testing into a select-by-angle region, and is
	/// designed to be used in Editable Poly and Edit Poly's "Select by angle" feature
	///
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport int SelectPolygonsByAngle (int startPolygon, float angle, BitArray & polySel);

	///  \brief 
	/// This method is used at the beginning of a series of cuts.  It sets things up so that "CutCleanup"
	/// will work.
	/// 
	///  <br><b>Example: </b> See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly
	DllExport void CutPrepare ();

	///  \brief 
	/// This method is used after a series of cuts, to "clean up" any extra edges which are necessary to have
	/// while cutting, but which aren't what the user actually intended to create.  "CutPrepare" should be called
	/// at the beginning of the series of cuts.
	/// 
	///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
	DllExport void CutCleanup ();
};

///  \brief 
/// This class is used to "preserve" mapping coordinates while moving vertices.
/// The "Preserve" name is actually a little confusing; what this class helps us do is modify
/// map vertices such that the overall map on a surface looks about the same, even though the
/// regular vertices have moved.  For instance, if you start with a Plane object, and move the
/// middle vertex to the right, the map is typically distorted even though all the original
/// map vertices are unaffected.  This class will help by adjusting the middle map vertex so
/// that the overall regular texture pattern is preserved.
/// 
///  <br><b>Example: </b>  See example usage in the Editable Poly object, in maxsdk\samples\mesh\EditablePoly 
class MNMapPreserveData: public MaxHeapOperators
{
private:
	Tab<Point3> mFcx, mFcy;	// face corner x and y directions.
	Tab<Point3 *> mMapXDir, mMapYDir;
	int mDegreeSum;
	bool mProtectVertexColors;

public:
	///  \brief 
	/// Constructor.  Initializes to empty data.
	/// 
	MNMapPreserveData () : mDegreeSum(0), mProtectVertexColors(true) { }

	///  \brief 
	/// Destructor.  Frees allocated data.
	/// 
	~MNMapPreserveData () { Clear(); }

	///  \brief 
	/// Clears out all allocated data, and resets to empty.
	/// 
	DllExport void Clear ();

	///  \brief 
	/// Creates caches of data based on the mesh given and the set of maps to be preserved.  Sets the
	/// "initial state" which the maps should try to adhere to.
	/// 
	///  \param mm  The mesh whose maps should be preserved 
	///  \param mapsToDo  A MapBitArray indicating which map channels should be preserved.
	/// See class MapBitArray for more details. 
	DllExport void Initialize (MNMesh & mm, const MapBitArray & mapsToDo);

	///  \brief 
	/// Given a mesh and a set of offsets for vertices, manipulate map channels to preserve the initial UV mapping.
	/// 
	///  \param mm  The mesh being affected.  Note that this algorithm uses the face information in the mesh,
	/// and affects the maps in the mesh, but it doesn't use the vertex information at all.  (All relevant vertex
	/// information was digested in Initialize.)  So this method may be called before or after applying the offsets
	/// given in pDelta. 
	///  \param pDelta  The set of offsets for the vertices. 
	///  \param numDeltas  The length of the pDelta array 
	DllExport void Apply (MNMesh & mm, Point3 *pDelta, int numDeltas);
};


// New interface id for mnmeshutilities8 (max 8 bridge edge methods , lr oct 2004 ) 
#define IMNMESHUTILITIES8_INTERFACE_ID Interface_ID(0xf5235e73, 0x3b304334)


//! \brief IMNMeshUtilities8 extends the MNMeshUtilities class, adding new functionality  applied on a MNMesh.
/*! \n Such an interface is obtained through the call to GetInterface, using the class id: IMNMESHUTILITIES8_INTERFACE_ID 
    \n on an instance of an MNMesh. 
    \n usage is : IMNMeshUtilities8* l_meshToBridge = static_cast<IMNMeshUtilities8*>(mesh.GetInterface( IMNMESHUTILITIES8_INTERFACE_ID ));
    \n 
	\b Edge \b bridging: This is an extension of the existing bridge border method. It bridges 2 sets od edges of different size.
    you can bridge 4 edges with 5 edges. 
	\n Limitations. the 2 edge lists must be disjoint, or at least separated by a sharp angle.
	\n this angular threshhold can be adjusted by using the in_adjacentAngle parameter,
	\n \b Connect \b edges: 2 new parameters are added for better precision: in_pinch and in_slide.
	\n \b SelectEdgeRingShift: selects edges in the ring direction 
*/

class IMNMeshUtilities8 : public BaseInterface
{
	public:
		virtual Interface_ID	GetID() { return IMNMESHUTILITIES8_INTERFACE_ID; }

		/// 
		/// bridges 2 edges 
		/// \return true if the bridging operation was succesfull 
		/// \return false if the bridging operation failed
		/// \pre in_sourceEdge and in_targetEdge must be different 
		/// \pre in_sourceEdge and in_targetEdge must on border
		/// \post the mesh geometry is valid, even if the operation failed
		/// \param[in] in_Edge1 1st edge index to bridge
		/// \param[in] in_Edge2 2nd edge index to bridge
		/// \param[in] in_segmentsNumber number of polys between the edges
		/// \param[in] in_prevColumnIndex internal value, do not use 
		/// \param[in] in_smoothingGroup new face smoothing Group index 
		/// \param[in] in_materialId new face material id 
		/// \param[in] in_triangleAdded internal value, do not use
		/// 

		virtual bool BridgeTwoEdges(	
			const int 	in_Edge1,						
			const int 	in_Edge2,						
			const int	in_segmentsNumber,				
			const int 	in_prevColumnIndex = 0,			
			const DWORD in_smoothingGroup = 0,			
			const MtlID in_materialId = 0,				
			const bool  in_triangleAdded = false) = 0 ;	

		//
		/// Bridges edge selection
		/// \return true if the bridging operation was succesfull 
		/// \return false if the bridging operation failed
		/// \pre in_sourceEdge and in_targetEdge must be different 
		/// \pre in_sourceEdge and in_targetEdge must on border
		/// \post the mesh geometry is valid, even if the operation failed
		/// \param[in] in_edgeFlag edge cluster flag. 
		/// \param[in] in_smoothThresh threshold angle, in radians, for smoothing successive columns ofthe bridge. 
		/// \param[in] in_segmentsNumber number of polys between the edges
		/// \param[in] in_adjacentAngle angular threshold in radians for splitting one contiguous edge list in 2
		///	in order to perform the bridging on 2 separate edge lists
		/// \param[in] in_reverseTriangle reverse triangles's position: if true, triangles will be added at the 
		/// end of the bridge, if they are needed ( when bridging 4 edges with 5 edges. 



		virtual bool BridgeSelectedEdges (	
			const DWORD in_edgeFlag,				
			const float	in_smoothThresh,			
			const int	in_segmentsNumber,						
			const float	in_adjacentAngle,			
			const bool	in_reverseTriangle ) = 0;	

		//
		/// New Edge Connection: 2 new paramters are added, pinch and slide, for better precision
		/// \return true if the connection operation was succesfull 
		/// \return false if the connection operation failed
		/// \pre in_segments must be greater than 1 
		/// \pre in_pinch and in_slide must be between -100 and 100 
		/// \post the mesh geometry is valid, even if the operation failed
		///< \param[in] in_edgeFlag edge cluster flag
		///< \param[in] in_segments number of connection edges between successive selected edges
		///< \param[in] in_pinch disctance between succesive new connection edges. Can be < 0. 
		///< \param[in] in_slide position of the new connection edges, relative to the existing edges. Can be < 0. 

		virtual bool ConnectEdges (
			const DWORD in_edgeFlag,				
			const int	in_segments = 2,			
			const int	in_pinch = 0,				
			const int	in_slide = 0 ) = 0 ;		

		/// edge selection shifting routine: shifts the current edge selection in the ring direction
		/// \pre the io_edgeSel BitArray, must exist with the proper size 
		/// \post the io_edgeSel BitArray contains the list of the shifted edges 
		///< \param[in] in_loop_shift shifting value, can be positive or negative  
		///< \param[in,out] io_edgeSel the new edge selection list

		virtual void SelectEdgeRingShift( 
			const int	in_ring_shift,				
			BitArray&	io_edgeSel) = 0;			

		/// edge selection shifting routine: shifts the current edge selection in the loop direction
		/// \pre the io_edgeSel BitArray must exist with the proper size 
		/// \post the io_edgeSel BitArray contains the list of the shifted edges 
		///< \param[in] in_loop_shift shifting value, can be positive or negative  
		///< \param[in,out] io_edgeSel the new edge selection list

		virtual void SelectEdgeLoopShift( 
			const int	in_loop_shift,				
			BitArray&	io_edgeSel)=0;				
	
		/// vertex splitting method, using a distance value for specifying the distance 
		/// between the split vertices. 
		/// \post the  MNmesh is valid, with the selected vertices ,split apart, using the in_distance. 
		///< \param[in] in_vertexFlag the vertex flag used
		///< \param[in] in_distance to split the vertices apart

		virtual bool SplitFlaggedVertices (
			const DWORD in_vertexFlag	= MN_SEL,		
			const float in_distance		= 0.0 ) =0;		

		/// edge splitting method, using a distance value for specifying the distance 
		/// between the split edges. 
		/// \post the  MNmesh is valid with the selected vertices ,split apart, using the in_distance. 
		///< \param[in] in_vertexFlag the vertex flag used
		///< \param[in] in_distance, to split the selected edges apart

		virtual bool SplitFlaggedEdges (
			const DWORD in_edgeFlag = MN_SEL,		
			const float in_distance = 0.0 )= 0;		

		/// Creates a bitArray indicating the directions of the MNMesh's edges relative to 2 arbitrary edges: 
		/// the 2 first edges of the first face of the MNMesh. For a MNesh made of quads ( like a converted plane),
		/// they are the 2 first edges of the lower left face. 
		/// \pre the array io_edgeFlip does not need to exist
		///< \post the array exists and contains number of edges entries.  
		///< \param[in,out] io_edgeFlip array of flipped edges
		///< \param[in] in_useOnlySelectedEdges if this is true, only selected edges will be tested against the 2 original edges.  

		virtual void GetFlippedEdges(  
			BitArray& io_edgeFlip,	
			const bool in_useOnlySelectedEdges = false) = 0;
		//
		/// New Edge chamfer method: A new paramter is added, allowing the creation of holes when chamfering.
		/// \return true if the chamfer operation was succesfull 
		/// \return false if the chamfer operation failed
		/// \post the mesh geometry is valid, even if the operation failed
		///< \param[in] in_flag the edge flag used 
		///< \param[in] in_mcd the chamfer data
		///< \param[in] in_open If true, the newly created faces are removed, creating a whole between chamfered edges. 

		virtual bool ChamferEdges ( 
			const DWORD			in_flag = MN_SEL,		
			      MNChamferData *in_mcd	= NULL,			
			const bool			in_open = false) = 0;	
		
		//
		/// New Vertex chamfer method  : A new paramter is added, allowing the creation of holes when chamfering.
		/// \return true if the chamfer operation was succesfull 
		/// \return false if the chamfer operation failed
		/// \post the mesh geometry is valid, even if the operation failed
		///< \param[in] in_flag the vertices flag used 
		///< \param[in] in_mcd the chamfer data
		///< \param[in] in_open If true, the newly created faces are removed, creating a whole between chamfered vertices. 

		virtual bool ChamferVertices (
			const DWORD			flag	= MN_SEL,		
			      MNChamferData *mcd	= NULL,			
			const bool			in_open = false) = 0 ;	

		// MNMesh optimization
		//! \brief Check if a face's cache is invalid
		/*!	The function first checks if the whole mesh or the face have been flagged as
			cache invalid (MN_CACHEINVALID). If none of these is true, it then cycles through the face's
			vertices and returns true as soon as it finds a vertex flagged with MN_CACHEINVALID.
			\pre faceIndex must be a valid index in the faces (f) array.
			\param[in] faceIndex The index of the face for which you want to determine cache validity
			\return true if the face is invalid, else false.
		*/
		virtual bool isFaceCacheInvalid(int faceIndex) = 0;

		//! \brief Invalidates a single vertex's cache indicating it needs to be rebuilt.
		/*!	The function flags the specified vertex with the MN_CACHEINVALID flag, telling the rebuild
			functions that this vertex, the faces surrounding it and the other vertices of those faces
			need to have their normals recomputed. It also sets normalsBuilt to 0 to trigger normals' rebuild.
			This function should be called when one to several vertices have been modified. If all or almost
			all vertices of the mesh have been invalidated, it's more efficient to call InvalidateGeomCache() instead.
			\pre vertex must be a valid index in the vertex (v) array.
			\param[in] vertex The index of the vertex that needs to be invalidated.
		*/
		virtual void InvalidateVertexCache (int vertex) = 0;

};


#endif
