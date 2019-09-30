#ifndef adskDataStructureSerializerDebug_h
#define adskDataStructureSerializerDebug_h

#include <maya/adskDataStructureSerializer.h>	// for base class
#include <maya/adskCommon.h>

namespace adsk {
	namespace Data {
		class Structure;

// ****************************************************************************
/*!
	\class adsk::Data::StructureSerializerDebug
 	\brief Class handling the data structure format type "Debug"

	The "Debug" format is taken directly from the Print request handler
	on the Structure and owned object classes. The formatting will be
	self-describing so either print a structure or look at the Debug method
	in the adsk::Data::Structure class to see what information is printed.
*/

class METADATA_EXPORT StructureSerializerDebug : public StructureSerializer
{
	DeclareSerializerFormat(StructureSerializerDebug, StructureSerializer);
public:
	virtual ~StructureSerializerDebug();

	// Mandatory implementation overrides
	virtual Structure*	read		(std::istream&		cSrc,
								 std::string&		errors)		const;
	virtual int		write		(const Structure&	dataToWrite,
								 std::ostream&		cDst)		const;
	virtual void	getFormatDescription(std::ostream& info)	const;

private:
	StructureSerializerDebug();					 //! Use theDebugFormat() to create.
	static const char* structureFormatType;		 //! Name of the Debug format type
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
#endif // adskDataStructureSerializerDebug_h
