#ifndef adskDataStructureIterator_h
#define adskDataStructureIterator_h

#include <maya/adskCommon.h>

namespace adsk {
	namespace Debug {
		class Print;
		class Footprint;
	};

	namespace Data {
		class Member;
		class Structure;
		class StructureIteratorImpl;

// ****************************************************************************
/*!
	\class adsk::Data::StructureIterator
	\brief Helper class for iterating over structure members

	Structure member storage will be done as efficiently as possible so
	this iterator should be used to provide a consistent access to all
	members defining the structure.

	For technical reasons this class lives outside the Structure though really
	it should be part of it. A typedef is set up inside the Structure so that
	you can use it like a standard iterator:

		for( Structure::iterator iterator = myStructure.begin();
			 iterator != myStructure.end();
			 ++iterator )
		{
			processMember( *iterator );
		}
*/
class METADATA_EXPORT StructureIterator
{
public:
	StructureIterator	();
	~StructureIterator	();
	StructureIterator	(const StructureIterator&);
	StructureIterator&	operator= (const StructureIterator&);
	bool				operator!=(const StructureIterator&)	const;
	bool				operator==(const StructureIterator&)	const;

	// Standard iterator functions for walking Members
	Member&				operator*	();
	const Member&		operator*	()			const;
	Member*				operator->	();
	const Member*		operator->	()			const;
	StructureIterator&	operator++	();
	StructureIterator	operator++	(int);
	bool				valid		()			const;

	// Iterator-specific accessors
	const Structure&	structure	()			const;
	unsigned int		index		()			const;

protected:
	StructureIteratorImpl&			impl();
	StructureIteratorImpl const&	impl()	const;
	void*	fStorage[2];	// Enough space to store the implementation

	// Structure uses this constructor to make the begin() and end() iterators
	friend class Structure;
	enum IterLocation { kCreateAsEnd, kCreateAsBegin };
	StructureIterator	(const Structure&, IterLocation beginOrEnd);

public:
	//------------------------------------------------------------
	// Debugging support
	static bool	Debug(const StructureIterator* me, Debug::Print& request);
	static bool	Debug(const StructureIterator* me, Debug::Footprint& request);
};

	} // namespace Data
} // namespace adsk

//-
// ==================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// 
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//+
#endif // adskDataStructureIterator_h
