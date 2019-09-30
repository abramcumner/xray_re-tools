#ifndef _MLibrary
#define _MLibrary
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
// CLASS:    MLibrary
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MTypes.h>


#ifndef SWIG
//	Make sure those functions have the proper export/visibility
	PLUGIN_EXPORT MStatus initialize (const char* applicationName, bool useBatchLicense = false);
	PLUGIN_EXPORT MStatus initialize (bool wantScriptOutput, const char* applicationName, bool useBatchLicense = false);
	PLUGIN_EXPORT void cleanup( int exitStatus = 0, bool exitWhenDone = true );
#endif

OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MLibrary)

//! Set up Maya to run in library mode. (OpenMaya)
/*!
 Initialize and cleanup routines for Maya running in library mode.

When creating a "library mode" Maya application, this class must be
used in the program's main routine to initialize Maya, and later
to cleanUp allocated Maya state.  A typical usage would be:

\code
    main(int argc, char **argv)
    {
        MStatus status;
        status = MLibrary::initialize (argv[0], true);
        if ( !status ) {
            status.perror("MLibrary::initialize");
            return (1);
        }

        // Maya API code goes here

        MLibrary::cleanup();
        return (0);
    }
\endcode

If the call to cleanup is omitted, you will get errors when the program
exits as the static destructors in the Maya libraries are run.
*/
class OPENMAYA_EXPORT MLibrary
{
public:
						MLibrary ();
	virtual				~MLibrary ();
	static MStatus		initialize (const char* applicationName,
									bool useBatchLicense = false);
	static MStatus		initialize (bool wantScriptOutput,
									const char* applicationName,
									bool useBatchLicense = false);

	static void			cleanup( int exitStatus = 0, bool exitWhenDone = true );

protected:
// No protected members

private:
// No private members

};

OPENMAYA_NAMESPACE_CLOSE

#ifdef _WIN32
#if _MSC_VER < 1500
	// When  external code is compiling, MLIBRARY_DONTUSE_MFC_MANIFEST
	// will not be defined.  This will force the manifest on
	// x86 and amd64 to mfc80.dll.  We do not force the manifest
	// to mfc80d.dll for a Debug build since a released version of
	//  Maya does not use Debug libraries.
	#if !defined(MLIBRARY_DONTUSE_MFC_MANIFEST)
		#include <crtassem.h>
		#pragma comment(lib, "mfc80")
		#if defined(_M_IX86)
			#pragma comment(linker,"/manifestdependency:\"type='win32' "	\
				"name='" __LIBRARIES_ASSEMBLY_NAME_PREFIX ".MFC' "			\
				"version='" _CRT_ASSEMBLY_VERSION "' "						\
				"processorArchitecture='x86' "								\
				"publicKeyToken='" _VC_ASSEMBLY_PUBLICKEYTOKEN "'\"")
		#elif defined(_M_AMD64)
			#pragma comment(linker,"/manifestdependency:\"type='win32' "	\
				"name='" __LIBRARIES_ASSEMBLY_NAME_PREFIX ".MFC' "			\
				"version='" _CRT_ASSEMBLY_VERSION "' "						\
				"processorArchitecture='amd64' "							\
				"publicKeyToken='" _VC_ASSEMBLY_PUBLICKEYTOKEN "'\"")
		#endif	// _M_AMD64
	#endif	// MLIBRARY_DONTUSE_MFC_MANIFEST
#endif
#endif // _WIN32

#endif /* __cplusplus */
#endif /* _MLibrary */
