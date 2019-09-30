#ifndef adskDataAttach_h
#define adskDataAttach_h

#include <maya/adskCommon.h>

namespace adsk {
	namespace Data {
		class Associations;

// ****************************************************************************
/*!
	\class adsk::Data::Attach
 	\brief Helper class to manage attachment of metadata to other class objects

	Since in the purest sense metadata means "data about data" it is often
	useful to attach the metadata directly to the data which it informs. This
	helper class provides a simple interface for doing just that.

	Use it as a mixin on any class to which metadata is to be attached.
	Implement the abstract methods to access the metadata from wherever your
	class wishes to store it.
*/
class METADATA_EXPORT Attach
{
public:
	Attach			();
	virtual ~Attach	();
	Attach			(const Attach&);

	// Methods to handle accessing the per-component metadata
	//
	//! \fn adsk::Data::Associations* editableMetadata()
	//! Retrieve the metadata uniquely associated with this object
	virtual adsk::Data::Associations*			editableMetadata()			= 0;

	//! \fn const adsk::Data::Associations* metadata() const
	//! Retrieve the metadata uniquely associated with this object (const form)
	virtual const	adsk::Data::Associations*	metadata		()	const	= 0;

	//! \fn bool setMetadata(const adsk::Data::Associations&)
	//! Attach new metadata to this object
	virtual bool	setMetadata		(const adsk::Data::Associations&)		= 0;

	//! \fn bool deleteMetadata()
	//! Remove all metadata attached to this object
	virtual bool	deleteMetadata	()										= 0;
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
#endif // adskDataAttach_h
