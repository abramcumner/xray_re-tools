#ifndef adskDataStreamIterator_h
#define adskDataStreamIterator_h

#include <maya/adskCommon.h>
#include <maya/adskDataIndex.h>	// for iterator value type

// Forward declaration of member data
namespace adsk {
	namespace Data {
		class StreamIterator;
		class StreamIteratorImpl;
		class Stream;
		class Handle;

//----------------------------------------------------------------------
/*!
	\class adsk::Data::StreamIterator
 	\brief Class handling iteration over Stream members

	The Stream data is not necessarily stored sequentially or as a
	packed array so this iterator is necessary in order to provide a
	consistent way to access all Stream elements.

	By default the Handle pointed to by the iterator will be positioned
	at the first Structure member. If you wish a different member you
	should reposition it after retrieval. (It's recommended for
	efficiency that you position by Member index rather than by name
	since that works faster.)

	For technical reasons this class lives outside the Stream though really
	it is part of it. A typedef is set up inside the Stream so that you
	can use it like a standard iterator:

		for( Stream::iterator iterator = myStream.begin();
			 iterator != myStream.end();
			 ++iterator )
		{
			processMember( *iterator );
		}
	
	If you are processing multiple independent values in your Structure 
	it's most efficient to run multiple loops repositioning the Handle
	between each loop:

		Stream::iterator idIterator = myStream.begin();
		colorIterator->setPositionByMemberName( "ID" );
		for( ; iterator != myStream.end(); ++iterator )
		{
			processIDMember( iterator->asInt32() );
		}
		Stream::iterator colorIterator = myStream.begin();
		colorIterator->setPositionByMemberName( "Color" );
		for( ; iterator != myStream.end(); ++iterator )
		{
			processColorMember( iterator->asFloat() );
		}
	
	If the values are not independent and are needed at the same time
	you can still access both values within the same loop by
	repositioning the Handle each time through the loop:

		for( Stream::iterator iterator = myStream.begin();
			 iterator != myStream.end();
			 ++iterator )
		{
			iterator->setPositionByMemberIndex( 0 );
			int* idValue = iterator->asInt32();
			iterator->setPositionByMemberIndex( 1 );
			float* colorValues = iterator->asFloat();
			processData( idValue, colorValues );
		}
*/
class METADATA_EXPORT StreamIterator
{
public:
	StreamIterator ();
	~StreamIterator();
	StreamIterator ( const StreamIterator& );
	StreamIterator&	operator= (const StreamIterator&);
	bool			operator!=(const StreamIterator&)	const;
	bool			operator==(const StreamIterator&)	const;

	// Standard iterator functions for walking data
	Handle&			operator*	();
	const Handle&	operator*	()						const;
	Handle*			operator->	();
	const Handle*	operator->	()						const;
	StreamIterator&	operator++	();
	StreamIterator	operator++	(int);
	bool			valid		()						const;

	// Iterator-specific accessors
	Index			index		()						const;

protected:
    StreamIteratorImpl const& impl() const;
    StreamIteratorImpl      & impl();

	StreamIteratorImpl*	fImpl;	//! Real implementation of the iterator

	// StreamImpl uses this constructor to make the begin() and end() iterators
	friend class Stream;
	StreamIterator	(StreamIteratorImpl*);
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
#endif // adskDataStreamIterator_h
