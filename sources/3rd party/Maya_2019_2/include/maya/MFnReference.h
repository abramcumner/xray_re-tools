#ifndef _MFnReference
#define _MFnReference
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
// CLASS:    MFnReference
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFnReference)

//! \ingroup OpenMaya MFn
//! \brief Function set for querying information about file references.
/*!
  
  Reference nodes are used to track data about file references. Referenced
  files may be loaded and unloaded. They also store data about changes
  made to objects in the referenced file. The class MItEdits may 
  be used to retrieve information about such changes.

  The class MFileIO may be used to perform actual operations on references
  such as loading, unloading, adding and removing references.

*/

class OPENMAYA_EXPORT MFnReference : public MFnDependencyNode
{
    declareMFn(MFnReference, MFnDependencyNode);

	MString 			fileName( bool resolvedName,
								  bool includePath,
								  bool includeCopyNumber,
								  MStatus* ReturnStatus = NULL ) const;
	MString 			associatedNamespace( bool shortName,
								   MStatus* ReturnStatus = NULL ) const;

	MString 			parentFileName( bool resolvedName,
										bool includePath,
										bool includeCopyNumber,
										MStatus* ReturnStatus = NULL ) const;
	MObject 			parentReference( MStatus* ReturnStatus = NULL ) const;
	MObject 			parentAssembly( MStatus* ReturnStatus = NULL ) const;

	bool 				containsNode( const MObject& node,
									  MStatus* ReturnStatus = NULL ) const;
	bool 				containsNodeExactly( const MObject& node,
											 MStatus* ReturnStatus = NULL ) const;
	void 				nodes( MObjectArray& nodes,
							   MStatus* ReturnStatus = NULL ) const;

	bool				isLoaded( MStatus* ReturnStatus = NULL ) const;
	bool				isLocked( MStatus* ReturnStatus = NULL ) const;
	bool				isExportEditsFile( MStatus* ReturnStatus = NULL ) const;	

    static bool         ignoreReferenceEdits();
    static void         setIgnoreReferenceEdits(bool ignoreEdits);

public:

BEGIN_NO_SCRIPT_SUPPORT:
    declareMFnConstConstructor(MFnReference, MFnDependencyNode);
END_NO_SCRIPT_SUPPORT:
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFnReference */



