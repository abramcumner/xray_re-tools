#include "StdAfx.h"
#include "GameMaterialsLib.h"
#include "Reader.h"
#include "Prerequisites.h"
#include "FileSystem.h"

namespace xrFSL
{
	GameMaterialsLib::~GameMaterialsLib(void)
	{
		DeleteElements(m_vecMaterials);
		DeleteElements(m_vecMaterialPairs);
	}

	bool GameMaterialsLib::load( const char* pszPath, const char* pszName )
	{
		FileSystem& fs = FileSystem::Instance();

		Reader* pR = fs.rOpen(pszPath, pszName);
		if (!pR)
			return false;

		load(*pR);
		fs.rClose(pR);

		return true;
	}

	//************************************
	// Method:    load
	// FullName:  xrFSL::GameMaterialsLib::load
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void GameMaterialsLib::load( Reader& r )
	{
		uint16 version;
		ASSERT_F(r.r_chunk<uint16>(GAMEMTLS_CHUNK_VERSION, version), (""));
		ASSERT_W(version == GAMEMTLS_VERSION, (""));

		ASSERT_F(r.find_chunk(GAMEMTLS_CHUNK_AUTOINC), (""));

		m_uMaterialIndex = r.r_u32();
		m_uMaterialPairIndex = r.r_u32();

		Reader* s = r.open_chunk(GAMEMTLS_CHUNK_MATERIALS);
		if (s) 
		{
			Reader* f;
			for (uint32 id = 0; (f = s->open_chunk(id)); ++id) 
			{
				SGameMaterial* material = new SGameMaterial;
				material->load(*f);
				m_vecMaterials.push_back(material);
				s->close_chunk(f);
			}
			r.close_chunk(s);
		}

		s = r.open_chunk(GAMEMTLS_CHUNK_MATERIAL_PAIRS);
		if (s) 
		{
			Reader* f;
			for (uint32 id = 0; (f = s->open_chunk(id)); ++id) 
			{
				SGameMaterialPair* gamemtlpair = new SGameMaterialPair;
				gamemtlpair->load(*f);
				m_vecMaterialPairs.push_back(gamemtlpair);
				s->close_chunk(f);
			}
			r.close_chunk(s);
		}
	}

	//************************************
	// Method:    load
	// FullName:  xrFSL::SGameMaterial::load
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void SGameMaterial::load( Reader& r )
	{
		ASSERT_F(r.find_chunk(GAMEMTL_CHUNK_MAIN), (""));

		id = r.r_u32();
		r.r_sz(sName);
		
		if (r.find_chunk(GAMEMTL_CHUNK_DESC))
			r.r_sz(sDescription);
		
		ASSERT_F(r.find_chunk(GAMEMTL_CHUNK_FLAGS), (""));
			
		flags = r.r_u32();

		ASSERT_F(r.find_chunk(GAMEMTL_CHUNK_PHYSICS), (""));
		
		fFriction = r.r_float();
		fDamping = r.r_float();
		fSpring = r.r_float();
		fBouncingStartVelocity = r.r_float();
		fBounce = r.r_float();

		ASSERT_F(r.find_chunk(GAMEMTL_CHUNK_FACTORS), (""));
		
		fShootFactor = r.r_float();
		fBounceDamageFactor = r.r_float();
		fVisTransparencyFactor = r.r_float();
		fSndOcclusionFactor = r.r_float();

		ASSERT_F(r.r_chunk<float>(GAMEMTL_CHUNK_FLOTATION, fFlotationFactor), (""));
		ASSERT_F(r.r_chunk<float>(GAMEMTL_CHUNK_INJURY, fInjuriousSpeed), (""));
	}

	//************************************
	// Method:    load
	// FullName:  xrFSL::SGameMaterialPair::load
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void SGameMaterialPair::load( Reader& r )
	{
		ASSERT_F(r.find_chunk(GAMEMTLPAIR_CHUNK_PAIR), (""));

		uMtl0 = r.r_u32();
		uMtl1 = r.r_u32();
		id = r.r_u32();
		id_parent = r.r_u32();
		OwnProps = r.r_u32();

		ASSERT_F(r.find_chunk(GAMEMTLPAIR_CHUNK_BREAKING), (""));
		r.r_sz(sBreakingSounds);

		ASSERT_F(r.find_chunk(GAMEMTLPAIR_CHUNK_STEP), (""));
		r.r_sz(sStepSounds);

		ASSERT_F(r.find_chunk(GAMEMTLPAIR_CHUNK_COLLIDE), (""));
		r.r_sz(sCollideSounds);
		r.r_sz(sCollideParticles);
		r.r_sz(sCollideMarks);
	}
}
