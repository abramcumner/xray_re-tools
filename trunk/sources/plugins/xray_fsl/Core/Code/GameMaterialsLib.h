#pragma once

#include "Types.h"

namespace xrFSL
{
	// forward decl
	class Reader;

	const uint16 GAMEMTLS_VERSION = 1;

	enum 
	{
		GAMEMTLS_CHUNK_VERSION			= 0x1000,
		GAMEMTLS_CHUNK_AUTOINC			= 0x1001,
		GAMEMTLS_CHUNK_MATERIALS		= 0x1002,
		GAMEMTLS_CHUNK_MATERIAL_PAIRS	= 0x1003,
	};

	enum 
	{
		GAMEMTL_CHUNK_MAIN			= 0x1000,
		GAMEMTL_CHUNK_FLAGS			= 0x1001,
		GAMEMTL_CHUNK_PHYSICS		= 0x1002,
		GAMEMTL_CHUNK_FACTORS		= 0x1003,
		GAMEMTL_CHUNK_FLOTATION		= 0x1004,
		GAMEMTL_CHUNK_DESC			= 0x1005,
		GAMEMTL_CHUNK_INJURY		= 0x1006,
	};

	enum 
	{
		GAMEMTLPAIR_CHUNK_PAIR		= 0x1000,
		GAMEMTLPAIR_CHUNK_BREAKING	= 0x1002,
		GAMEMTLPAIR_CHUNK_STEP		= 0x1003,
		GAMEMTLPAIR_CHUNK_COLLIDE	= 0x1005,
	};

	struct xrFSL_CORE_API SGameMaterial 
	{
		enum 
		{
			MF_BREAKABLE		= 0x00000001,
			MF_BOUNCEABLE		= 0x00000004,
			MF_SKIDMARK			= 0x00000008,
			MF_BLOODMARK		= 0x00000010,
			MF_CLIMABLE			= 0x00000020,
			MF_PASSABLE			= 0x00000080,
			MF_DYNAMIC			= 0x00000100,
			MF_LIQUID			= 0x00000200,
			MF_SUPPRESS_SHADOWS	= 0x00000400,
			MF_SUPPRESS_WALLMARKS	= 0x00000800,
			MF_ACTOR_OBSTACLE	= 0x00001000,
			MF_INJURIOUS		= 0x10000000,
			MF_SHOOTABLE		= 0x20000000,
			MF_TRANSPARENT		= 0x40000000,
			MF_SLOW_DOWN		= 0x80000000,
		};

		unsigned id;
		String sName;
		String sDescription;

		uint32 flags;

		float fFriction;
		float fDamping;
		float fSpring;
		float fBouncingStartVelocity;
		float fBounce;

		float fShootFactor;
		float fBounceDamageFactor;
		float fVisTransparencyFactor;
		float fSndOcclusionFactor;
		float fFlotationFactor;

		float fInjuriousSpeed;

		void load(Reader& r);
	};

	TYPEDEF_STD_VECTOR_PTR(SGameMaterial)

	struct xrFSL_CORE_API SGameMaterialPair 
	{
		enum 
		{
			MPF_BREAKING_SOUNDS	= 0x02,
			MPF_STEP_SOUNDS		= 0x04,
			MPF_COLLIDE_SOUNDS	= 0x10,
			MPF_COLLIDE_PARTICLES	= 0x20,
			MPF_COLLIDE_MARKS	= 0x40,
		};

		unsigned uMtl0;
		unsigned uMtl1;
		unsigned id;
		unsigned id_parent;
		uint32 OwnProps;

		String sBreakingSounds;
		String sStepSounds;
		String sCollideSounds;
		String sCollideParticles;
		String sCollideMarks;

		void load(Reader& r);
	};

	TYPEDEF_STD_VECTOR_PTR(SGameMaterialPair)	
// 	EXPORT_STD_VECTOR(SGameMaterialPair)	
// 	EXPORT_STD_VECTOR_CONST_ITERATOR(SGameMaterialPair)

	class xrFSL_CORE_API GameMaterialsLib
	{
	public:
		GameMaterialsLib(void) {};
		virtual ~GameMaterialsLib(void);

		bool load(const char* pszPath, const char* pszName);
		void load(Reader& r);

		const SGameMaterial* GetMaterial(uint32 id) const;
		const SGameMaterial* GetMaterial(const char* pszName) const;

		const SGameMaterial_vec& materials() const;
		const SGameMaterialPair_vec& material_pairs() const;
	private:
		unsigned m_uMaterialIndex;
		unsigned m_uMaterialPairIndex;

		SGameMaterial_vec m_vecMaterials;
		SGameMaterialPair_vec m_vecMaterialPairs;
	};

	// Inline impl
	inline const SGameMaterial_vec& GameMaterialsLib::materials() const { return m_vecMaterials; }
	inline const SGameMaterialPair_vec& GameMaterialsLib::material_pairs() const { return m_vecMaterialPairs; }
}
