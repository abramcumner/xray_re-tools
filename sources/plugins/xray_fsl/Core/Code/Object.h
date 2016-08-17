#pragma once

#include "Surface.h"
#include "Mesh.h"
#include <hash_map>

namespace xrFSL
{
	// Forward decl
	class Reader;
	class Writer;

	// EOBJ_CHUNK_FLAGS
	enum 
	{
		EOF_STATIC			= 0x00,
		EOF_DYNAMIC			= 0x01,
		EOF_PROGRESSIVE		= 0x02,	// use SWPM
		EOF_USING_LOD		= 0x04,	// generate billboards (unused?)
		EOF_HOM				= 0x08,	// level.hom
		EOF_MULTIPLE_USAGE	= 0x14,	// MT4_LOD (trees and vehicles)
		EOF_SOUND_OCCLUDER	= 0x20,	// level.som
	};

	/**
	 *	
	 */
	class xrFSL_CORE_API Object: public SurfaceFactory
	{
	public:
		Object(void);
		Object(const SurfaceFactory* pSurfaceFactory);
		virtual ~Object();
/*
		virtual void clear();
		virtual void to_object();
		virtual void denominate();
		virtual void denominate_surfaces();
		*/

		virtual bool Load(const char* pszPath);
		virtual void Load(Reader& r);
		virtual bool Save(const char* pszPath) const;
		virtual bool Save(const char* pszPath, const String& sName) const;
		virtual void Save(Writer& w) const;
/*
		virtual bool load_bones(const char* path);
		virtual void load_bones(Reader& r);
		//virtual bool	save_bones(const char* path) const;
		//virtual void	save_bones(xr_writer& w) const;

		virtual bool load_skls(const char* path);
		virtual void load_skls(Reader& r);
		//virtual bool	save_skls(const char* path) const;
		//virtual void	save_skls(xr_writer& w) const;

		Surface* attach(const RawSurface& raw_surface);

		void calculate_bind();
*/
		Surface* FindSurface(const String& sName) { return FindByName(m_vecSurfaces, sName.c_str()); };
/*
		Mesh* find_mesh(const String& name);
		xr_bone* find_bone(const String& name);
		xr_partition* find_partition(const String& name);
		xr_skl_motion* find_motion(const String& name);
*/
		uint32& flags() { return m_uFlags; };
		uint32 flags() const { return m_uFlags; };
		const String& userdata() const { return m_sUserData; };
		const String& motion_refs() const { return m_sMotionRefs; };
		Surface_vec& surfaces() { return m_vecSurfaces; };
		const Surface_vec& surfaces() const { return m_vecSurfaces; };
		Mesh_vec& meshes() { return m_vecMeshes; };
		const Mesh_vec& meshes() const { return m_vecMeshes; };
/*
		xr_bone_vec& bones();
		const xr_bone_vec& bones() const;
		xr_skl_motion_vec& motions();
		const xr_skl_motion_vec& motions() const;
		xr_partition_vec& partitions();
		const xr_partition_vec&	partitions() const;
		
		const xr_bone* root_bone() const;

	protected:
		void setup_bones();
		void setup_partitions();
		*/
		virtual Surface* CreateSurface(const RawSurface& raw_surface) const { ASSERT_F(0, ("Please impl me...")); return NULL;};

	protected:
		uint32 m_uFlags;	// EOBJ_CHUNK_FLAGS
		String m_sUserData;	// EOBJ_CHUNK_USERDATA
		String m_sLODRef;	// EOBJ_CHUNK_LOD_REF
		Surface_vec m_vecSurfaces;	// EOBJ_CHUNK_SURFACES_X
		Mesh_vec m_vecMeshes;	// EOBJ_CHUNK_MESHES
		//xr_bone_vec m_bones;	// EOBJ_CHUNK_BONES_X
		//xr_skl_motion_vec m_motions;	// EOBJ_CHUNK_MOTIONS
		String m_sMotionRefs;	// EOBJ_CHUNK_MOTION_REFS
		//xr_partition_vec m_partitions;	// EOBJ_CHUNK_PARTITIONS_X
		fVector3 m_fv3Position;	// EOBJ_CHUNK_TRANSFORM
		fVector3 m_fv3Rotation;
		String m_sOwnerName;	// EOBJ_CHUNK_REVISION
		uint32 m_uCreationTime;
		String m_sModifierName;
		uint32 m_uModifiedTime;

		const SurfaceFactory* m_pSurfaceFactory;
		stdext::hash_map<RawSurface, Surface*> m_hmapRawSurfaces;
	};
}
