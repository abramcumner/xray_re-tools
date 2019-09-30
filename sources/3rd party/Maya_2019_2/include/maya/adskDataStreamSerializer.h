#ifndef adskDataStreamSerializer_h
#define adskDataStreamSerializer_h

#include <istream>	// for std::istream
#include <ostream>	// for std::ostream
#include <string>	// for std::string
#include <maya/adskCommon.h>
#include <maya/adskDataSerializer.h>	// for DeclarerSerializerFormatType

namespace adsk {
	namespace Data {
		class Stream;

// ****************************************************************************
/*!
	\class adsk::Data::StreamSerializer
 	\brief Class handling the definition of the format for serialization of
	data streams

	The adsk::Data::Stream class manages stream definitions. They
	are persisted using a serialization format implemented through the
	adsk::Data::StreamSerializer hierarchy. The base class defines the
	interface and manages the list of available stream serialization
	formats.
*/

class METADATA_EXPORT StreamSerializer
{
	DeclareSerializerFormatType( StreamSerializer );
public:
	StreamSerializer			();
	virtual ~StreamSerializer();

	/*!
		\fn Stream* StreamSerializer::read(std::istream& cSrc, std::string& errors) const;

		Implement this to parse the serialized form of an adsk::Data::Stream object

		Given an input stream containing your serialization of an
		adsk::Data::Stream object parse the data and create the
		adsk::Data::Stream object it describes.
		
		If there are any problems the detailed error information
		should be returned in the errors string.
		
		The adsk::Data::Stream parsing should also recursively
		populate any data within the adsk::Data::Stream.  This
		method should be capable of understanding any data which
		your adsk::Data::StreamSerializer::write method can provide.

		\param[in] cSrc Input stream containing serialization of the adsk::Data::Stream
		\param[out] errors String containing description of parse errors

		\return Pointer to the newly created adsk::Data::Stream object
	*/
	virtual Stream*	read	( std::istream&	cSrc,
							  std::string&	errors )				const = 0;

	/*!
		\fn int StreamSerializer::write(const Stream& dataToWrite, std::ostream& cDst, std::string& errors) const;

		Implement this to output the adsk::Data::Stream definition in your serialization format

		Given an adsk::Data::Stream object and an output stream as destination
		write out enough information so that you can recreate the
		adsk::Data::Stream from data in the output stream using your
		adsk::Data::StreamSerializer::read() method.
		
		This serialization should include all data values within the
		adsk::Data::Stream obejct.

		\param[in] dataToWrite adsk::Data::Stream to be serialized
		\param[out] cDst stream to which the object is to be serialized
		\param[out] errors String containing description of write errors

		\return number of errors found during write (0 means success)
	*/
	virtual int		write	( const Stream&	dataToWrite,
							  std::ostream&	cDst,
							  std::string&	errors )				const = 0;

	/*!
		\fn void StreamSerializer::getFormatDescription(std::ostream& info) const;

		Implement this to provide a description of your adsk::Data::Stream serialization format

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
#endif // adskDataStreamSerializer_h
