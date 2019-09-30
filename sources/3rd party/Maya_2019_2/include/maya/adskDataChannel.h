#ifndef adskDataChannel_h
#define adskDataChannel_h

#include <string>
#include <maya/adskCommon.h>
#include <maya/adskDebugCount.h>	// for Debug Count macro

// Forward declaration of member data
namespace adsk {
	namespace Debug {
		class Print;
		class Footprint;
	};
	namespace Data {
		class Index;
		class Stream;
		class ChannelImpl;
		class ChannelIterator;

// ****************************************************************************
/*!
	\class adsk::Data::Channel
 	\brief Class handling a named association of a data array with other data.

	This class is used in conjunction with adsk::Data::Associations. See the
	documentation for adsk::Data::Associations for the bigger picture in how
	the classes work together.

	adsk::Data::Channel is responsible for maintaining a list of named data
	streams, where a "stream" can be thought of as equivalent to a named array
	of data.

	The main reason for this class to exist is to allow attachment of
	multiple unrelated data streams. For instance a simulator can attach an
	arbitrary data structure to every vertex of a mesh (that's one stream)
	and a shader can attach a different color-based data structure to every
	vertex of a mesh (that's a different stream).

	Having separate streams makes it easier to find and handle the stream
	of data in which a particular user is interested. Having this channel
	class manage the streams as a unit makes it easier for changes in the
	indexing to do the proper thing to the data in the streams all in one
	place (e.g. deleting one of the associated indexes will cause the
	matching indexed data in each of the streams to be deleted)
*/

class METADATA_EXPORT Channel
{
public:
	~Channel			();
	Channel				();
	explicit Channel	(const std::string&);
	Channel				(const Channel&);
	Channel& operator=	(const Channel&);
	bool	 operator==	(const Channel&) const;

	//----------------------------------------------------------------------
	Stream*			setDataStream	( const Stream& newStream );
	const Stream*	findDataStream 	( const std::string& ) const;
	Stream*			findDataStream 	( const std::string& );
    bool   			removeDataStream( const std::string& );
    bool   			renameDataStream( const std::string&, const std::string& );

	//----------------------------------------------------------------------
	// To create a unique copy of this class and all owned classes.
	// Only use this if you want to create unique copies of all data
	// below this, otherwise use the similarly named method at a lower
	// level.
	bool	makeUnique	();

	//----------------------------------------------------------------------
	const std::string&	name()	const;

	//----------------------------------------------------------------------
	// Support for Channel iteration over Streams.
	//
	// For technical reasons these types live outside this class even
	// though they are a logical part of it. Declaring the typedefs lets
	// them be referenced as though they were still part of the class.
	typedef ChannelIterator iterator;
	typedef ChannelIterator const_iterator;

	Channel::iterator		begin	()	const;
	Channel::iterator		end		()	const;
	Channel::const_iterator	cbegin	()	const;
	Channel::const_iterator	cend	()	const;

	unsigned int			size	()	const;
	bool					empty	()	const;

	//----------------------------------------------------------------------
    // These are the methods needed for handling changes to the structure of
    // the associated data channel. They are kept simple on purpose; higher
    // level intelligence such as interpolating across neighboring vertices
    // in a polygon mesh is left for the channel owner to handle.
    bool removeElement   ( const Index& elementIndex );
    bool addElement      ( const Index& elementIndex);

	//----------------------------------------------------------------------
    // Obsolete iteration support for Stream members. Use
	// Channel::iterator instead.
	unsigned int	dataStreamCount	() const;
	Stream*			dataStream		(unsigned int streamIndex);
	const Stream*	dataStream		(unsigned int streamIndex)	const;
	//----------------------------------------------------------------------
	// Obsolete variations of the more standard findDataStream() methods
	Stream*			dataStream		( const std::string& streamName );
	const Stream*	dataStream		( const std::string& streamName )	const;

	//----------------------------------------------------------------------
	// Debug support
	static bool	Debug(const Channel* me, Debug::Print& request);
	static bool	Debug(const Channel* me, Debug::Footprint& request);
	DeclareObjectCounter( Channel );

private:
	friend class ChannelIterator;
	ChannelImpl*	fImpl; //! Implementation details
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
#endif // adskDataChannel_h
