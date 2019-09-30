#ifndef adskDataChannelIterator_h
#define adskDataChannelIterator_h

#include <maya/adskCommon.h>

// Forward declaration of member data
namespace adsk {
	namespace Debug {
		class Footprint;
		class Print;
	};
	namespace Data {
		class ChannelIteratorImpl;
		class Channel;
		class Stream;

//----------------------------------------------------------------------
/*!
	\class adsk::Data::ChannelIterator
 	\brief Class handling iteration over Streams in a Channel object

	The Stream list is sorted by name so this iterator class will walk
	in that order.

	For technical reasons this class lives outside the Channel class
	though really it is part of it. A typedef is set up inside that class
	so that you can use it like a standard iterator:

		for( Channel::iterator iterator = myChannel.begin();
			 iterator != myChannel.end();
			 ++iterator )
		{
			processStream( *iterator );
		}
*/
class METADATA_EXPORT ChannelIterator
{
public:
	ChannelIterator 			  ();
	~ChannelIterator			  ();
	ChannelIterator				  (const ChannelIterator&);
	ChannelIterator&	operator= (const ChannelIterator&);
	bool				operator!=(const ChannelIterator&)	const;
	bool				operator==(const ChannelIterator&)	const;

	// Standard iterator functions for walking data
	Stream&				operator*	();
	const Stream&		operator*	()						const;
	Stream*				operator->	();
	const Stream*		operator->	()						const;
	ChannelIterator&	operator++	();
	ChannelIterator		operator++	(int);
	bool				valid		()						const;

protected:
    ChannelIteratorImpl const& impl() const;
    ChannelIteratorImpl      & impl();
	void*	fStorage[3];	// Enough space to store the implementation

	// Channel uses this constructor to make the begin() and end() iterators
	friend class Channel;
	enum IterLocation { kCreateAsEnd, kCreateAsBegin };
	ChannelIterator	(const Channel&, IterLocation);

public:
	//------------------------------------------------------------
	// Debugging support
	static bool	Debug(const ChannelIterator* me, Debug::Print& request);
	static bool	Debug(const ChannelIterator* me, Debug::Footprint& request);
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
#endif // adskDataChannelIterator_h
