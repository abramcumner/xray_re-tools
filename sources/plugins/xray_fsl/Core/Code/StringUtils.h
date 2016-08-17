#pragma once

#include "Assert.h"
#include <algorithm>

namespace xrFSL
{
	class StringUtil
	{
	public:
		/**
		*	Lower-cases all the characters in the string.
		*/
		static inline void ToLowerCase( String& str );

		/**
		*	
		*/
		static inline void SplitPath(const char* pPath, String* psFolder = 0, String* psName = 0, String* psExt = 0);

		/**
		 *	
		 */
		static inline void AppendPathSeparator(String& sPath);
	};

	//====================================================
	// Inline impl
	//====================================================
	inline void StringUtil::ToLowerCase(String &str)
	{
		std::transform(str.begin(), str.end(), str.begin(), tolower);
	}

	inline void StringUtil::SplitPath(const char* pPath, String* psFolder /* = 0 */, String* psName /* = 0 */, String* psExt /* = 0 */)
	{
		char _drive[_MAX_DRIVE];
		char _dir[_MAX_DIR];
		char _name[_MAX_FNAME];
		char _extension[_MAX_EXT];

#if defined(_MSC_VER) && _MSC_VER >= 1400
		errno_t err = _splitpath_s(pPath,
			_drive, sizeof(_drive),
			_dir, sizeof(_dir),
			_name, sizeof(_name),
			_extension, sizeof(_extension));
		ASSERT_F(!err, ("Unable to split path. Path: %s", pPath));
#else
		_splitpath(pPath, _drive, _dir, _name, _extension);
#endif
		if (psFolder) 
		{
			psFolder->assign(_drive);
			psFolder->append(_dir);
			StringUtil::ToLowerCase(*psFolder);
		}

		if (psName) 
		{
			psName->assign(_name);
			ToLowerCase(*psName);
		}

		if (psExt) 
		{
			psExt->assign(_extension);
			StringUtil::ToLowerCase(*psExt);
		}
	}

	inline void StringUtil::AppendPathSeparator(String& sPath)
	{
		if (!sPath.empty() && *(sPath.end()-1) != '\\')
			sPath += '\\';
	}
}