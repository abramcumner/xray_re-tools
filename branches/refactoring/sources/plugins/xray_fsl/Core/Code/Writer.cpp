#include "StdAfx.h"
#include "Writer.h"

namespace xrFSL
{
	Writer::Writer(void)
	{
	}

	Writer::~Writer(void)
	{
	}

	//************************************
	// Method:    open_chunk
	// FullName:  xrFSL::Writer::open_chunk
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: uint32 id
	//************************************
	void Writer::open_chunk( uint32 id )
	{
		w_u32(id);
		w_u32(0);
		m_OpenChunks.push(tell());
	}

	//************************************
	// Method:    close_chunk
	// FullName:  xrFSL::Writer::close_chunk
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void Writer::close_chunk()
	{
		ASSERT_F(!m_OpenChunks.empty(), (""));

		size_t pos = tell();
		size_t chunk_pos = m_OpenChunks.top();

		ASSERT_F(chunk_pos <= pos, (""));

		seek(chunk_pos - 4);
		w_size_u32(pos - chunk_pos);
		seek(pos);

		m_OpenChunks.pop();
	}

	//************************************
	// Method:    w_raw_chunk
	// FullName:  xrFSL::Writer::w_raw_chunk
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: uint32 id
	// Parameter: const void * pData
	// Parameter: size_t size
	//************************************
	void Writer::w_raw_chunk( uint32 id, const void* pData, size_t size )
	{
		w_u32(id);
		w_size_u32(size);
		w_raw(pData, size);
	}

	//************************************
	// Method:    w_sz
	// FullName:  xrFSL::Writer::w_sz
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: const String & value
	//************************************
	void Writer::w_sz( const String& value )
	{
		w_raw(value.data(), value.length() + 1);
	}


	//************************************
	// Method:    MemoryWriter
	// FullName:  xrFSL::MemoryWriter::MemoryWriter
	// Access:    public 
	// Returns:   
	// Qualifier:
	//************************************
	MemoryWriter::MemoryWriter(): m_pos(0)
	{

	}

	//************************************
	// Method:    ~MemoryWriter
	// FullName:  xrFSL::MemoryWriter::~MemoryWriter
	// Access:    public 
	// Returns:   
	// Qualifier:
	//************************************
	MemoryWriter::~MemoryWriter()
	{

	}

	//************************************
	// Method:    w_raw
	// FullName:  xrFSL::MemoryWriter::w_raw
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: const void * pData
	// Parameter: size_t size
	//************************************
	void MemoryWriter::w_raw( const void* pData, size_t size )
	{
		if (size) 
		{
			if (m_pos + size > m_buffer.size())
				m_buffer.resize(m_pos + size);
			std::memmove(&m_buffer[m_pos], pData, size);
			m_pos += size;
		}
	}

	//************************************
	// Method:    seek
	// FullName:  xrFSL::MemoryWriter::seek
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: size_t offset
	//************************************
	void MemoryWriter::seek( size_t offset )
	{
		ASSERT_F(offset <= m_buffer.size(), (""));
		m_pos = offset;
	}

	//************************************
	// Method:    tell
	// FullName:  xrFSL::MemoryWriter::tell
	// Access:    public 
	// Returns:   size_t
	// Qualifier:
	//************************************
	size_t MemoryWriter::tell()
	{
		return m_pos;
	}

	//************************************
	// Method:    Save
	// FullName:  xrFSL::MemoryWriter::Save
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * pszPath
	//************************************
	bool MemoryWriter::Save( const char* pszPath )
	{
		FileSystem& fs = FileSystem::Instance();

		Writer* pW = fs.wOpen(pszPath);
		if (pW == 0)
			return false;

		pW->w_raw(&m_buffer[0], m_buffer.size());
		fs.wClose(pW);

		return true;
	}

	//************************************
	// Method:    Save
	// FullName:  xrFSL::MemoryWriter::Save
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const String & sPath
	//************************************
	bool MemoryWriter::Save( const String& sPath )
	{
		return Save(sPath.c_str());
	}

	//************************************
	// Method:    Save
	// FullName:  xrFSL::MemoryWriter::Save
	// Access:    public 
	// Returns:   bool
	// Qualifier:
	// Parameter: const char * pszPath
	// Parameter: const String & sName
	//************************************
	bool MemoryWriter::Save( const char* pszPath, const String& sName )
	{
		FileSystem& fs = FileSystem::Instance();

		Writer* pW = fs.wOpen(pszPath, sName);
		if (pW == 0)
			return false;

		pW->w_raw(&m_buffer[0], m_buffer.size());
		fs.wClose(pW);

		return true;
	}

	//************************************
	// Method:    FileWriter
	// FullName:  xrFSL::FileWriter::FileWriter
	// Access:    public 
	// Returns:   
	// Qualifier:
	//************************************
	FileWriter::FileWriter(): m_hFile(INVALID_HANDLE_VALUE)
	{

	}

	//************************************
	// Method:    FileWriter
	// FullName:  xrFSL::FileWriter::FileWriter
	// Access:    public 
	// Returns:   
	// Qualifier:
	// Parameter: HANDLE hFile
	//************************************
	FileWriter::FileWriter( HANDLE hFile ): m_hFile(hFile)
	{
	
	}

	//************************************
	// Method:    ~FileWriter
	// FullName:  xrFSL::FileWriter::~FileWriter
	// Access:    public 
	// Returns:   
	// Qualifier:
	//************************************
	FileWriter::~FileWriter()
	{
		CloseHandle(m_hFile);
	}

	//************************************
	// Method:    w_raw
	// FullName:  xrFSL::FileWriter::w_raw
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: const void * pSrc
	// Parameter: size_t src_size
	//************************************
	void FileWriter::w_raw( const void* pSrc, size_t src_size )
	{
		DWORD dwdWritten;
		BOOL ok = WriteFile(m_hFile, pSrc, DWORD(src_size & MAXDWORD), &dwdWritten, NULL);
		ASSERT_F(ok && src_size == dwdWritten, (""));
	}

	//************************************
	// Method:    seek
	// FullName:  xrFSL::FileWriter::seek
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: size_t offset
	//************************************
	void FileWriter::seek( size_t offset )
	{
		DWORD dwdNewPos = SetFilePointer(m_hFile, LONG(offset & MAXLONG), NULL, FILE_BEGIN);
		ASSERT_F(offset == dwdNewPos, (""));
	}

	//************************************
	// Method:    tell
	// FullName:  xrFSL::FileWriter::tell
	// Access:    public 
	// Returns:   size_t
	// Qualifier:
	//************************************
	size_t FileWriter::tell()
	{
		return SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
	}
}
