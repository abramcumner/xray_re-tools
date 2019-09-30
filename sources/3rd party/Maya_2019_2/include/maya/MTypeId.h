#ifndef _MTypeId
#define _MTypeId
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
// CLASS:    MTypeId
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MTypeId)

//! \ingroup OpenMaya
//! \brief Manage Maya Object type identifiers. 
/*!
In Maya, both intrinsic and user-defined Maya Objects are registered and
recognized by their type identifier or type id.  The basis of the type id system
is a tag which is used at run-time to determine how to create and destroy Maya
Objects, and how they are to be input/output from/to files.  These tag-based
identifiers are implemented by the class MTypeId.

Use the MTypeId class to create, copy and query Maya Object type identifiers.

It is very important to note that these ids are written into the Maya binary
file format.  So, once an id is assigned to a node or data type it can
<i>never</i> be changed while any existing Maya file contains an instance of
that node or data type.  If a change is made, such files will become
unreadable.

Thus, even though we provide a range of reserved ids that you can use for
internal plug-ins, Autodesk highly recommends that you obtain a
globally unique id range (see below) and use ids from this range for all
your plug-ins, even internal ones.  This can prevent significant headaches
later if the plans for your plug-ins change.

There are 2 forms of the constructor for this class that can be used
depending on whether the plug-in id is internal or globally unique.

For plug-ins that will forever be internal to your site use the constructor
that takes a single unsigned int parameter.  The numeric range 0 - 0x7ffff
(524288 ids) has been reserved for such plug-ins.

The example plug-ins provided with Maya in the plug-in development
kit will use ids in the range 0x80000 - 0xfffff (524288 ids).  If you
customize one of these example plug-ins, you should change the id to avoid
future conflicts.

Plug-ins that are intended to be shared between sites will need to have a
globally unique id.  The Autodesk Developer Network (ADN) will provide
such id's in blocks of 256.  You will be assigned one or more 24-bit
prefixes.  Once this has been obtained, used the MTypeId constructor that
takes 2 unsigned int parameters.  The prefix goes in the first parameter,
and you are responsible for managing the allocation of the 256 ids that
go into the second parameter.
*/
class OPENMAYA_EXPORT MTypeId
{
public:
				MTypeId();
				~MTypeId();

				MTypeId( unsigned int id );
				MTypeId( unsigned int prefix, unsigned int id );

				MTypeId( const MTypeId& src );
	MTypeId&	operator=( const MTypeId& rhs );
	bool		operator==( const MTypeId& rhs ) const;
	bool		operator!=( const MTypeId& rhs ) const;
	unsigned int	id( MStatus * ReturnStatus = NULL ) const;

	static const char* className();

protected:
// No protected members

private:
	union {
		unsigned int fId;
		unsigned char fTag[4];
	};
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MTypeId */
