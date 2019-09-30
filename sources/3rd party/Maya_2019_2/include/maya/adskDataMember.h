#ifndef adskDataMember_h
#define adskDataMember_h

#include <maya/adskCommon.h>
#include <maya/adskDebugCount.h>
#include <stddef.h> 			// for size_t

namespace adsk {
	namespace Debug {
		class Print;
		class Footprint;
	};

	namespace Data {

//######################################################################
//
//	This helper holds the information required for each individual member
//	of an adsk::Data::Structure. The name and type identify the member.
//	The offset is used in constructing the packed data block; it's the
//	byte offset from the start of the block where this particular member
//	will live.
//
class METADATA_EXPORT Member
{
public:
	enum eDataType
	{
		// Note that typeName relies on a sequential numbering of
		// enums so keep it that way.
		
		kFirstType				// Must be the first on the list
	,	kBoolean = kFirstType
	,	kDouble
	,	kDoubleMatrix4x4
	,	kFloat
	,	kFloatMatrix4x4
	,	kInt8
	,	kInt16
	,	kInt32
	,	kInt64
	,	kString
	,	kUInt8
	,	kUInt16
	,	kUInt32
	,	kUInt64
	,	kLastType				// Must be the last two in the list
	,	kInvalidType=kLastType	// Must be the last two in the list
	};

	// Create/Destroy and operators
	Member();
	~Member();
	Member&	operator=(const Member& rhs);
	bool	operator==(const Member& rhs)	const;
	bool	operator!=(const Member& rhs)	const;

	// Access the member data
	unsigned int			length	()					const { return length(this); }
	const char*				name	()					const { return name(this); }
	size_t					offset	()					const { return offset(this); }
	size_t					offset	(unsigned int dim)	const { return offset(this, dim); }
	Member::eDataType		type	()					const { return type(this); }

	// Access the member data, returning good efaults for NULL pointers
	static unsigned int		 length	(const Member*);
	static const char*		 name	(const Member*);
	static size_t			 offset	(const Member*);
	static size_t			 offset	(const Member*, unsigned int dim);
	static Member::eDataType type	(const Member*);

	// Interact with the data types
	static int				 typeAlignment	(Member::eDataType);
	static const char*		 typeName		(Member::eDataType);
	static Member::eDataType typeFromName	(const char*);
	static int				 typeSize		(Member::eDataType);

private:
	friend class Structure;
	enum { kInvalidOffset = (unsigned int)-1 }; //! Magic number for uncomputed offset value

	bool fillWithDefault	(char* dataPtr)							const;
	bool constructDuplicate	(char* newData, const char* oldData)	const;
	bool destroy			(char* oldData)							const;
	bool isDefault			(const char* oldData)					const;

	Member::eDataType	fType;		//! Type of data to store
	char*				fName;		//! Name of this member
	unsigned int		fLength;	//! Number of elements per member (e.g. 3 for a float[3])
	size_t				fOffset;	//! Alignment from the structure start required to efficiently store the entire data array
	static bool Debug(const Member* me, adsk::Debug::Print& request);
	static bool Debug(const Member* me, adsk::Debug::Footprint& request);
	DeclareObjectCounter( adsk::Data::Member );

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
#endif // adskDataMember_h
