#include "StdAfx.h"
#include "EngineShadersLib.h"
#include "FileSystem.h"
#include "Reader.h"

namespace xrFSL
{
	// guessed names
	enum {
		SHADERS_CHUNK_CONSTANTS		= 0,
		SHADERS_CHUNK_MATRICES		= 1,
		SHADERS_CHUNK_BLENDERS		= 2,
		SHADERS_CHUNK_NAMES			= 3,
	};

	EngineShadersLib::EngineShadersLib(void)
	{
	}

	EngineShadersLib::~EngineShadersLib(void)
	{
	}

	//************************************
	// Method:    load
	// FullName:  xrFSL::EngineShadersLib::load
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * pszPath
	// Parameter: const char * pszName
	//************************************
	bool EngineShadersLib::load( const char* pszPath, const char* pszName )
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
	// FullName:  xrFSL::EngineShadersLib::load
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	void EngineShadersLib::load( Reader& r )
	{
		Reader* pChunk = r.open_chunk(SHADERS_CHUNK_NAMES);
		if (pChunk)
		{
			pChunk->r_seq(pChunk->r_u32(), m_vecNames, Reader::f_r_sz());
			r.close_chunk(pChunk);
		}
	}
}
