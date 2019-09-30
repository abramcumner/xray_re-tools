#ifndef _MExternalContentLocationTable
#define _MExternalContentLocationTable
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#if defined __cplusplus

#include <maya/MTypes.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

//! \ingroup OpenMaya
/*! \brief This is a table of the all the external content locations for a given
	MPxNode.

	It is a subset of the information contained in MExternalContentInfo: it has
	as many entries, but each entry only states the key and location.

	While MExternalContentInfoTable is meant to query external content from a
    MPxNode, MExternalContentLocationTable is meant to pass modified
    locations back to the MPxNode.

	Key: Same as for MExternalContentInfoTable.

    Value: New location of the content for this entry (path, URI, ...).
*/
class OPENMAYA_EXPORT MExternalContentLocationTable
{
public:
	MExternalContentLocationTable();
	~MExternalContentLocationTable();
	
	MStatus addEntry( const MString& key, const MString& location );
	
BEGIN_NO_SCRIPT_SUPPORT:
	//!	NO SCRIPT SUPPORT
	MStatus getEntry( unsigned int index, MString& key, MString& location ) const;
	//!	NO SCRIPT SUPPORT
	MStatus getLocation( const MString& key, MString& location ) const;
END_NO_SCRIPT_SUPPORT:	
	
	unsigned int length() const;
	
private:
	friend class MPxNode;
	friend class MFnDependencyNode;

	void* getData() const;
	MExternalContentLocationTable( const MExternalContentLocationTable& );
	MExternalContentLocationTable& operator=( const MExternalContentLocationTable& );
	MExternalContentLocationTable( const void* const data );	
	
	void* api_data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MExternalContentLocationTableTable */
