#include "StdAfx.h"
#include "CompilerShadersLib.h"
#include "Reader.h"

namespace xrFSL
{
	CompilerShadersLib::CompilerShadersLib(void)
	{
	}

	CompilerShadersLib::~CompilerShadersLib(void)
	{
		DeleteElements(m_vecShaders);
	}

	bool CompilerShadersLib::load( const char* pszPath, const char* pszName )
	{
		FileSystem& fs = FileSystem::Instance();

		Reader* pR = fs.rOpen(pszPath, pszName);
		if (!pR)
			return false;

		load(*pR);
		fs.rClose(pR);

		return true;
	}

	struct SReadxrlcShader 
	{ 
		void operator()(SCompilerShader*& _s, Reader& r) 
		{
			SCompilerShader* s = new SCompilerShader;
			_s = s;

			size_t pos = r.tell();
			r.r_sz(s->sName);
			r.seek(pos + 128);
			s->dummy = r.r_u32();
			s->fVertTranslucency = r.r_float();
			s->fVertAmbient = r.r_float();
			s->fLmDensity = r.r_float();
		}
	};

	void CompilerShadersLib::load( Reader& r )
	{
		ASSERT_F(r.size() % SHADER_XRLC_SIZE == 0, (""));
		r.r_seq(r.size() / SHADER_XRLC_SIZE, m_vecShaders, SReadxrlcShader());
	}
}
