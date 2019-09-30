#ifndef _MFnCompoundAttribute
#define _MFnCompoundAttribute
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
// CLASS:    MFnCompoundAttribute
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnCompoundAttribute)

//! \ingroup OpenMaya MFn
//! \brief Compound attribute function set. 
/*!
   MFnCompoundAttribute is the function set for compound dependency node
   attributes.

   Compound attributes allow the grouping of related attributes into a larger
   unit.  It is possible to connect to a compound attribute as a whole, or
   to any of the individual children.

   For example, the three attributes RED, GREEN, and BLUE could be grouped into
   a compound attribute of type COLOR.  It is then possible to connect two
   COLOR attributes together.  This removes the need to connect each child
   explicitly.

   A second use for compound attributes is when there are multi attributes that
   relate to each other on an element-by-element basis.  An example of this
   is the weighted matrix node that has a multi attribute with matrices that
   must be matched with the multi attribute that provides the weights for each
   matrix.
*/
class OPENMAYA_EXPORT MFnCompoundAttribute : public MFnAttribute
{
	declareMFn(MFnCompoundAttribute, MFnAttribute);

public:
	MObject 	create( const MString& full,
						const MString& brief,
						MStatus* ReturnStatus = NULL );
	MStatus		addChild( const MObject & child );
	MStatus		removeChild( const MObject & child );
	unsigned int	numChildren( MStatus* ReturnStatus = NULL ) const;
	MObject 	child( unsigned int index, MStatus* ReturnStatus = NULL ) const;
	MStatus		getAddAttrCmds(
					MStringArray&	cmds,
					bool			useLongNames = false
				) const;

BEGIN_NO_SCRIPT_SUPPORT:

	//!	NO SCRIPT SUPPORT
 	declareMFnConstConstructor( MFnCompoundAttribute, MFnAttribute );

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

private:
// No private members
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnCompoundAttribute */
