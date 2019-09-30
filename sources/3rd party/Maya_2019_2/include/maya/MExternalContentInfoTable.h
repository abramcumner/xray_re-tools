#ifndef _MexternalContentInfoTable
#define _MexternalContentInfoTable
//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStringArray.h>
#include <maya/MTypes.h>
#include <maya/MDeprecate.h>


// ****************************************************************************
// FORWARD DECLARATIONS



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MExternalContentInfoTable)

//! \ingroup OpenMaya
/*! \brief This is a table of all the external content for a given MPxNode.

	External content is defined as any data that is not stored with the node
	and that is needed by the node to perform its work.  It can be texture
    files, other scenes, audio files, etc.  

	Each node that references external content should report it through 
	MPxNode::getExternalContent so that a complete list of external content can
	be saved with the scene in a way that makes it easy to inspect and edit 
	offline.
*/
class OPENMAYA_EXPORT MExternalContentInfoTable
{
public:
	MExternalContentInfoTable();
	~MExternalContentInfoTable();
	
	// obsolete, use the one with contextNodeFullName param
    OPENMAYA_DEPRECATED(2019, "Context (owner Node full name) must be provided to the Maya resolver. Use the one with the contextNodeFullName input param.")
	MStatus addResolvedEntry(
		const MString&		key,
		const MString& 		unresolvedLocation,
		const MString& 		resolvedLocation,
		const MStringArray& roles
	);
	MStatus addResolvedEntry(
		const MString&		key,
		const MString& 		unresolvedLocation,
		const MString& 		resolvedLocation,
		const MString&      contextNodeFullName,
		const MStringArray& roles
	);
	// obsolete, use the one with contextNodeFullName param
	MStatus addUnresolvedEntry(
		const MString&				  key,
		const MString&                unresolvedLocation,
		const MStringArray& roles =   MStringArray()
	);
	MStatus addUnresolvedEntry(
		const MString&				  key,
		const MString&                unresolvedLocation,
		const MString&				  contextNodeFullName,
		const MStringArray& roles =   MStringArray()
	);
	
BEGIN_NO_SCRIPT_SUPPORT:
	//! obsolete, use the one with contextNodeFullName param
	//!	NO SCRIPT SUPPORT
    OPENMAYA_DEPRECATED(2019, "Use the one with the contextNodeFullName output param.")
	MStatus getInfo(
		const MString&		key,
		MString& 			unresolvedLocation,
		MString& 			resolvedLocation,
		MStringArray& 		roles
	) const;
	//!	NO SCRIPT SUPPORT
	MStatus getInfo(
		const MString&		key,
		MString& 			unresolvedLocation,
		MString& 			resolvedLocation,
		MString&			contextNodeFullName,
		MStringArray& 		roles
	) const;
	//! obsolete, use the one with contextNodeFullName param
	//!	NO SCRIPT SUPPORT
	MStatus getEntry(
		unsigned int		index,
		MString&	   		key,
		MString& 			unresolvedLocation,
		MString& 			resolvedLocation,
		MStringArray& 		roles
	) const;
	//!	NO SCRIPT SUPPORT
	MStatus getEntry(
		unsigned int		index,
		MString&	   		key,
		MString& 			unresolvedLocation,
		MString& 			resolvedLocation,
		MString&			contextNodeFullName,
		MStringArray& 		roles
	) const;
END_NO_SCRIPT_SUPPORT:
	unsigned int length() const;
	
private:
	friend class MPxNode;
	friend class MFnDependencyNode;
	
	void* getData() const;
	void resetCache();
	MExternalContentInfoTable( const MExternalContentInfoTable& );
	MExternalContentInfoTable& operator=( const MExternalContentInfoTable& );
	MExternalContentInfoTable( void* data );

	void* api_data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MexternalContentInfoTable */
