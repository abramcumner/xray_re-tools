#include "StdAfx.h"
#include "Reader.h"

namespace xrFSL
{
	const uint32 CHUNK_ID_MASK = ~Reader::CHUNK_COMPRESSED;

	//************************************
	// Method:    Reader
	// FullName:  xrFSL::Reader::Reader
	// Access:    public 
	// Returns:   // Qualifier: : m_pData(0), m_pCaret(0), m_pEnd(0), m_pNext(0)
	// Parameter: void
	//************************************
	Reader::Reader(void): m_pData(0), m_pCaret(0), m_pEnd(0), m_pNext(0)
	{
	}

	//************************************
	// Method:    Reader
	// FullName:  xrFSL::Reader::Reader
	// Access:    public 
	// Returns:   // Qualifier:
	// Parameter: const void * pBuffer
	// Parameter: size_t size
	//************************************
	Reader::Reader( const void* pBuffer, size_t size )
	{
		m_pNext = m_pCaret = m_pData = static_cast<const uint8*>(pBuffer);
		m_pEnd = m_pData + size;
	}

	//************************************
	// Method:    ~Reader
	// FullName:  xrFSL::Reader::~Reader
	// Access:    public 
	// Returns:   // Qualifier:
	// Parameter: void
	//************************************
	Reader::~Reader(void)
	{
 		
	}

	//************************************
	// Method:    ~BufferReader
	// FullName:  xrFSL::BufferReader::~BufferReader
	// Access:    public 
	// Returns:   // Qualifier:
	//************************************
	BufferReader::~BufferReader()
	{
		if (m_pData != 0)
		{
			free(const_cast<uint8*>(m_pData));
			m_pData = 0;
		}
	}

	//************************************
	// Method:    MMapReader
	// FullName:  xrFSL::MMapReader::MMapReader
	// Access:    public 
	// Returns:   // Qualifier: : m_hFile(INVALID_HANDLE_VALUE), m_hMMap(INVALID_HANDLE_VALUE)
	//************************************
	MMapReader::MMapReader(): m_hFile(INVALID_HANDLE_VALUE), m_hMMap(INVALID_HANDLE_VALUE)
	{
	}

	//************************************
	// Method:    MMapReader
	// FullName:  xrFSL::MMapReader::MMapReader
	// Access:    public 
	// Returns:   // Qualifier: : m_hFile(hFile), m_hMMap(hMMap)
	// Parameter: HANDLE hFile
	// Parameter: HANDLE hMMap
	// Parameter: const void * pData
	// Parameter: size_t size
	//************************************
	MMapReader::MMapReader( HANDLE hFile, HANDLE hMMap, const void* pData, size_t size ):
	m_hFile(hFile), m_hMMap(hMMap)
	{
		m_pNext = m_pCaret = m_pData = static_cast<const uint8*>(pData);
		m_pEnd = m_pData + size;
	}

	//************************************
	// Method:    ~MMapReader
	// FullName:  xrFSL::MMapReader::~MMapReader
	// Access:    public 
	// Returns:   // Qualifier:
	//************************************
	MMapReader::~MMapReader()
	{
		ASSERT_W(m_pData != 0, (""));
		ASSERT_W(m_hMMap != INVALID_HANDLE_VALUE, (""));
		ASSERT_W(m_hFile != INVALID_HANDLE_VALUE, (""));

		UnmapViewOfFile(const_cast<uint8*>(m_pData));
		CloseHandle(m_hMMap);
		CloseHandle(m_hFile);
	}

	//************************************
	// Method:    find_chunk
	// FullName:  xrFSL::Reader::find_chunk
	// Access:    public 
	// Returns:   size_t
	// Qualifier:
	// Parameter: uint32 uiID
	// Parameter: bool * pbCompressed
	// Parameter: bool bReset
	//************************************
	size_t Reader::find_chunk( uint32 uiID, bool* pbCompressed, bool bReset /* = true */ )
	{
		if (bReset)
			m_pCaret = m_pData;

		while (m_pCaret < m_pEnd)
		{
			ASSERT_F(m_pCaret + 8 <= m_pEnd, (""));

			uint32 id = r_u32();	// читаем ID блока
			size_t size = r_u32();	// читаем размер блока

			ASSERT_F(m_pCaret + size <= m_pEnd, (""));
			
			if (uiID == (id & CHUNK_ID_MASK))
			{
				ASSERT_W(pbCompressed || (id & CHUNK_COMPRESSED) == 0, (""));
			
				if (pbCompressed != 0)
					*pbCompressed = (id & CHUNK_COMPRESSED) != 0;

				return size;
			}
			m_pCaret += size;
		}
		return 0;
	}

	//************************************
	// Method:    find_chunk
	// FullName:  xrFSL::Reader::find_chunk
	// Access:    public 
	// Returns:   size_t
	// Qualifier:
	// Parameter: uint32 uiID
	//************************************
	size_t Reader::find_chunk( uint32 uiID )
	{
		return find_chunk(uiID, 0);
	}

	//************************************
	// Method:    r_raw
	// FullName:  xrFSL::Reader::r_raw
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: void * pDest
	// Parameter: size_t DestSize
	//************************************
	void Reader::r_raw( void* pDest, size_t DestSize )
	{
		ASSERT_F(m_pCaret + DestSize <= m_pEnd, (""));

		std::memmove(pDest, m_pCaret, DestSize);
		m_pCaret += DestSize;
	}

	//************************************
	// Method:    open_chunk
	// FullName:  xrFSL::Reader::open_chunk
	// Access:    public 
	// Returns:   Reader*
	// Qualifier:
	// Parameter: uint32 id
	//************************************
	Reader* Reader::open_chunk( uint32 id )
	{
		bool bCompressed;
		size_t size = find_chunk(id, &bCompressed);
		if (size == 0)
			return 0;

		ASSERT_F(!bCompressed, ("In current version compressed files doesn't supported."));
		
		return new Reader(m_pCaret, size);		
	}

	//************************************
	// Method:    close_chunk
	// FullName:  xrFSL::Reader::close_chunk
	// Access:    public 
	// Returns:   void
	// Qualifier: const
	// Parameter: Reader * & r
	//************************************
	void Reader::close_chunk( Reader*& r ) const
	{
		ASSERT_F(r == 0 || (r != this && r->m_pCaret <= r->m_pEnd), (""));

		delete r;
		r = 0;
	}

	//************************************
	// Method:    r_sz
	// FullName:  xrFSL::Reader::r_sz
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: String & sValue
	//************************************
	void Reader::r_sz( String& sValue )
	{
		const uint8* pData = m_pCaret;

		ASSERT_W(pData < m_pEnd, (""));

		while (*pData++) 
		{
			if (pData >= m_pEnd)
			{
				sValue.assign(m_pCaret, pData);
				m_pCaret = pData;
				return;
			}
		}

		sValue.assign(m_pCaret, pData - 1);
		m_pCaret = pData;
	}

	//************************************
	// Method:    r_raw_chunk
	// FullName:  xrFSL::Reader::r_raw_chunk
	// Access:    public 
	// Returns:   size_t
	// Qualifier:
	// Parameter: uint32 id
	// Parameter: void * pDest
	// Parameter: size_t uDestSize
	//************************************
	size_t Reader::r_raw_chunk( uint32 id, void* pDest, size_t uDestSize )
	{
		bool bCompressed;
		size_t size = find_chunk(id, &bCompressed);
		if (size == 0)
			return 0;

		ASSERT_F(!bCompressed, (""));
		ASSERT_F(size <= uDestSize, (""));
		r_raw(pDest, size);
		return size;
	}

	//************************************
	// Method:    skip_sz
	// FullName:  xrFSL::Reader::skip_sz
	// Access:    public 
	// Returns:   const char*
	// Qualifier:
	//************************************
	const char* Reader::skip_sz()
	{
		const uint8* p = m_pCaret;

		while (m_pCaret < m_pEnd) 
		{
			if (*m_pCaret++ == 0)
				return reinterpret_cast<const char*>(p);
		}
		// always crash if no '\0' in the chunk
		ASSERT_F(m_pCaret < m_pEnd, ("zero-terminate symbol not found in chunk"));
		return reinterpret_cast<const char*>(p);
	}
}
