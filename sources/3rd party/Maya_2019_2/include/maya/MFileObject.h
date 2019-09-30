#ifndef _MFileObject
#define _MFileObject
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
// CLASS:    MFileObject
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>

#include <maya/MString.h>
#include <maya/MURI.h>
#include <maya/MDeprecate.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MFileObject)

//! \ingroup OpenMaya
//! \brief Manipulate filenames and search paths. 
/*!
The MFileObject class implements an object that contains both a
filename and a search path.  The search path is specified by a single
string in which multiple elements are separated by ':' characters.  As
well this string can contain Unix environment variables, specified as
$VARNAME,

Filenames can be produced by the class by combining the path element
with the filename element of the MFileObject.  As well, methods are
available to test for the existance of the files produced.
*/
class OPENMAYA_EXPORT MFileObject
{
public:
				MFileObject();
				MFileObject( const MFileObject & other );
				virtual ~MFileObject();
	MFileObject& operator=( const MFileObject & other );

	MStatus   	setRawName( const MString & fileName );
	MStatus   	setRawPath( const MString & filePath );
	MStatus   	setRawFullName( const MString & fullFileName );
	MStatus   	setRawURI( const MString & uri );
	MStatus   	setRawURI( const MURI & uri );

	MStatus		overrideResolvedFullName( const MString & fullFileName, bool reresolveType = false );

	MString  	rawName() const;
	MString  	rawPath() const;
	MString  	rawFullName() const;
	MURI		rawURI() const;

	MString  	expandedPath() const;
	MString  	expandedFullName() const;

	MString  	resolvedName() const;
	MString  	resolvedPath() const;
	MString  	resolvedFullName() const;

	unsigned int	pathCount();
	MString  	ithPath( unsigned int index );
	MString  	ithFullName( unsigned int index );
	bool        exists();
	bool        exists( unsigned int index );
	bool        isSet() const;


	/*! 
	    Options to be used when resolving a file path.
	    When more than one resolve method is used, Maya will perform checks in
		a pre-determined sequence, stopping when it encounters an existing file.
	    The default mode MFileResolveMethod::kNone cannot be used in combination with other
		modes.
	    Note that not all resolve methods check whether or not a file actually exists. 
		Some resolution modes, such as MFileResolveMethod::kInputFile, imply that the file 
		is being used for input, and is expected to exist.  These modes will check if the 
		file exists to determine whether or not the resolution is successful. 
	*/

	enum MFileResolveMethod 
	{
		//! (Default) The resolved path is simply the resulting path after converting
		//! the raw value to its expanded form. If the path contains environment variables,
		//! the resolved value will be the first path returned from their expansion.
		//! Relative paths will be considered to be relative to root of the current project.
		//! The resolution algorithm will not check if this file actually exists - the
		//! resolution will be considered successful whether it exists or not.
		//! With this mode, the resolver will not continue on to attempt to resolve 
		//! using any other resolve method. 
		//! The user must explicitly check MFileObject::exists() to determine if it is an 
		//! existing path. 
		kNone 				= (1<<0),

		//! Checks if expanded paths exist. If paths are relative, assume it's relative to
		//! the current workspace (so check workspace current directory, file-rule directory and
		//! root directory)
		kExact 			    = (1<<1),

		//! Checks path against mappings defined with the dirmap command. Only for absolute paths
		kDirMap			    = (1<<2),

		//! Check path against any previously re-mapped reference locations. If kRelative/kBaseName
		//! are set, then even if we have an absolute path, convert to relative and/or baseName and
		//! look for them in directories provided to the missing reference dialog.
		kReferenceMappings	= (1<<3),

		//! Strips away the project directory, and treats path as relative. Relative to the current
		//! workspace, that is. So look in the workspace current directory, file-rules directory
		//! and the root directory
		kRelative		    = (1<<4),

		//! Strips away everything but the base file name and look in the current workspace
		kBaseName		    = (1<<5),

		//! This mode is the default on file open and import, and is suitable for 
		//! files that are to be used as input files.  The file will be checked for 
		//! existence.
		kInputFile		    = kExact | kDirMap | kRelative | kBaseName,

		//! This mode is the default on file reference. In addition to the checks done for
		//! a regular input file, it will also check the reference mappings. 
		kInputReference		= kInputFile | kReferenceMappings,

		//! Equivalent to the file -strict flag
		kStrict			    = kExact | kDirMap
	};

	void					setResolveMethod(MFileResolveMethod method);
	MFileResolveMethod		resolveMethod() const;

	// The following methods are deprecated.
	//
	OPENMAYA_DEPRECATED(2019, "Use the method MFileObject::setRawName instead.")
	MStatus   	setName( const MString & fileName );
	OPENMAYA_DEPRECATED(2019, "Use the method MFileObject::setRawFullName instead.")
	MStatus   	setFullName( const MString & fileName );
	OPENMAYA_DEPRECATED(2019, "Use the method MFileObject::resolvedName instead.")
	MString  	name() const;
	OPENMAYA_DEPRECATED(2019, "Use the methods MFileObject::expandedPath or MFileObject::resolvedPath instead.")
	MString  	path() const;
	OPENMAYA_DEPRECATED(2019, "Use the methods MFileObject::expandedFullName or MFileObject::resolvedFullName instead.")
	MString  	fullName() const;

    // Static member functions
    //
    static bool isAbsolutePath(const MString & fileName );
	static MString getResolvedFullName(const MString &rawFullName);
	static MString getResolvedFullName(const MString &rawFullName, bool &exists, MFileObject::MFileResolveMethod method=MFileObject::kNone);

protected:
// No protected members

private:
	void * data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MFileObject */
