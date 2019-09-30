#ifndef _MFileIO
#define _MFileIO
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
// CLASS:    MFileIO
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MPlugArray.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MSelectionList.h>
#include <maya/MTypes.h>
#include <maya/MDeprecate.h>

namespace adsk {
	namespace Data {
		class Associations;
	}
}

// ****************************************************************************
// DECLARATIONS


OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFileIO)

//! \ingroup OpenMaya
//! \brief I/O operations on scene files. 
/*!
  Methods for opening, saving, importing, exporting and referencing files.

  The MFileIO class provides a set of global functions that can be used to
  load and store scene elements into maya files.  Methods are also provided
  to import elements from other files into the current scene, and export
  elements of the current scene into files.  As well, references to other
  files can be created and removed.

  This class implements only the most common operations from the full set
  possible with the MEL command "file."  See the documentation of this
  command for a complete listing of all the possiblities.  If more control
  is required than provided by this class, a MEL command can be constructed
  according to the instructions for the "file" command, and issued via the
  executeCommand method of the MGlobal class.

  Note: Be cautious about using MFileIO::importFile, MFileIO::Save, MFIleIO::saveAs 
  MFileIO::open, MFileIO::newFile and MFileIO::reference methods while any 
  of MFileIO::isReadingFile, MFileIO::isWritingFile, MFileIO::isOpeningFile, 
  MFileIO::isNewingFile, MFileIO::isSavingReference, MFileIO::isImportingFile 
  or MFileIO::isReferencingFile are returning true as it might lead to inconsitent 
  scenes or unpredictable results.
  
*/
class OPENMAYA_EXPORT MFileIO
{
public:
	//! Specifies which references to load.
	enum ReferenceMode {
		kLoadDefault,	//!< Load references that were loaded when scene was saved.
		kLoadAllReferences,	//!< Load all references in the scene.
		kLoadNoReferences	//!< Don't load any references.
	};

	//! Specifies the unique identifier for official Maya file format versions
	enum MayaFileFormatVersion {
		kVersion4_0 = 74,
		kVersion4_5 = 83,
		kVersion5_0 = 90,
		kVersion6_0 = 105,
		kVersion6_5 = 117,
		kVersion7_0 = 128,
		kVersion8_0 = 133,
		kVersion8_5 = 139,
		kVersion2008 = 144,
		kVersion2009 = 147,
		kVersion2010 = 152,
		kVersion2011 = 156,
		kVersion2012 = 161,
		kVersion2013 = 167,
		kVersion2014 = 179,
		kVersion2015 = 184,
		kVersion2016 = 192,
		kVersion2016R2 = 198,
		kVersion2017 = 201,
		kVersion2017Update3 = 202,
        kVersion2017Update4 = 203,
		kVersion2018 = 210,
        kVersion2018Update2 = 211,
        kVersion2018Update3 = 212,
		kVersion2018Update4 = 213,
		kVersion2019 = 232

	};

    //! Specifies what to do with objects remaining in the file reference's namespace, when the file reference is removed
    enum NamespaceMergeMethod {
        kRemoveNamespaceIfEmpty, //!< If the reference is alone in its namespace, remove the namespace. Else, keep the objects and the namespace.
        kMergeNamespaceWithRootNamespace, //!< Move remaining objects to root namespace, and delete namespace. Cannot be used if the reference resides in the root namespace.
        kMergeNamespaceWithParentNamespace, //!< Move remaining objects to parent namespace, and delete namespace. Cannot be used if the reference resides in the root namespace.
        kForceDeleteNamespaceContent //!< Delete all namespace content, and delete namespace. Cannot be used if the reference resides in the root namespace.
    };

	//! Types of export operation.
	enum ExportType {
		//! Export all.
		kExportTypeAll,
		//! Export selected.
		kExportTypeSelected,
		//! Export anim.
		kExportTypeAnim,
		//! Export anim from reference.
		kExportTypeAnimFromReference,
		//! Export as reference.
		kExportTypeAsReference,
		//! Export edits.
		kExportTypeEdits,
		//! Export pref objects.
		kExportTypePrefObjects
	};
	
						MFileIO ();
						MFileIO ( const MString& fileName );
	virtual				~MFileIO ();
	static MString		currentFile ();
	static MStatus		setCurrentFile ( const MString& fileName );
	static MString		fileType();
	static MStatus		getFileTypes ( MStringArray& types );
	static MStatus		newFile ( bool force = false );
	static MStatus		open ( const MString& fileName,
							   const char* type = NULL,
							   bool force = false,
							   ReferenceMode refMode = kLoadDefault,
							   bool ignoreVersion = false );
	static MStatus		save ( bool force = false );
	static MStatus		saveAs ( const MString& fileName,
							     const char* type = NULL,
								 bool force = false );
	static MStatus		importFile ( const MString& fileName,
								 const char* type = NULL,
								 bool  preserveReferences = false,
								 const char* nameSpace = NULL,
								 bool  ignoreVersion = false);

	static MStatus		exportSelected ( const MString& fileName,
								 const char* type = NULL,
								 bool preserveReferences = false);
	static MStatus		exportAll ( const MString& fileName,
								 const char* type = NULL,
								 bool preserveReferences = false);
	static MStatus		exportAnimFromReference ( const MString& fileName, const char* type = NULL);
	static MStatus		exportAnimFromReference ( const MString& fileName, const MString& refNodeName, const char* type = NULL);
	static MStatus		exportSelectedAnimFromReference ( const MString& fileName, const char* type = NULL);
	static MStatus		exportSelectedAnimFromReference ( const MString& fileName, const MString& refNodeName, const char* type = NULL);
	static MStatus		exportAnim ( const MString& fileName, const char* type = NULL);
	static MStatus		exportSelectedAnim ( const MString& fileName, const char* type = NULL);
	static MStatus		exportAsReference ( const MString& fileName, const char* type = NULL);

	static MStatus		getReferences ( MStringArray& references, 
										bool wantUnresolvedNames = false );
	static MStatus		getFiles( MStringArray& files,
								  bool wantUnresolvedNames = false,
								  bool wantReferencesOnly = false );
	static MStatus		getReferenceNodes ( const MString &fileName,
											MStringArray& nodes );
	static MStatus		getReferenceNodes ( const MString &fileName, 
											MSelectionList& list);	
	static MStatus		getReferenceConnectionsMade ( const MString &fileName,
													  MStringArray& connections );
	static MStatus		getReferenceConnectionsBroken(
							const MString&	fileName,
							MStringArray&	connections
						);
	static MStatus		getReferenceConnectionsBroken(
							const MString&	fileName,
							MPlugArray&		srcPlugs,
							MPlugArray&		destPlugs
						);
	static MStatus		getReferenceConnectionsBroken(
							const MObject&	node,
							MStringArray&	connections,
							bool			append = false,
							bool			removeDuplicates = true
						);
	static MStatus		getReferenceConnectionsBroken(
							const MObject&	node,
							MPlugArray&		srcPlugs,
							MPlugArray&		destPlugs,
							bool			append = false,
							bool			removeDuplicates = true
						);
	static MStatus		reference ( const MString& fileName, bool deferReference = false, bool lockReference = false, const MString& nameSpace = "" );
	static MStatus		removeReference ( const MString& fileName, NamespaceMergeMethod namespaceMergeMethod = kRemoveNamespaceIfEmpty );
	static bool			isReadingFile();
	static bool			isWritingFile();
	static bool			isOpeningFile();
	static bool			isNewingFile();
	static bool			isSavingReference();
	static bool			mustRenameToSave(MStatus *ReturnStatus = NULL);
	static MStatus		setMustRenameToSave(bool);
	static MString		mustRenameToSaveMsg(MStatus *ReturnStatus = NULL);
	static MStatus		setMustRenameToSaveMsg(const MString &);

	static MString		beforeOpenFilename(MStatus * ReturnStatus = NULL);
	static MString		beforeImportFilename(MStatus * ReturnStatus = NULL);
	static MString		beforeSaveFilename(MStatus * ReturnStatus = NULL);
	static MString		beforeExportFilename(MStatus * ReturnStatus = NULL);
	static MString		beforeReferenceFilename(MStatus * ReturnStatus = NULL);

	static MPxFileTranslator*	beforeOpenUserFileTranslator(MStatus* ReturnStatus = NULL);
	static MPxFileTranslator*	beforeImportUserFileTranslator(MStatus* ReturnStatus = NULL);
	static MPxFileTranslator*	beforeSaveUserFileTranslator(MStatus* ReturnStatus = NULL);
	static MPxFileTranslator*	beforeExportUserFileTranslator(MStatus* ReturnStatus = NULL);
	static MPxFileTranslator*	beforeReferenceUserFileTranslator(MStatus* ReturnStatus = NULL);

	static ExportType	exportType(MStatus* ReturnStatus = NULL);

	static MString		getLastTempFile(MStatus * ReturnStatus = NULL);
	static bool			getErrorStatus(MStatus * ReturnStatus = NULL);
	static MStatus		resetError();
    static MStatus     	setError();

	static MString		loadReference(const MString& referenceFileName, MStatus *returnStatus = NULL);
	static MString		loadReferenceByNode(const MString& referenceNodeName, MStatus *returnStatus = NULL);
	static MString		loadReferenceByNode(MObject& referenceNode, MStatus *returnStatus = NULL);

	static MString		unloadReference(const MString& referenceFileName, MStatus *returnStatus = NULL);
	static MString		unloadReferenceByNode(const MString& referenceNodeName, MStatus *returnStatus = NULL);
	static MString		unloadReferenceByNode(MObject& referenceNode, MStatus *returnStatus = NULL);
	static MString		getReferenceFileByNode(MObject& referenceNode, MStatus *returnStatus = NULL);
	static MStatus		cleanReference(const MString& referenceNodeName, const char* editCommand = NULL, MStatus *returnStatus = NULL);
	static MStatus		saveReference(const MString& referenceNodeName, MStatus *returnStatus = NULL);

	static MString		fileCurrentlyLoading ();
	static bool			isImportingFile();
	static bool			isReferencingFile();

	static int			currentlyReadingFileVersion();
	static int 			latestMayaFileVersion();

	static const char* 	className();

BEGIN_NO_SCRIPT_SUPPORT:

	// Obsolete & no script support
    OPENMAYA_DEPRECATED(2019, "Use the MFileIO::importFile method instead.")
	static MStatus		import ( const MString& fileName,
								 const char* type = NULL,
								 bool  preserveReferences = false,
								 const char* nameSpace = NULL);

	//----------------------------------------------------------------------
	//
	// File-level Metadata methods, attaches to the current file.
	//
	static	const adsk::Data::Associations*	metadata( MStatus* ReturnStatus=NULL );
	static	MStatus	setMetadata		( const adsk::Data::Associations& );
	static	MStatus	deleteMetadata	();
	//
	//----------------------------------------------------------------------

END_NO_SCRIPT_SUPPORT:

protected:
// No protected members

};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFileIO */
