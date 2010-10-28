#pragma once

namespace xrFSL
{
	// forward decl
	class Reader;

	const size_t SHADER_XRLC_SIZE = 128 + 16;

	struct SCompilerShader
	{
		String sName;
		union 
		{
			uint32 dummy;
			struct 
			{
				uint32 collision:1;
				uint32 rendering:1;
				uint32 optimize_uv:1;
				uint32 light_vertex:1;
				uint32 light_cast_shadow:1;
				uint32 light_sharp:1;
				uint32 unused:26;
			};
		};
		float fVertTranslucency;
		float fVertAmbient;
		float fLmDensity;
	};

	TYPEDEF_STD_VECTOR_PTR(SCompilerShader)

	class xrFSL_CORE_API CompilerShadersLib
	{
	public:
		CompilerShadersLib(void);
		~CompilerShadersLib(void);

		bool load(const char* pszPath, const char* pszName);
		void load(Reader& r);

		const SCompilerShader_vec& shaders() const;

	private:
		SCompilerShader_vec m_vecShaders;
	};

	inline const SCompilerShader_vec& CompilerShadersLib::shaders() const { return m_vecShaders; }
}
