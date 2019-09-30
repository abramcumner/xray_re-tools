#ifndef adskDataChannelSerializer_h
#define adskDataChannelSerializer_h

#include <istream>	// for std::istream
#include <ostream>	// for std::ostream
#include <string>	// for std::string
#include <maya/adskCommon.h>
#include <maya/adskDataSerializer.h>	// for DeclareSerializerFormatType

namespace adsk {
	namespace Data {
		class Channel;

// ****************************************************************************
/*!
	\class adsk::Data::ChannelSerializer
 	\brief Class handling the definition of the format for serialization of
	data Channels

	The adsk::Data::Channel class manages channel definitions. They
	are persisted using a serialization format implemented through the
	adsk::Data::ChannelSerializer hierarchy. The base class defines the
	interface and manages the list of available channel serialization formats.
*/

class METADATA_EXPORT ChannelSerializer
{
	DeclareSerializerFormatType( ChannelSerializer );
public:
	ChannelSerializer			();
	virtual ~ChannelSerializer	();

	/*!
		\fn Channel* ChannelSerializer::read(std::istream& cSrc, std::string& errors) const;

		Implement this to parse the serialized form of an adsk::Data::Channel
		object.

		Given an input stream containing your serialization of an
		adsk::Data::Channel object parse the data and create the
		adsk::Data::Channel object it describes.
		
		If there are any problems the detailed error information
		should be returned in the errors string.

		The adsk::Data::Channel parsing should also recursively
		populate any adsk::Data::Streams within the adsk::Data::Channel.
		
		This method should be capable of understanding any data your
		adsk::Data::ChannelSerializer::write() method can provide.

		\param[in] cSrc Input stream containing serialization of the adsk::Data::Channel object
		\param[out] errors String containing description of parse errors

		\return Pointer to the newly created adsk::Data::Channel object
	*/
	virtual Channel*	read	( std::istream&	cSrc,
								  std::string&	errors )		const = 0;

	/*!
		\fn int ChannelSerializer::write(const Channel& dataToWrite, std::ostream& cDst, std::string& errors) const;

		Implement this to output the adsk::Data::Channel definition in your serialization format

		Given an adsk::Data::Channel object and an output stream
		as destination write out enough information so that you can
		recreate the adsk::Data::Channel from data in the output
		stream using your adsk::Data::ChannelSerializer::read()
		method.

		This serialization should recursively include all of the
		adsk::Data::Stream in the adsk::Data::Channel.

		\param[in] dataToWrite adsk::Data::Channel to be serialized
		\param[out] cDst Output stream to which the object is to be serialized
		\param[out] errors String containing description of output errors

		\return number of errors found during write (0 means success)
	*/
	virtual int	write		( const Channel& dataToWrite,
							  std::ostream&	 cDst,
							  std::string&	 errors)			const = 0;

	/*!
		\fn void ChannelSerializer::getFormatDescription(std::ostream& info) const;

		Implement this to provide a description of your adsk::Data::Channel
		serialization format.

		Output a textual description of your serialization format into the
		given stream.

		\param[out] info Output stream to receive your serialization format description
	*/
	virtual void	getFormatDescription(std::ostream& info)	const = 0;
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
#endif // adskDataChannelSerializer_h
