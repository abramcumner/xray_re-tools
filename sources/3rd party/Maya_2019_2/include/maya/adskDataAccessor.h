#ifndef adskDataAccessor_h
#define adskDataAccessor_h

#include <maya/adskCommon.h>

#include <map>
#include <memory> // for std::auto_ptr
#include <set>
#include <string>

namespace adsk {
	namespace Data {
		class Structure;
		class Associations;

// *****************************************************************************
/*!
	\class adsk::Data::Accessor
	\brief Class used to read and write metadata from/to an existing file.

	This class is used to read and rewrite a collection of adsk::Data::Structure
	and adsk::Data::Associations from/to an existing file.
*/

class METADATA_EXPORT Accessor {

public:

	//---------------------------------------------------------------------------
	/*!
		\struct adsk::Data::Accessor::StructureNameLessThan
		\brief Functor for Structure ordering in a std::set.

		This functor compares Structures to keep them ordered by name in a
		std::set.
	*/
	struct METADATA_EXPORT StructureNameLessThan {
		bool operator()( const Structure* lhs,
						 const Structure* rhs ) const;
	};

	//! \brief Structures are kept in a set that forbids duplicated names.
	typedef std::set< const Structure*, StructureNameLessThan > StructureSet;

	/*! \brief Associations are kept in a map that forbids duplicated names.

		The actual file format defines the semantic for the Associations names.
		For example, for certain formats, an empty-name may mean file-level
		associations rather than per-object associations.
	*/
	typedef std::map< std::string, Associations > AssociationsMap;

	virtual ~Accessor();

	// File read/write.
	bool 				read	( const std::string& fileName,
								  std::string&       errors );
	bool				read	( const std::string&             fileName,
								  const std::set< std::string >* wantedStructures,
								  const std::set< std::string >* wantedAssociations,
								  std::string&                   errors );
	virtual bool		write	( std::string& errors )			const;
	virtual bool		isFileSupported( const std::string& fileName ) const;
	const std::string&	fileName()								const;

	// Get/set the structures and associations.
	const StructureSet&		structures		() const;
	void					setStructures	( const StructureSet& structures );
	const AssociationsMap&	associations	() const;
	AssociationsMap&		associations	();
	void 					clear			();

	static std::set< std::string >		supportedExtensions	();
	static std::auto_ptr< Accessor >	accessorByExtension( const std::string& extension );
	static std::auto_ptr< Accessor >	readFile			( const std::string& fileName,
															  std::string& errors );
	
	class Impl;

protected:

	Accessor();

	/*!
		\fn bool performRead(
			const std::set< std::string >* wantedStructures,
			const std::set< std::string >* wantedAssociations,
			std::string&                   errors );

		Invoked by read() to access the current file.

		The current file's name is obtained with fileName().  Concrete
		implementations must read the specified structures/associations, if any,
		or all of them, otherwise.  Upon success, the structures are read from
		adsk::Data::Structure::allStructures() and kept in this instance.  The
		associations, on the other hand, must be set by the derived class by
		editing the associations map returned from associations().
		
		The caller (the read() method) deals with saving/restoring the content
		of adsk::Data::Structure::allStructures() before/after the call, and
		clearing the existing structures/associations present in the instance
		before calling performRead().

 		\param[in]  wantedStructures Name of the structures to be read.  If
		null, all structures must be read.
 		\param[in]  wantedAssociations Name of the associations to be read.  If
		null, all asociations must be read.  Associations for which one or more
		Structures were not read due to wantedStructures will not be read.
		\param[out] errors Errors that happended during reading, if any.

		\return True upon success.
	*/
	virtual bool performRead(
		const std::set< std::string >* wantedStructures,
		const std::set< std::string >* wantedAssociations,
		std::string&                   errors
	) = 0;

private:

	// Disabled.
	Accessor( const Accessor& );
	Accessor& operator=( const Accessor& );
	
#ifdef _WIN32
#pragma warning( push )
	// MSC incorrectly emits this warning on private templated members.
#pragma warning( disable : 4251 )
#endif
	std::auto_ptr< Impl > fImpl;
#ifdef _WIN32
#pragma warning( pop )
#endif
};

// *****************************************************************************
/*!
	\class adsk::Data::AccessorFactoryBase
	\brief Base class for Accessor factories.

	This class must be derived from in order to register/deregister Accessor
	factories and associate them with specific filename extensions.

	See adsk::Data::AccessorFactory for a concrete implementation that should
	satisfy most needs.
*/
class METADATA_EXPORT AccessorFactoryBase {

public:

	virtual ~AccessorFactoryBase();
	
	/*!
		\fn std::auto_ptr< Accessor > create() const;
		
		Returns a factory for creating accessors handling the supported file type.
		
		\return A new accessor for the file type supported by the concrete
		factory.
	*/
	virtual std::auto_ptr< Accessor > create() const = 0;

protected:

	AccessorFactoryBase( const std::string& fileNameExtension );

private:

	// Disabled.
	AccessorFactoryBase( const AccessorFactoryBase& );
	AccessorFactoryBase& operator=( const AccessorFactoryBase& );
};

// *****************************************************************************
/*!
	\class adsk::Data::AccessorFactory
	\brief Calls the specified Accessor type's default constructor.
	
	Accessor factories automatically register themselves upon construction and
	deregister themselves upon destruction.  So the creator of a factory must
	maintain it in-scope as long as the factory for the supported file
	extension(s) is needed.  Typically, this will be a file-scope variable
	declared with the accessor implementation.
	
	For example (in myAccessor.cpp), to support *.myext files:
	
	class MyAccessor : public adsk::Data::Accessor {
		...
	};
	
	adsk::Data::AccessorFactory< MyAccessor > myFactory( "myext" );
*/
template < typename AccessorType > class AccessorFactory : public AccessorFactoryBase {

public:

	// Constructor, taking the filename extension that is supported by this
	// factory.  If more than one extension are supported, create one instance
	// per extension.
	AccessorFactory( const std::string& fileNameExtension )
		: AccessorFactoryBase( fileNameExtension )
	{}
	
	// Unregisters the factory.
	virtual ~AccessorFactory()
	{}
	
	// Creates the factory for the concrete accessor type.
	virtual std::auto_ptr< Accessor > create() const
	{
		return std::auto_ptr< Accessor >( new AccessorType );
	}

private:

	// Disabled.
	AccessorFactory( const AccessorFactory& );
	AccessorFactory& operator=( const AccessorFactory& );
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
#endif
