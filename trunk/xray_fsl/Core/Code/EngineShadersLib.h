#pragma once

#include "Types.h"

namespace xrFSL
{
	// Forward decl
	class Reader;

	/**
	 *	
	 */
	class xrFSL_CORE_API EngineShadersLib
	{
	public:
		EngineShadersLib(void);
		virtual ~EngineShadersLib(void);

		bool load(const char* szPath, const char* szName);
		void load(Reader& r);

		const std::vector<String>& names() const;

	private:
		std::vector<String> m_vecNames;
	};

	// Inline impl
	inline const std::vector<String>& EngineShadersLib::names() const { return m_vecNames; }
}
