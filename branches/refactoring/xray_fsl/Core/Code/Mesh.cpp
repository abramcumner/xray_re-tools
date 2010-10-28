#include "StdAfx.h"
#include "Mesh.h"
#include "Reader.h"
#include "ObjectFormat.h"
#include "Object.h"

namespace xrFSL
{
	void VMap::SRead0::operator()(VMap*& vmap, Reader& r)
	{
		const char* name = r.skip_sz();
		vmap = VMap::Create(name, VMT_UV, 2, false);
		vmap->Load(r);
	};

	void VMap::SRead1::operator()(VMap*& vmap, Reader& r)
	{
		const char* name = r.skip_sz();
		unsigned dimension = r.r_u8() & 0x3;
		unsigned type = r.r_u8() & 0x3;
		vmap = VMap::Create(name, type, dimension, false);
		vmap->Load(r);
	};

	void VMap::SRead2::operator()(VMap*& vmap, Reader& r)
	{
		const char* name = r.skip_sz();
		unsigned dimension = r.r_u8() & 0x3;
		bool discontiguous = (r.r_u8() & 0x1) != 0;
		unsigned type = r.r_u8() & 0x3;
		vmap = VMap::Create(name, type, dimension, discontiguous);
		vmap->Load(r);
	};

	void VMap::SWrite::operator()( const VMap* vmap, Writer& w ) const
	{
		w.w_sz(vmap->name());
		vmap->Save(w);
	}

	struct SReadFace 
	{ 
		void operator()(LWFace& face, Reader& r) const
		{
		face.v0 = r.r_u32();
		face.ref0 = r.r_u32();
		face.v1 = r.r_u32();
		face.ref1 = r.r_u32();
		face.v2 = r.r_u32();
		face.ref2 = r.r_u32();
		}
	};

	struct SReadvmref 
	{ 
		void operator()(lw_vmref& ref, Reader& r) const
		{
			r.r_seq(r.r_u8(), ref);
		}
	};

	struct SReadSurfaceMap 
	{
		Object& object;
		SReadSurfaceMap(Object& _object): object(_object) {}
		void operator()(SurfaceMap*& _smap, Reader& r) const 
		{
			SurfaceMap* smap = new SurfaceMap;
			_smap = smap;

			String name;
			r.r_sz(name);
			smap->pSurface = object.FindSurface(name.c_str());
			r.r_seq(r.r_u32(), smap->vecFaces);
		}
	};


	//////////////////////////////////////////////////////////////////////////
	Mesh::Mesh(void): m_uiFlags(EMF_VALID)
	{
		m_bbox.Null();
		m_SOptions.unk1 = 0;
		m_SOptions.unk2 = 0;
	}

	Mesh::~Mesh(void)
	{
		DeleteElements(m_vecVMaps);
		DeleteElements(m_vecSurfaceMaps);
	}

	//************************************
	// Method:    Load
	// FullName:  xrFSL::Mesh::Load
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	// Parameter: Object & object
	//************************************
	void Mesh::Load( Reader& r, Object& object )
	{
		size_t size;

		uint16 version;
		ASSERT_F(r.r_chunk<uint16>(EMESH_CHUNK_VERSION, version), (""));
		ASSERT_F(version == EMESH_VERSION, (""));

		ASSERT_F(r.r_chunk(EMESH_CHUNK_MESHNAME, m_sName), (""));
		ASSERT_F(r.r_chunk<fBox>(EMESH_CHUNK_BBOX, m_bbox), (""));		
		ASSERT_F(r.r_chunk<uint8>(EMESH_CHUNK_FLAGS, m_uiFlags), (""));

		r.r_chunk<LWOptions>(EMESH_CHUNK_OPTIONS, m_SOptions);

		ASSERT_F(r.find_chunk(EMESH_CHUNK_VERTS), (""));
		r.r_seq(r.r_u32(), m_vecPoints);

		ASSERT_F(r.find_chunk(EMESH_CHUNK_FACES), (""));
		r.r_seq(r.r_u32(), m_vecFaces, SReadFace());

		if ((size = r.find_chunk(EMESH_CHUNK_SG))) 
		{
			ASSERT_F(size == m_vecFaces.size()*sizeof(uint32_t), (""));
			r.r_seq(m_vecFaces.size(), m_vecSGroups);
		}

		ASSERT_F(r.find_chunk(EMESH_CHUNK_VMREFS), (""));
		r.r_seq(r.r_u32(), m_vmrefs, SReadvmref());

		ASSERT_F(r.find_chunk(EMESH_CHUNK_SFACE), (""));
		r.r_seq(r.r_u16(), m_vecSurfaceMaps, SReadSurfaceMap(object));

		if (r.find_chunk(EMESH_CHUNK_VMAPS_2)) 
		{
			r.r_seq(r.r_u32(), m_vecVMaps, VMap::SRead2());
		} 
		else if (r.find_chunk(EMESH_CHUNK_VMAPS_1)) 
		{
			r.r_seq(r.r_u32(), m_vecVMaps, VMap::SRead1());
		} 
		else if (r.find_chunk(EMESH_CHUNK_VMAPS_0)) 
		{
			r.r_seq(r.r_u32(), m_vecVMaps, VMap::SRead0());
		} 
		else 
		{
			ASSERT_F(0, ("can't find VMAPS chunk"));
		}

		for (LWFace_vec_it it = m_vecFaces.begin(); it != m_vecFaces.end(); ++it) 
		{
			ASSERT_F(it->v0 < m_vecPoints.size(), (""));
			ASSERT_F(it->v1 < m_vecPoints.size(), (""));
			ASSERT_F(it->v2 < m_vecPoints.size(), (""));
			ASSERT_F(it->ref0 < m_vmrefs.size(), (""));
			ASSERT_F(it->ref1 < m_vmrefs.size(), (""));
			ASSERT_F(it->ref2 < m_vmrefs.size(), (""));
		}
	}

	struct SWriteFace 
	{ 
		void operator()(const LWFace& face, Writer& w) const 
		{
			w.w_u32(face.v0);
			w.w_u32(face.ref0);
			w.w_u32(face.v1);
			w.w_u32(face.ref1);
			w.w_u32(face.v2);
			w.w_u32(face.ref2);	
		}
	};

	/////////////////////////////////////////////////////////////////////////
	struct SWriteVMRef 
	{ 
		void operator()(const lw_vmref& ref, Writer& w) const 
		{
			w.w_size_u8(ref.size());
			w.w_seq(ref);
		}
	};

	struct SWriteSurfMap 
	{ 
		void operator()(const SurfaceMap* sm, Writer& w) const
		{
			w.w_sz(sm->pSurface->name());
			w.w_size_u32(sm->vecFaces.size());
			w.w_seq(sm->vecFaces);
		}
	};

	//************************************
	// Method:    Save
	// FullName:  xrFSL::Mesh::Save
	// Access:    public 
	// Returns:   void
	// Qualifier: const
	// Parameter: Writer & w
	//************************************
	void Mesh::Save( Writer& w ) const
	{
		// mesh version
		w.w_chunk<uint16>(EMESH_CHUNK_VERSION, EMESH_VERSION);

		// common chunks
		w.w_chunk(EMESH_CHUNK_MESHNAME, m_sName);
		w.w_chunk<fBox>(EMESH_CHUNK_BBOX, m_bbox);
		w.w_chunk<uint8>(EMESH_CHUNK_FLAGS, m_uiFlags);
		w.w_chunk<LWOptions>(EMESH_CHUNK_OPTIONS, m_SOptions);

		// vertices
		w.open_chunk(EMESH_CHUNK_VERTS);
		w.w_size_u32(m_vecPoints.size());
		w.w_seq(m_vecPoints);
		w.close_chunk();

		// faces
		w.open_chunk(EMESH_CHUNK_FACES);
		w.w_size_u32(m_vecFaces.size());
		w.w_seq(m_vecFaces, SWriteFace());
		w.close_chunk();

		// smooth groups
		if (!m_vecSGroups.empty())
		{
			w.open_chunk(EMESH_CHUNK_SG);
			w.w_seq(m_vecSGroups);
			w.close_chunk();
		}

		// VMaps refs
		w.open_chunk(EMESH_CHUNK_VMREFS);
		w.w_size_u32(m_vmrefs.size());
		w.w_seq(m_vmrefs, SWriteVMRef());
		w.close_chunk();

		// surfaces maps
		w.open_chunk(EMESH_CHUNK_SFACE);
		w.w_size_u16(m_vecSurfaceMaps.size());
		w.w_seq(m_vecSurfaceMaps, SWriteSurfMap());
		w.close_chunk();

		// VMaps
		w.open_chunk(EMESH_CHUNK_VMAPS_2);
		w.w_size_u32(m_vecVMaps.size());
		w.w_seq(m_vecVMaps, VMap::SWrite());
		w.close_chunk();
	}

	//************************************
	// Method:    Create
	// FullName:  xrFSL::VMap::Create
	// Access:    public static 
	// Returns:   VMap*
	// Qualifier:
	// Parameter: const char * pszName
	// Parameter: unsigned uType
	// Parameter: unsigned uDimension
	// Parameter: bool bDiscontiguous
	//************************************
	VMap* VMap::Create( const char* pszName, unsigned uType, unsigned uDimension, bool bDiscontiguous )
	{
		if (uType == VMT_UV && uDimension == 2)
		{
			if (bDiscontiguous)
				return new FaceUVVmap(pszName);
			else
				return new UVVMap(pszName);
		} 
		else if (uType == VMT_WEIGHT && uDimension == 1) 
		{
			return new WeightVMap(pszName);
		}

		ASSERT_F(0, ("Unknown VMap type"));
		return 0;
	}

	//************************************
	// Method:    AddVertex
	// FullName:  xrFSL::VMap::AddVertex
	// Access:    public 
	// Returns:   xrFSL::uint32
	// Qualifier:
	// Parameter: uint32 vert_idx
	//************************************
	xrFSL::uint32 VMap::AddVertex( uint32 vert_idx )
	{
		uint32 index = uint32(size() & UINT32_MAX);
		m_vecVertices.push_back(vert_idx);
		return index;
	}

	//************************************
	// Method:    Load
	// FullName:  xrFSL::UVVMap::Load
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void UVVMap::Load( Reader& r )
	{
		size_t n = r.r_u32();
		r.r_seq(n, m_vecUVs);
		r.r_seq(n, m_vecVertices);
	}

	//************************************
	// Method:    Save
	// FullName:  xrFSL::UVVMap::Save
	// Access:    public 
	// Returns:   void
	// Qualifier: const
	// Parameter: Writer & w
	//************************************
	void UVVMap::Save( Writer& w ) const
	{
		w.w_u8(2);
		w.w_bool(false);
		w.w_u8(VMT_UV);
		w.w_size_u32(size());
		w.w_seq(m_vecUVs);
		w.w_seq(m_vecVertices);
	}

	//************************************
	// Method:    AddUV
	// FullName:  xrFSL::UVVMap::AddUV
	// Access:    public 
	// Returns:   xrFSL::uint32
	// Qualifier:
	// Parameter: const fVector2 & fv2UV
	// Parameter: uint32 uiVertIdx
	//************************************
	xrFSL::uint32 UVVMap::AddUV( const fVector2& fv2UV, uint32 uiVertIdx )
	{
		m_vecUVs.push_back(fv2UV);
		return AddVertex(uiVertIdx);
	}

	//************************************
	// Method:    Load
	// FullName:  xrFSL::FaceUVVmap::Load
	// Access:    virtual public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void FaceUVVmap::Load( Reader& r )
	{
		size_t n = r.r_u32();
		r.r_seq(n, m_vecUVs);
		r.r_seq(n, m_vecVertices);
		r.r_seq(n, m_vecFaces);
	}

	//************************************
	// Method:    Save
	// FullName:  xrFSL::FaceUVVmap::Save
	// Access:    public 
	// Returns:   void
	// Qualifier: const
	// Parameter: Writer & w
	//************************************
	void FaceUVVmap::Save( Writer& w ) const
	{
		w.w_u8(2);
		w.w_bool(true);
		w.w_u8(VMT_UV);
		w.w_size_u32(size());
		w.w_seq(m_vecUVs);
		w.w_seq(m_vecVertices);
		w.w_seq(m_vecFaces);
	}

	//************************************
	// Method:    AddUV
	// FullName:  xrFSL::FaceUVVmap::AddUV
	// Access:    public 
	// Returns:   xrFSL::uint32
	// Qualifier:
	// Parameter: const fVector2 & fv2UV
	// Parameter: uint32 uiVertIdx
	// Parameter: uint32 uiFaceIdx
	//************************************
	xrFSL::uint32 FaceUVVmap::AddUV( const fVector2& fv2UV, uint32 uiVertIdx, uint32 uiFaceIdx )
	{
		m_vecFaces.push_back(uiFaceIdx);
		return UVVMap::AddUV(fv2UV, uiVertIdx);
	}

	//************************************
	// Method:    Load
	// FullName:  xrFSL::WeightVMap::Load
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void WeightVMap::Load( Reader& r )
	{
		size_t n = r.r_u32();
		r.r_seq(n, m_vecWeights);
		r.r_seq(n, m_vecVertices);
	}

	//************************************
	// Method:    Save
	// FullName:  xrFSL::WeightVMap::Save
	// Access:    public 
	// Returns:   void
	// Qualifier: const
	// Parameter: Writer & w
	//************************************
	void WeightVMap::Save( Writer& w ) const
	{
		w.w_u8(1);
		w.w_bool(false);
		w.w_u8(VMT_WEIGHT);
		w.w_size_u32(size());
		w.w_seq(m_vecWeights);
		w.w_seq(m_vecVertices);
	}
}
