#ifndef adskDataIndexType_h
#define adskDataIndexType_h

#include <maya/adskCommon.h>
#include <maya/adskDataIndex.h>		// For IndexCount
#include <maya/adskRefCounted.h>		// For base class
#include <maya/adskDebugCount.h>		// For CRTP methods

// Forward declaration of member data
namespace adsk {
	namespace Data {

// ****************************************************************************
/*!
	\class adsk::Data::IndexType
 	\brief Complex index type for referencing metadata.

	adsk::Data::Stream objects contain a list of data elements. Each element
	has to be accessed by Index. This is the base class for the complex index
	types.
*/

class METADATA_EXPORT IndexType : public RefCounted
								// Since this class is virtual there is no
								// need to use an extern template to prevent
								// multiple instantiations since it cannot
								// be instantiated by itself at all.
								// Derived classes still need it though.
{
protected:
	friend class Index;
	virtual	~IndexType	();
	IndexType			();
public:
	//----------------------------------------------------------------------
	// Constructors and assignment operator. Base class provides as little
	// implementation as possible. The derived classes do the heavy lifting.
	IndexType			( const IndexType& rhs );

	/*! \fn IndexType*	clone() const
		\brief Create a duplicate of this object.
		This is virtual so that derived types create the correct type of object.
		\return Pointer to copy of this object
	*/
	virtual IndexType*	clone				()						 const	= 0;

	/*! \fn std::string asString() const
		\brief Get this index type value as a string
		\return The index type value in string form (as expected by the string constructor)
	*/
	virtual std::string	asString			()						 const	= 0;

	/*! \fn bool supportsDenseMode() const
		\brief Check if this object type supports dense packing indexing
		\return true if the index type supports dense packing
	*/
	virtual bool		supportsDenseMode	()						 const	= 0;

	/*! \fn IndexCount denseSpaceBetween(const IndexType& rhs) const
		\brief Calculate the number of elements to be packed between this one and the rhs
		\param[in] rhs Object from which the packing size is calculated
		\return Number of index values between this one and the rhs
	*/
	virtual IndexCount	denseSpaceBetween	( const IndexType& rhs ) const	= 0;

	//----------------------------------------------------------------------
	// Comparison operators for easy sorting
	//
	/*! \fn bool operator==( const IndexType& rhs ) const
		\brief Equality comparison of this object and the rhs
		\param[in] rhs Object being compared against
		\return true if the two objects are equal
	*/
	virtual bool	operator==	(const IndexType& rhs)	const				= 0;

	/*! \fn bool operator!=( const IndexType& rhs ) const
		\brief Unequality comparison of this object and the rhs
		\param[in] rhs Object being compared against
		\return true if the two objects are not equal
	*/
	virtual bool	operator!=	(const IndexType& rhs)	const				= 0;

	/*! \fn bool operator==( const IndexType& rhs ) const
		\brief GE-inequality comparison of this object and the rhs
		\param[in] rhs Object being compared against
		\return true if this object is less than the rhs
	*/
	virtual bool	operator<	(const IndexType& rhs)	const				= 0;

	/*! \fn bool operator<( const IndexType& rhs ) const
		\brief LT-inequality comparison of this object and the rhs
		\param[in] rhs Object being compared against
		\return true if this object is less than or equal to the rhs
	*/
	virtual bool	operator<=	(const IndexType& rhs)	const				= 0;

	/*! \fn bool operator<=( const IndexType& rhs ) const
		\brief LE-inequality comparison of this object and the rhs
		\param[in] rhs Object being compared against
		\return true if this object is greater than the rhs
	*/
	virtual bool	operator>	(const IndexType& rhs)	const				= 0;

	/*! \fn bool operator==( const IndexType& rhs ) const
		\brief GT-inequality comparison of this object and the rhs
		\param[in] rhs Object being compared against
		\return true if this object is greater than or equal to the rhs
	*/
	virtual bool	operator>=	(const IndexType& rhs)	const				= 0;

	/*! \fn std::string typeName() const
		\brief Get the unique name of this index type
		\return String containing the index type name
	*/
	virtual	std::string typeName		()				const				= 0;

protected:
	//! \class adsk::Data::IndexType::IndexRegistration
 	//! \brief Helper class to automatically register new index types.
	//!		   Construction of an object of this class will register a
	//!		   new index type with the global registry.
	class IndexRegistration {
		public: IndexRegistration( const std::string&	typeName,
								   Index::IndexCreator	creator )
				{ (void) Index::registerType(typeName, creator); }
	};
};

//----------------------------------------------------------------------
/*!
	This CRTP (Curiously Recurring Template Pattern) class implements some
	shared methods which all derived classes can use, and for whom the
	implementations use the same pattern.

	For all derived index types use this declaration for the class:

		class METADATA_EXPORT MyIndex : public CRTP_IndexType<MyIndex>
		{ ... }
	
	This will automatically instantiate a common clone() method as well as
	establishing the abstract interface each index type needs to implement.
*/
template <typename Derived>
class CRTP_IndexType : public IndexType
{
public:
	//----------------------------------------------------------------------
	//	clone() virtual method creates an object of the derived type
	virtual IndexType*	clone	()									const
		{ return new Derived(static_cast<Derived const&>(*this)); }

	// Static creation method used to define a new Index from a description.
	// Class must provide a constructor from a string.
	static  Index		doCreate( const std::string& value )
			{ return Index( Derived( value ) ); }

	// Virtual and static methods to get the name of the index type
	virtual	std::string typeName()									const
			{ return Derived::myTypeName; }
	static	std::string theTypeName()
			{ return Derived::myTypeName; }
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
#endif // adskDataIndexType_h
