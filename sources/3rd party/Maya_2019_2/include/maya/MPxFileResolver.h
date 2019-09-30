#ifndef _MPxFileResolver
#define _MPxFileResolver
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
// CLASS:    MPxFileResolver
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MTypes.h>
#include <maya/MURI.h>



OPENMAYA_MAJOR_NAMESPACE_OPEN

// ****************************************************************************
// CLASS DECLARATION (MPxFileResolver)

//! \ingroup OpenMaya MPx
//! \brief Base Class for creating custom Maya File Resolvers. 
/*!

  This class provides a base class from which a custom file resolver
  plug-in can be derived. A file resolver plug-in allows the user
  to override the manner in which an unresolved file path value 
  is converted to a resolved file path. The resolved file path
  is the location Maya will use to access the physical file.  

  Maya supports plug-in File Resolvers for file paths defined using
  URI values. When an unresolved URI file path is encountered, Maya will
  search for any registered URI file resolver plug-ins that can handle it.

  URI file resolvers are registered for a given URI scheme.  For
  example, a custom file resolver plug-in could be registered to 
  handle the "http" URI scheme.  If Maya attempts to resolve a URI file path
  that has the scheme "http", the custom resolver plug-in will be called. 
  URI schemes are treated in a case-insensitive manner.  For example, the
  schemes 'http', 'HTTP', and 'Http' are considered equivalent. 

  MPxFileResolvers for URI schemes are registered using the 
  MFnPlugin::registerURIFileResolver method. 

*/
class OPENMAYA_EXPORT MPxFileResolver
{

public:

	/*! 
	    The MFileResolverMode provides additional information about
	    the reason for the resolution call that can be used by the resolver
	    implementation.
		Note that this enum is implemented as a bitfield. 
	*/
	enum MPxFileResolverMode {
		/*! 
		(Default) The resolved path is being requested, 
		but there is no additional information specified.
	    When kNone is used, it will not be 
		combined with any other modes listed below.
		In this case, the resolver should simply 
		return the resolved path as efficiently as possible. 
		The path returned by the resolver will not be checked for 
		existence.
		*/
		kNone = (1<<0),
		/*! 
		The resolved path is being requested for an input file.
		In this case, the resolver plug-in may need to do 
		additional work to ensure that the resolved path is
		available to the application. 
		The path returned by the resolver will be checked for 
		existence.
		*/
		kInput = (1<<1)	
	};

						MPxFileResolver ();
	virtual				~MPxFileResolver ();
	virtual MString		resolveURI ( const MURI& uriValue,
									 MPxFileResolver::MPxFileResolverMode mode,
									 MStatus * ReturnStatus = NULL);
	virtual MString		resolveURIWithContext ( const MURI& uriValue,
									 MPxFileResolver::MPxFileResolverMode mode,
									 const MString &contextNodeFullName,
									 MStatus * ReturnStatus = NULL);
	virtual MString     uriScheme () const;
	MString     		resolverName () const;
	static const char*  className();

	virtual void		performAfterSaveURI( const MURI& uriValue,
											 const MString &resolvedFullPath);


	// Static member functions 
	static MPxFileResolver *findURIResolverByScheme( const MString &uriScheme );
	static MPxFileResolver *findURIResolverByName( const MString &resolverName );
	static MStringArray     getURIResolversByScheme();
	static MStringArray     getURIResolversByName();
	static int 		        numURIResolvers();

protected:
// No protected members

private:
	void* data;
};

OPENMAYA_NAMESPACE_CLOSE

#endif /* __cplusplus */
#endif /* _MPxFileResolver */
