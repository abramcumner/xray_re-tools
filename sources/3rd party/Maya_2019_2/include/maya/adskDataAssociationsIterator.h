#ifndef adskDataAssociationsIterator_h
#define adskDataAssociationsIterator_h

#include <maya/adskCommon.h>

// Forward declaration of member data
namespace adsk {
	namespace Debug {
		class Print;
		class Footprint;
	};

	namespace Data {
		class Associations;
		class AssociationsIteratorImpl;
		class Channel;

//----------------------------------------------------------------------
/*!
	\class adsk::Data::AssociationsIterator
 	\brief Class handling iteration over Channels in an Associations object

	The Channel list is sorted by name so this iterator class will walk
	in that order.

	For technical reasons this class lives outside the Associations class
	though really it is part of it. A typedef is set up inside that class
	so that you can use it like a standard iterator:

		for( Associations::iterator iterator = myMetadata.begin();
			 iterator != myMetadata.end();
			 ++iterator )
		{
			processChannel( *iterator );
		}
*/
class METADATA_EXPORT AssociationsIterator
{
public:
	AssociationsIterator 			  ();
	~AssociationsIterator			  ();
	AssociationsIterator			  (const AssociationsIterator&);
	AssociationsIterator&	operator= (const AssociationsIterator&);
	bool					operator!=(const AssociationsIterator&)	const;
	bool					operator==(const AssociationsIterator&)	const;

	// Standard iterator functions for walking data
	Channel&				operator*	();
	const Channel&			operator*	()						const;
	Channel*				operator->	();
	const Channel*			operator->	()						const;
	AssociationsIterator&	operator++	();
	AssociationsIterator	operator++	(int);
	bool					valid		()						const;

protected:
    AssociationsIteratorImpl      & impl();
    AssociationsIteratorImpl const& impl() const;
	void*	fStorage[3];	// Enough space to store the implementation

	// Associations uses this constructor to make the begin() and end() iterators
	friend class Associations;
	enum IterLocation { kCreateAsEnd, kCreateAsBegin };
	AssociationsIterator	(const Associations&, IterLocation);

public:
	//------------------------------------------------------------
	// Debugging support
	static bool	Debug(const AssociationsIterator* me, Debug::Print& request);
	static bool	Debug(const AssociationsIterator* me, Debug::Footprint& request);
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
#endif // adskDataAssociationsIterator_h
