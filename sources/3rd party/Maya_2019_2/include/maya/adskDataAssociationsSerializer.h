#ifndef adskDataAssociationsSerializer_h
#define adskDataAssociationsSerializer_h

#include <istream>	// for std::istream
#include <ostream>	// for std::ostream
#include <string>	// for std::string
#include <maya/adskCommon.h>
#include <maya/adskDataSerializer.h>	// for DeclarerSerializerFormatType

namespace adsk {
	namespace Data {
		class Associations;

// ****************************************************************************
/*!
	\class adsk::Data::AssociationsSerializer
 	\brief Class handling the definition of the format for serialization of
	adsk::Data::Associations

	The adsk::Data::Associations class manages Associations definitions. They
	are persisted using a serialization format implemented through the
	adsk::Data::AssociationsSerializer hierarchy. The base class defines the
	interface and manages the list of available Associations serialization
	formats.
*/

class METADATA_EXPORT AssociationsSerializer
{
	DeclareSerializerFormatType( AssociationsSerializer );
public:
	AssociationsSerializer			();
	virtual ~AssociationsSerializer	();

	/*!
		\fn Associations* AssociationsSerializer::read(std::istream& cSrc, std::string& errors) const;

		Implement this to parse the serialized form of an
		adsk::Data::Associations object

		Given an input stream containing your serialization of an
		adsk::Data::Associations object parse the data and create the
		adsk::Data::Associations object it describes.
		
		If there are any problems the detailed error information
		should be returned in the errors string.

		The adsk::Data::Associations parsing should also recursively
		populate any adsk::Data::Channels and adsk::Data::Streams
		within the adsk::Data::Associations.
		
		This method should be capable of understanding any data your
		adsk::Data::AssociationsSerializer::write() method can provide.

		\param[in] cSrc Input stream containing serialization of the adsk::Data::Associations object
		\param[out] errors String containing description of parse errors

		\return Pointer to the newly created adsk::Data::Associations object
	*/
	virtual Associations*	read	( std::istream&	cSrc,
									  std::string&	errors )		const = 0;

	/*!
		\fn int AssociationsSerializer::write(const Associations& dataToWrite, std::ostream& cDst, std::string& errors) const;

		Implement this to output the adsk::Data::Associations definition in
		your serialization format.

		Given an adsk::Data::Associations object and an output stream
		as destination write out enough information so that you can
		recreate the adsk::Data::Associations from data in the output
		stream using your adsk::Data::AssociationsSerializer::read()
		method.

		This recursively includes all adsk::Data::Channels used by
		the adsk::Data::Associations, adsk::Data::Streams within
		those adsk::Data::Channels, and data within the adsk::Data::Streams.
		
		\param[in] dataToWrite adsk::Data::Associations to be serialized
		\param[out] cDst Output stream to which the object is to be serialized
		\param[out] errors String containing description of output errors

		\return number of errors found during write (0 means success)
	*/
	virtual int	write	( const Associations&	dataToWrite,
						  std::ostream&			cDst,
						  std::string&			errors)				const = 0;

	/*!
		\fn void AssociationsSerializer::getFormatDescription(std::ostream& info) const;

		Implement this to provide a description of your
		adsk::Data::Associations serialization format

		Output a textual description of your serialization format into
		the given stream.

		\param[out] info Output stream to receive your serialization format description
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
#endif // adskDataAssociationsSerializer_h
