#pragma once

#include "Types.h"

namespace xrFSL
{
	// Forward decl
	class Reader;

	enum
	{
		RSF_COLLISION	= 0x0001,
		RSF_RENDERING	= 0x0002,
		RSF_LIGHTMAP	= 0x0004,
		RSF_TWO_SIDED	= 0x0008,
		RSF_AUTOMATIC	= 0x0010,	// assign texture/engine shader basing on game material
		RSF_DEBUG		= 0x0020,
	};

	// editable object surface flags
	const uint32 ESF_TWO_SIDED = 0x00000001;
	const uint32 ESURFACE_DEFAULT_FVF = 0x112;

	/**
	 *	
	 */
	class RawSurface 
	{
	public:
// 		void set(uint16_t _texture, uint16_t _eshader = UINT16_MAX,
// 			uint16_t _gamemtl = UINT16_MAX, uint16_t _flags = RSF_RENDERING);
// 		void		clear();
// 
// 		bool		collision() const;
// 		bool		rendering() const;
// 		bool		lightmap() const;
// 		bool		two_sided() const;
// 		bool		automatic() const;
// 		bool		debug() const;
// 
// 		int		compare(const RawSurface& right) const;
// 		bool		operator<(const RawSurface& right) const;
// 		bool		operator==(const RawSurface& right) const;

		union 
		{
			struct 
			{
				uint16 texture;
				uint16 eshader;
				uint16 gamemtl;
				uint16 flags;
			};
			// for comparison
			struct 
			{
				uint32 blob32[2];
			};
			UINT64 blob64;
		};
	};

	/**
	 *	
	 */
	class xrFSL_CORE_API Surface
	{
	public:
		Surface(bool bSkeletal = false);
		virtual ~Surface(void);

		void Load_0(Reader& r);
		void Load_1(Reader& r);
		void Load_2(Reader& r);
		void Save(Writer& w) const;

		//int		compare(const Surface& right) const;
		//bool		operator==(const Surface& right) const;
		//bool		operator<(const Surface& right) const;

		String& name();
		const String& name() const;
		String& eshader();
		const String& eshader() const;
		String& cshader();
		const String& cshader() const;
		String& gamemtl();
		const String& gamemtl() const;
		String&	texture();
		const String& texture() const;
		String&	vmap();
		const String& vmap() const;
		uint32&	flags();
		uint32 flags() const;

		// shortcuts
		bool two_sided() const;
		void set_two_sided();

	private:
		String m_sName;
		String m_sEShader;
		String m_sCShader;
		String m_sGameMtl;
		String m_sTexture;
		String m_sVmap;
		uint32 m_uFlags;
		uint32 m_uFvf;
	};

	TYPEDEF_STD_VECTOR_PTR(Surface)

	/**
	 *	
	 */
	class xrFSL_CORE_API SurfaceFactory 
	{
	public:
		virtual ~SurfaceFactory() {};
		virtual Surface* CreateSurface(const RawSurface& raw_surface) const = 0;
	};


	//=============================================
	// Inline impl
	//=============================================
	inline String& Surface::name() { return m_sName; }
	inline const String& Surface::name() const { return m_sName; }
	inline String& Surface::eshader() { return m_sEShader; }
	inline const String& Surface::eshader() const { return m_sEShader; }
	inline String& Surface::cshader() { return m_sCShader; }
	inline const String& Surface::cshader() const { return m_sCShader; }
	inline String& Surface::gamemtl() { return m_sGameMtl; }
	inline const String& Surface::gamemtl() const { return m_sGameMtl; }
	inline String& Surface::texture() { return m_sTexture; }
	inline const String& Surface::texture() const { return m_sTexture; }
	inline String& Surface::vmap() { return m_sVmap; }
	inline const String& Surface::vmap() const { return m_sVmap; }
	inline uint32& Surface::flags() { return m_uFlags; }
	inline uint32 Surface::flags() const { return m_uFlags; }

	inline bool Surface::two_sided() const { return !!(m_uFlags & ESF_TWO_SIDED); }
	inline void Surface::set_two_sided() { m_uFlags = ESF_TWO_SIDED; }
}
