#ifndef adskDataIndexString_h
#define adskDataIndexString_h

#include <maya/adskCommon.h>
#include <string>				// For member fStringIndex
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
	\class adsk::Data::IndexString
 	\brief Index type which uses a String for the index mapping

	This is a more general type of indexing that can only be used in mapping
	mode since it doesn't make sense to define "all strings between two
	strings".
*/

class METADATA_EXPORT IndexString : public CRTP_IndexType<IndexString>
								  , public adsk::Debug::CRTP_Debug<IndexString,adsk::Debug::Print>
								  , public adsk::Debug::CRTP_Debug<IndexString,adsk::Debug::Footprint>
{
public:
	//----------------------------------------------------------------------
	// Constructor and type conversions
	IndexString			( const std::string&  stringIndex );
	IndexString			( const IndexString& rhs );
	virtual	~IndexString	();
	//
	virtual IndexString&	operator=	( const IndexString& rhs );
	virtual IndexString&	operator=	( const std::string& rhs );
			std::string		indexString	()							 const;

	virtual std::string	asString			()						 const;
	virtual bool		supportsDenseMode	()						 const;
	virtual IndexCount	denseSpaceBetween	( const IndexType& rhs ) const;

	//----------------------------------------------------------------------
	// Comparison operators for easy sorting
	// Sorting order is by primary (inherited) index first, secondary index
	// (in this class) second.
	virtual bool	operator==	(const IndexType& rhs)		const;
	virtual bool	operator!=	(const IndexType& rhs)		const;
	virtual bool	operator<	(const IndexType& rhs)		const;
	virtual bool	operator<=	(const IndexType& rhs)		const;
	virtual bool	operator>	(const IndexType& rhs)		const;
	virtual bool	operator>=	(const IndexType& rhs)		const;
	//
	virtual bool	operator==	(const IndexString& rhs)	const;
	virtual bool	operator!=	(const IndexString& rhs)	const;
	virtual bool	operator<	(const IndexString& rhs)	const;
	virtual bool	operator<=	(const IndexString& rhs)	const;
	virtual bool	operator>	(const IndexString& rhs)	const;
	virtual bool	operator>=	(const IndexString& rhs)	const;

	//----------------------------------------------------------------------
	// Debug support
	static bool	Debug(const IndexString* me, adsk::Debug::Print& request);
	static bool	Debug(const IndexString* me, adsk::Debug::Footprint& request);
	DeclareObjectCounter( adsk::Data::IndexString );

protected:
	IndexString();				 // Put here to prevent accidental usage
	std::string	fStringIndex;	 //! String index used for mapping

	friend	class CRTP_IndexType<IndexString>;
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
#endif // adskDataIndexString_h
