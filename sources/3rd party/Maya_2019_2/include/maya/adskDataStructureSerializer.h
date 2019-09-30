#ifndef adskDataStructureSerializer_h
#define adskDataStructureSerializer_h

#include <istream>	// for std::istream
#include <ostream>	// for std::ostream
#include <string>	// for std::string
#include <maya/adskCommon.h>
#include <maya/adskDataSerializer.h>	// for DeclarerSerializerFormatType

namespace adsk {
	namespace Data {
		class Structure;

// ****************************************************************************
/*!
	\class adsk::Data::StructureSerializer
 	\brief Class handling the definition of the format for serialization of
	data structures

	The adsk::Data::Structure class manages structure definitions. They
	are persisted using a serialization format implemented through the
	adsk::Data::StructureSerializer hierarchy. The base class defines the
	interface and manages the list of available structure serialization formats.
*/

class METADATA_EXPORT StructureSerializer
{
	DeclareSerializerFormatType( StructureSerializer );
public:
	StructureSerializer			();
	virtual ~StructureSerializer	();

	/*!
		\fn Structure* StructureSerializer::read(std::istream& cSrc, std::string& errors) const;

		Implement this to parse the serialized form of an adsk::Data::Structure object

		Given an input stream containing your serialization of an 
		adsk::Data::Structure parse the data and create the
		adsk::Data::Structure it describes.
		
		If there are any problems the detailed error information
		should be returned in the errors string.

		\param[in] cSrc Stream containing the serialization of the structure
		\param[out] errors String containing description of parse errors

		\return Newly created structure, NULL if creation failed
	*/
	virtual Structure* read	( std::istream&	cSrc,
							  std::string&	errors )				const = 0;

	/*!
		\fn int StructureSerializer::write(const Structure& dataToWrite, std::ostream& cDst) const;

		Implement this to output the adsk::Data::Structure definition in your serialization format

		Given an adsk::Data::Structure object and an output stream
		as destination write out enough information so that you can
		recreate the adsk::Data::Structure from data in the output
		stream using your adsk::Data::StructureSerializer::read()
		method.

		\param[in] dataToWrite adsk::Data::Structure to be serialized
		\param[out] cDst stream to which the object is to be serialized

		\return number of errors found during write, 0 means success
	*/
	virtual int	write	( const Structure&	dataToWrite,
						  std::ostream&		cDst )					const = 0;

	/*!
		\fn void StructureSerializer::getFormatDescription(std::ostream& info) const;

		Implement this to provide a description of your adsk::Data::Structure serialization format

		Output a textual description of your serialization format into the given stream.

		\param[out] info stream in which to output your serialization format description
	*/
	virtual void	getFormatDescription(std::ostream& info)		const = 0;
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
#endif // adskDataStructureSerializer_h
