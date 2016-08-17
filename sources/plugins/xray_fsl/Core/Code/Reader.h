#pragma once

#include "Prerequisites.h"
#include <functional>

namespace xrFSL
{
	class Reader
	{
	public:
		enum {
			CHUNK_COMPRESSED	= 0x80000000,
		};

		Reader(void);
		Reader(const void* pBuffer, size_t size);
		virtual ~Reader(void);

		/**
		* @brief
		*/
		size_t find_chunk(uint32 uiID, bool* pbCompressed, bool bReset = true);
		size_t find_chunk(uint32 uiID);

		Reader* open_chunk(uint32 id);
		void close_chunk(Reader*& r) const;

		template<typename T> size_t r_chunk(uint32 id, T& value);
		template<typename T, typename F> void r_chunks(T& container, F read);

		/*
		size_t		find_chunk(uint32_t id, bool* compressed, bool reset = true);
		size_t		find_chunk(uint32_t id);
		void		debug_find_chunk();
		xr_reader*	open_chunk(uint32_t id);
		xr_reader*	open_chunk(uint32_t id, const xr_scrambler& scrambler);
		xr_reader*	open_chunk_next(uint32_t& id, xr_reader* iter);
		void		close_chunk(xr_reader*& r) const;

		size_t		size() const;
		const void*	data() const;		
		bool		eof() const;
		
		size_t		elapsed() const;
		*/

		size_t r_raw_chunk(uint32 id, void* pDest, size_t uDestSize);
		

		void r_raw(void* pDest, size_t DestSize);

		uint32 r_u32();
		int32 r_s32();
		uint32 r_u24();
		uint16 r_u16();
		int16 r_s16();
		uint8 r_u8();
		int8 r_s8();
		bool r_bool();
		float r_float();
		void r_sz(String& sValue);
		const char*	skip_sz();

		/**
		 *	
		 */
		template<typename T> void r_seq(size_t n, T& container);
		/**
		 *	
		 */
		template<typename T, typename F> void r_seq(size_t n, T& container, F read);
		
		/**
		*	
		*/
		template<typename T> struct f_r: public std::binary_function<T, Reader, void> {};

		/**
		 *	
		 */
		struct f_r_sz: public f_r<String> 
		{
			void operator()( String& s, Reader& r ) { r.r_sz(s); }
		};

		/**
		 *	
		 */
		template<typename T> struct f_r_new: public f_r<T> 
		{
			explicit f_r_new(void (T::*_pmf)(Reader& r)): pmf(_pmf) {}
			void operator()(T*& p, Reader& r) { T* _p = new T; (_p->*pmf)(r); p = _p; }
		private:
			void (T::*pmf)(Reader& r);
		};

		/**
		* @brief Return data size.	
		*/
		size_t size() const;

		size_t tell() const;
		void seek(size_t pos);
		void advance(size_t offset);

		/**
		* @brief Return type casted data pointer.
		*/
		template<typename T> const T* pointer() const;

	protected:
		const uint8* m_pData;

		union {
			const uint8* m_pCaret;
			const int8* m_pCaret_s8;
			const uint16* m_pCaret_u16;
			const int16* m_pCaret_s16;
			const uint32* m_pCaret_u32;
			const int32* m_pCaret_s32;
			const float* m_pCaret_f;
		};

		const uint8* m_pEnd;
		const uint8* m_pNext;
	};

	/**
	*	
	*/
	class BufferReader: public Reader
	{
	public:
		BufferReader(const uint8* pBuffer, size_t size);
		virtual ~BufferReader();
	};

	/**
	*	
	*/
	class MMapReader: public Reader
	{
	public:
		MMapReader();
		MMapReader(HANDLE hFile, HANDLE hMMap, const void* pData, size_t size);
		virtual	~MMapReader();

	private:
		HANDLE m_hFile;
		HANDLE m_hMMap;
	};

	//==================================================
	// inline impl
	//==================================================
	inline size_t Reader::size() const { ASSERT_F(m_pData <= m_pEnd, ("")); return m_pEnd - m_pData; }
	inline void Reader::seek(size_t ofs) { m_pCaret = m_pData + ofs; ASSERT_F(m_pCaret <= m_pEnd, ("")); }
	inline void Reader::advance(size_t ofs) { m_pCaret += ofs; ASSERT_F(m_pCaret <= m_pEnd, ("")); }
	//inline bool Reader::eof() const { assert(m_p <= m_end); return m_p == m_end; }
	inline size_t Reader::tell() const { ASSERT_F(m_pCaret <= m_pEnd, ("")); return m_pCaret - m_pData; }

	template<typename T> inline const T* Reader::pointer() const 
	{ 
		return reinterpret_cast<const T*>(m_pCaret);
	}

	template<typename T> inline void Reader::r_seq(size_t n, T& container)
	{
		typename T::const_pointer p = pointer<typename T::value_type>();
		container.reserve(n);
		container.assign(p, p + n);
		advance(n*sizeof(typename T::value_type));
	}

	template<typename T, typename F> inline void Reader::r_seq(size_t n, T& container, F read)
	{
		container.reserve(n);
		while (n--) 
		{
			container.push_back(typename T::value_type());
			read(container.back(), *this);
		}
	}

	template<typename T> inline size_t Reader::r_chunk(uint32 id, T& value)
	{
		return r_raw_chunk(id, &value, sizeof(T));
	}

	template<> inline size_t Reader::r_chunk(uint32 id, String& sValue)
	{
		size_t size = find_chunk(id);
		if (size) 
		{
			r_sz(sValue);
		}
		return size;
	}

	template<typename T, typename F> inline void Reader::r_chunks(T& container, F read)
	{
		Reader* s;
		for (uint32_t id = 0; (s = open_chunk(id)); ++id) 
		{
			container.push_back(typename T::value_type());
			read(container.back(), *s);
			close_chunk(s);
		}
	}

	inline BufferReader::BufferReader(const uint8* pBuffer, size_t size): Reader(pBuffer, size) {}

	inline uint32 Reader::r_u32() { return *m_pCaret_u32++; }
	inline int32 Reader::r_s32() { return *m_pCaret_s32++; }
	inline uint32 Reader::r_u24() { uint32 u24 = 0; r_raw(&u24, 3); return u24; }
	inline uint16 Reader::r_u16() { return *m_pCaret_u16++; }
	inline int16 Reader::r_s16() { return *m_pCaret_s16++; }
	inline uint8 Reader::r_u8() { return *m_pCaret++; }
	inline int8 Reader::r_s8() { return *m_pCaret_s8++; }
	inline bool Reader::r_bool() { return *m_pCaret++ != 0; }
	inline float Reader::r_float() { return *m_pCaret_f++; }
}	// end of namespace xrFSL
