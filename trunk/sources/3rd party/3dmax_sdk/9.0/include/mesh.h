//**************************************************************************/
// Copyright (c) 1998-2006 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// FILE:        mesh.h
// DESCRIPTION: Main include file for triangle meshes.
// AUTHOR:      Don Brittain
// HISTORY:     
//**************************************************************************/

#ifndef _MESH_H_

#define _MESH_H_

#include "maxheap.h"
#include "channels.h"
#include "snap.h"
#include <ioapi.h>
#include "export.h"
#include "vedge.h"  //DS
#include "utillib.h"
#include "tab.h"
#include "baseinterface.h"
#include "channels.h"

#include "assert.h"

// These typedefs must be the same as each other, since
// vertex colors are contained in a MeshMap.
typedef Point3 UVVert;
typedef Point3 VertColor;

#define MESH_USE_EXT_CVARRAY	(-32767)

#define MESH_MULTI_PROCESSING	TRUE		// TRUE turns on mp vertex transformation

#define MESH_CAGE_BACKFACE_CULLING	// for "cage" orange gizmo meshes in EMesh, EPoly, Mesh Select, etc.

class ISave;
class ILoad;
class IHardwareShader;
class TriStrip;
class MeshNormalSpec;

#define NEWMESH

class RNormal: public MaxHeapOperators {
	public:
		RNormal()	{ smGroup = mtlIndex = 0; }
		void		setNormal(const Point3 &nor) { normal = nor; }
		void		addNormal(const Point3 &nor) { normal += nor; }	
		void		normalize(void) 	{ normal = Normalize(normal); }
		Point3 &	getNormal(void) 	{ return normal; }
      inline const Point3& getNormal() const { return normal; }
		void		setSmGroup(DWORD g)	{ smGroup = g; }
		void		addSmGroup(DWORD g) { smGroup |= g; }
		DWORD		getSmGroup(void)	{ return smGroup; }
		void		setMtlIndex(MtlID i){ mtlIndex = i; }
		MtlID		getMtlIndex(void)	{ return mtlIndex; }
		void		setRGB(Point3 &clr)	{ rgb = clr; };
		Point3 &	getRGB(void)		{ return rgb; }
      inline const Point3& getRGB() const { return rgb; }
		
	private:	
		Point3		normal;	   
		DWORD		smGroup;    
		MtlID		mtlIndex;   
		Point3		rgb;	   
	};					   

// RVertex flags: contain clip flags, number of normals at the vertex
// and the number of normals that have already been rendered.
// fine PLANE_MASK	0x00003f00UL -- now in gfx.h
#define NORCT_MASK			0x000000ffUL
#define SPECIFIED_NORMAL	0x00004000UL
#define OUT_LEFT			0x00010000UL
#define OUT_RIGHT			0x00020000UL
#define OUT_TOP				0x00040000UL
#define OUT_BOTTOM			0x00080000UL
#define RECT_MASK			0x000f0000UL
#define RND_MASK			0xfff00000UL
#define RND_NOR0			0x00100000UL
#define RND_NOR(n)  		(RND_NOR0 << (n))

class RVertex: public MaxHeapOperators {
	public:
      enum { 
         MAX_NUM_NORMALS  = 0xff
      };

   public:
		RVertex()	{ rFlags = 0; ern = NULL; }
		DllExport ~RVertex();	

		DWORD		rFlags;     
		int			pos[3];	
		RNormal		rn;		   
		RNormal 	*ern;		 
	};					  

class Face: public MaxHeapOperators {	
	public:
		DWORD	v[3];
		DWORD	smGroup;
		DWORD	flags;

		Face()	{ smGroup = flags = 0; }
		MtlID	getMatID() {return (int)((flags>>FACE_MATID_SHIFT)&FACE_MATID_MASK);}
		void    setMatID(MtlID id) {flags &= 0xFFFF; flags |= (DWORD)(id<<FACE_MATID_SHIFT);}
		void	setSmGroup(DWORD i) { smGroup = i; }
		DWORD	getSmGroup(void)	{ return smGroup; }
		DllExport void	setVerts(DWORD *vrt);
		void	setVerts(int a, int b, int c)  { v[0]=a; v[1]=b; v[2]=c; }
		DllExport void	setEdgeVis(int edge, int visFlag);
		DllExport void    setEdgeVisFlags(int va, int vb, int vc); 
		int		getEdgeVis(int edge){ return flags & (VIS_BIT << edge); }
		DWORD	getVert(int index)	{ return v[index]; }
		DWORD *	getAllVerts(void)	{ return v; }
		BOOL	Hidden() {return flags&FACE_HIDDEN?TRUE:FALSE;}
		void	Hide() {flags|=FACE_HIDDEN;}
		void	Show() {flags&=~FACE_HIDDEN;}
		void	SetHide(BOOL hide) {if (hide) Hide(); else Show();}

		BOOL	InForeground() {return flags&FACE_INFOREGROUND?TRUE:FALSE;}
		void	PushToForeground() {flags|=FACE_INFOREGROUND;}
		void	PushToBackground() {flags&=~FACE_INFOREGROUND;}
		void	SetInForegound(BOOL foreground) {if (foreground) PushToForeground(); else PushToBackground();}

		BOOL	IsBackFacing() {return flags&FACE_BACKFACING?TRUE:FALSE;}
		void	SetBackFacing() {flags|=FACE_BACKFACING;}
		void	SetFrontFacing() {flags&=~FACE_BACKFACING;}
		void	SetBackFacing(BOOL backFacing) {if (backFacing) SetBackFacing(); else SetFrontFacing();}



		DllExport DWORD GetOtherIndex (DWORD v0, DWORD v1);
		DllExport DWORD GetEdgeIndex (DWORD v0, DWORD v1);
		DllExport int Direction (DWORD v0, DWORD v1);
		DllExport DWORD GetVertIndex (DWORD v0);
		DllExport void OrderVerts (DWORD & v0, DWORD & v1);	// switches v0,v1 if needed to put them in face-order.
	};


// This is used both for UVWs and color verts
class TVFace: public MaxHeapOperators {
public:
	DWORD	t[3];  // indices into tVerts

	TVFace() {}
	TVFace(DWORD a, DWORD b, DWORD c) {t[0]=a; t[1]=b; t[2]=c;}
	DllExport void	setTVerts(DWORD *vrt);
	void	setTVerts(int a, int b, int c)  { t[0]=a; t[1]=b; t[2]=c; }	
	DWORD	getTVert(int index)	{ return t[index]; }
	DWORD *	getAllTVerts(void)	{ return t; }

	DllExport DWORD GetVertIndex (DWORD v0);
	DllExport DWORD GetOtherIndex (DWORD v0, DWORD v1);
	DllExport int Direction (DWORD v0, DWORD v1);
	DllExport void OrderVerts (DWORD & v0, DWORD & v1);	// switches v0,v1 if needed to put them in face-order.
};


// MeshMap stuff:

#define MAX_MESHMAPS 100

#define MESHMAP_USED 0x0001
#define MESHMAP_TEXTURE 0x0002
#define MESHMAP_VERTCOLOR 0x0004
#define MESHMAP_USER 0x0100

#define NUM_HIDDENMAPS 2
// indexes for hidden maps are -1-(their position in the hidden map array).
// (These negative indexes are valid for methods such as mapSupport, mapVerts, etc.)
#define MAP_SHADING -1
#define MAP_ALPHA -2
//#define MAP_NORMALS -3

class MeshMap: public MaxHeapOperators {
public:
	DWORD flags;
	UVVert *tv;
	TVFace *tf;
	int vnum, fnum;

	MeshMap () { flags=0x0; tv=NULL; tf=NULL; vnum = fnum = 0; }
	DllExport ~MeshMap ();

	int getNumVerts () { return vnum; }
	DllExport void setNumVerts (int vn, BOOL keep=FALSE);
	int getNumFaces () { return fnum; }
	DllExport void setNumFaces (int fn, BOOL keep=FALSE, int oldCt=0);
	DllExport void Clear ();
	DllExport BitArray GetIsoVerts ();
	DllExport void DeleteVertSet (BitArray set, BitArray *delFace=NULL);
	DllExport void DeleteFaceSet (BitArray set, BitArray *isoVert=NULL);

	void SetFlag (DWORD fl) { flags |= fl; }
	void ClearFlag (DWORD fl) { flags &= ~fl; }
	BOOL GetFlag (DWORD fl) { return (flags & fl) ? TRUE : FALSE; }
	BOOL IsUsed () const { return (flags & MESHMAP_USED) ? TRUE : FALSE; }

	DllExport void SwapContents (MeshMap & from);
	DllExport MeshMap & operator= (MeshMap & from);
};

// Usually returns TEXMAP_CHANNEL or VERTCOLOR_CHANNEL:
DllExport DWORD MapChannelID (int mp);
// Usually returns TEXMAP_CHAN_NUM, etc:
DllExport int MapChannelNum (int mp);

///  \brief 
/// This class can be used to store a bit per map channel,
/// including both negative and positive map channels, and it works with any
/// size index.
/// 
class MapBitArray: public MaxHeapOperators {
private:
	bool mTexture, mColor;
	BitArray mTextureFlip, mColorFlip;

public:
	///  \brief 
	/// Constructor.  Sets all channels to false.
	/// 
	MapBitArray () : mTexture(false), mColor(false) { }

	///  \brief 
	/// Constructor.  Sets all channels to the given default value.
	/// 
	MapBitArray (bool defaultValue)
		: mTexture(defaultValue), mColor(defaultValue) { }

	///  \brief 
	/// Constructor.  Sets all texture channels (1 and above) to the texture default,
	/// and all color channels (0 and below) to the color default.
	/// 
	MapBitArray (bool textureDefault, bool colorDefault)
		: mTexture(textureDefault), mColor(colorDefault) { }

	///  \brief 
	/// Returns the highest index that this MapBitArray could have a non-default value for.
	/// All indices above this are guaranteed to have value "TextureDefault()".
	/// 
	int Largest() const { return (mTextureFlip.GetSize()>1) ? mTextureFlip.GetSize()-1 : 0; }

	///  \brief 
	/// Returns the lowest index that this MapBitArray could have a non-default value for.
	/// All indices below this are guaranteed to have value "ColorDefault()".
	/// 
	int Smallest() const { return (mColorFlip.GetSize()>1) ? 1-mColorFlip.GetSize() : 0; }

	///  \brief 
	/// Sets the value of the given channel.
	/// 
	DllExport void Set (int mapChannel, bool val=true);

	///  \brief 
	/// Clears the value of the given channel.
	/// 
	void Clear (int mapChannel) { Set (mapChannel, false); }

	///  \brief 
	/// Gets the value of the given channel.  (If this particular channel has
	/// not been set before, the default for the channel will be returned.)
	/// 
	DllExport bool Get (int mapChannel) const;

	///  \brief 
	/// Returns the default value for texture channels, channels 1 and above.
	/// 
	bool TextureDefault () const { return mTexture; }

	///  \brief 
	/// Returns the default value for color channels, channels 0 and below.
	/// 
	bool ColorDefault () const { return mColor; }

	///  \brief 
	/// Inverts the value of all texture channels (1 and above).
	/// 
	void InvertTextureChannels () { mTexture=!mTexture; }

	///  \brief 
	/// Inverts the value of all color channels (0 and below).
	/// 
	void InvertColorChannels () { mColor=!mColor; }

	///  \brief 
	/// Inverts all channel values.
	/// 
	void InvertAll () { mColor = !mColor; mTexture = !mTexture; }

	///  \brief 
	/// Saves data to stream.
	/// 
	DllExport IOResult Save(ISave* isave);

	///  \brief 
	/// Loads data from stream.
	/// 
	DllExport IOResult Load(ILoad* iload);

	///  \brief 
	/// Indexing operator
	/// 
	bool operator[](int i) const { return Get(i); }

	///  \brief 
	/// Comparison operator.
	/// 
	DllExport BOOL operator==(const MapBitArray& b) const;

	///  \brief 
	/// Inequality operator.
	/// 
	BOOL operator!=(const MapBitArray& b) const { return !(*this == b); }

	///  \brief 
	/// Assignment operator
	/// 
	DllExport MapBitArray& operator=(const MapBitArray& b);
};

// Following is used for arbitrary per-element info in meshes, such as weighted verts
// or weighted vert selections.  Methods are deliberately made to look like Tab<> methods.

// For per-vertex info: set a maximum, and reserve first ten channels
// for Discreet's use only.
#define MAX_VERTDATA 100
#define VDATA_USER 10	// Third parties should use this channel or higher.

// Indices of important per-vertex data
#define VDATA_SELECT  0   // Soft Selection
#define VDATA_WEIGHT  1   // Vertex weights (for NURMS MeshSmooth)
#define VDATA_ALPHA   2   // Vertex Alpha values
#define VDATA_CORNER  3	// NOT USED

// Related constants:
#define MAX_WEIGHT ((float)1e5)
#define MIN_WEIGHT ((float)1e-5)

// Types of data
#define PERDATA_TYPE_FLOAT 0

// Vertex-specific methods:
DllExport int VertexDataType (int vdID);
DllExport void *VertexDataDefault (int vdID);

class PerData: public MaxHeapOperators {
public:
	int dnum, type, alloc;
	void *data;

	PerData () { data=NULL; dnum=0; alloc=0; type=0; }
	PerData (int n, int tp) { data=NULL; dnum=0; alloc=0; type=tp; setAlloc (n, FALSE); }
	~PerData () { Clear (); }

	// Following only depend on type:
	DllExport void *AllocData (int num);
	DllExport void FreeData (void *addr);
	DllExport int DataSize ();
	void *Addr (void *ptr, int at) { BYTE *vd=(BYTE *)ptr; return (void *)(vd+at*DataSize()); }
	void *Addr (int at) { return Addr(data,at); }
	DllExport void CopyData (void *to, void *from, int num=1);
	void CopyData ( int to,  int from, int num=1) { CopyData (Addr(to), Addr(from), num); }
	DllExport void WeightedSum (void *to, void *fr1, float prop1, void *fr2, float prop2);
	void WeightedSum (int to, int fr1, float prop1, int fr2, float prop2) { WeightedSum (Addr(to), Addr(fr1), prop1, Addr(fr2), prop2); }

	DllExport void setAlloc (int num, BOOL keep=TRUE);
	void SetCount (int num, BOOL keep = FALSE) { setAlloc (num, keep); dnum=num; }
	void Shrink () { if (alloc>dnum) setAlloc(dnum); }
	int Count () { return dnum; }
	DllExport void Clear ();
	DllExport void DeleteSet (BitArray del);
	DllExport void Delete (int at, int num);
	DllExport void Insert (int at, int num, void *el);
	DllExport void Append (int num, void *el);
	DllExport void InsertCopies (int at, int num, void *el);
	DllExport void AppendCopies (int num, void *el);

	DllExport void SwapContents (PerData & from);
	DllExport PerData & operator= (const PerData & from);
	DllExport void MyDebugPrint ();
};

// Mesh::flags definitions
/*! \defgroup Mesh, Vertex and Face Flags
The following represent flags describing various flags that can be set on the mesh, vertices and faces.  Not all
flags can be applied to each sub object type.
*/
//@{
#define MESH_EDGE_LIST     (1<<1)			//!< Set this to prevent renderData from being deleted (except when mesh is deleted)
#define MESH_LOCK_RENDDATA (1<<2)
#define MESH_SMOOTH_BIT1   (1<<3)
#define MESH_SMOOTH_BIT2   (1<<4)
#define MESH_SMOOTH_BIT3   (1<<5)
#define MESH_SMOOTH_BIT4   (1<<6)
#define MESH_SMOOTH_MASK   0x78				//!< Face? flag mask for SMOOTH_BIT's 1 thru 4
#define MESH_BEEN_DSP	   (1<<9)			//!< Mesh? flag that marks the mesh as has been drawn I think
#define MESH_SMOOTH_SUBSEL (1<<10)
#define MESH_FACENORMALSINVALID  (1<<11)	//!< Mesh flag that Signals that face normals should be rebuilt
#define MESH_CACHEINVALID  (1<<12)			//!< Mesh, vertices and Face flag Signals that the they have changed and RVertices normals should be rebuilt 
#define MESH_BACKFACESCOMPUTED (1<<16)		//!< Mesh flag to determine whether the back facing attribute on the face list has been computed or not
#define MESH_PARTIALCACHEINVALID (1<<17)	//!< Mesh flag that means that the mesh has only set some of the vertices as invalid and not to reprocess the entire mesh just the vertices that changed
//@}

// For internal use only!
#define MESH_TEMP_1 (1<<13)
#define MESH_TEMP_2 (1<<14)
#define MESH_DONTTRISTRIP (1<<15)	// Mesh flag to determine whether to use tristripping or not



#define COMP_TRANSFORM	0x0001	// forces recalc of model->screen transform; else will attempt to use cache
#define COMP_IGN_RECT	0x0002	// forces all polys to be rendered; else only those intersecting the box will be
#define COMP_LIGHTING	0x0004	// forces re-lighting of all verts (as when a light moves); else only relight moved verts

#define COMP_ALL		0x00ff

// If this bit is set then the node being displayed by this mesh is selected.
// Certain display flags only activate when this bit is set.
#define COMP_OBJSELECTED	(1<<8)


class StripData: public MaxHeapOperators {
public:
	int ct;
	DWORD f[6];
	void AddFace(DWORD face)
		{ if(ct < 6) f[ct++] = face; }
};

typedef int (*INTRFUNC)();

DllExport void setMeshIntrFunc(INTRFUNC fn);


class MeshSubHitRec: public MaxHeapOperators {
	private:		
		MeshSubHitRec *next;
	public:
		DWORD	dist;
		int		index;
		DWORD	flags;

		MeshSubHitRec(DWORD dist, int index, MeshSubHitRec *next) 
			{this->dist = dist; this->index = index; this->next = next;}
		MeshSubHitRec(DWORD dist, int index, DWORD flags, MeshSubHitRec *next) 
			{this->dist = dist; this->index = index; this->next = next;this->flags = flags;}

		MeshSubHitRec *Next() { return next; }		
	};

class SubObjHitList: public MaxHeapOperators {
	private:
		MeshSubHitRec *first;
	public:
		SubObjHitList() { first = NULL; }
		DllExport ~SubObjHitList();
		MeshSubHitRec *First() { return first; }
		DllExport void AddHit( DWORD dist, int index );
	};



// Flags for sub object hit test

// NOTE: these are the same bits used for object level.
#define SUBHIT_SELONLY		(1<<0)
#define SUBHIT_UNSELONLY	(1<<2)
#define SUBHIT_ABORTONHIT	(1<<3)
#define SUBHIT_SELSOLID		(1<<4)

#define SUBHIT_USEFACESEL	(1<<23)   // When this bit is set, the sel only and unsel only tests will use the faces selection when doing a vertex level hit test
#define SUBHIT_VERTS		(1<<24)
#define SUBHIT_FACES		(1<<25)
#define SUBHIT_EDGES		(1<<26)
#define SUBHIT_TYPEMASK		(SUBHIT_VERTS|SUBHIT_FACES|SUBHIT_EDGES)

// Selection level bits.
#define MESH_OBJECT		(1<<0)
#define MESH_VERTEX		(1<<1)
#define MESH_FACE		(1<<2)
#define MESH_EDGE		(1<<3)

// Normal Display flags
#define MESH_DISP_NO_NORMALS		0
#define MESH_DISP_FACE_NORMALS		(1<<0)
#define MESH_DISP_VERTEX_NORMALS	(1<<1)


class MeshOpProgress;
class UVWMapper;

class MeshRenderData: public MaxHeapOperators {
	public:
	virtual void DeleteThis()=0;
	};

class AdjFaceList;

class Mesh : public BaseInterfaceServer {
	friend class Face;
	friend class MeshAccess;
	friend class HardwareMesh;
	friend void gfxCleanup(void *data);

	private:

#if MESH_MULTI_PROCESSING
		static int		refCount;
		static HANDLE	xfmThread;
		static HANDLE	xfmMutex;
		static HANDLE	xfmStartEvent;
		static HANDLE	xfmEndEvent;
		friend DWORD WINAPI xfmFunc(LPVOID ptr);
		static HANDLE	fNorThread;
		static HANDLE	fNorMutex;
		static HANDLE	fNorStartEvent;
		static HANDLE	fNorEndEvent;
		friend DWORD WINAPI fNorFunc(LPVOID ptr);

		static HANDLE	workThread;
		static HANDLE	workMutex;
		static HANDLE	workStartEvent;
		static HANDLE	workEndEvent;
		friend DWORD WINAPI workFunc(LPVOID ptr);
#endif
		// derived data-- can be regenerated
		RVertex 		*rVerts;		// <<< instance specific.
		GraphicsWindow 	*cacheGW;  		// identifies rVerts cache
		Point3	 		*faceNormal;	// object space--depends on geom+topo
		Box3			bdgBox;			// object space--depends on geom+topo
		int 			numVisEdges;	// depends on topo 
		int				edgeListHasAll;	// depends on topo
		VEdge 			*visEdge;		// depends on topo 	
 
 		// Vertex and face work arrays -- for snap code
		int			snapVCt;
		int			snapFCt;
		char		*snapV;
		char		*snapF;

		// A simple edge list used so we can draw edges fast in object or vertex sub object mode
		Tab<DWORD>	mDisplayEdges;
		//these are counter of various derived data so we can track when to delete them when they become unused
		UINT		mDXMeshCacheDrawCount;  //this the number of times that the direct mesh cache has been used to draw the mesh exclusively
		UINT		mRedrawCountFromLoad;  //this is the number of times the mesh has been redrawn from load.  This is used to prevent an initial memory spike on load
											//this will be set to -1 if the mesh was created and not loaded

		// Reserved maps for special purposes, such as vertex shading.
		MeshMap hmaps[NUM_HIDDENMAPS];

		// -------------------------------------
		//
		long   		flags;		  	// work flags- 

		float 		norScale;	    // scale of normals -- couldn't this be done
		 							// automatically relative to bdgBox?




		// Rolf: these are instance specific and should be pulled out of here,
		// and just passed in from the Node.
		BYTE		dspNormals;    // display surface normals--- put in flags?
		BYTE		dspAllEdges;   // shows hidden edges  ---- put in flags?
		BYTE		dspVertTicks;  // shows vertex ticks

		//! \brief This only draws the selected edges used to draw them over the directX cache 
		/*!	This draws the selected edges of the mesh.  Backface culling is not supported
		\param[in] gw The graphics window associated with the draw.
		*/
		void	DrawOnlySelectedEdges(GraphicsWindow *gw);

		//! \brief This only draws the selected faces used to draw them over the directX cache 
		/*!	This draws the selected faces of the mesh.  Backface culling is not supported
		\param[in] gw The graphics window associated with the draw.
		*/
		void	DrawOnlySelectedFaces(GraphicsWindow *gw);

		int 		renderFace(GraphicsWindow *gw, DWORD index, int *custVis=NULL);
		int			renderEdge(GraphicsWindow *gw, DWORD face, DWORD edge);
		int			renderFaceVerts(GraphicsWindow *gw, DWORD index);
		void		renderStrip(GraphicsWindow *gw, Strip *s);
		void 		render3DFace(GraphicsWindow *gw, DWORD index, int *custVis=NULL);
		void		render3DFaceVerts(GraphicsWindow *gw, DWORD index);
		void		render3DStrip(GraphicsWindow *gw, Strip *s);
		void		render3DWireStrip(GraphicsWindow *gw, Strip *s);
		BOOL		CanDrawStrips(DWORD rndMode, Material *mtl, int numMtls);
		BOOL		NormalsMatchVerts();
		void		checkRVertsAlloc(void);
		void  		calcNormal(int i);
		void		buildFaceNormals();		// calcs just the face normals
		void		setCacheGW(GraphicsWindow *gw)	{ cacheGW = gw; }
		GraphicsWindow *getCacheGW(void)			{ return cacheGW; }

		// New Mesh routines to drive HardwareShaders
		bool		CanDrawTriStrips(DWORD rndMode, int numMtls, Material *mtl);
		bool		BuildTriStrips(DWORD rndMode, int numMtls, Material *mtl);
		void		TriStripify(DWORD rndMode, int numTex, TVFace *tvf[], TriStrip *s, StripData *sd, int vtx);
		void 		Draw3DTriStrips(IHardwareShader *phs, int numMat, Material *ma);
		void 		Draw3DWireTriStrips(IHardwareShader *phs, int numMat, Material *ma);
		void 		Draw3DVisEdgeList(IHardwareShader *phs, DWORD flags);
		int			render3DTriStrips(IHardwareShader *phs, int kmat, int kstrips);
		int			render3DWireTriStrips(IHardwareShader *phs, int kmat, int kstrips);
		int 		render3DFaces(IHardwareShader *phs, DWORD index, int *custVis=NULL);

		void 		freeVerts();
		void        freeVertCol();
		void  		freeFaces();
		void		freeFaceNormals();  
		void  		freeRVerts(BOOL forceDelete=FALSE);
		void  		freeTVerts();
		void  		freeTVFaces();
		void  		freeVCFaces();
		void		freeSnapData();
		int			buildSnapData(GraphicsWindow *gw,int verts,int edges);
		void		ComputeBackFaceFlags(GraphicsWindow *gw);

		//Just some temporary structs to hold a list of only changed faces and verts.  They are used when building normals.
		BitArray	mInvalidVert;
		BitArray	mInvalidFace;
		Tab<int>	mInvalidVertList;
		Tab<int>	mInvalidFaceList;
		//Frees one rvert in the list
		void		FreeRVert(int index);
		void		ComputeFaceAngle(int index, float *angles);
		void		ComputeRVert(int index, float *faceAngles);

	public:

		// Topology
		int			numVerts;
		int	 		numFaces;
		Face *		faces;

		// Geometry
		Point3 *	verts;		

		// Texture Coord assignment 
		int			numTVerts;
		UVVert *	tVerts;
		TVFace *	tvFace;  	 

		// Color per vertex
		int numCVerts;
		VertColor * vertCol;
		TVFace *    vcFace;

		int			curVCChan;	// current map channel to use for colors (default = 0)
		VertColor *	curVCArray;	// possible external color array (default = NULL)
		TVFace	  * curVCFace;	// possible external face array (default = NULL)

		// for rendering, the color values come from the vertColArray variable.  This array defaults
		// to the internal vertCol, but can be set to an external array, or a map channel
		VertColor * vertColArray;
		// for rendering, the vertex lookup comes from the vcFaceData structure.  This defaults to
		// the vcFace data, but if a map channel is used for color lookup, we use its TVFace structure
		TVFace *	vcFaceData;

		// More maps:
		int numMaps;
		MeshMap *maps;

		// Per-vertex info (of any kind):
		BitArray vdSupport;
		PerData *vData;

		// Material assignment
		MtlID		mtlIndex;		// object material

		// Selection
		BitArray	vertSel;  		// selected vertices
		BitArray	faceSel;  		// selected faces
		BitArray	edgeSel;		// selected edges, identified as 3*faceIndex + edgeIndex
		BitArray	vertHide;		// Hide flags for vertices

		// Display attribute flags
		DWORD		dispFlags;

		// Selection level
		DWORD		selLevel;

		// true if normals have been built for the current mesh
		// SCA revision for Max 5.0 - value of 0 for unbuilt, 1 for built according to legacy 4.0 scheme,
		// and 2 for built according to new scheme - this is so that we refresh normals if the user changes
		// the "Use Legacy 4.0 Vertex Normals" checkbox.
		int			normalsBuilt;

		MeshRenderData*	 renderData;  // used by the renderer

		// derived data-- can be regenerated
		StripTab		*stab;		// depends on topo
		DWTab		norInd;			// indirection array for fast normal lookup
		int			normalCount;	// total number of normals
		Point3 *	gfxNormals;		// flattened list of normals pointers back in the rverts data.  We need to get rid of this or something this is big memory eater

		//Builds the GFX Normals
		void		BuildGFXNormals();

		// Derived arrays to contain generated texture coordinates
		int			numTexCoords[GFX_MAX_TEXTURES];
		Point3 *	texCoords[GFX_MAX_TEXTURES];

		// Derived table of TriStrips, depends on topology
		Tab<TriStrip *>	*tstab;

		DllExport Mesh();
		DllExport Mesh(const Mesh& fromMesh);
		DllExport ~Mesh(); 
		DllExport void Init();
		DllExport void DeleteThis();

		DllExport Mesh& operator=(const Mesh& fromMesh);
		
		DllExport BOOL 	setNumVerts(int ct, BOOL keep=FALSE, BOOL synchSel=TRUE);
		int				getNumVerts(void) const	{ return numVerts; }
		
		DllExport BOOL	setNumFaces(int ct, BOOL keep=FALSE, BOOL synchSel=TRUE);
		int				getNumFaces(void) const{ return numFaces; }
		
		// Original mapping coordinates (map channel 1)
		DllExport BOOL	setNumTVerts(int ct, BOOL keep=FALSE);
		int				getNumTVerts(void) const { return numTVerts; }
		DllExport BOOL 	setNumTVFaces(int ct, BOOL keep=FALSE, int oldCt=0);

		// Color per vertex array (map channel 0)
		// these methods only affect the vertColArray, even when the vertex colors
		// come from a different array (as set by the setCVertArray method below)
		DllExport BOOL 	setNumVertCol(int ct,BOOL keep=FALSE);
		int             getNumVertCol() const {return numCVerts;}
		DllExport BOOL 	setNumVCFaces(int ct, BOOL keep=FALSE, int oldCt=0);

		// To use a different source array for displaying vertex color data:
		//  -- to use a different map channel, call with args: mapChanNum, NULL, NULL
		//  -- to use an external array, call with: MESH_USE_EXT_CVARRAY, vcArray, face_data_if_available
		//     (if no face array is supplied, then we will use the internal vertex color face array)
		//  -- to revert to the internal color vert array, call with "0 , NULL, NULL" (or no args)

		// This method would typically be called right before display, as with a node display callback, 
		// or through an extension object.
		DllExport void	setVCDisplayData(int mapChan = 0, VertColor *VCArray=NULL, TVFace *VCf=NULL);

		// For mp in following: 0=vert colors, 1=original TVerts, 2&up = new map channels
		DllExport void setNumMaps (int ct, BOOL keep=FALSE);
		int getNumMaps () const { return numMaps; }

		DllExport BOOL mapSupport (int mp) const;
		DllExport void setMapSupport (int mp, BOOL support=TRUE);
		DllExport void setNumMapVerts (int mp, int ct, BOOL keep=FALSE);
		DllExport int getNumMapVerts (int mp) const;
		DllExport void setNumMapFaces (int mp, int ct, BOOL keep=FALSE, int oldCt=0);

		DllExport UVVert *mapVerts (int mp) const;
		DllExport TVFace *mapFaces (int mp) const;
		void		setMapVert (int mp, int i, const UVVert&xyz) { if (mapVerts(mp)) mapVerts(mp)[i] = xyz; }
		DllExport void MakeMapPlanar (int mp);	// Copies mesh topology, vert locations into map.
		DllExport BitArray GetIsoMapVerts (int mp);
		DllExport void DeleteMapVertSet (int mp, BitArray set, BitArray *fdel=NULL);
		DllExport void DeleteIsoMapVerts ();	//	 do all active maps
		DllExport void DeleteIsoMapVerts (int mp);
		DllExport void freeMapVerts (int mp);
		DllExport void freeMapFaces (int mp);
		MeshMap & Map(int mp) { return (mp<0) ? hmaps[-1-mp] : maps[mp]; }

		DllExport void setNumVData (int ct, BOOL keep=FALSE);
		int getNumVData () const { return vdSupport.GetSize(); }

		DllExport BOOL vDataSupport (int vd) const;
		DllExport void setVDataSupport (int vd, BOOL support=TRUE);
		void *vertexData (int vd) const { return vDataSupport(vd) ? vData[vd].data : NULL; }
		float *vertexFloat (int vd) const { return (float *) vertexData (vd); }
		DllExport void freeVData (int vd);
		DllExport void freeAllVData ();

		// Two specific vertex scalars.
		float *getVertexWeights () { return vertexFloat(VDATA_WEIGHT); }
		void SupportVertexWeights () { setVDataSupport (VDATA_WEIGHT); }
		void ClearVertexWeights() { setVDataSupport (VDATA_WEIGHT, FALSE); }
		void freeVertexWeights () { freeVData (VDATA_WEIGHT); }
		float *getVSelectionWeights () { return vertexFloat(VDATA_SELECT); }
		void SupportVSelectionWeights () { setVDataSupport (VDATA_SELECT); }
		void ClearVSelectionWeights() { setVDataSupport (VDATA_SELECT, FALSE); }
		void freeVSelectionWeights () { freeVData (VDATA_SELECT); }

		// these flags are restricted to 4 bits and force the topology (strips & edges)
		// to be invalidated when they change.  Used by primitives with smoothing checkboxes
		DllExport void  setSmoothFlags(int f);
		DllExport int   getSmoothFlags();

		void		setVert(int i, const Point3 &xyz)	{ verts[i] = xyz; }
		void		setVert(int i, float x, float y, float z)	{ verts[i].x=x; verts[i].y=y; verts[i].z=z; }
		void		setTVert(int i, const UVVert &xyz)	{ tVerts[i] = xyz; }
		void		setTVert(int i, float x, float y, float z)	{ tVerts[i].x=x; tVerts[i].y=y; tVerts[i].z=z; }
		
		DllExport void		setNormal(int i, const Point3 &xyz); 
		DllExport Point3 &	getNormal(int i) const; // mjm - made const - 2.16.99

		void		setFaceNormal(int i, const Point3 &xyz) { faceNormal[i] =  xyz; }
		Point3 &	getFaceNormal(int i) { return faceNormal[i]; }

		Point3 &	getVert(int i)		{ return verts[i];  }
		Point3 *	getVertPtr(int i)	{ return verts+i; }
		UVVert &	getTVert(int i)		{ return tVerts[i];  }
		UVVert *	getTVertPtr(int i)	{ return tVerts+i; }
		RVertex &	getRVert(int i)		{ return rVerts[i]; }
		RVertex *	getRVertPtr(int i)	{ return rVerts+i; }
		
		void		setMtlIndex(MtlID	i)	{ mtlIndex = i; }
		MtlID		getMtlIndex(void) 		{ return mtlIndex; }

		// Face MtlIndex access methods;
	    DllExport MtlID		getFaceMtlIndex(int i);
		DllExport void		setFaceMtlIndex(int i, MtlID id); 	
		
		DllExport void		buildNormals();			// calcs face and vertex normals
		DllExport void 		buildRenderNormals();	// like buildNormals, but ignores mtlIndex
		// checkNormals can be used to build the normals and allocate RVert space 
		// only if necessary.  This is a very cheap call if the normals are already calculated.
		// When illum is FALSE, only the RVerts allocation is checked (since normals aren't
		// needed for non-illum rendering).  When illum is TRUE, normals will also be built, if
		// they aren't already.  So, to make sure normals are built, call this with illum=TRUE.
		DllExport void		checkNormals(BOOL illum);

		DllExport void		render(GraphicsWindow *gw, Material *ma, RECT *rp, int compFlags, int numMat=1, InterfaceServer *pi = NULL);
		DllExport BOOL		select(GraphicsWindow *gw, Material *ma, HitRegion *hr, int abortOnHit = FALSE, int numMat=1);
		DllExport void		snap(GraphicsWindow *gw, SnapInfo *snap, IPoint2 *p, Matrix3 &tm);
		DllExport BOOL 		SubObjectHitTest(GraphicsWindow *gw, Material *ma, HitRegion *hr,
								DWORD flags, SubObjHitList& hitList, int numMat=1 );

		void		displayNormals(int b, float sc)	{ dspNormals = b; if(sc != (float)0.0) norScale = sc; }
		void		displayAllEdges(int b)	{ dspAllEdges = b; }
		DllExport void		buildBoundingBox(void);
		DllExport Box3 		getBoundingBox(Matrix3 *tm=NULL); // RB: optional TM allows the box to be calculated in any space.
		                                              // NOTE: this will be slower becuase all the points must be transformed.
		// Cache invalidation
		DllExport void 		InvalidateGeomCache();
		DllExport void 		InvalidateTopologyCache();
		DllExport void 		FreeAll(); //DS
		DllExport void      ZeroTopologyCache(); // RB set pointers to NULL but don't delete from mem.

		// edge list functions		
		DllExport void		EnableEdgeList(int e);
		DllExport void     	BuildVisEdgeList();
		DllExport void 		DrawVisEdgeList(GraphicsWindow *gw, DWORD flags);
		DllExport void 		Draw3DVisEdgeList(GraphicsWindow *gw, DWORD flags);
		DllExport void		HitTestVisEdgeList(GraphicsWindow *gw, int abortOnHit ); // RB
		DllExport void		InvalidateEdgeList(); // RB

		// strip functions				
		DllExport BOOL     	BuildStrips();
		DllExport void		Stripify(Strip *s, StripData *sd, int vtx);
		DllExport void		Stripify(Strip *s, StripData *sd, int vtx,
									 DWTab	&v,	DWTab	&n,	DWTab	&tv);
		DllExport void		getStripVertColor(GraphicsWindow *gw, int cv, int flipped, MtlID mID, DWORD smGroup, Point3 &rgb);
		DllExport void		getStripNormal(int cv, MtlID mID, DWORD smGroup, Point3 &nor);
		DllExport int		getStripNormalIndex(int cv, MtlID mID, DWORD smGroup);
		DllExport BOOL		getStripTVert(GraphicsWindow *gw, int cv, int ctv, Point3 &uvw, int texNum = 0);
		DllExport void 		DrawStrips(GraphicsWindow *gw, Material *ma, int numMat);
		DllExport void 		Draw3DStrips(GraphicsWindow *gw, Material *ma, int numMat);
		DllExport void 		Draw3DWireStrips(GraphicsWindow *gw, Material *ma, int numMat);
		DllExport void		InvalidateStrips();

		DllExport void		BuildStripsAndEdges();

		// functions for use in data flow evaluation
      DllExport void       ShallowCopy(Mesh *amesh, ChannelMask channels);
      DllExport void       DeepCopy(Mesh *amesh, ChannelMask channels);
      DllExport void       NewAndCopyChannels(ChannelMask channels);
      DllExport void       FreeChannels(ChannelMask channels, int zeroOthers=1);

		// Mesh flags
		void		SetFlag(DWORD f) { flags |= f; }
		DWORD		GetFlag(DWORD f) { return flags & f; }
		void		ClearFlag(DWORD f) { flags &= ~f; }

		// Display flags
		void		SetDispFlag(DWORD f) { dispFlags |= f; }
		DWORD		GetDispFlag(DWORD f) { return dispFlags & f; }
		void		ClearDispFlag(DWORD f) { dispFlags &= ~f; }

		// Selection access
		BitArray& 	VertSel() { return vertSel;  }	
		BitArray& 	FaceSel() { return faceSel;  }	

		// Constructs a vertex selection list based on the current selection level.
		DllExport BitArray 	VertexTempSel();

		DllExport IOResult Save(ISave* isave);
		DllExport IOResult Load(ILoad* iload);

		// RB: added so all objects can easily support the GeomObject method of the same name.
		DllExport int IntersectRay(Ray& ray, float& at, Point3& norm);
		DllExport int IntersectRay(Ray& ray, float& at, Point3& norm, DWORD &fi, Point3 &bary);

		// RB: I couldn't resist adding these <g>
		DllExport Mesh operator+(Mesh &mesh);  // Union
		DllExport Mesh operator-(Mesh &mesh);  // Difference
		DllExport Mesh operator*(Mesh &mesh);  // Intersection
		DllExport void MyDebugPrint ();

		DllExport void WeldCollinear(BitArray &set);

		DllExport void Optimize(
			float normThresh, float edgeThresh, 
			float bias, float maxEdge, DWORD flags, 
			MeshOpProgress *prog=NULL);

		DllExport void ApplyUVWMap(int type,
			float utile, float vtile, float wtile,
			int uflip, int vflip, int wflip, int cap,
			const Matrix3 &tm, int channel=1);
		DllExport void ApplyMapper (UVWMapper & map, int channel=1);

		DllExport void FlipNormal(int i);
		DllExport void UnifyNormals(BOOL selOnly);
		DllExport void AutoSmooth(float angle,BOOL useSel,BOOL preventIndirectSmoothing=FALSE);

		DllExport Edge *MakeEdgeList(int *edgeCount, int flagdbls=0);
		DllExport int DeleteFlaggedFaces(); // deletes all faces with FACE_WORK flag set
				
		
		// deletes all seleted elements of the current selection level
		DllExport void DeleteSelected();	
		
		// Deletes vertices as specified by the bit array
		DllExport void DeleteVertSet(BitArray set);
		
		// Deletes faces as specified by the bit array. If isoVert is non
		// null then it will be setup to flag vertices that were isolated
		// by the face deletetion. This set can then be passed to
		// DeleteVertSet to delete isolated vertices.
		DllExport void DeleteFaceSet(BitArray set, BitArray *isoVert=NULL);

		// Returns TRUE if an equivalent face already exists.
		DllExport BOOL DoesFaceExist(DWORD v0, DWORD v1, DWORD v2);

		// Removes faces that have two or more equal indices.
		// Returns TRUE if any degenerate faces were found
		DllExport BOOL RemoveDegenerateFaces();

		// Removes faces that have indices that are out of range
		// Returns TRUE if any illegal faces were found
		DllExport BOOL RemoveIllegalFaces();

		DllExport Point3 FaceNormal (DWORD fi, BOOL nrmlize=FALSE);
		DllExport Point3 FaceCenter (DWORD fi);
		DllExport float AngleBetweenFaces(DWORD f0, DWORD f1);

		// Compute the barycentric coords of a point in the plane of
		// a face relative to that face.
		DllExport Point3 BaryCoords(DWORD face, Point3 p);

		// Some edge operations
		DllExport void DivideEdge(DWORD edge, float prop=.5f, bool visDiag1=TRUE,
			bool fixNeighbors=TRUE, bool visDiag2=TRUE);
		DllExport void DivideFace(DWORD face, DWORD e1, DWORD e2, 
			float prop1=.5f, float prop2=.5f, bool fixNeighbors=TRUE, bool split=FALSE);
		DllExport void TurnEdge (DWORD edge, DWORD *otherEdge=NULL);

		// Tessellation
		DllExport void FaceCenterTessellate(BOOL ignoreSel=FALSE, MeshOpProgress *mop=NULL);
		DllExport void EdgeTessellate(float tens,BOOL ignoreSel=FALSE, MeshOpProgress *mop=NULL);

		// Extrudes selected faces. Note that this is just a topological
		// change. The new extruded faces do not change position but
		// are left on top of the original faces.
		// If doFace is FALSE then selected edges are extruded.
		DllExport void ExtrudeFaces(BOOL doFace=TRUE);

		// Indents selected faces, in a manner consistent with the outlining used in Bevel.
		// Added by SteveA for Shiva, 6/98
		DllExport void IndentSelFaces (float amount);

		// Splits verts specified in bitarray so that they are only
		// used by a single face
		DllExport void BreakVerts(BitArray set);

		// Deletes verts that aren't used by any faces
		DllExport BitArray GetIsoVerts ();
		DllExport void DeleteIsoVerts ();

		// Clone faces (and verts used by those faces)
		DllExport void CloneFaces(BitArray fset);
		DllExport void PolyFromFace (DWORD f, BitArray &set, float thresh, BOOL ignoreVisEdges, AdjFaceList *af=NULL);
		DllExport void ElementFromFace (DWORD f, BitArray &set, AdjFaceList *af=NULL);
		DllExport void FindVertsUsedOnlyByFaces (BitArray & fset, BitArray & vset);
		DllExport void FindOpenEdges (BitArray & edges);
		DllExport void FindVertexAngles (float *vang, BitArray *set=NULL);

		// used by the renderer
		void  SetRenderData(MeshRenderData *p) {renderData = p; } 
		MeshRenderData * GetRenderData() { return renderData; }

		// Quick access to specified normal interface.
		DllExport void ClearSpecifiedNormals ();
		DllExport MeshNormalSpec *GetSpecifiedNormals ();
		DllExport void SpecifyNormals ();

		// Copy only vertices and faces - no maps, selection, per-vertex data, etc.
		DllExport void CopyBasics (const Mesh & from);

		// --- from InterfaceServer
		DllExport BaseInterface* GetInterface(Interface_ID id);

      // TO DO: OLP This should probably be put in an interface.
      //! \brief Check if a face's cache is invalid
      /*!   The function first checks if the whole mesh or the face have been flagged as
         cache invalid (MESH_CACHEINVALID). If none of these is true, it then cycles through the face's
         vertices and returns true as soon as it finds a vertex flagged with MESH_CACHEINVALID.
         \pre faceIndex must be a valid index in the faces (f) array.
         \param[in] faceIndex The index of the face for which you want to determine cache validity
         \return true if the face is invalid, else false.
      */
      DllExport bool isFaceCacheInvalid(int faceIndex);

      //! \brief Invalidates a single vertex's cache indicating it needs to be rebuilt.
      /*!   The function flags the specified vertex with the MESH_CACHEINVALID flag, telling the rebuild
         functions that this vertex, the faces surrounding it and the other vertices of those faces
         need to have their normals recomputed. It also sets normalsBuilt to 0 to trigger normals' rebuild.
         This function should be called when one to several vertices have been modified. If all or almost
         all vertices of the mesh have been invalidated, it's more efficient to call InvalidateGeomCache() instead.
         \pre vertex must be a valid index in the vertex (v) array.
         \param[in] vertex The index of the vertex that needs to be invalidated.
      */
      DllExport void InvalidateVertexCache(int vertex);

      //! \brief Holds flags specific to the corresponding vertex in the verts array.
      long *      vFlags;

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

	};

class ImbMesh: public Mesh {
	public:
		~ImbMesh();
	};

// Mapping types passed to ApplyUVWMap()
#define MAP_PLANAR		0
#define MAP_CYLINDRICAL	1
#define MAP_SPHERICAL	2
#define MAP_BALL		3
#define MAP_BOX			4
#define MAP_FACE		5


// Optimize flags
#define OPTIMIZE_SAVEMATBOUNDRIES		(1<<0)
#define OPTIMIZE_SAVESMOOTHBOUNDRIES	(1<<1)
#define OPTIMIZE_AUTOEDGE				(1<<2)

DllExport void setUseVisEdge(int b);
DllExport int getUseVisEdge();

#define SMALL_VERTEX_DOTS	0
#define LARGE_VERTEX_DOTS	1

// CAL-05/07/03: new vertex dot types with radius from 2 to 7 (release 6.0)
#define VERTEX_DOT2	0		// equivalent to SMALL_VERTEX_DOTS
#define VERTEX_DOT3	1		// equivalent to LARGE_VERTEX_DOTS
#define VERTEX_DOT4	2
#define VERTEX_DOT5	3
#define VERTEX_DOT6	4
#define VERTEX_DOT7	5

// CAL-05/07/03: new handle box types with radius from 2 to 7 (release 6.0)
#define HANDLE_BOX2	0
#define HANDLE_BOX3	1
#define HANDLE_BOX4	2
#define HANDLE_BOX5	3
#define HANDLE_BOX6	4
#define HANDLE_BOX7	5

// CAL-05/07/03: get vertex dot marker from vertex dot type
#define VERTEX_DOT_MARKER(vtype) (MarkerType)(vtype - VERTEX_DOT2 + DOT2_MRKR)

// CAL-05/07/03: get vertex dot marker from vertex dot type
#define HANDLE_BOX_MARKER(htype) (MarkerType)(htype - HANDLE_BOX2 + BOX2_MRKR)

DllExport void setUseVertexDots(int b);
DllExport int getUseVertexDots();

DllExport void setVertexDotType(int t);
DllExport int getVertexDotType();

DllExport void setHandleBoxType(int t);
DllExport int getHandleBoxType();

DllExport void setDisplayBackFaceVertices(int b);
DllExport int getDisplayBackFaceVertices();

// Steve Anderson - new for Max 5.0 - we have a better way of evaluating
// vertex normals now, weighting the contribution of each face by the angle
// the face makes at that vertex, but users might want to access the old way.
// This is controlled by the "Use Legacy R4 Vertex Normals" checkbox
// in the General Preferences dialog.
class VertexNormalsCallback: public MaxHeapOperators {
public:
	virtual void SetUseFaceAngles (bool value) = 0;
};

class VertexNormalsControl: public MaxHeapOperators {
	bool mUseFaceAngles;
	Tab<VertexNormalsCallback *> mCallbacks;
public:
	DllExport VertexNormalsControl () : mUseFaceAngles(true) { }

	DllExport void RegisterCallback (VertexNormalsCallback *pCallback);
	DllExport void UnregisterCallback (VertexNormalsCallback *pCallback);

	DllExport void SetUseFaceAngles (bool value);
	DllExport bool GetUseFaceAngles ();
};

DllExport VertexNormalsControl *GetVertexNormalsControl ();

// a callback to update progress UI while doing a
// lengthy operation to a mesh
class MeshOpProgress: public MaxHeapOperators {
	public:
		// called once with the total increments
		virtual void Init(int total)=0;

		// Called to update progress. % done = p/total
		virtual BOOL Progress(int p)=0;
	};

// Boolean operations for meshes:
#define MESHBOOL_UNION 				1
#define MESHBOOL_INTERSECTION  		2
#define MESHBOOL_DIFFERENCE 		3

//
// mesh = mesh1 op mesh2
// If tm1 or tm2 are non-NULL, the points of the corresponding
// mesh will be transformed by these tm before the bool op
// The mesh will be transformed back by either Inverse(tm1) or
// Inverse(tm2) depending whichInv (0=>tm1, 1=>tm2)
// unless whichInv is -1 in which case it will not be transformed
// back.
//
DllExport int CalcBoolOp(
	Mesh &mesh, Mesh &mesh1, Mesh &mesh2, int op,
	MeshOpProgress *prog = NULL,
	Matrix3 *tm1 = NULL,
	Matrix3 *tm2 = NULL,
	int whichInv = 0,
	int weld = TRUE);


// Combines two meshes. The matrix and whichInv parameters have
// the same meaning as they do for the CalcBoolOp above.
DllExport void CombineMeshes(
		Mesh &mesh,Mesh &mesh1,Mesh &mesh2,
		Matrix3 *tm1=NULL, Matrix3 *tm2=NULL, int whichInv=0);

// Slices a single mesh.  The Point3 N and the float offset define a
// slicing plane (by DotProd (N,X) = offset).  Default behavior is to
// split faces that cross the plane, producing 1-2 new faces on each side
// and a new vert in the middle of each edge crossing the plane.  split
// means to add 2 different but coincident points to the top and bottom
// sets of faces, splitting the mesh into two meshes.  remove means to
// delete all faces & verts below the plane.
DllExport void SliceMesh (Mesh & mesh,
						  Point3 N, float off, bool split=FALSE, bool remove=FALSE);

// Handy utilities to go with meshes:

// Translates map type into 3d location -> uvw coord mapper:
class UVWMapper: public MaxHeapOperators {
public:
	int     type, cap;
	float   utile, vtile, wtile;
	int     uflip, vflip, wflip;
	Matrix3 tm;

	DllExport UVWMapper();
	DllExport UVWMapper(int type, const Matrix3 &tm, int cap=FALSE,
		float utile=1.0f, float vtile=1.0f, float wtile=1.0f,
		int uflip=FALSE, int vflip=FALSE, int wflip=FALSE);
	DllExport UVWMapper(UVWMapper& m);

	DllExport UVVert MapPoint(Point3 p, const Point3 & norm, int *nan=NULL);
	DllExport UVVert TileFlip (UVVert uvw);
	DllExport int MainAxis (const Point3 & n);
	bool NormalMatters () { return ((type==MAP_BOX)||((type==MAP_CYLINDRICAL)&&cap)) ? TRUE:FALSE; }
};

DllExport Mesh * CreateNewMesh();

#endif // _MESH_H_
