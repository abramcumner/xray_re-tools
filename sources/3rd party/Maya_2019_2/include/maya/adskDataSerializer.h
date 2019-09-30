#ifndef _adskDataSerializer_h
#define _adskDataSerializer_h

#include <set>
#include <maya/adskCommon.h>

//----------------------------------------------------------------------
/*! \defgroup serialization Defining serialization formats
	Really should have been implemented completely as templates, it would
	have been much cleaner, but support is not good enough on all platforms.
		
	Here are the steps for creating a set of serialization formats.
		
	Create a base class for each type of serialization format which uses
	the first set of macros to set up its interface and then create your
	derived classes which implement specific formats.
	
	Here's a full example with a class "MyClass" to be serialized in XML
	and JSON format. For simplicity they can be assumed to be in a single
	header and source file but it's not necessary.

	Class declarations look like this:

	\code
		class MyClassSerializer
		{
			DeclareSerializerFormatType( MyClassSerializer );
		...
		};

		class MyClassSerializerXML : public MyClassSerializer
		{
			DeclareSerializerFormat( MyClassSerializerXML, MyClassSerializer );
		...
		};

		class MyClassSerializerJSON : public MyClassSerializer
		{
			DeclareSerializerFormat( MyClassSerializerJSON, MyClassSerializer );
		...
		};
	\endcode
	
	The implementation source file starts with this:

	\code
		ImplementSerializerFormatType( MyClassSerializer );
		ImplementSerializerFormat( MyClassSerializerXML, MyClassSerializer, "XML" );
		ImplementSerializerFormat( MyClassSerializerJSON, MyClassSerializer, "JSON" );
		...
	\endcode
	
	If you wish automatic registration/deregistration include the lines below,
	otherwise allocate and destroy an object of these class types when you wish
	the registration/deregistration to happen.

	\code
		static SerializerInitializer<MyClassSerializerXML> _initializerXML( MyClassSerializerXML::theFormat() );
		static SerializerInitializer<MyClassSerializerJSON> _initializerJSON( MyClassSerializerJSON::theFormat() );
		...
	\endcode

	This provides a common static interface to the class types. The most common
	examples are to access the implementation of a particular format by name:

	\code
		MyClassSerializer* xmlFormat = MyClassSerializer::formatByName( "XML" );
	\endcode

	And to iterate over all available formats of a given type:

	\code
		MyClassSerializer::FormatSet::iterator serIt;
		for( serIt = MyClassSerializer::allFormats().begin();
		     serIt != MyClassSerializer::allFormats().end(); ++serIt )
		{
			MyClassSerializer* currentFormat = (*serIt);
			...
		}
	\endcode
	
	Note that in the case of dynamically loaded code (e.g. plug-ins) you may
	not get the static object destructor called in the individual formats so
	instead you will have to dynamically allocate and destroy your
	SerializerInitializer<> objects on load and unload of your code.
*/
namespace adsk {
	namespace Data {

//----------------------------------------------------------------------
//! \class SerializerInitializer
//! \brief Helper : Instantiate to automatically (de)register a serializaton format
//!			Requires that the template parameter class has implemented the
//!			registerFormat() and deregisterFormat() methods.
template <typename TheType>
class SerializerInitializer
{
public:
	//---------------------------------------------------------------
	/*! \fn SerializerInitializer( const TheType& fmt )
		\brief Constructor, automatically register the new format
		\param[in] fmt New format to be registered
	*/
	SerializerInitializer ( const TheType& fmt ) : myFormat( &fmt )
				{ TheType::registerFormat( fmt ); }

	//---------------------------------------------------------------
	/*! \fn ~SerializerInitializer()
		\brief Destructor, automatically deregister the format
	*/
	~SerializerInitializer()
				{ TheType::deregisterFormat( *myFormat ); }
private:
	//! Format that this object registered
	const TheType* myFormat;
};

	} // namespace Data
} // namespace adsk

//----------------------------------------------------------------------
// Macros to define a common serialization type interface. Should have been
// implementated as templates, prevented by shortcomings in come compilers.
//
#define DeclareSerializerFormatType(CLASS)									\
		public:																\
			virtual const char* formatType		() const = 0;				\
			typedef std::set<const CLASS*>		FormatSet;					\
			static void	 	registerFormat		(const CLASS& fmt);			\
			static void	 	deregisterFormat	(const CLASS& fmt);			\
			static void		setDefaultFormat	(const CLASS* newDefault);	\
			static const CLASS* defaultFormat	();							\
			static const CLASS* formatByName	(const std::string& name);	\
			static const FormatSet&	allFormats	();							\
		protected:															\
			static FormatSet&	allFormatsToEdit();							\
			static const CLASS*	theDefault;

#define ImplementSerializerFormatType(CLASS)									\
		const CLASS* CLASS::theDefault( 0 );									\
		const CLASS::FormatSet& CLASS::allFormats()								\
				{ return allFormatsToEdit(); }									\
		CLASS::FormatSet& CLASS::allFormatsToEdit() {							\
				static CLASS::FormatSet	formatList;								\
				return formatList; }											\
		void	CLASS::registerFormat	(const CLASS& fmt)						\
				{ allFormatsToEdit().insert( &fmt ); }							\
		void	CLASS::deregisterFormat(const CLASS& fmt) {						\
					allFormatsToEdit().erase( &fmt );							\
					if( &fmt == theDefault ) {									\
						if( allFormats().size() > 0 ) {							\
							CLASS::FormatSet::iterator it =allFormats().begin();\
							theDefault = (*it);									\
						} else {												\
							theDefault = (const CLASS*)0;						\
						}														\
					}															\
				}																\
		void CLASS::setDefaultFormat(const CLASS* newDefault)					\
				{ theDefault = newDefault; }									\
		const CLASS* CLASS::defaultFormat	()									\
				{ return theDefault; }											\
		const CLASS* CLASS::formatByName	(const std::string& name) {			\
				CLASS::FormatSet::iterator it;									\
				for( it=allFormats().begin(); it!= allFormats().end(); ++it )	\
				{																\
					if( *it && (name == (*it)->formatType()) )					\
						return (*it);											\
				}																\
				return (const CLASS*)0; }

//----------------------------------------------------------------------
// Macros that each derived format type should use to declare and implement
// the initializer that will perform the automatic (de)registration.
// The derived classes must define a default constructor.
//
#define DeclareSerializerFormat(CLASS,BASE)					\
		public:												\
			static const CLASS& theFormat	();				\
			virtual const char* formatType	() const;

#define ImplementSerializerFormat(CLASS,BASE,NAME)								\
		const char* CLASS::formatType() const { return NAME; }					\
		const CLASS& CLASS::theFormat()	{										\
			static CLASS *_neo = (CLASS*)0;										\
			if( !_neo ) _neo = new CLASS();										\
			return *_neo; }

//=========================================================
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// Use of  this  software is  subject to  the terms  of the
// Autodesk  license  agreement  provided  at  the  time of
// installation or download, or which otherwise accompanies
// this software in either  electronic  or hard  copy form.
//+
//=========================================================
#endif // _adskDataSerializer_h
