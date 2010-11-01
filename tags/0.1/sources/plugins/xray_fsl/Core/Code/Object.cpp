#include "StdAfx.h"
#include "Object.h"
#include "FileSystem.h"
#include "ObjectFormat.h"
#include "Writer.h"

namespace xrFSL
{
	Object::Object(void): m_uFlags(EOF_STATIC),
		m_sOwnerName("unknown"), m_uCreationTime(0),
		m_sModifierName("unknown"), m_uModifiedTime(0),
		m_pSurfaceFactory(0)
	{
		m_fv3Position.Set();
		m_fv3Rotation.Set();
	}

	Object::Object( const SurfaceFactory* pSurfaceFactory ): m_uFlags(EOF_STATIC),
		m_sOwnerName("unknown"), m_uCreationTime(0),
		m_sModifierName("unknown"), m_uModifiedTime(0),
		m_pSurfaceFactory(pSurfaceFactory)
	{
		m_fv3Position.Set();
		m_fv3Rotation.Set();
	}

	Object::~Object(void)
	{
		DeleteElements(m_vecSurfaces);
		DeleteElements(m_vecMeshes);
// 		DeleteElements(m_bones);
// 		DeleteElements(m_motions);
// 		DeleteElements(m_partitions);
	}

	//************************************
	// Method:    Load
	// FullName:  xrFSL::Object::Load
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * pszPath
	//************************************
	bool Object::Load( const char* pszPath )
	{
		FileSystem& fs = FileSystem::Instance();

		Reader* pR = fs.rOpen(pszPath);
		if (!pR)
			return false;

		Reader* pS = pR->open_chunk(EOBJ_CHUNK_MAIN);
		if (pS)
		{
			Load(*pS);
			pR->close_chunk(pS);
		}
		fs.rClose(pR);

		return true;
	}

	struct SReadMesh
	{
		Object& object;
		SReadMesh(Object& _object): object(_object) {}
		void operator()(Mesh*& mesh, Reader& r) 
		{
			mesh = new Mesh;
			mesh->Load(r, object);
		}
	};

	//************************************
	// Method:    Load
	// FullName:  xrFSL::Object::Load
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void Object::Load( Reader& r )
	{
		uint16 version;
		ASSERT_F(r.r_chunk<uint16>(EOBJ_CHUNK_VERSION, version), (""));
		ASSERT_F(version == EOBJ_VERSION, (""));

		ASSERT_F(r.r_chunk<uint32>(EOBJ_CHUNK_FLAGS, m_uFlags), (""));

		if (r.find_chunk(EOBJ_CHUNK_USERDATA)) 
		{
			r.r_sz(m_sUserData);
		}
// 
// 		if (r.find_chunk(EOBJ_CHUNK_LOD_REF)) {
// 			r.r_sz(m_lod_ref);
// 			r.debug_find_chunk();
// 		}

		if (r.find_chunk(EOBJ_CHUNK_SURFACES_2)) 
		{
			r.r_seq(r.r_u32(), m_vecSurfaces, Reader::f_r_new<Surface>(&Surface::Load_2));
		}
		else if (r.find_chunk(EOBJ_CHUNK_SURFACES_1)) 
		{
			r.r_seq(r.r_u32(), m_vecSurfaces, Reader::f_r_new<Surface>(&Surface::Load_1));
		} 
		else if (r.find_chunk(EOBJ_CHUNK_SURFACES_0)) 
		{
			r.r_seq(r.r_u32(), m_vecSurfaces, Reader::f_r_new<Surface>(&Surface::Load_0));

			if (r.find_chunk(EOBJ_CHUNK_SHADERS_0))
			{
				for (Surface_vec_it it = m_vecSurfaces.begin(), end = m_vecSurfaces.end(); it != end; ++it)
				{
						(*it)->cshader() = r.skip_sz();
				}
			}
		} 
		else 
		{
			ASSERT_F(0, ("Unknown surface type"));
		}

		Reader* s = r.open_chunk(EOBJ_CHUNK_MESHES);
		if (s) 
		{
			s->r_chunks(m_vecMeshes, SReadMesh(*this));
			r.close_chunk(s);
		}

		/*
		s = r.open_chunk(EOBJ_CHUNK_BONES_1);
		if (s) {
			s->r_chunks(m_bones, Reader::f_r_new<xr_bone>(&xr_bone::Load_1));
			r.close_chunk(s);
		} else if (r.find_chunk(EOBJ_CHUNK_BONES_0)) {
			s->r_seq(r.r_u32(), m_bones, Reader::f_r_new<xr_bone>(&xr_bone::Load_0));
			r.debug_find_chunk();
		}
		setup_bones();
		if (r.find_chunk(EOBJ_CHUNK_MOTIONS)) {
			r.r_seq(r.r_u32(), m_motions, Reader::f_r_new<xr_skl_motion>(&xr_skl_motion::load));
			r.debug_find_chunk();
		}
		if (r.find_chunk(EOBJ_CHUNK_MOTION_REFS)) {
			r.r_sz(m_motion_refs);
			r.debug_find_chunk();
		}
		if (r.find_chunk(EOBJ_CHUNK_PARTITIONS_0)) {
			r.r_seq(r.r_u32(), m_partitions, read_partition_0(m_bones));
			r.debug_find_chunk();
		} else if (r.find_chunk(EOBJ_CHUNK_PARTITIONS_1)) {
			r.r_seq(r.r_u32(), m_partitions, Reader::f_r_new<xr_partition>(&xr_partition::Load_1));
			r.debug_find_chunk();
		}
		if (r.find_chunk(EOBJ_CHUNK_TRANSFORM)) {
			r.r_fvector3(m_position);
			r.r_fvector3(m_rotation);
			r.debug_find_chunk();
		}
		if (r.find_chunk(EOBJ_CHUNK_REVISION)) {
			r.r_sz(m_owner_name);
			m_creation_time = r.r_u32();
			r.r_sz(m_modif_name);
			m_modified_time = r.r_u32();
			r.debug_find_chunk();
		}
		*/
	}

	//************************************
	// Method:    Save
	// FullName:  xrFSL::Object::Save
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: const
	// Parameter: const char * pszPath
	//************************************
	bool Object::Save( const char* pszPath ) const
	{
		MemoryWriter* pW = new MemoryWriter();

		pW->open_chunk(EOBJ_CHUNK_MAIN);
		Save(*pW);
		pW->close_chunk();

		bool status = pW->Save(pszPath);
		delete pW;

		return status;
	}

	//************************************
	// Method:    Save
	// FullName:  xrFSL::Object::Save
	// Access:    virtual public 
	// Returns:   bool
	// Qualifier: const
	// Parameter: const char * pszPath
	// Parameter: const String & sName
	//************************************
	bool Object::Save( const char* pszPath, const String& sName ) const
	{
		MemoryWriter* pW = new MemoryWriter();

		pW->open_chunk(EOBJ_CHUNK_MAIN);
		Save(*pW);
		pW->close_chunk();

		bool status = pW->Save(pszPath, sName);
		delete pW;

		return status;
	}

	//************************************
	// Method:    Save
	// FullName:  xrFSL::Object::Save
	// Access:    public 
	// Returns:   void
	// Qualifier: const
	// Parameter: Writer & w
	//************************************
	void Object::Save( Writer& w ) const
	{
		// version chunk
		w.w_chunk<uint16>(EOBJ_CHUNK_VERSION, EOBJ_VERSION);
		
		// common chunks
		w.w_chunk(EOBJ_CHUNK_USERDATA, m_sUserData);
		w.w_chunk(EOBJ_CHUNK_LOD_REF, m_sLODRef);
		w.w_chunk<uint32>(EOBJ_CHUNK_FLAGS, m_uFlags);

		// meshes
		w.open_chunk(EOBJ_CHUNK_MESHES);
		w.w_chunks(m_vecMeshes, Writer::f_w_const<Mesh>(&Mesh::Save));
		w.close_chunk();

		// surfaces
		w.open_chunk(EOBJ_CHUNK_SURFACES_2);
		w.w_size_u32(m_vecSurfaces.size());
		w.w_seq(m_vecSurfaces, Writer::f_w_const<Surface>(&Surface::Save));
		w.close_chunk();

		/*
		if (!m_bones.empty()) {
			w.open_chunk(EOBJ_CHUNK_BONES_1);
			w.w_chunks(m_bones, xr_writer::f_w_const<xr_bone>(&xr_bone::save));
			w.close_chunk();
		}
		if (!m_motions.empty()) {
			w.open_chunk(EOBJ_CHUNK_MOTIONS);
			w.w_size_u32(m_motions.size());
			w.w_seq(m_motions, xr_writer::f_w_const<xr_skl_motion>(&xr_skl_motion::save));
			w.close_chunk();
		}
		if (!m_motion_refs.empty()) {
			w.w_chunk(EOBJ_CHUNK_MOTION_REFS, m_motion_refs);
		}
		if (!m_partitions.empty()) {
			w.open_chunk(EOBJ_CHUNK_PARTITIONS_1);
			w.w_size_u32(m_partitions.size());
			w.w_seq(m_partitions, xr_writer::f_w_const<xr_partition>(&xr_partition::save));
			w.close_chunk();
		}
		if (m_flags & EOF_DYNAMIC) {
			w.open_chunk(EOBJ_CHUNK_TRANSFORM);
			w.w_fvector3(m_position);
			w.w_fvector3(m_rotation);
			w.close_chunk();
		}
		*/

		w.open_chunk(EOBJ_CHUNK_REVISION);
		w.w_sz(m_sOwnerName);
		w.w_u32(m_uCreationTime);
		w.w_sz(m_sModifierName);
		w.w_u32(m_uModifiedTime);
		w.close_chunk();
	}
}
