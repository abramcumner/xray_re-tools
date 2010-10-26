#pragma once


#include <vector>
#include "Types.h"

#ifdef _DEBUG
#  define class_it_base class
#else
#  define class_it_base struct
#endif

// Disable warning
/// class 'std::container' needs to have dll-interface
#   pragma warning (disable : 4251)

//-------------------------------------------
// DLL Export macros
//-------------------------------------------
#ifdef xrFSL_CORE_EXPORTS
#define xrFSL_CORE_API __declspec(dllexport)
#else
#define xrFSL_CORE_API __declspec(dllimport)
#endif

namespace xrFSL
{
	//-------------------------------------------
	// Type defs helpers
	//-------------------------------------------
#define TYPEDEF_STD_VECTOR(type) \
	typedef std::vector< type > type##_vec; \
	typedef std::vector< type >::iterator type##_vec_it; \
	typedef std::vector< type >::const_iterator type##_vec_cit;

#define TYPEDEF_STD_VECTOR_PTR(type) \
	typedef std::vector< type* > type##_vec; \
	typedef std::vector< type* >::iterator type##_vec_it; \
	typedef std::vector< type* >::const_iterator type##_vec_cit;

#define EXPORT_STD_VECTOR(type) \
	template class xrFSL_CORE_API std::allocator< type* >; \
	template class xrFSL_CORE_API std::vector< type*, std::allocator< type* > >;	
// 	template class xrFSL_CORE_API std::vector< type*, std::allocator< type > >::iterator type##_vec_it;	\
// 	template class xrFSL_CORE_API std::vector< type*, std::allocator< type > >::const_iterator type##_vec_cit;
#define EXPORT_STD_VECTOR_CONST_ITERATOR(type) \
	class_it_base xrFSL_CORE_API std::_Iterator_base; \
	template class xrFSL_CORE_API std::_Vector_const_iterator<type, std::allocator<type> >;



	/**
	 *	
	 */
	template<typename T> void DeleteElements(T* container[], size_t n)
	{
		for (T **p = container, **end = p + n; p != end; ++p)
			delete *p;
	}

	/**
	 *	
	 */
	template<typename T> void DeleteElements(T& container)
	{
		for (typename T::iterator it = container.begin(), end = container.end(); it != end; ++it)
			delete *it;
	}

	/**
	 *	
	 */
	template<typename T> static inline typename T::value_type FindByName(T& container, const char* pszName)
	{
		for (typename T::iterator it = container.begin(),
			end = container.end(); it != end; ++it) {
				if ((*it)->name() == pszName)
					return *it;
		}
		return 0;
	}

	/**
	*	ѕоиск заданного значени€ в std::Map по ключу, если значение не найдено, 
	*	возращает значение по умолчанию
	*	@param map - где ищем, std::Map
	*	@param key - ключ по которому ищем
	*	@param defaultValue - значение по умолчанию
	*	@return значение по ключу, если не найдено, тогда defaultValue.
	*	@code
	std::Map<String, int> var;
	var["Key"] = 1;

	int i = find_in_map(var, "Key", 0);
	@endcode
	*/
	template <typename Map>
	inline typename Map::mapped_type FindInMap(const Map& map, 
		const typename Map::key_type& key,
		typename Map::mapped_type defaultValue)
	{
		typename Map::const_iterator it = map.find (key);
		if (it == map.end())
			return defaultValue;
		else
			return it->second;
	}
}