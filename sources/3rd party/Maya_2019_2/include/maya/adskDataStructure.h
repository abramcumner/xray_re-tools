#ifndef adskDataStructure_h
#define adskDataStructure_h

#include <maya/adskCommon.h>
#include <maya/adskRefCounted.h>				// for base class adsk::RefCounted
#include <maya/adskCheckpointed.h>			// for base class adsk::Checkpoint
#include <maya/adskDataMember.h>				// for Member::eDataType
#include <maya/adskDataStructureIterator.h>	// for begin()/end()
#include <maya/adskDebugCount.h>				// for DeclareObjectCounter()
#include <stddef.h> 					// for size_t
#include <set>		 					// for Structure set iteration

namespace adsk {
	namespace Debug {
		class Print;
		class Count;
		class Footprint;
	};

	namespace Data {
		class Structure;

// ****************************************************************************
/*!
	\class adsk::Data::Structure
 	\brief Class handling the definition of the structure of a piece of data

	The adsk::Data::Handle class manages the handling of a single piece of
	structured data. adsk::Data::Structure class is responsible for defining
	and managing what the structure of that data looks like. It includes the
	naming of the structure members as well as maintaining the metadata
	necessary to efficiently store the collection of data values.
*/
class METADATA_EXPORT Structure : public RefCounted
								, public Checkpointed
{
protected:
	// Reference counting handles destruction
	~Structure			();

public:
	// For technical reasons these types live outside this class even
	// though they are a logical part of it. Declaring the typedefs lets
	// them be referenced as though they were still part of the class.
	typedef StructureIterator iterator;
	typedef StructureIterator const_iterator;

	Structure			();
	Structure			(const char* name);
	Structure			(const Structure& rhs);
	Structure& operator=(const Structure& rhs);
	bool	operator==	(const Structure& rhs)	const;
	bool	operator!=	(const Structure& rhs)	const;

	// Use this when creating a Structure from a DLL so that on Windows the
	// allocation and destruction don't mix heaps.
	static Structure*	create	();

	bool		addMember		(Member::eDataType,
								 unsigned int len, const char* name);
	size_t		totalSize		()												const;

	// Some utilities for dealing with chunks of structure memory
	const char*	defaultData				()										const;
	char*		allocateDefaultChunk	()										const;
	char*		duplicateChunk			(const char* dataPtr)					const;
	bool		fillWithDefaultChunk	(char* dataPtr)							const;
	bool		fillWithDuplicateChunk	(char* newData, const char* oldData)	const;
	bool		destroyChunk			(char* dataPtr)							const;
	bool		chunkIsDefault			(const char* dataPtr)					const;
	bool		chunkMemberIsDefault	(const char* dataPtr,
										 const char* memberName)				const;

	// Member access
	const char* name		()													const;
	void		setName		(const char*);

	bool		isDeleting	()	const { return fDeleting; }

	// Member iterator support. Only const iteration is supported.
	// The iteration is always done in the order in which the Members were
	// added.
	Structure::iterator			begin	()	const;
	Structure::iterator			end		()	const;
	Structure::const_iterator	cbegin	()	const;
	Structure::const_iterator	cend	()	const;
	unsigned int				size	()	const;
	bool						empty	()	const;

	// Manage structure visibility to the world
	static bool	registerStructure	(Structure& newStruct);
	static bool	deregisterStructure	(Structure& oldStruct);

	// Interact with the global structure list
	static Structure*			structureByName	(const char*);
	//
	// Iterate over all available structures in this standard way:
	//
	// 		for( Structure::ListIterator it = allStructures().begin();
	// 			 it != allStructures.end();
	// 			 ++it )
	//
	typedef std::set<Structure*>::iterator		ListIterator;
	typedef std::set<Structure*>				List;
	//
	static void		deleteAllStructures	();
	static List&	allStructures		();

	// Use size() instead since it's more standard
	unsigned int	memberCount			()	const;

	//----------------------------------------------------------------------
	// Debug support
	static bool	Debug(const Structure* me, Debug::Print& request);
	static bool	Debug(const Structure* me, Debug::Footprint& request);
	DeclareObjectCounter( adsk::Data::Structure );

protected:
	friend class StructureIteratorImpl; // Direct access for efficient iteration
	void	calculateBlock	();

	Member*			fMemberList;	//! Allocated array of structure members
	char*			fDefault;		//! Memory block containing a default structure value
	char*			fName;			//! Name of this structure
	size_t			fSize;			//! Total size of all structure members, including padding
	unsigned int	fMemberCount;	//! Number of members in fMemberList
	bool			fDeleting;		//! Is the structure in the middle of destruction?
};

    } // namespace Data
} // namespace adsk
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
#endif // adskDataStructure_h
