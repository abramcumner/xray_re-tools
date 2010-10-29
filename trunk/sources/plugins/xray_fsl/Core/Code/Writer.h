#pragma once

#include <stack>

namespace xrFSL
{
	/**
	 *	
	 */
	class Writer
	{
	public:
		Writer(void);
		virtual ~Writer(void);

		virtual void w_raw(const void* pData, size_t size) = 0;
		virtual void seek(size_t offset) = 0;
		virtual size_t tell() = 0;

		/* Chunks */
		void open_chunk(uint32 id);
		void close_chunk();
		void w_raw_chunk(uint32 id, const void* pData, size_t size);
		void w_chunk(uint32 id, const String& s);
		template<typename T> void w_chunk(uint32 id, const T& value);
		template<typename T, typename F> void w_chunks(const T& container, F writer);

		/* Sequences methods */
		template<typename T> void w_seq(const T& container);
		template<typename T, typename F> void w_seq(const T& container, F writer);

		/* Write methods */
		template<typename T> void w(const T& value);

		void w_sz(const String& value);
		void w_u32(uint32 value);
		void w_s32(int32 value);
		void w_u24(uint32 value);
		void w_u16(uint16 value);
		void w_s16(int16 value);
		void w_u8(uint8 value);
		void w_s8(int8 value);
		void w_bool(bool value);

		void w_size_u32(size_t value);
		void w_size_u16(size_t value);
		void w_size_u8(size_t value);

		template<typename T> struct f_w: public std::binary_function<T, Writer, void> {};
		struct f_w_sz: public f_w<std::string> 
		{
			void operator()(const String& s, Writer& w) { w.w_sz(s); }
		};
		template<typename T> struct f_w_const: public std::const_mem_fun1_t<void, T, Writer&> 
		{
			explicit f_w_const(void (T::*_pmf)(Writer& w) const): std::const_mem_fun1_t<void, T, Writer&>(_pmf) {}
		};

	private:
		std::stack<size_t> m_OpenChunks;
	};


	/**
	 *	
	 */
	class MemoryWriter: public Writer
	{
	public:
		MemoryWriter();
		~MemoryWriter();

		virtual void w_raw(const void* pData, size_t size);
		virtual void seek(size_t offset);
		virtual size_t tell();

		const uint8* data() const;

		bool Save(const char* pszPath);
		bool Save(const String& sPath);
		bool Save(const char* pszPath, const String& sName);

	private:
		std::vector<uint8> m_buffer;
		size_t m_pos;
	};


	/**
	 *	
	 */
	class FileWriter: public Writer
	{
	public:
		FileWriter();
		FileWriter(HANDLE hFile);
		virtual ~FileWriter();

		virtual void w_raw(const void* pSrc, size_t src_size);
		virtual void seek(size_t offset);
		virtual size_t tell();

	private:
		HANDLE		m_hFile;
	};

	/************************************************************************/
	/* Inline impl                                                          */
	/************************************************************************/
	inline void Writer::w_chunk(uint32 id, const String& s)
	{
		open_chunk(id);
		w_sz(s);
		close_chunk();
	}

	template<typename T> inline void Writer::w_chunk(uint32 id, const T& value)
	{
		w_raw_chunk(id, &value, sizeof(T));
	}

	template<typename T, typename F> inline void Writer::w_chunks(const T& container, F writer)
	{
		typename T::const_iterator it = container.begin(), end = container.end();
		for (uint32_t id = 0; it != end; ++it) 
		{
			open_chunk(id++);
			writer(*it, *this);
			close_chunk();
		}
	}

	template<typename T> inline void Writer::w_seq(const T& container)
	{
		if (!container.empty())
			w_raw(&container[0], container.size() * sizeof(typename T::value_type));
	}

	template<typename T, typename F> inline void Writer::w_seq(const T& container, F writer)
	{
		for (typename T::const_iterator it = container.begin(),
			end = container.end(); it != end; ++it) 
		{
				writer(*it, *this);
		}
	}

	template<typename T> inline void Writer::w(const T& value) { w_raw(&value, sizeof(T)); }
	inline void Writer::w_u32(uint32 value) { w<uint32>(value); }
	inline void Writer::w_s32(int32 value) { w<int32>(value); }
	inline void Writer::w_u24(uint32 value) { w_raw(&value, 3); }
	inline void Writer::w_u16(uint16 value) { w<uint16>(value); }
	inline void Writer::w_s16(int16 value) { w<int16>(value); }
	inline void Writer::w_u8(uint8 value) { w<uint8>(value); }
	inline void Writer::w_s8(int8 value) { w<int8>(value); }
	inline void Writer::w_bool(bool value) { w_u8(value ? 1 : 0); }

	inline void Writer::w_size_u32(size_t value) { w_u32(static_cast<uint32>(value & UINT32_MAX)); }
	inline void Writer::w_size_u16(size_t value) { w_u16(static_cast<uint16>(value & UINT16_MAX)); }
	inline void Writer::w_size_u8(size_t value) { w_u8(static_cast<uint8>(value & UINT8_MAX)); }
}
