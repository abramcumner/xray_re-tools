#ifndef adskDataHandle_h
#define adskDataHandle_h

#include <iostream>				// For std::iostream
#include <maya/adskCommon.h>
#include <maya/adskDataStructure.h>	// For member fStructureIt
#include <maya/adskDebugCount.h>		// For DeclareObjectCounter()

// Forward declaration of member data
namespace adsk {
	namespace Debug {
		class Print;
		class Count;
		class Footprint;
	};
	namespace Data {

// ****************************************************************************
/*!
	\class adsk::Data::Handle
 	\brief Class handling access to specific data members of a structured type

	This class provides an interface to the structured data through which any
	individual member can be accessed. It works in the class hierarchy
	controlled by adsk::Data::Associations; see the documentation for that
	class for the bigger picture of how the classes interrelate.

	The adsk::Data::Structure class contains the description of the data
	type being referenced, including a description of how the data is stored
	so that the Handle knows how to access any particular member directly.
*/

class METADATA_EXPORT Handle
{
public:
	~Handle				();
	Handle				(); // Only present so that Handle can be in a map
	Handle				(const Handle& rhs);
	Handle				(const Structure& dataStructure);
	Handle				(const Structure& dataStructure, void* dataPointer);
	Handle& operator=	(const Handle& rhs);

	// Move the Handle to specific pieces of the structure
	bool	setPositionByMemberIndex(unsigned int memberIndex);
	bool	setPositionByMemberName	(const char*  memberName);

	// Point the Handle at new data
	void	pointToData	(char* newData, bool ownsNewData);
	void	swap		(Handle& rhs);
	bool	makeUnique	();

	// Does the Handle have data to reference?
	bool	hasData				() const;
	// Is the Handle pointing to data containing only default values
	// for its structure?
	bool	isDefault			() const;
	bool	isDefaultMember		(const char* memberName) const;
	// What type and length is the data?
	Member::eDataType	dataType	() const;
	unsigned int		dataLength	() const;

	// Does the Handle use the given structure, or a matching one?
	bool	usesStructure		(const Structure& comparedStructure) const;

	// Get the current data value converted to a string
	std::string		str					(unsigned int dim)			 const;
	unsigned int	fromStr				(const std::string& value,
										 unsigned int		dim,
										 std::string&		errors);

	// Type-specific data access. The return pointer points to the 0th element
	// of the element array. (If the element array length is 1 then it simply
	// points to the single element.)
	bool*			asBoolean			();
	double*			asDouble			();
	double*			asDoubleMatrix4x4	();
	float*			asFloat				();
	float*			asFloatMatrix4x4	();
	signed char*	asInt8				();
	short*			asInt16				();
	int*			asInt32				();
	int64_t*		asInt64				();
	char**			asString			();
	unsigned char*	asUInt8				();
	unsigned short*	asUInt16			();
	unsigned int*	asUInt32			();
	uint64_t*		asUInt64			();

	//
	// Generic data access. Cast the return pointer to the correct type and
	// it will point to the 0th element of the element array. (If the element
	// array length is 1 then it simply points to the single element.)
	void*	asType	(Member::eDataType type);

	//----------------------------------------------------------------------
	// Debug support
	static bool	Debug(const Handle* me, Debug::Print& request);
	static bool	Debug(const Handle* me, Debug::Footprint& request);
	DeclareObjectCounter( adsk::Data::Handle );
	static const char* kDebugHex;	//! Flag indicating to print as hex
	static const char* kDebugType;	//! Flag indicating to include type info
									//! in non-hex debug output.

private:
	friend class StreamImpl;
	Structure::iterator	fStructureIt;	   //! Data structure position describing this Handle's data
	char*				fData = nullptr;   //! Location of the piece of data the Handle references
	bool				fOwnsData = false; //! Is this Handle responsible for deleting the data?
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
#endif // adskDataHandle_h
