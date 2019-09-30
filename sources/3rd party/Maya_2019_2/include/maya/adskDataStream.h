#ifndef adskDataStream_h
#define adskDataStream_h

#include <maya/adskCommon.h>
#include <string>					// for std::string
#include <maya/adskDataHandle.h>			// for return type of element()
#include <maya/adskDataIndex.h>			// for IndexCount and Index
#include <maya/adskDebugCount.h>			// for Debug Count macro
#include <maya/adskDataStreamIterator.h>	// for iterator

// Forward declaration of member data
namespace adsk {
	namespace Debug {
		class Print;
		class Footprint;
	};
	namespace Data {
		class Structure;
		class StreamImpl;

// ****************************************************************************
/*!
	\class adsk::Data::Stream
 	\brief Class handling a list of generic data

	This class maintains a list of data with efficient operations for
	manipulating elements on the list. It works in the class hierarchy
	controlled by adsk::Data::Associations; see the documentation for that
	class for the bigger picture of how the classes interrelate.

	adsk::Data::Stream is responsible for managing an indexed list of data.
	It's conceptually like an array, optimized for operations expected
	to be common for this class hierarchy (deleting from the middle,
	inserting into several different spots in the middle, duplicating
	entries, filling sections of the array in one operation, etc.)
*/
class METADATA_EXPORT Stream
{
public:
	~Stream();
	Stream(const Stream&);
	Stream(const Structure& dataStructure, const std::string& streamName);
	Stream&		operator=	(const Stream&);
	bool		operator==	(const Stream&)					const;

	// For technical reasons these types live outside this class even
	// though they are a logical part of it. Declaring the typedefs lets
	// them be referenced as though they were still part of the class.
	typedef StreamIterator iterator;
	typedef StreamIterator const_iterator;

	// Element iterator support. Only const iteration is supported.
	// The iteration is always done in Index order.
	Stream::iterator		begin	();
	Stream::iterator		end		();
	Stream::const_iterator	cbegin	()						const;
	Stream::const_iterator	cend	()						const;

	// Manage the index type for this Stream
	bool		setIndexType(const std::string& indexTypeName);
	std::string	indexType	()								const;

	// Make this Stream's copy of the data unique
	bool		makeUnique	();

	// Remove all data from this Stream
	bool		clear		();

	// Manage the structure which defines the Stream data
	const Structure&	structure	()						const;
	bool				setStructure(const Structure& newStructure);

	// Get the Stream name. Renaming happens in the Channel
	const std::string&	name		()						const;

	// Methods operating independent of index type
	IndexCount	elementCount()						const;
    bool		mergeStream	( const Stream& streamEdits );

	// Methods for configuring storage modes
	bool	setElementRange 	( Index firstEl, Index lastEl );
	bool	useDenseStorage		( bool isDataDense );
	bool	setUseDefaults		( bool useTheDefaults );
	bool	useDefaults			()							const;

    // These are the methods needed for handling changes to the structure of
    // the associated data channel. They are kept simple on purpose; higher
    // level intelligence such as interpolating across neighboring vertices
    // in a polygon mesh is left for the channel owner to handle.
    bool	setElement      ( Index elementIndex, const Handle& newElement );
	Handle	element			( Index elementIndex );
	bool	hasElement		( Index elementIndex )			const;
    bool	reindexElement	( Index oldIndex, Index newIndex );
    bool	swapElements	( Index oldIndex, Index newIndex );
    bool	removeElement	( Index elementIndex );

	//----------------------------------------------------------------------
	// Debug support
	static bool	Debug(const Stream* me, Debug::Print& request);
	static bool	Debug(const Stream* me, Debug::Footprint& request);
	static const char* kDebugHex;	//! Flag indicating to print as hex
	DeclareObjectCounter( Stream );

	//----------------------------------------------------------------------

private:
	friend class StreamIterator;
	void*	fStorage;		//! Implementation details
	Stream ();

    inline StreamImpl const& impl() const;
    inline StreamImpl      & impl();
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
#endif // adskDataStream_h
