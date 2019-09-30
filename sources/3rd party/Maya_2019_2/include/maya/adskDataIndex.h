#ifndef adskDataIndex_h
#define adskDataIndex_h

#include <maya/adskCommon.h>
#include <maya/adskDebugCount.h>		// For DeclareObjectCounter()
#include <map>
#include <string>

// Forward declaration of member data
namespace adsk {
	namespace Debug {
		class Print;
		class Count;
		class Footprint;
	};
	namespace Data {
		class IndexType;
		typedef unsigned int IndexCount;

// ****************************************************************************
/*!
	\class adsk::Data::Index
 	\brief Lightweight class handling index values

	adsk::Data::Stream objects contain a list of data elements. Each element
	has to be accessed by index. In the simplest case the index is an array
	index (of type IndexCount, whose typedef appears above) but there can be
	more complex cases as well, such as a pair of integers, or a string.

	This is the base class used by the adsk::Data:Stream indexing system. It
	is configured to be as efficient as possible in the simple index case
	while allowing extension to arbitrary index types. The simple integer
	index value is contained within this class. More complex index types
	are stored in a letter class of type "IndexType".

	It can be used as the basis of a mapping from a general index type onto
	an adsk::Data::Index value, used for data referencing. Its purpose is
	to overload the concept of a general std::map<Index, Index> so that
	in the simple case where Index is the same as Index no mapping is
	required. The calling classes are responsible for creating that relationship,
	the methods in this class are defined to support that use.
*/

class METADATA_EXPORT Index
{
public:

	//----------------------------------------------------------------------
	// Constructor and type conversions
	Index	() : fComplexIndex( nullptr ), fIndex( 0 ) {}
	Index	( IndexCount value );
	Index	( const Index& rhs );
	Index	( const IndexType& rhs );
	Index	( const std::string& indexValue );
	~Index	();
	//
	Index&		operator=	( IndexCount rhs );
	Index&		operator=	( const Index& rhs );
	Index&		operator=	( const IndexType& rhs );
	operator	IndexCount	() const;
	IndexCount	index		() const;
	operator	IndexType*	();
	std::string	asString	() const;

	//! \fn IndexType* complexIndex()
	//! \brief Quick access to the complex index, if it's used
	//! \return Pointer to the complex index type, or NULL if the index is simple
	IndexType*		 complexIndex()		  { return fComplexIndex; }
	const IndexType* complexIndex() const { return fComplexIndex; }

	//----------------------------------------------------------------------
	// Dense mapping support
	bool		supportsDenseMode	() const;
	IndexCount	denseSpaceBetween	( const Index& rhs ) const;

	//----------------------------------------------------------------------
	// Comparison operators for easy sorting
	bool	operator==	(const Index& rhs)	const;
	bool	operator!=	(const Index& rhs)	const;
	bool	operator<	(const Index& rhs)	const;
	bool	operator<=	(const Index& rhs)	const;
	bool	operator>	(const Index& rhs)	const;
	bool	operator>=	(const Index& rhs)	const;

	//----------------------------------------------------------------------
	// Debug support
	static bool	Debug(const Index* me, Debug::Print& request);
	static bool	Debug(const Index* me, Debug::Footprint& request);
	DeclareObjectCounter( adsk::Data::Index );

private:
	IndexCount	fIndex;			//! Array index for direct data storage
	IndexType*	fComplexIndex;	//! Further data for complex index types

public:
	// Support for creation of arbitrary index types and values from a pair of
	// strings (one for the type, one for the value)
	//
	typedef Index		 	(*IndexCreator)	( const std::string& );
	enum eCreationErrors { kNoCreator, kBadSyntax, kExcessData };
	static  Index			doCreate		( const std::string&, const std::string& );
	static  IndexCreator	creator			( const std::string& );
	static  std::string		theTypeName		();
			std::string 	typeName		()			const;

private:
	// Everything down here is for internal use to manage the creation list.
	// The public access methods above is all of the useful functionality.
	friend class IndexType; // Friended to allow for automatic registration
	typedef std::map<std::string, IndexCreator> IndexTypeRegistry;
	static  bool		  registerType	( const std::string&, IndexCreator );
	static  Index		  TypeCreator	( const std::string& );
	static	IndexTypeRegistry& creators	();
};

	} // namespace Data
} // namespace adsk

//=========================================================
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================
#endif // adskDataIndex_h
