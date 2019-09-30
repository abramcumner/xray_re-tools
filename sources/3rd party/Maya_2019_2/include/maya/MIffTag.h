#ifndef _MIffTag
#define _MIffTag
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+
//
// CLASS:    MIffTag
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MString.h>
#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MIffTag)

//! \ingroup OpenMaya
//! \brief Encapsulates IFF's 4 character block structure. 
/*!
	Class MIffTag is used to create the 4 byte tags that are used to
	distinguish the different block types within an IFF file.
*/
class OPENMAYA_EXPORT MIffTag
{
public:
	MIffTag();

	MIffTag(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

	MIffTag(unsigned int tag);

	~MIffTag();

	bool  operator==( const MIffTag& tag ) const;

	//! Marks the start of a group of data blocks.
	static const MIffTag kFORM;

	//! Defines an unsorted group of FORM, e.g. images or sounds.
    static const MIffTag kCAT;

	//! Defines a sorted group of FORM, e.g. sequence of images
    static const MIffTag kLIST;

	//! Marks the start of a group of data blocks containing
	//! properties shared between forms that are a part of a LIST
    static const MIffTag kPROP;

	//! Four byte aligned FORM block
	static const MIffTag kFOR4;

	//! Eight byte aligned FORM block
    static const MIffTag kFOR8;

	//! Four byte aligned CAT block
    static const MIffTag kCAT4;

	//! Eight byte aligned CAT block
    static const MIffTag kCAT8;

	//! Four byte aligned LIST block
    static const MIffTag kLIS4;

	//! Eight byte aligned LIST block
    static const MIffTag kLIS8;

	//! Four byte aligned PROP block
    static const MIffTag kPRO4;

	//! Eight byte aligned PROP block
    static const MIffTag kPRO8;

protected:
	// No protected members

private:
	friend class MIffFile;

	union {
		int			 _force_alignment;
		char		 f_data[4];
	};
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MIffTag */
