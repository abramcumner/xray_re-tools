#include "StdAfx.h"
#include "FileSystem.h"

namespace xrFSL
{
	FileSystem::FileSystem(void): m_uFlags(0)
	{
	}

	FileSystem::~FileSystem(void)
	{
	}

	//************************************
	// Method:    Initialize
	// FullName:  xrFSL::FileSystem::Initialize
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * pFSSpec
	// Parameter: unsigned uFlags
	//************************************
	bool FileSystem::Initialize( const char* pFSSpec, unsigned uFlags )
	{
		if (pFSSpec && pFSSpec[0] != '\0')
		{
			Reader* pR = rOpen(pFSSpec);
			if (!pR)
				return false;

			if (ParseFSSpec(*pR))
			{
				String sFolder;
				StringUtil::SplitPath(pFSSpec, &sFolder);
				AddPathAlias(PA_FS_ROOT, sFolder, "");
			}
			rClose(pR);
		}
		// ѕроводить ли пустую инициализацию
// 		else
// 		{
// 			AddPathAlias(PA_FS_ROOT, "", "");
// 		}

		m_uFlags = uFlags;

		return !m_vecAliases.empty();
	}

	static inline const char* NextLine(const char* p, const char* end)
	{
		while (p < end && *p++ != '\n') {}
		return p;
	}

	static inline const char* ReadAlias(const char* p, const char* end)
	{
		if (p >= end || *p++ != '$')
			return 0;
		if (p >= end || (!std::isalnum(*p) && *p != '_'))
			return 0;

		for (++p; p < end;) 
		{
			int ch = *p;
			if (ch == '$')
				return p + 1;
			else if (!std::isalnum(ch) && ch != '_')
				break;
			++p;
		}
		return 0;
	}

	static inline const char* SkipSS(const char* p, const char* end)
	{
		while (p < end) {
			int c = *p;
			if (c != ' ' && c != '\t')
				break;
			++p;
		}
		return p;
	}

	static inline const char* ReadValue(const char*& _p, const char* end)
	{
		const char* p = SkipSS(_p, end);
		_p = p;
		const char* last_ss = 0;
		while (p < end) {
			int ch = *p;
			if (ch == ' ' || ch =='\t') {
				if (last_ss == 0)
					last_ss = p;
			} else if (ch == '\n' || ch == '\r' || ch == '|') {
				if (last_ss == 0)
					last_ss = p;
				break;
			} else {
				last_ss = 0;
			}
			++p;
		}
		return last_ss ? last_ss : p;
	}

	//************************************
	// Method:    ParseFSSpec
	// FullName:  xrFSL::FileSystem::ParseFSSpec
	// Access:    private 
	// Returns:   bool
	// Qualifier:
	// Parameter: Reader & r
	//************************************
	bool FileSystem::ParseFSSpec( Reader& r )
	{
		const char* pCaret = r.pointer<const char>();
		const char* pEnd = pCaret + r.size();

		String sAlias, sValues[4];

		for (unsigned line = 1; pCaret < pEnd; pCaret = NextLine(pCaret, pEnd), ++line)
		{
			int ch = *pCaret;
			if (ch == '$')
			{
				const char* pLast = ReadAlias(pCaret, pEnd);
				if (pLast == 0)
				{
					LOGSE << "Can't parse FS specification file, line: " << line;
					return false;
				}

				sAlias.assign(pCaret, pLast);

				pCaret = SkipSS(pLast, pEnd);
				if (pCaret == pEnd || *pCaret++ != '=')
				{
					LOGSE << "Can't parse FS specification file, line: " << line;
					return false;
				}

				int i;
				for (i = -2; i < 4;)
				{
					pLast = ReadValue(pCaret, pEnd);

					// ≈сли не дошли до фактических параметров алиаса, но в тоже врем€
					// достигли конца файла или за параметром не следует разделитель,
					// тогда считаем строку ошибочной
					if (i < 0 && (pLast == pEnd || *SkipSS(pLast, pEnd) != '|'))
					{
						LOGSE << "Can't parse FS specification file, line: " << line;
						return false;
					}

					// ѕосле пропуска неинтересующих нас параметров сохран€ем все последующие
					// параметры алиаса
					if (i >= 0)
						sValues[i].assign(pCaret, pLast);

					// ѕропускаем разделитель опций
					pCaret = SkipSS(pLast, pEnd) + 1;
					++i;
					
					// ≈сли после значени€ нет разделител€ или достигли конца файла - завершаем цикл
					if (pCaret == pEnd || *SkipSS(pLast, pEnd) != '|')
						break;
				}

				ASSERT_W(i > 0, ("Invalid line %u format.", line));

				if (i < 2)
					sValues[1].clear();

				// –егистрируем алиас
				SPathAlias* pPA = AddPathAlias(sAlias, sValues[0], sValues[1]);
				if (pPA == 0)
				{
					LOGSE << "Can't parse FS specification file, line: " << line;
					return false;
				}

				if (i > 2)
					pPA->sFilter = sValues[2];
				if (i > 3)
					pPA->sCaption = sValues[3];
			}
			else if (ch != ';' && !std::isspace(ch))
			{
				LOGSE << "Can't parse FS specification file, line: " << line;
				return false;
			}
		}
		return true;
	}
	
	//************************************
	// Method:    AddPathAlias
	// FullName:  xrFSL::FileSystem::AddPathAlias
	// Access:    private 
	// Returns:   FileSystem::SPathAlias*
	// Qualifier:
	// Parameter: const String & sPath
	// Parameter: const String & sRoot
	// Parameter: const String & sAdd
	//************************************
	FileSystem::SPathAlias* FileSystem::AddPathAlias( const String& sPathAlias, const String& sRoot, const String& sAdd )
	{
		// убедимс€ что такого алиаса ещЄ не существует
		const SPathAlias* pPA = FindPathAlias(sPathAlias.c_str());
		ASSERT_W(pPA == 0, ("Such alias([%s]:%s) alredy exists.", sPathAlias, sRoot));

		if (pPA)
			return 0;

		// добавим алиас
		SPathAlias* pNewPA = new SPathAlias;
		pNewPA->sPathAlias = sPathAlias;
		m_vecAliases.push_back(pNewPA);

		pPA = FindPathAlias(sRoot.c_str());
		if (pPA)
		{
			pNewPA->sRoot = pPA->sRoot;
		}
		else
		{
			pNewPA->sRoot = sRoot;
			StringUtil::AppendPathSeparator(pNewPA->sRoot);
		}

		pNewPA->sRoot += sAdd;
		StringUtil::AppendPathSeparator(pNewPA->sRoot);

		return pNewPA;
	}

	//************************************
	// Method:    FindPathAlias
	// FullName:  xrFSL::FileSystem::FindPathAlias
	// Access:    private 
	// Returns:   const FileSystem::SPathAlias*
	// Qualifier: const
	// Parameter: const char * pPath
	//************************************
	const FileSystem::SPathAlias* FileSystem::FindPathAlias( const char* pAlias ) const
	{
		for (SPathAlias_vec_cit it = m_vecAliases.begin(), end = m_vecAliases.end(); it != end; ++it)
		{
			if ((*it)->sPathAlias == pAlias)
				return *it;
		}
		return 0;
	}

	//************************************
	// Method:    rOpen
	// FullName:  xrFSL::FileSystem::rOpen
	// Access:    public 
	// Returns:   Reader*
	// Qualifier: const
	// Parameter: const char * pPath
	//************************************
	Reader* FileSystem::rOpen( const char* pPath ) const
	{
		HANDLE hFile = CreateFileA(pPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			LOGSE << "Error occurred at opening file, ErrCode: " << GetLastError();
			return 0;
		}

		LARGE_INTEGER size64;
		if (!GetFileSizeEx(hFile, &size64) || size64.HighPart != 0)
		{
			CloseHandle(hFile);
			return 0;
		}

		DWORD dwdLen = size64.LowPart;
		
		SYSTEM_INFO si;
		GetSystemInfo(&si);

		Reader* pR = 0;
		LPVOID lpData;

		if (dwdLen < si.dwAllocationGranularity)
		{
			DWORD dwdRead;
			PBYTE pBuffer = (PBYTE)malloc(dwdLen);

			if (pBuffer != NULL)
			{
				if (ReadFile(hFile, pBuffer, dwdLen, &dwdRead, NULL) && dwdRead == dwdLen)
				{
					pR = new BufferReader(pBuffer, dwdLen);
				}
				else
				{
					free(pBuffer);
				}
			}

			CloseHandle(hFile);
			return pR;
		}
		
		HANDLE hMMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, dwdLen, NULL);
		if (hFile == NULL)
		{
			LOGSE << "Error occurred at mapping file, ErrCode: " << GetLastError();
			CloseHandle(hFile);
			return 0;
		}

		lpData = MapViewOfFile(hMMap, FILE_MAP_READ, 0, 0, dwdLen);
		if (lpData != NULL)
		{
			pR = new MMapReader(hFile, hMMap, lpData, dwdLen);
			if (pR)
				return pR;

			UnmapViewOfFile(lpData);
		}

		CloseHandle(hMMap);
		CloseHandle(hFile);

		return 0;
	}

	//************************************
	// Method:    rOpen
	// FullName:  xrFSL::FileSystem::rOpen
	// Access:    public 
	// Returns:   Reader*
	// Qualifier: const
	// Parameter: const char * pszPath
	// Parameter: const char * pszName
	//************************************
	Reader* FileSystem::rOpen( const char* pszPath, const char* pszName ) const
	{
		const SPathAlias* pPA = FindPathAlias(pszPath);
		return pPA ? rOpen(pPA->sRoot + pszName) : 0;
	}

	//************************************
	// Method:    ResolvePath
	// FullName:  xrFSL::FileSystem::ResolvePath
	// Access:    public 
	// Returns:   bool
	// Qualifier: const
	// Parameter: const char * pAlias
	// Parameter: const char * pName
	// Parameter: String & sFullPath
	//************************************
	bool FileSystem::ResolvePath( const char* pAlias, const char* pName, String& sFullPath ) const
	{
		const SPathAlias* pPA = FindPathAlias(pAlias);

		if (pPA == 0)
			return false;

		sFullPath = pPA->sRoot;

		if (pName)
			sFullPath.append(pName);

		return true;
	}

	//************************************
	// Method:    FileExist
	// FullName:  xrFSL::FileSystem::FileExist
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * pszPath
	//************************************
	bool FileSystem::FileExist( const char* pszPath )
	{
		DWORD attrs = GetFileAttributesA(pszPath);
		if (attrs == INVALID_FILE_ATTRIBUTES || (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0)
			return false;
		return true;
	}

	//************************************
	// Method:    FileExist
	// FullName:  xrFSL::FileSystem::FileExist
	// Access:    public 
	// Returns:   bool
	// Qualifier: const
	// Parameter: const char * pszAlias
	// Parameter: const char * pszName
	//************************************
	bool FileSystem::FileExist( const char* pszAlias, const char* pszName ) const
	{
		const SPathAlias* pPA = FindPathAlias(pszAlias);
		return pPA ? FileExist(pPA->sRoot + pszName) : false;
	}

	//************************************
	// Method:    wOpen
	// FullName:  xrFSL::FileSystem::wOpen
	// Access:    public 
	// Returns:   Writer*
	// Qualifier: const
	// Parameter: const char * pszPath
	// Parameter: bool bIgnoreRO
	//************************************
	Writer* FileSystem::wOpen( const char* pszPath ) const
	{
		HANDLE hFile = CreateFileA(pszPath, GENERIC_WRITE, FILE_SHARE_READ, 
			NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		ASSERT_F(hFile != INVALID_HANDLE_VALUE, ("Can't create file %s, Reason: %d", pszPath, GetLastError()));

		if (hFile == INVALID_HANDLE_VALUE)
			return 0;

		Writer* pW = new FileWriter(hFile);
		if (pW == 0)
			CloseHandle(hFile);

		return pW;
	}

	//************************************
	// Method:    wOpen
	// FullName:  xrFSL::FileSystem::wOpen
	// Access:    public 
	// Returns:   Writer*
	// Qualifier: const
	// Parameter: const char * pszPath
	// Parameter: const char * pszName
	//************************************
	Writer* FileSystem::wOpen( const char* pszPath, const char* pszName ) const
	{
		// »щем алиас заданного пути
		const SPathAlias* pPA = FindPathAlias(pszPath);

		// если нашли алиас, тогда пробуем открыть файл на запись по заданному пути и имиени файла
		return pPA ? wOpen(pPA->sRoot + pszName) : false;
	}
}
