#ifndef _MPxFileTranslator
#define _MPxFileTranslator
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
// CLASS:    MPxFileTranslator
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MTypes.h>
#include <maya/MFileObject.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxFileTranslator)

//! \ingroup OpenMaya MPx
//! \brief Base Class for creating Maya File Translators. 
/*!
  This class provides the connection to Maya by which user written file
  translators can be added as plug-ins.

  This class provides a base class from which one derives in order to
  implement Maya "File Translator Plug-Ins."  A file translator
  plug-in allows Maya to read or write 3rd party file formats.

  The identifyFile method is called whenever Maya's file save or
  restore dialogs are run, and identifyFile uses the filename and
  first few bytes of the file to determine whether it is of a type
  supported by the translator.  If it is, then Maya will display the
  file with the "name" and "icon" specified as arguments to
  MFnPlugin::registerFileTranslator.

  If an attempt is made to read the file, the "reader" method in the
  derived class is called, if an attempt is made to write the file,
  the "writer" method is called.
*/
class OPENMAYA_EXPORT MPxFileTranslator
{

public:

	//! How the translator identifies this file
	enum MFileKind {
		kIsMyFileType,		//!< Translator understands how to read/write this file
		kCouldBeMyFileType,	//!< Translator is not best available to read/write this file
		kNotMyFileType		//!< Translator does not understand how to read/write this file
	};

	//! Type of file access
	enum FileAccessMode {
		kUnknownAccessMode,			//!< This mode is set when no file operation is currently in progress.
		kOpenAccessMode,			//!< This mode is set when data is being read into a new scene. (i.e.: file -open True)
		kReferenceAccessMode,		//!< This mode is set when a referenced file is being read.  This mode can be temporary if the parent file was opened or imported in.  For example, if a parent file is being opened, the file access mode will be set to kOpenAccessMode but when the reference file is being read, it will be set to kReferenceAccessMode.  It will be set back to kOpenAccessMode once the reference file has been read.
		kImportAccessMode,			//!< This mode is set when data is being read into the current scene.
		kSaveAccessMode,			//!< This mode is set when the user saves the file.  (i.e.: file -save True)
		kExportAccessMode,			//!< This mode is set when the user chooses to export all or a referenced file is being written out.  This mode can be temporary if the parent file was saved or exported out.  For example, if a parent file is being saved, the file access mode will be set to kSaveAccessMode but when the reference file is being written, it will be set to kExportAccessMode.  It will be set back to kSaveAccessMode once the reference file has been written.
		kExportActiveAccessMode		//!< This mode is set when only the selected items are to be exported.
	};
						MPxFileTranslator ();
	virtual				~MPxFileTranslator ();
	virtual MStatus		reader ( const MFileObject& file,
								 const MString& optionsString,
								 FileAccessMode mode);
	virtual MStatus		writer ( const MFileObject& file,
								 const MString& optionsString,
								 FileAccessMode mode);
	virtual bool		haveReadMethod () const;
	virtual bool		haveWriteMethod () const;
	virtual bool		haveNamespaceSupport () const;
	virtual bool		haveReferenceMethod () const;
	virtual bool		allowMultipleFileOptimization() const;

	virtual MString     defaultExtension () const;
	virtual MString     filter () const;
	virtual bool        canBeOpened () const;
	virtual MPxFileTranslator::MFileKind identifyFile (	const MFileObject& file,
								const char* buffer, short size) const;
	static MPxFileTranslator::FileAccessMode fileAccessMode();

protected:
// No protected members

private:
	void*				data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxFileTranslator */
