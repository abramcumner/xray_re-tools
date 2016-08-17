#pragma once

#include "Prerequisites.h"
#include "Reader.h"
#include "Writer.h"

namespace xrFSL
{
	/*
	 *	
	 */
	class xrFSL_CORE_API FileSystem
	{
	public:
		FileSystem(void);
		~FileSystem(void);

		/*
		 *	Singletone method.	
		 */
		static FileSystem& Instance(void);

		bool Initialize(const char* pFSSpec, unsigned uFlags);

		Reader*	rOpen(const char* pszPath) const;
 		Reader*	rOpen(const String& sPath) const;
 		Reader*	rOpen(const char* pszPath, const char* pszName) const;
// 		Reader*	rOpen(const char* pPath, const String& sName) const;
		void rClose(Reader*& r) const;

		Writer*	wOpen(const char* pszPath) const;
		Writer*	wOpen(const String& sPath) const;
		Writer*	wOpen(const char* pszPath, const char* pszName) const;
		Writer*	wOpen(const char* pszPath, const String& sName) const;
		void wClose(Writer*& w) const;

		static bool FileExist(const char* pszPath);
		static bool	FileExist(const String& sPath);
		bool FileExist(const char* pszAlias, const char* pszName) const;
		bool FileExist(const char* pszAlias, const String& sName) const;

		bool ResolvePath(const char* pszAlias, const char* pszName, String& sFullPath) const;
		bool ResolvePath(const char* pszAlias, const String& sName, String& sFullPath) const;
	private:
		/*
		 *	
		 */
		struct SPathAlias 
		{
			String sPathAlias;
			String sRoot;
			String sFilter;
			String sCaption;
		};
		TYPEDEF_STD_VECTOR_PTR(SPathAlias)


		SPathAlias_vec m_vecAliases;		///< path aliases vector
		unsigned m_uFlags;					///< 

		/**
		* @brief Registering new path alias.
		* @param sPathAlias - alias name.
		* @param sRoot - root folder path.
		* @param sAdd - additional path component.
		*/
		SPathAlias* AddPathAlias(const String& sPathAlias, const String& sRoot, const String& sAdd);

		/**
		 * @brief Return path alias struct.
		 * @param pAlias - alias name.
		 */
		const SPathAlias* FindPathAlias(const char* pAlias) const;

		/**
		 * @brief Parse FS specification file.
		 */
		bool ParseFSSpec(Reader& r); 
	};

	//===================================================
	//	fs spec path tokens
	//===================================================
	const char PA_FS_ROOT[] = "$fs_root$";
	const char PA_SDK_ROOT[] = "$sdk_root$";
	const char PA_GAME_DATA[] = "$game_data$";
	const char PA_GAME_CONFIG[] = "$game_config$";
	const char PA_GAME_SCRIPTS[] = "$game_scripts$";
	const char PA_GAME_MESHES[] = "$game_meshes$";
	const char PA_GAME_TEXTURES[] = "$game_textures$";
	const char PA_GAME_LEVELS[] = "$game_levels$";
	const char PA_GAME_SPAWN[] = "$game_spawn$";
	const char PA_LEVEL[] = "$level$";
	const char PA_LOGS[] = "$logs$";
	const char PA_SOUNDS[] = "$sounds$";
	const char PA_TEXTURES[] = "$textures$";
	const char PA_OBJECTS[] = "$objects$";
	const char PA_CLIPS[] = "$clips$";
	const char PA_MAPS[] = "$maps$";
	const char PA_GROUPS[] = "$groups$";
	const char PA_TEMP[] = "$temp$";
	const char PA_IMPORT[] = "$import$";
	const char PA_DETAIL_OBJECTS[] = "$detail_objects$";

	//=====================================================
	// Inline impl
	//=====================================================
	inline FileSystem& FileSystem::Instance(void)
	{
		static FileSystem instance0;
		return instance0;
	}

	inline Reader* FileSystem::rOpen(const String& sPath) const { return rOpen(sPath.c_str()); }
	inline void FileSystem::rClose(Reader*& r) const { delete r; r = 0; }

	inline Writer* FileSystem::wOpen(const String& sPath) const { return wOpen(sPath.c_str()); }
	inline Writer* FileSystem::wOpen(const char* pszPath, const String& sName) const
	{
		return wOpen(pszPath, sName.c_str());
	}
	inline void FileSystem::wClose(Writer*& w) const { delete w; w = 0; }

	inline bool FileSystem::FileExist(const String& sPath) { return FileExist(sPath.c_str()); }
	inline bool FileSystem::FileExist(const char* pszAlias, const String& sName) const
	{
		return FileExist(pszAlias, sName.c_str());
	}

	inline bool FileSystem::ResolvePath(const char* pszAlias, const String& sName, String& sFullPath) const
	{
		return ResolvePath(pszAlias, sName.c_str(), sFullPath);
	}
}
