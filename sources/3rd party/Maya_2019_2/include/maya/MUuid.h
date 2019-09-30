#ifndef _MUUID
#define _MUUID
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
// CLASS:    MUuid
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MUuid)

//! \ingroup OpenMaya
//! \brief Class to manipulate UUIDs.
/*!

Provides a class to manipulate UUIDs (Universally Unique Identifiers).

UUIDs (as implemented here) are 128-bit values, used to identify objects 'practically' uniquely.
Their main use in Maya is to identify DG nodes. Nodes have a UUID which persists even if the node's
name is changed, or its DAG relationship alters, and which is stored in the Maya scene file.

See http://en.wikipedia.org/wiki/Universally_unique_identifier

*/
class OPENMAYA_EXPORT MUuid
{

public:
										MUuid();
										MUuid( const MUuid & other );
										MUuid( const unsigned int* uuid );
										MUuid( const unsigned char* uuid );
										MUuid( const MString & value, MStatus *ReturnStatus=NULL );
										~MUuid();
    MUuid&								operator=(const MUuid & rhs);
    bool								operator==(const MUuid & rhs) const;
    bool                                operator!=(const MUuid & rhs) const;
	MStatus								get( unsigned char* uuid ) const;
	MString								asString() const;
	operator							MString() const;
	void								copy( const MUuid & rhs );

	bool								valid() const;
	MStatus								generate();

	static const char*	className();

private:
    void * fPtr;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MUUID */
