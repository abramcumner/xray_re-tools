#ifndef adskDataAttachDirectly_h
#define adskDataAttachDirectly_h

#include <maya/adskCommon.h>
#include <maya/adskDataAttach.h>

namespace adsk {
	namespace Data {
		class Associations;

// ****************************************************************************
/*!
	\class adsk::Data::AttachDirectly
 	\brief Helper class to provide a simple implementation the the adsk::Data::Attach
	interface. Simply stores the metadata directly in the class.

	Use this class as a mix-in to any class to which you want metadata added
*/
class METADATA_EXPORT AttachDirectly : public Attach
{
public:
	AttachDirectly			();
	virtual ~AttachDirectly	();
	AttachDirectly			(const AttachDirectly&);

	virtual adsk::Data::Associations*			editableMetadata();
	virtual const	adsk::Data::Associations*	metadata		()	const;
	virtual bool	setMetadata		(const adsk::Data::Associations&);
	virtual bool	deleteMetadata	();

protected:
	//! Metadata stored as a pointer to allow differentiation between
	//! "no metadata" and "empty metadata" if necessary
	adsk::Data::Associations* fMetadata;
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
#endif // adskDataAttachDirectly_h
