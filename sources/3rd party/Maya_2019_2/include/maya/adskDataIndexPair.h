#ifndef adskDataIndexPair_h
#define adskDataIndexPair_h

#include <maya/adskCommon.h>
#include <maya/adskDataIndexType.h>	// For base class
#include <maya/adskDebugCount.h>		// For base class
#include <maya/adskDebugCRTP.h>		// For base class

// Forward declaration of member data
namespace adsk {
	namespace Debug {
		class Print;
		class Footprint;
	};
	namespace Data {

// ****************************************************************************
/*!
	\class adsk::Data::IndexPair
 	\brief Index type which uses a pair of IndexCount values for the index mapping

	adsk::Data::Stream objects contain a list of data elements. Each element
	has to be accessed by Index. This is one of the more general index types,
	a pair of IndexCount values. An adsk::Data::IndexPair element is used to
	lookup the physical location of a data element, either directly in an array
	using the dense mode or indirectly through a mapping in the sparse, or
	mapping, mode.
*/

class METADATA_EXPORT IndexPair : public CRTP_IndexType<IndexPair>
								, public adsk::Debug::CRTP_Debug<IndexPair,adsk::Debug::Print>
								, public adsk::Debug::CRTP_Debug<IndexPair,adsk::Debug::Footprint>
{
public:
	//----------------------------------------------------------------------
	// Constructor and type conversions
	//
	IndexPair			( IndexCount firstValue, IndexCount secondValue );
	IndexPair			( const IndexPair& rhs );
	IndexPair			( const std::string& value );
	virtual	~IndexPair	();

			IndexPair&	operator=	( const IndexPair& rhs );
	//
			void		getIndexPair(IndexCount& first, IndexCount& second) const;

	virtual	std::string	asString			()								const;
	virtual bool		supportsDenseMode	()								const;
	virtual IndexCount	denseSpaceBetween	( const IndexType& rhs )		const;

	//----------------------------------------------------------------------
	// Comparison operators for easy sorting
	// Primary sort by fFirstIndex, secondary sort by fSecondIndex
	//
	virtual bool	operator==	(const IndexType& rhs)	const;
	virtual bool	operator!=	(const IndexType& rhs)	const;
	virtual bool	operator<	(const IndexType& rhs)	const;
	virtual bool	operator<=	(const IndexType& rhs)	const;
	virtual bool	operator>	(const IndexType& rhs)	const;
	virtual bool	operator>=	(const IndexType& rhs)	const;
	//
	virtual bool	operator==	(const IndexPair& rhs)	const;
	virtual bool	operator!=	(const IndexPair& rhs)	const;
	virtual bool	operator<	(const IndexPair& rhs)	const;
	virtual bool	operator<=	(const IndexPair& rhs)	const;
	virtual bool	operator>	(const IndexPair& rhs)	const;
	virtual bool	operator>=	(const IndexPair& rhs)	const;

	//----------------------------------------------------------------------
	// Debug support
	static bool	Debug(const IndexPair* me, adsk::Debug::Print& request);
	static bool	Debug(const IndexPair* me, adsk::Debug::Footprint& request);
	DeclareObjectCounter( adsk::Data::IndexPair );

protected:
	IndexPair	();				// Put here to prevent accidental usage
	IndexCount	fFirstIndex;	//! First index used for mapping
	IndexCount	fSecondIndex;	//! Second index used for mapping

	friend	class CRTP_IndexType<IndexPair>;
	static	std::string			myTypeName;		//! Name used to identify this type
	static	IndexRegistration	myRegistration; //! Automatically registers this type
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
#endif // adskDataIndexPair_h
